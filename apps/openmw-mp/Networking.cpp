#include "Player.hpp"
#include "processors/ProcessorInitializer.hpp"
#include <RakPeer.h>
#include <Kbhit.h>

#include <components/misc/stringops.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/TimedLog.hpp>
#include <components/openmw-mp/Version.hpp>
#include <components/openmw-mp/Packets/PacketPreInit.hpp>

#include <iostream>
#include <Script/Script.hpp>
#include <Script/API/TimerAPI.hpp>
#include <chrono>
#include <thread>
#include <csignal>

#include "Networking.hpp"
#include "MasterClient.hpp"
#include "Cell.hpp"
#include "CellController.hpp"
#include "processors/PlayerProcessor.hpp"
#include "processors/ActorProcessor.hpp"
#include "processors/ObjectProcessor.hpp"
#include "processors/WorldstateProcessor.hpp"

using namespace mwmp;

Networking *Networking::sThis = 0;

static int currentMpNum = 0;
static bool dataFileEnforcementState = true;
static bool scriptErrorIgnoringState = false;
bool killLoop = false;

Networking::Networking(RakNet::RakPeerInterface *peer) : mclient(nullptr)
{
    sThis = this;
    this->peer = peer;
    players = Players::getPlayers();

    rakNetManager = new mwmp::RakNetManager(peer);
    mwmp::RakNetManager::setInstance(rakNetManager);

    CellController::create();

    systemPacketController = new SystemPacketController(rakNetManager);
    playerPacketController = new PlayerPacketController(rakNetManager);
    actorPacketController = new ActorPacketController(rakNetManager);
    objectPacketController = new ObjectPacketController(rakNetManager);
    worldstatePacketController = new WorldstatePacketController(rakNetManager);

    // Set send stream
    systemPacketController->SetStream(0, &bsOut);
    playerPacketController->SetStream(0, &bsOut);
    actorPacketController->SetStream(0, &bsOut);
    objectPacketController->SetStream(0, &bsOut);
    worldstatePacketController->SetStream(0, &bsOut);

    running = true;
    exitCode = 0;

    Script::Call("OnServerInit");

    serverPassword = TES3MP_DEFAULT_PASSW;

    ProcessorInitializer();
}

Networking::~Networking()
{
    Script::Call("OnServerExit", false);

    CellController::destroy();

    sThis = 0;
    mwmp::RakNetManager::setInstance(nullptr);
    delete rakNetManager;
    delete systemPacketController;
    delete playerPacketController;
    delete actorPacketController;
    delete objectPacketController;
    delete worldstatePacketController;
}

void Networking::setServerPassword(std::string password) noexcept
{
    serverPassword = password.empty() ? TES3MP_DEFAULT_PASSW : password;
}

bool Networking::isPassworded() const
{
    return serverPassword != TES3MP_DEFAULT_PASSW;
}

void Networking::processSystemPacket(mwmp::ReceivedPacket &rp)
{
    Player *player = Players::getPlayer(rp.sender);

    SystemPacket *myPacket = systemPacketController->GetPacket(rp.packetId);

    if (rp.packetId == ID_SYSTEM_HANDSHAKE)
    {
        myPacket->setSystem(&baseSystem);
        myPacket->Read();

        if (!myPacket->isPacketValid())
        {
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Invalid handshake packet from client at %s", rp.senderAddress.c_str());
            kickPlayer(player->guid);
            return;
        }

        if (player->isHandshaked())
        {
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Wrong handshake with client at %s", rp.senderAddress.c_str());
            kickPlayer(player->guid);
            return;
        }

        if (baseSystem.serverPassword != serverPassword)
        {
            if (isPassworded())
            {
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Wrong server password %s used by client at %s",
                    baseSystem.serverPassword.c_str(), rp.senderAddress.c_str());
                kickPlayer(player->guid);
                return;
            }
            else
            {
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "Client at %s tried to join using password, despite the server not being passworded",
                    rp.senderAddress.c_str());
            }
        }
        player->setHandshake();
        return;
    }
}

void Networking::processPlayerPacket(mwmp::ReceivedPacket &rp)
{
    Player *player = Players::getPlayer(rp.sender);

    PlayerPacket *myPacket = playerPacketController->GetPacket(rp.packetId);

    if (!player->isHandshaked())
    {
        player->incrementHandshakeAttempts();
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Have not completed handshake with client at %s", rp.senderAddress.c_str());
        LOG_APPEND(TimedLog::LOG_WARN, "- Attempts so far: %i", player->getHandshakeAttempts());

        if (player->getHandshakeAttempts() > 20)
            kickPlayer(player->guid, false);
        else if (player->getHandshakeAttempts() > 5)
            kickPlayer(player->guid, true);

        return;
    }

    if (rp.packetId == ID_LOADED)
    {
        player->setLoadState(Player::LOADED);

        unsigned short slotId = Players::getPlayer(rp.sender)->getId();
        Script::Call("OnPlayerConnect", slotId);

        if (player->getLoadState() == Player::KICKED) // kicked inside in OnPlayerConnect
        {
            playerPacketController->GetPacket(ID_USER_DISCONNECTED)->setPlayer(Players::getPlayer(rp.sender));
            playerPacketController->GetPacket(ID_USER_DISCONNECTED)->Send(false);
            Players::deletePlayer(rp.sender);
            return;
        }
    }
    else if (rp.packetId == ID_PLAYER_BASEINFO)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "Received ID_PLAYER_BASEINFO about %s", player->npc.mName.c_str());

        myPacket->setPlayer(player);
        myPacket->Read();
        myPacket->Send(true);
    }

    if (player->getLoadState() == Player::NOTLOADED)
        return;
    else if (player->getLoadState() == Player::LOADED)
    {
        player->setLoadState(Player::POSTLOADED);
        newPlayer(rp.sender);
        return;
    }


    if (!PlayerProcessor::Process(rp))
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled PlayerPacket with identifier %i has arrived", rp.packetId);

}

void Networking::processActorPacket(mwmp::ReceivedPacket &rp)
{
    Player *player = Players::getPlayer(rp.sender);

    if (!player->isHandshaked() || player->getLoadState() != Player::POSTLOADED)
        return;

    if (!ActorProcessor::Process(rp, baseActorList))
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled ActorPacket with identifier %i has arrived", rp.packetId);

}

void Networking::processObjectPacket(mwmp::ReceivedPacket &rp)
{
    Player *player = Players::getPlayer(rp.sender);

    if (!player->isHandshaked() || player->getLoadState() != Player::POSTLOADED)
        return;

    if (!ObjectProcessor::Process(rp, baseObjectList))
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled ObjectPacket with identifier %i has arrived", rp.packetId);

}

void Networking::processWorldstatePacket(mwmp::ReceivedPacket &rp)
{
    Player *player = Players::getPlayer(rp.sender);

    if (!player->isHandshaked() || player->getLoadState() != Player::POSTLOADED)
        return;

    if (!WorldstateProcessor::Process(rp, baseWorldstate))
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled WorldstatePacket with identifier %i has arrived", rp.packetId);

}

bool Networking::preInit(mwmp::ReceivedPacket &rp, mwmp::PlayerId pid)
{
    if (rp.packetId != ID_GAME_PREINIT)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "%s sent wrong first packet (ID_GAME_PREINIT was expected)",
                           rp.senderAddress.c_str());
        rakNetManager->CloseConnection(pid, true);
    }

    LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "Received ID_GAME_PREINIT from %s", rp.senderAddress.c_str());
    PacketPreInit::PluginContainer dataFiles;

    PacketPreInit packetPreInit(rakNetManager);
    packetPreInit.SetReadStream(&rp.data);
    packetPreInit.setChecksums(&dataFiles);
    packetPreInit.Read();

    if (!packetPreInit.isPacketValid() || dataFiles.empty())
    {
        LOG_APPEND(TimedLog::LOG_ERROR, "- Packet was invalid");
        rakNetManager->CloseConnection(pid, false); // close connection without notification
        return false;
    }

    auto dataFile = dataFiles.begin();
    if (samples.size() == dataFiles.size())
    {
        for (int i = 0; dataFile != dataFiles.end(); dataFile++, i++)
        {
            LOG_APPEND(TimedLog::LOG_INFO, "- idx: %i\tchecksum: %X\tfile: %s", i, dataFile->second[0], dataFile->first.c_str());
            // Check if the filenames match, ignoring case
            if (Misc::StringUtils::ciEqual(samples[i].first, dataFile->first))
            {
                auto &hashList = samples[i].second;
                // Proceed if no checksums have been listed for this dataFile on the server
                if (hashList.empty())
                    continue;
                auto it = find(hashList.begin(), hashList.end(), dataFile->second[0]);
                // Break the loop if the client's checksum isn't among those accepted by
                // the server
                if (it == hashList.end())
                    break;
            }
            else // name is incorrect
                break;
        }
    }
    mwmp::NetBuffer bs;
    packetPreInit.SetSendStream(&bs);

    // If the loop above was broken, then the client's data files do not match the server's
    if (dataFileEnforcementState && dataFile != dataFiles.end())
    {
        LOG_APPEND(TimedLog::LOG_INFO, "- Client was not allowed to connect due to incompatible data files");
        packetPreInit.setChecksums(&samples);
        packetPreInit.Send(pid);
        rakNetManager->CloseConnection(pid, true);
    }
    else
    {
        LOG_APPEND(TimedLog::LOG_INFO, "- Client was allowed to connect");
        PacketPreInit::PluginContainer tmp;
        packetPreInit.setChecksums(&tmp);
        packetPreInit.Send(pid);
        Players::newPlayer(pid); // create player if connection allowed
        systemPacketController->SetStream(&rp.data, nullptr); // and request handshake
        systemPacketController->GetPacket(ID_SYSTEM_HANDSHAKE)->RequestData(pid);
        return true;
    }

    return false;
}

void Networking::update(mwmp::ReceivedPacket &rp)
{
    if (systemPacketController->ContainsPacket(rp.packetId))
    {
        systemPacketController->SetStream(&rp.data, nullptr);
        processSystemPacket(rp);
    }
    else if (playerPacketController->ContainsPacket(rp.packetId))
    {
        playerPacketController->SetStream(&rp.data, nullptr);
        processPlayerPacket(rp);
    }
    else if (actorPacketController->ContainsPacket(rp.packetId))
    {
        actorPacketController->SetStream(&rp.data, 0);
        processActorPacket(rp);
    }
    else if (objectPacketController->ContainsPacket(rp.packetId))
    {
        objectPacketController->SetStream(&rp.data, 0);
        processObjectPacket(rp);
    }
    else if (worldstatePacketController->ContainsPacket(rp.packetId))
    {
        worldstatePacketController->SetStream(&rp.data, 0);
        processWorldstatePacket(rp);
    }
    else
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled packet with identifier %i has arrived", rp.packetId);
}

void Networking::newPlayer(mwmp::PlayerId guid)
{
    playerPacketController->GetPacket(ID_PLAYER_BASEINFO)->RequestData(guid);
    playerPacketController->GetPacket(ID_PLAYER_STATS_DYNAMIC)->RequestData(guid);
    playerPacketController->GetPacket(ID_PLAYER_POSITION)->RequestData(guid);
    playerPacketController->GetPacket(ID_PLAYER_CELL_CHANGE)->RequestData(guid);
    playerPacketController->GetPacket(ID_PLAYER_EQUIPMENT)->RequestData(guid);

    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Sending info about other players to %lu", guid);

    for (TPlayers::iterator pl = players->begin(); pl != players->end(); pl++) //sending other players to new player
    {
        // If we are iterating over the new player, don't send the packets below
        if (pl->first == guid) continue;

        // If an invalid key makes it into the Players map, ignore it
        else if (pl->first == mwmp::INVALID_PLAYER_ID) continue;

        // if player not fully connected
        else if (pl->second == nullptr) continue;

        // If we are iterating over a player who has inputted their name, proceed
        else if (pl->second->getLoadState() == Player::POSTLOADED)
        {
            playerPacketController->GetPacket(ID_PLAYER_BASEINFO)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_STATS_DYNAMIC)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_ATTRIBUTE)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_SKILL)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_POSITION)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_CELL_CHANGE)->setPlayer(pl->second);
            playerPacketController->GetPacket(ID_PLAYER_EQUIPMENT)->setPlayer(pl->second);

            playerPacketController->GetPacket(ID_PLAYER_BASEINFO)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_STATS_DYNAMIC)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_ATTRIBUTE)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_SKILL)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_POSITION)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_CELL_CHANGE)->Send(guid);
            playerPacketController->GetPacket(ID_PLAYER_EQUIPMENT)->Send(guid);
        }
    }

    LOG_APPEND(TimedLog::LOG_WARN, "- Done");

}

void Networking::disconnectPlayer(mwmp::PlayerId guid)
{
    Player *player = Players::getPlayer(guid);
    if (!player)
        return;
    Script::Call("OnPlayerDisconnect", player->getId());

    playerPacketController->GetPacket(ID_USER_DISCONNECTED)->setPlayer(player);
    playerPacketController->GetPacket(ID_USER_DISCONNECTED)->Send(true);
    rakNetManager->UnregisterGuid(rakNetManager->ToGuid(guid));
    Players::deletePlayer(guid);
}

PlayerPacketController *Networking::getPlayerPacketController() const
{
    return playerPacketController;
}

ActorPacketController *Networking::getActorPacketController() const
{
    return actorPacketController;
}

ObjectPacketController *Networking::getObjectPacketController() const
{
    return objectPacketController;
}

WorldstatePacketController *Networking::getWorldstatePacketController() const
{
    return worldstatePacketController;
}

BaseActorList *Networking::getReceivedActorList()
{
    return &baseActorList;
}

BaseObjectList *Networking::getReceivedObjectList()
{
    return &baseObjectList;
}

BaseWorldstate *Networking::getReceivedWorldstate()
{
    return &baseWorldstate;
}

int Networking::getCurrentMpNum()
{
    return currentMpNum;
}

void Networking::setCurrentMpNum(int value)
{
    currentMpNum = value;
}

int Networking::incrementMpNum()
{
    currentMpNum++;
    Script::Call("OnMpNumIncrement", currentMpNum);
    return currentMpNum;
}

bool Networking::getDataFileEnforcementState()
{
    return dataFileEnforcementState;
}

void Networking::setDataFileEnforcementState(bool state)
{
    dataFileEnforcementState = state;
}

bool Networking::getScriptErrorIgnoringState()
{
    return scriptErrorIgnoringState;
}

void Networking::setScriptErrorIgnoringState(bool state)
{
    scriptErrorIgnoringState = state;
}

const Networking &Networking::get()
{
    return *sThis;
}


Networking *Networking::getPtr()
{
    return sThis;
}

std::string Networking::getSystemAddress(mwmp::PlayerId guid)
{
    return rakNetManager->GetAddress(guid);
}

mwmp::RakNetManager *Networking::getRakNetManager() const
{
    return rakNetManager;
}

void Networking::stopServer(int code)
{
    running = false;
    exitCode = code;
}

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    //15 is SIGTERM(Normal OS stop call), 2 is SIGINT(Ctrl+C)
    if(signum == 15 || signum == 2)
    {
        killLoop = true;
    }
}

int Networking::mainLoop()
{
    RakNet::Packet *packet;

#ifndef _WIN32
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
#endif

    while (running && !killLoop)
    {
#ifndef _WIN32
        sigaction(SIGTERM, &sigIntHandler, NULL);
        sigaction(SIGINT, &sigIntHandler, NULL);
#endif
        if (kbhit() && getch() == '\n')
            break;
        for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
        {
            if (getMasterClient()->Process(packet))
                continue;

            switch (packet->data[0])
            {
                case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Client at %s has disconnected", packet->systemAddress.ToString());
                    break;
                case ID_REMOTE_CONNECTION_LOST:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Client at %s has lost connection", packet->systemAddress.ToString());
                    break;
                case ID_REMOTE_NEW_INCOMING_CONNECTION:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Client at %s has connected", packet->systemAddress.ToString());
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:    // client to server
                {
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Our connection request has been accepted");
                    break;
                }
                case ID_NEW_INCOMING_CONNECTION:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "A connection is incoming from %s", packet->systemAddress.ToString());
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "The server is full");
                    break;
                case ID_DISCONNECTION_NOTIFICATION:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN,  "Client at %s has disconnected", packet->systemAddress.ToString());
                    disconnectPlayer(rakNetManager->ToPlayerId(packet->guid));
                    break;
                case ID_CONNECTION_LOST:
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Client at %s has lost connection", packet->systemAddress.ToString());
                    disconnectPlayer(rakNetManager->ToPlayerId(packet->guid));
                    break;
                case ID_SND_RECEIPT_ACKED:
                case ID_CONNECTED_PING:
                case ID_UNCONNECTED_PING:
                    break;
                default:
                {
                    // Build ReceivedPacket: payload is header-stripped (skip packetID + PlayerId)
                    const size_t hdrLen = 1 + sizeof(mwmp::PlayerId);
                    mwmp::ReceivedPacket rp;
                    rp.packetId = packet->data[0];
                    rp.data = mwmp::NetBuffer(
                        packet->data + hdrLen,
                        packet->length > hdrLen ? packet->length - hdrLen : 0);

                    mwmp::PlayerId pid = rakNetManager->ToPlayerId(packet->guid);

                    if (Players::doesPlayerExist(pid))
                    {
                        rp.sender = pid;
                        rp.senderAddress = rakNetManager->GetAddress(pid);
                        update(rp);
                    }
                    else
                    {
                        // First contact from this GUID — register it to get a PlayerId
                        pid = rakNetManager->RegisterGuid(packet->guid);
                        rp.sender = pid;
                        rp.senderAddress = rakNetManager->GetAddress(pid);
                        preInit(rp, pid);
                    }
                    break;
                }
            }
        }
        TimerAPI::Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    TimerAPI::Terminate();
    return exitCode;
}

void Networking::kickPlayer(mwmp::PlayerId guid, bool sendNotification)
{
    RakNet::RakNetGUID rakGuid = rakNetManager->ToGuid(guid);
    peer->CloseConnection(rakGuid, sendNotification);
}

void Networking::banAddress(const char *ipAddress)
{
    peer->AddToBanList(ipAddress);
}

void Networking::unbanAddress(const char *ipAddress)
{
    peer->RemoveFromBanList(ipAddress);
}

unsigned short Networking::numberOfConnections() const
{
    return peer->NumberOfConnections();
}

unsigned int Networking::maxConnections() const
{
    return peer->GetMaximumIncomingConnections();
}

int Networking::getAvgPing(mwmp::PlayerId pid) const
{
    RakNet::RakNetGUID rakGuid = rakNetManager->ToGuid(pid);
    return peer->GetAveragePing(rakGuid);
}

unsigned short Networking::getPort() const
{
    return peer->GetMyBoundAddress().GetPort();
}

MasterClient *Networking::getMasterClient()
{
    return mclient;
}

void Networking::InitQuery(std::string queryAddr, unsigned short queryPort)
{
    mclient = new MasterClient(rakNetManager, queryAddr, queryPort);
}

void Networking::postInit()
{
    Script::Call("OnRequestDataFileList");
    Script::Call("OnServerPostInit");
}

PacketPreInit::PluginContainer &Networking::getSamples()
{
    return samples;
}
