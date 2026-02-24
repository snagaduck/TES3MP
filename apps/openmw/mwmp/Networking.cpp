#include <stdexcept>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <components/openmw-mp/TimedLog.hpp>
#include <components/openmw-mp/Utils.hpp>
#include <components/openmw-mp/Version.hpp>
#include <components/openmw-mp/Packets/PacketPreInit.hpp>

#include <components/esm/cellid.hpp>
#include <components/files/configurationmanager.hpp>

#include "../mwbase/environment.hpp"
#include "../mwbase/world.hpp"

#include "../mwclass/npc.hpp"

#include "../mwmechanics/combat.hpp"
#include "../mwmechanics/npcstats.hpp"

#include "../mwstate/statemanagerimp.hpp"

#include "../mwworld/cellstore.hpp"
#include "../mwworld/esmstore.hpp"
#include "../mwworld/inventorystore.hpp"

#include <SDL_messagebox.h>
#include <iomanip>
#include <components/version/version.hpp>

#include "Networking.hpp"
#include "Main.hpp"
#include "processors/ProcessorInitializer.hpp"
#include "processors/SystemProcessor.hpp"
#include "processors/PlayerProcessor.hpp"
#include "processors/ObjectProcessor.hpp"
#include "processors/ActorProcessor.hpp"
#include "processors/WorldstateProcessor.hpp"
#include "GUIController.hpp"
#include "CellController.hpp"

using namespace mwmp;

std::string listDiscrepancies(PacketPreInit::PluginContainer checksums, PacketPreInit::PluginContainer checksumsResponse)
{
    std::ostringstream sstr;
    sstr << "Your plugins or their load order don't match the server's. A full comparison is included in your debug window and latest log file. In short, the following discrepancies have been found:\n\n";

    int discrepancyCount = 0;

    for (size_t fileIndex = 0; fileIndex < checksums.size() || fileIndex < checksumsResponse.size(); fileIndex++)
    {
        if (fileIndex >= checksumsResponse.size())
        {
            discrepancyCount++;

            if (discrepancyCount > 1)
                sstr << "\n";

            std::string clientFilename = checksums.at(fileIndex).first;

            sstr << fileIndex << ": ";
            sstr << clientFilename << " is past the number of plugins used by the server";
        }
        else if (fileIndex >= checksums.size())
        {
            discrepancyCount++;

            if (discrepancyCount > 1)
                sstr << "\n";

            std::string serverFilename = checksumsResponse.at(fileIndex).first;

            sstr << fileIndex << ": ";
            sstr << serverFilename << " is completely missing from the client but required by the server";
        }
        else
        {
            std::string clientFilename = checksums.at(fileIndex).first;
            std::string serverFilename = checksumsResponse.at(fileIndex).first;

            std::string clientChecksum = Utils::intToHexStr(checksums.at(fileIndex).second.at(0));

            bool filenameMatches = false;
            bool checksumMatches = false;
            std::string eligibleChecksums = "";

            if (Misc::StringUtils::ciEqual(clientFilename, serverFilename))
                filenameMatches = true;

            if (checksumsResponse.at(fileIndex).second.size() > 0)
            {
                for (size_t checksumIndex = 0; checksumIndex < checksumsResponse.at(fileIndex).second.size(); checksumIndex++)
                {
                    std::string serverChecksum = Utils::intToHexStr(checksumsResponse.at(fileIndex).second.at(checksumIndex));

                    if (checksumIndex != 0)
                        eligibleChecksums = eligibleChecksums + " or ";

                    eligibleChecksums = eligibleChecksums + serverChecksum;

                    if (Misc::StringUtils::ciEqual(clientChecksum, serverChecksum))
                    {
                        checksumMatches = true;
                        break;
                    }
                }
            }
            else
                checksumMatches = true;

            if (!filenameMatches || !checksumMatches)
            {
                discrepancyCount++;

                if (discrepancyCount > 1)
                    sstr << "\n";

                sstr << fileIndex << ": ";

                if (!filenameMatches)
                    sstr << clientFilename << " doesn't match " << serverFilename;

                if (!filenameMatches && !checksumMatches)
                    sstr << ", ";

                if (!checksumMatches)
                    sstr << "checksum " << clientChecksum << " doesn't match " << eligibleChecksums;
            }
        }
    }

    return sstr.str();
}

std::string listComparison(PacketPreInit::PluginContainer checksums, PacketPreInit::PluginContainer checksumsResponse,
                      bool full = false)
{
    std::ostringstream sstr;
    size_t pluginNameLen1 = 0;
    size_t pluginNameLen2 = 0;
    for (const auto &checksum : checksums)
        if (pluginNameLen1 < checksum.first.size())
            pluginNameLen1 = checksum.first.size();

    for (const auto &checksum : checksums)
        if (pluginNameLen2 < checksum.first.size())
            pluginNameLen2 = checksum.first.size();

    Utils::printWithWidth(sstr, "Your current plugins are:", pluginNameLen1 + 16);
    sstr << "To join this server, use:\n";

    Utils::printWithWidth(sstr, "name", pluginNameLen1 + 2);
    Utils::printWithWidth(sstr, "hash", 14);
    Utils::printWithWidth(sstr, "name", pluginNameLen2 + 2);
    sstr << "hash\n";

    for (size_t i = 0; i < checksums.size() || i < checksumsResponse.size(); i++)
    {
        std::string plugin;
        unsigned val;

        if (i < checksums.size())
        {
            plugin = checksums.at(i).first;
            val = checksums.at(i).second[0];

            Utils::printWithWidth(sstr, plugin, pluginNameLen1 + 2);
            Utils::printWithWidth(sstr, Utils::intToHexStr(val), 14);
        }
        else
            Utils::printWithWidth(sstr, "", pluginNameLen1 + 16);

        if (i < checksumsResponse.size())
        {
            Utils::printWithWidth(sstr, checksumsResponse[i].first, pluginNameLen2 + 2);
            if (checksumsResponse[i].second.size() > 0)
            {
                if (full)
                    for (size_t j = 0; j < checksumsResponse[i].second.size(); j++)
                        Utils::printWithWidth(sstr, Utils::intToHexStr(checksumsResponse[i].second[j]), 14);
                else
                    sstr << Utils::intToHexStr(checksumsResponse[i].second[0]);
            }
            else
                sstr << "any";
        }

        sstr << "\n";
    }

    return sstr.str();
}

Networking::Networking():
    systemPacketController(&gnsManager),
    playerPacketController(&gnsManager), actorPacketController(&gnsManager),
    objectPacketController(&gnsManager), worldstatePacketController(&gnsManager)
{
    systemPacketController.SetStream(0, &bsOut);
    playerPacketController.SetStream(0, &bsOut);
    actorPacketController.SetStream(0, &bsOut);
    objectPacketController.SetStream(0, &bsOut);
    worldstatePacketController.SetStream(0, &bsOut);

    serverPlayerId = gnsManager.getServerPlayerId();
    connected = false;
    ProcessorInitializer();
}

Networking::~Networking()
{
    gnsManager.Disconnect();
}

void Networking::update()
{
    // Check for disconnect events
    for (auto& ev : gnsManager.PollEvents())
    {
        if (ev.type == GNSNetworkManager::EventType::Disconnected
            || ev.type == GNSNetworkManager::EventType::Failed)
        {
            const char* errmsg = "Connection lost.";
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, errmsg);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "tes3mp", errmsg, 0);
            MWBase::Environment::get().getStateManager()->requestQuit();
            return;
        }
    }

    for (auto& rp : gnsManager.PollMessages())
        receiveMessage(rp);
}

void Networking::connect(const std::string &ip, unsigned short port, std::vector<std::string> &content, Files::Collections &collections)
{
    if (!gnsManager.Connect(ip, port))
    {
        std::string errmsg = "Connection failed.\n"
            "Either the IP address is wrong or a firewall is blocking UDP on the chosen port.";
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, errmsg.c_str());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "tes3mp", errmsg.c_str(), 0);
        return;
    }

    serverPlayerId = gnsManager.getServerPlayerId();
    BaseClientPacketProcessor::SetServerPlayerId(serverPlayerId);
    connected = true;

    LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "GNS client: connected to %s:%d", ip.c_str(), port);

    preInit(content, collections);

    // Assign client's local guid — use a stable hash of the server address as a
    // stand-in since GNS doesn't expose a self-GUID.
    getLocalPlayer()->guid = getLocalSystem()->guid =
        static_cast<mwmp::PlayerId>(std::hash<std::string>{}(ip + ":" + std::to_string(port)));
}

void Networking::preInit(std::vector<std::string> &content, Files::Collections &collections)
{
    PacketPreInit::PluginContainer checksums;
    std::vector<std::string>::const_iterator it(content.begin());
    for (int idx = 0; it != content.end(); ++it, ++idx)
    {
        boost::filesystem::path filename(*it);
        const Files::MultiDirCollection& col = collections.getCollection(filename.extension().string());
        if (col.doesExist(*it))
        {
            PacketPreInit::HashList hashList;
            unsigned crc32 = Utils::crc32Checksum(col.getPath(*it).string());
            hashList.push_back(crc32);
            checksums.push_back(make_pair(*it, hashList));

            LOG_APPEND(TimedLog::LOG_WARN, "idx: %d\tchecksum: %X\tfile: %s\n", idx, crc32, col.getPath(*it).string().c_str());
        }
        else
            throw std::runtime_error("Plugin doesn't exist.");
    }

    PacketPreInit packetPreInit(&gnsManager);
    mwmp::NetBuffer bs;
    packetPreInit.setChecksums(&checksums);
    packetPreInit.setGUID(mwmp::INVALID_PLAYER_ID);
    packetPreInit.SetSendStream(&bs);
    packetPreInit.Send(serverPlayerId);

    PacketPreInit::PluginContainer checksumsResponse;
    bool done = false;
    while (!done)
    {
        auto messages = gnsManager.PollMessages();
        if (messages.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        for (auto& rp : messages)
        {
            if (rp.packetId == ID_GAME_PREINIT)
            {
                mwmp::NetBuffer bsIn = rp.data;
                packetPreInit.setChecksums(&checksumsResponse);
                packetPreInit.Packet(&bsIn, false);
                done = true;
                break;
            }
        }
    }

    if (!checksumsResponse.empty()) // something wrong
    {
        std::string errmsg = listDiscrepancies(checksums, checksumsResponse);

        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, listDiscrepancies(checksums, checksumsResponse).c_str());
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, listComparison(checksums, checksumsResponse, true).c_str());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "tes3mp", errmsg.c_str(), 0);
        connected = false;
    }
}

void Networking::receiveMessage(mwmp::ReceivedPacket &rp)
{
    if (systemPacketController.ContainsPacket(rp.packetId))
    {
        if (!SystemProcessor::Process(rp))
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled SystemPacket with identifier %i has arrived", rp.packetId);
    }
    else if (playerPacketController.ContainsPacket(rp.packetId))
    {
        if (!PlayerProcessor::Process(rp))
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled PlayerPacket with identifier %i has arrived", rp.packetId);
    }
    else if (actorPacketController.ContainsPacket(rp.packetId))
    {
        if (!ActorProcessor::Process(rp, actorList))
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled ActorPacket with identifier %i has arrived", rp.packetId);
    }
    else if (objectPacketController.ContainsPacket(rp.packetId))
    {
        if (!ObjectProcessor::Process(rp, objectList))
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled ObjectPacket with identifier %i has arrived", rp.packetId);
    }
    else if (worldstatePacketController.ContainsPacket(rp.packetId))
    {
        if (!WorldstateProcessor::Process(rp, worldstate))
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "Unhandled WorldstatePacket with identifier %i has arrived", rp.packetId);
    }
}

SystemPacket *Networking::getSystemPacket(unsigned char id)
{
    return systemPacketController.GetPacket(id);
}

PlayerPacket *Networking::getPlayerPacket(unsigned char id)
{
    return playerPacketController.GetPacket(id);
}

ActorPacket *Networking::getActorPacket(unsigned char id)
{
    return actorPacketController.GetPacket(id);
}

ObjectPacket *Networking::getObjectPacket(unsigned char id)
{
    return objectPacketController.GetPacket(id);
}

WorldstatePacket *Networking::getWorldstatePacket(unsigned char id)
{
    return worldstatePacketController.GetPacket(id);
}

LocalSystem *Networking::getLocalSystem()
{
    return mwmp::Main::get().getLocalSystem();
}

LocalPlayer *Networking::getLocalPlayer()
{
    return mwmp::Main::get().getLocalPlayer();
}

ActorList *Networking::getActorList()
{
    return &actorList;
}

ObjectList *Networking::getObjectList()
{
    return &objectList;
}

Worldstate *Networking::getWorldstate()
{
    return &worldstate;
}

bool Networking::isConnected()
{
    return connected;
}
