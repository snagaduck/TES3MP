#ifndef OPENMW_CHARCLASSAPI_HPP
#define OPENMW_CHARCLASSAPI_HPP

#include "../Types.hpp"

#define CHARCLASSAPI(t) \
    t.set_function("GetDefaultClass", &CharClassFunctions::GetDefaultClass); \
    t.set_function("GetClassName", &CharClassFunctions::GetClassName); \
    t.set_function("GetClassDesc", &CharClassFunctions::GetClassDesc); \
    t.set_function("GetClassMajorAttribute", &CharClassFunctions::GetClassMajorAttribute); \
    t.set_function("GetClassSpecialization", &CharClassFunctions::GetClassSpecialization); \
    t.set_function("GetClassMajorSkill", &CharClassFunctions::GetClassMajorSkill); \
    t.set_function("GetClassMinorSkill", &CharClassFunctions::GetClassMinorSkill); \
    t.set_function("IsClassDefault", &CharClassFunctions::IsClassDefault); \
    t.set_function("SetDefaultClass", &CharClassFunctions::SetDefaultClass); \
    t.set_function("SetClassName", &CharClassFunctions::SetClassName); \
    t.set_function("SetClassDesc", &CharClassFunctions::SetClassDesc); \
    t.set_function("SetClassMajorAttribute", &CharClassFunctions::SetClassMajorAttribute); \
    t.set_function("SetClassSpecialization", &CharClassFunctions::SetClassSpecialization); \
    t.set_function("SetClassMajorSkill", &CharClassFunctions::SetClassMajorSkill); \
    t.set_function("SetClassMinorSkill", &CharClassFunctions::SetClassMinorSkill); \
    t.set_function("SendClass", &CharClassFunctions::SendClass);


class CharClassFunctions
{
public:

    /**
    * \brief Get the default class used by a player.
    *
    * \param pid The player ID.
    * \return The ID of the default class.
    */
    static const char *GetDefaultClass(unsigned short pid) noexcept;

    /**
    * \brief Get the name of the custom class used by a player.
    *
    * \param pid The player ID.
    * \return The name of the custom class.
    */
    static const char *GetClassName(unsigned short pid) noexcept;

    /**
    * \brief Get the description of the custom class used by a player.
    *
    * \param pid The player ID.
    * \return The description of the custom class.
    */
    static const char *GetClassDesc(unsigned short pid) noexcept;

    /**
    * \brief Get the ID of one of the two major attributes of a custom class used by a player.
    *
    * \param pid The player ID.
    * \param slot The slot of the major attribute (0 or 1).
    * \return The ID of the major attribute.
    */
    static int GetClassMajorAttribute(unsigned short pid, unsigned char slot);

    /**
    * \brief Get the specialization ID of the custom class used by a player.
    *
    * \param pid The player ID.
    * \return The specialization ID of the custom class (0 for Combat, 1 for Magic, 2 for Stealth).
    */
    static int GetClassSpecialization(unsigned short pid) noexcept;

    /**
    * \brief Get the ID of one of the five major skills of a custom class used by a player.
    *
    * \param pid The player ID.
    * \param slot The slot of the major skill (0 to 4).
    * \return The ID of the major skill.
    */
    static int GetClassMajorSkill(unsigned short pid, unsigned char slot);

    /**
    * \brief Get the ID of one of the five minor skills of a custom class used by a player.
    *
    * \param pid The player ID.
    * \param slot The slot of the minor skill (0 to 4).
    * \return The ID of the minor skill.
    */
    static int GetClassMinorSkill(unsigned short pid, unsigned char slot);

    /**
    * \brief Check whether the player is using a default class instead of a custom one.
    *
    * \param pid The player ID.
    * \return Whether the player is using a default class.
    */
    static int IsClassDefault(unsigned short pid) noexcept;

    /**
    * \brief Set the default class used by a player.
    *
    * If this is left blank, the custom class data set for the player will be used instead.
    *
    * \param pid The player ID.
    * \param id The ID of the default class.
    * \return void
    */
    static void SetDefaultClass(unsigned short pid, const char *id) noexcept;

    /**
    * \brief Set the name of the custom class used by a player.
    *
    * \param pid The player ID.
    * \param name The name of the custom class.
    * \return void
    */
    static void SetClassName(unsigned short pid, const char *name) noexcept;

    /**
    * \brief Set the description of the custom class used by a player.
    *
    * \param pid The player ID.
    * \param desc The description of the custom class.
    * \return void
    */
    static void SetClassDesc(unsigned short pid, const char *desc) noexcept;

    /**
    * \brief Set the ID of one of the two major attributes of the custom class used by a player.
    *
    * \param pid The player ID.
    * \param slot The slot of the major attribute (0 or 1).
    * \param attrId The ID to use for the attribute.
    * \return void
    */
    static void SetClassMajorAttribute(unsigned short pid, unsigned char slot, int attrId);

    /**
    * \brief Set the specialization of the custom class used by a player.
    *
    * \param pid The player ID.
    * \param spec The specialization ID to use (0 for Combat, 1 for Magic, 2 for Stealth).
    * \return void
    */
    static void SetClassSpecialization(unsigned short pid, int spec) noexcept;

     /**
     * \brief Set the ID of one of the five major skills of the custom class used by a player.
     *
     * \param pid The player ID.
     * \param slot The slot of the major skill (0 to 4).
     * \param skillId The ID to use for the skill.
     * \return void
     */
    static void SetClassMajorSkill(unsigned short pid, unsigned char slot, int skillId);

    /**
    * \brief Set the ID of one of the five minor skills of the custom class used by a player.
    *
    * \param pid The player ID.
    * \param slot The slot of the minor skill (0 to 4).
    * \param skillId The ID to use for the skill.
    * \return void
    */
    static void SetClassMinorSkill(unsigned short pid, unsigned char slot, int skillId);

    /**
    * \brief Send a PlayerCharClass packet about a player.
    *
    * It is only sent to the affected player.
    *
    * \param pid The player ID.
    * \return void
    */
    static void SendClass(unsigned short pid) noexcept;
};

#endif //OPENMW_CHARCLASSAPI_HPP
