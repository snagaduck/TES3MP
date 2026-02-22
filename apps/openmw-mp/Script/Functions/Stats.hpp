#ifndef OPENMW_STATAPI_HPP
#define OPENMW_STATAPI_HPP

#define STATAPI(t) \
    t.set_function("GetAttributeCount", &StatsFunctions::GetAttributeCount);\
    t.set_function("GetSkillCount", &StatsFunctions::GetSkillCount);\
    t.set_function("GetAttributeId", &StatsFunctions::GetAttributeId);\
    t.set_function("GetSkillId", &StatsFunctions::GetSkillId);\
    t.set_function("GetAttributeName", &StatsFunctions::GetAttributeName);\
    t.set_function("GetSkillName", &StatsFunctions::GetSkillName);\
    \
    t.set_function("GetName", &StatsFunctions::GetName);\
    t.set_function("GetRace", &StatsFunctions::GetRace);\
    t.set_function("GetHead", &StatsFunctions::GetHead);\
    t.set_function("GetHair", &StatsFunctions::GetHairstyle);\
    t.set_function("GetIsMale", &StatsFunctions::GetIsMale);\
    t.set_function("GetModel", &StatsFunctions::GetModel);\
    t.set_function("GetBirthsign", &StatsFunctions::GetBirthsign);\
    \
    t.set_function("GetLevel", &StatsFunctions::GetLevel);\
    t.set_function("GetLevelProgress", &StatsFunctions::GetLevelProgress);\
    \
    t.set_function("GetHealthBase", &StatsFunctions::GetHealthBase);\
    t.set_function("GetHealthCurrent", &StatsFunctions::GetHealthCurrent);\
    \
    t.set_function("GetMagickaBase", &StatsFunctions::GetMagickaBase);\
    t.set_function("GetMagickaCurrent", &StatsFunctions::GetMagickaCurrent);\
    \
    t.set_function("GetFatigueBase", &StatsFunctions::GetFatigueBase);\
    t.set_function("GetFatigueCurrent", &StatsFunctions::GetFatigueCurrent);\
    \
    t.set_function("GetAttributeBase", &StatsFunctions::GetAttributeBase);\
    t.set_function("GetAttributeModifier", &StatsFunctions::GetAttributeModifier);\
    t.set_function("GetAttributeDamage", &StatsFunctions::GetAttributeDamage);\
    \
    t.set_function("GetSkillBase", &StatsFunctions::GetSkillBase);\
    t.set_function("GetSkillModifier", &StatsFunctions::GetSkillModifier);\
    t.set_function("GetSkillDamage", &StatsFunctions::GetSkillDamage);\
    t.set_function("GetSkillProgress", &StatsFunctions::GetSkillProgress);\
    t.set_function("GetSkillIncrease", &StatsFunctions::GetSkillIncrease);\
    \
    t.set_function("GetBounty", &StatsFunctions::GetBounty);\
    \
    t.set_function("SetName", &StatsFunctions::SetName);\
    t.set_function("SetRace", &StatsFunctions::SetRace);\
    t.set_function("SetHead", &StatsFunctions::SetHead);\
    t.set_function("SetHair", &StatsFunctions::SetHairstyle);\
    t.set_function("SetIsMale", &StatsFunctions::SetIsMale);\
    t.set_function("SetModel", &StatsFunctions::SetModel);\
    t.set_function("SetBirthsign", &StatsFunctions::SetBirthsign);\
    t.set_function("SetResetStats", &StatsFunctions::SetResetStats);\
    \
    t.set_function("SetLevel", &StatsFunctions::SetLevel);\
    t.set_function("SetLevelProgress", &StatsFunctions::SetLevelProgress);\
    \
    t.set_function("SetHealthBase", &StatsFunctions::SetHealthBase);\
    t.set_function("SetHealthCurrent", &StatsFunctions::SetHealthCurrent);\
    t.set_function("SetMagickaBase", &StatsFunctions::SetMagickaBase);\
    t.set_function("SetMagickaCurrent", &StatsFunctions::SetMagickaCurrent);\
    t.set_function("SetFatigueBase", &StatsFunctions::SetFatigueBase);\
    t.set_function("SetFatigueCurrent", &StatsFunctions::SetFatigueCurrent);\
    \
    t.set_function("SetAttributeBase", &StatsFunctions::SetAttributeBase);\
    t.set_function("ClearAttributeModifier", &StatsFunctions::ClearAttributeModifier);\
    t.set_function("SetAttributeDamage", &StatsFunctions::SetAttributeDamage);\
    \
    t.set_function("SetSkillBase", &StatsFunctions::SetSkillBase);\
    t.set_function("ClearSkillModifier", &StatsFunctions::ClearSkillModifier);\
    t.set_function("SetSkillDamage", &StatsFunctions::SetSkillDamage);\
    t.set_function("SetSkillProgress", &StatsFunctions::SetSkillProgress);\
    t.set_function("SetSkillIncrease", &StatsFunctions::SetSkillIncrease);\
    \
    t.set_function("SetBounty", &StatsFunctions::SetBounty);\
    t.set_function("SetCharGenStage", &StatsFunctions::SetCharGenStage);\
    \
    t.set_function("SendBaseInfo", &StatsFunctions::SendBaseInfo);\
    \
    t.set_function("SendStatsDynamic", &StatsFunctions::SendStatsDynamic);\
    t.set_function("SendAttributes", &StatsFunctions::SendAttributes);\
    t.set_function("SendSkills", &StatsFunctions::SendSkills);\
    t.set_function("SendLevel", &StatsFunctions::SendLevel);\
    t.set_function("SendBounty", &StatsFunctions::SendBounty);

class StatsFunctions
{
public:

    /**
    * \brief Get the number of attributes.
    *
    * The number is 8 before any dehardcoding is done in OpenMW.
    *
    * \return The number of attributes.
    */
    static int GetAttributeCount() noexcept;

    /**
    * \brief Get the number of skills.
    *
    * The number is 27 before any dehardcoding is done in OpenMW.
    *
    * \return The number of skills.
    */
    static int GetSkillCount() noexcept;

    /**
    * \brief Get the numerical ID of an attribute with a certain name.
    *
    * If an invalid name is used, the ID returned is -1
    *
    * \param name The name of the attribute.
    * \return The ID of the attribute.
    */
    static int GetAttributeId(const char *name) noexcept;

    /**
    * \brief Get the numerical ID of a skill with a certain name.
    *
    * If an invalid name is used, the ID returned is -1
    *
    * \param name The name of the skill.
    * \return The ID of the skill.
    */
    static int GetSkillId(const char *name) noexcept;

    /**
    * \brief Get the name of the attribute with a certain numerical ID.
    *
    * If an invalid ID is used, "invalid" is returned.
    *
    * \param attributeId The ID of the attribute.
    * \return The name of the attribute.
    */
    static const char *GetAttributeName(unsigned short attributeId) noexcept;

    /**
    * \brief Get the name of the skill with a certain numerical ID.
    *
    * If an invalid ID is used, "invalid" is returned.
    *
    * \param skillId The ID of the skill.
    * \return The name of the skill.
    */
    static const char *GetSkillName(unsigned short skillId) noexcept;

    /**
    * \brief Get the name of a player.
    *
    * \param pid The player ID.
    * \return The name of the player.
    */
    static const char *GetName(unsigned short pid) noexcept;

    /**
    * \brief Get the race of a player.
    *
    * \param pid The player ID.
    * \return The race of the player.
    */
    static const char *GetRace(unsigned short pid) noexcept;

    /**
    * \brief Get the head mesh used by a player.
    *
    * \param pid The player ID.
    * \return The head mesh of the player.
    */
    static const char *GetHead(unsigned short pid) noexcept;

    /**
    * \brief Get the hairstyle mesh used by a player.
    *
    * \param pid The player ID.
    * \return The hairstyle mesh of the player.
    */
    static const char *GetHairstyle(unsigned short pid) noexcept;

    /**
    * \brief Check whether a player is male or not.
    *
    * \param pid The player ID.
    * \return Whether the player is male.
    */
    static int GetIsMale(unsigned short pid) noexcept;

    /**
    * \brief Get the model of a player.
    *
    * \param pid The player ID.
    * \return The model of the player.
    */
    static const char* GetModel(unsigned short pid) noexcept;

    /**
    * \brief Get the birthsign of a player.
    *
    * \param pid The player ID.
    * \return The birthsign of the player.
    */
    static const char *GetBirthsign(unsigned short pid) noexcept;

    /**
    * \brief Get the character level of a player.
    *
    * \param pid The player ID.
    * \return The level of the player.
    */
    static int GetLevel(unsigned short pid) noexcept;

    /**
    * \brief Get the player's progress to their next character level.
    *
    * \param pid The player ID.
    * \return The level progress.
    */
    static int GetLevelProgress(unsigned short pid) noexcept;

    /**
    * \brief Get the base health of the player.
    *
    * \param pid The player ID.
    * \return The base health.
    */
    static double GetHealthBase(unsigned short pid) noexcept;

    /**
    * \brief Get the current health of the player.
    *
    * \param pid The player ID.
    * \return The current health.
    */
    static double GetHealthCurrent(unsigned short pid) noexcept;

    /**
    * \brief Get the base magicka of the player.
    *
    * \param pid The player ID.
    * \return The base magicka.
    */
    static double GetMagickaBase(unsigned short pid) noexcept;

    /**
    * \brief Get the current magicka of the player.
    *
    * \param pid The player ID.
    * \return The current magicka.
    */
    static double GetMagickaCurrent(unsigned short pid) noexcept;

    /**
    * \brief Get the base fatigue of the player.
    *
    * \param pid The player ID.
    * \return The base fatigue.
    */
    static double GetFatigueBase(unsigned short pid) noexcept;

    /**
    * \brief Get the current fatigue of the player.
    *
    * \param pid The player ID.
    * \return The current fatigue.
    */
    static double GetFatigueCurrent(unsigned short pid) noexcept;

    /**
    * \brief Get the base value of a player's attribute.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \return The base value of the attribute.
    */
    static int GetAttributeBase(unsigned short pid, unsigned short attributeId) noexcept;

    /**
    * \brief Get the modifier value of a player's attribute.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \return The modifier value of the attribute.
    */
    static int GetAttributeModifier(unsigned short pid, unsigned short attributeId) noexcept;

    /**
    * \brief Get the amount of damage (as caused through the Damage Attribute effect)
    *        to a player's attribute.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \return The amount of damage to the attribute.
    */
    static double GetAttributeDamage(unsigned short pid, unsigned short attributeId) noexcept;

    /**
    * \brief Get the base value of a player's skill.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \return The base value of the skill.
    */
    static int GetSkillBase(unsigned short pid, unsigned short skillId) noexcept;

    /**
    * \brief Get the modifier value of a player's skill.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \return The modifier value of the skill.
    */
    static int GetSkillModifier(unsigned short pid, unsigned short skillId) noexcept;

    /**
    * \brief Get the amount of damage (as caused through the Damage Skill effect)
    *        to a player's skill.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \return The amount of damage to the skill.
    */
    static double GetSkillDamage(unsigned short pid, unsigned short skillId) noexcept;

    /**
    * \brief Get the progress the player has made towards increasing a certain skill by 1.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \return The skill progress.
    */
    static double GetSkillProgress(unsigned short pid, unsigned short skillId) noexcept;

    /**
    * \brief Get the bonus applied to a certain attribute at the next level up as a result
    *        of associated skill increases.
    *
    * Although confusing, the term "skill increase" for this is taken from OpenMW itself.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \return The increase in the attribute caused by skills.
    */
    static int GetSkillIncrease(unsigned short pid, unsigned int attributeId) noexcept;

    /**
    * \brief Get the bounty of the player.
    *
    * \param pid The player ID.
    * \return The bounty.
    */
    static int GetBounty(unsigned short pid) noexcept;

    /**
    * \brief Set the name of a player.
    *
    * \param pid The player ID.
    * \param name The new name of the player.
    * \return void
    */
    static void SetName(unsigned short pid, const char *name) noexcept;

    /**
    * \brief Set the race of a player.
    *
    * \param pid The player ID.
    * \param race The new race of the player.
    * \return void
    */
    static void SetRace(unsigned short pid, const char *race) noexcept;

    /**
    * \brief Set the head mesh used by a player.
    *
    * \param pid The player ID.
    * \param head The new head mesh of the player.
    * \return void
    */
    static void SetHead(unsigned short pid, const char *head) noexcept;

    /**
    * \brief Set the hairstyle mesh used by a player.
    *
    * \param pid The player ID.
    * \param hairstyle The new hairstyle mesh of the player.
    * \return void
    */
    static void SetHairstyle(unsigned short pid, const char *hairstyle) noexcept;

    /**
    * \brief Set whether a player is male or not.
    *
    * \param pid The player ID.
    * \param state Whether the player is male.
    * \return void
    */
    static void SetIsMale(unsigned short pid, int state) noexcept;

    /**
    * \brief Set the model of a player.
    *
    * \param pid The player ID.
    * \param model The new model of the player.
    * \return void
    */
    static void SetModel(unsigned short pid, const char *model) noexcept;

    /**
    * \brief Set the birthsign of a player.
    *
    * \param pid The player ID.
    * \param name The new birthsign of the player.
    * \return void
    */
    static void SetBirthsign(unsigned short pid, const char *name) noexcept;

    /**
    * \brief Set whether the player's stats should be reset based on their
    *        current race as the result of a PlayerBaseInfo packet.
    *
    * This changes the resetState for that player in the server memory, but does not by itself
    * send a packet.
    *
    * \param pid The player ID.
    * \param resetStats The stat reset state.
    * \return void
    */
    static void SetResetStats(unsigned short pid, bool resetStats) noexcept;
    
    /**
    * \brief Set the character level of a player.
    *
    * \param pid The player ID.
    * \param value The new level of the player.
    * \return void
    */
    static void SetLevel(unsigned short pid, int value) noexcept;

    /**
    * \brief Set the player's progress to their next character level.
    *
    * \param pid The player ID.
    * \param value The new level progress of the player.
    * \return void
    */
    static void SetLevelProgress(unsigned short pid, int value) noexcept;

    /**
    * \brief Set the base health of a player.
    *
    * \param pid The player ID.
    * \param value The new base health of the player.
    * \return void
    */
    static void SetHealthBase(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the current health of a player.
    *
    * \param pid The player ID.
    * \param value The new current health of the player.
    * \return void
    */
    static void SetHealthCurrent(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the base magicka of a player.
    *
    * \param pid The player ID.
    * \param value The new base magicka of the player.
    * \return void
    */
    static void SetMagickaBase(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the current magicka of a player.
    *
    * \param pid The player ID.
    * \param value The new current magicka of the player.
    * \return void
    */
    static void SetMagickaCurrent(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the base fatigue of a player.
    *
    * \param pid The player ID.
    * \param value The new base fatigue of the player.
    * \return void
    */
    static void SetFatigueBase(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the current fatigue of a player.
    *
    * \param pid The player ID.
    * \param value The new current fatigue of the player.
    * \return void
    */
    static void SetFatigueCurrent(unsigned short pid, double value) noexcept;

    /**
    * \brief Set the base value of a player's attribute.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \param value The new base value of the player's attribute.
    * \return void
    */
    static void SetAttributeBase(unsigned short pid, unsigned short attributeId, int value) noexcept;

    /**
    * \brief Clear the modifier value of a player's attribute.
    *
    * There's no way to set a modifier to a specific value because it can come from
    * multiple different sources, but clearing it is a straightforward process that
    * dispels associated effects on a client and, if necessary, unequips associated
    * items.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \return void
    */
    static void ClearAttributeModifier(unsigned short pid, unsigned short attributeId) noexcept;

    /**
    * \brief Set the amount of damage (as caused through the Damage Attribute effect) to
    *        a player's attribute.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \param value The amount of damage to the player's attribute.
    * \return void
    */
    static void SetAttributeDamage(unsigned short pid, unsigned short attributeId, double value) noexcept;

    /**
    * \brief Set the base value of a player's skill.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \param value The new base value of the player's skill.
    * \return void
    */
    static void SetSkillBase(unsigned short pid, unsigned short skillId, int value) noexcept;    

    /**
    * \brief Clear the modifier value of a player's skill.
    *
    * There's no way to set a modifier to a specific value because it can come from
    * multiple different sources, but clearing it is a straightforward process that
    * dispels associated effects on a client and, if necessary, unequips associated
    * items.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \return void
    */
    static void ClearSkillModifier(unsigned short pid, unsigned short skillId) noexcept;

    /**
    * \brief Set the amount of damage (as caused through the Damage Skill effect) to
    *        a player's skill.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \param value The amount of damage to the player's skill.
    * \return void
    */
    static void SetSkillDamage(unsigned short pid, unsigned short skillId, double value) noexcept;

    /**
    * \brief Set the progress the player has made towards increasing a certain skill by 1.
    *
    * \param pid The player ID.
    * \param skillId The skill ID.
    * \param value The progress value.
    * \return void
    */
    static void SetSkillProgress(unsigned short pid, unsigned short skillId, double value) noexcept;

    /**
    * \brief Set the bonus applied to a certain attribute at the next level up as a result
    *        of associated skill increases.
    *
    * Although confusing, the term "skill increase" for this is taken from OpenMW itself.
    *
    * \param pid The player ID.
    * \param attributeId The attribute ID.
    * \param value The increase in the attribute caused by skills.
    * \return void
    */
    static void SetSkillIncrease(unsigned short pid, unsigned int attributeId, int value) noexcept;

    /**
    * \brief Set the bounty of a player.
    *
    * \param pid The player ID.
    * \param value The new bounty.
    * \return void
    */
    static void SetBounty(unsigned short pid, int value) noexcept;

    /**
    * \brief Set the current and ending stages of character generation for a player.
    *
    * This is used to repeat part of character generation or to only go through part of it.
    *
    * \param pid The player ID.
    * \param currentStage The new current stage.
    * \param endStage The new ending stage.
    * \return void
    */
    static void SetCharGenStage(unsigned short pid, int currentStage, int endStage) noexcept;

    /**
    * \brief Send a PlayerBaseInfo packet with a player's name, race, head mesh,
    *        hairstyle mesh, birthsign and stat reset state.
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendBaseInfo(unsigned short pid) noexcept;

    /**
    * \brief Send a PlayerStatsDynamic packet with a player's dynamic stats (health,
    *        magicka and fatigue).
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendStatsDynamic(unsigned short pid) noexcept;

    /**
    * \brief Send a PlayerAttribute packet with a player's attributes and bonuses
    *        to those attributes at the next level up (the latter being called
    *        "skill increases" as in OpenMW).
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendAttributes(unsigned short pid) noexcept;

    /**
    * \brief Send a PlayerSkill packet with a player's skills.
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendSkills(unsigned short pid) noexcept;

    /**
    * \brief Send a PlayerLevel packet with a player's character level and
    *        progress towards the next level up
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendLevel(unsigned short pid) noexcept;

    /**
    * \brief Send a PlayerBounty packet with a player's bounty.
    *
    * It is always sent to all players.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendBounty(unsigned short pid) noexcept;
};

#endif //OPENMW_STATAPI_HPP
