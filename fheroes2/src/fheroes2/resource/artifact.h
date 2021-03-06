/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef H2ARTIFACT_H
#define H2ARTIFACT_H

#include <vector>
#include "game_io.h"
#include "gamedefs.h"

class Spell;

class Artifact
{
public:
    enum level_t
    {
	ART_NONE	= 0,
	ART_LEVEL1	= 0x01,
	ART_LEVEL2	= 0x02,
	ART_LEVEL3	= 0x04,
	ART_LEVEL123	= ART_LEVEL1|ART_LEVEL2|ART_LEVEL3,
	ART_ULTIMATE	= 0x08,
	ART_LOYALTY	= 0x10,
	ART_NORANDOM	= 0x20
    };

    enum
    {
	ULTIMATE_BOOK,
	ULTIMATE_SWORD,
	ULTIMATE_CLOAK,
	ULTIMATE_WAND,
	ULTIMATE_SHIELD,
	ULTIMATE_STAFF,
	ULTIMATE_CROWN,
	GOLDEN_GOOSE,
	ARCANE_NECKLACE,
	CASTER_BRACELET,
	MAGE_RING,
	WITCHES_BROACH,
	MEDAL_VALOR,
	MEDAL_COURAGE,
	MEDAL_HONOR,
	MEDAL_DISTINCTION,
	FIZBIN_MISFORTUNE,
	THUNDER_MACE,
	ARMORED_GAUNTLETS,
	DEFENDER_HELM,
	GIANT_FLAIL,
	BALLISTA,
	STEALTH_SHIELD,
	DRAGON_SWORD,
	POWER_AXE,
	DIVINE_BREASTPLATE,
	MINOR_SCROLL,
	MAJOR_SCROLL,
	SUPERIOR_SCROLL,
	FOREMOST_SCROLL,
	ENDLESS_SACK_GOLD,
	ENDLESS_BAG_GOLD,
	ENDLESS_PURSE_GOLD,
	NOMAD_BOOTS_MOBILITY,
	TRAVELER_BOOTS_MOBILITY,
	RABBIT_FOOT,
	GOLDEN_HORSESHOE,
	GAMBLER_LUCKY_COIN,
	FOUR_LEAF_CLOVER,
	TRUE_COMPASS_MOBILITY,
	SAILORS_ASTROLABE_MOBILITY,
	EVIL_EYE,
	ENCHANTED_HOURGLASS,
	GOLD_WATCH,
	SKULLCAP,
	ICE_CLOAK,
	FIRE_CLOAK,
	LIGHTNING_HELM,
	EVERCOLD_ICICLE,
	EVERHOT_LAVA_ROCK,
	LIGHTNING_ROD,
	SNAKE_RING,
	ANKH,
	BOOK_ELEMENTS,
	ELEMENTAL_RING,
	HOLY_PENDANT,
	PENDANT_FREE_WILL,
	PENDANT_LIFE,
	SERENITY_PENDANT,
	SEEING_EYE_PENDANT,
	KINETIC_PENDANT,
	PENDANT_DEATH,
	WAND_NEGATION,
	GOLDEN_BOW,
	TELESCOPE,
	STATESMAN_QUILL,
	WIZARD_HAT,
	POWER_RING,
	AMMO_CART,
	TAX_LIEN,
	HIDEOUS_MASK,
	ENDLESS_POUCH_SULFUR,
	ENDLESS_VIAL_MERCURY,
	ENDLESS_POUCH_GEMS,
	ENDLESS_CORD_WOOD,
	ENDLESS_CART_ORE,
	ENDLESS_POUCH_CRYSTAL,
	SPIKED_HELM,
	SPIKED_SHIELD,
	WHITE_PEARL,
	BLACK_PEARL,

	MAGIC_BOOK,

	DUMMY1,
	DUMMY2,
	DUMMY3,
	DUMMY4,

	SPELL_SCROLL,
	ARM_MARTYR,
	BREASTPLATE_ANDURAN,
	BROACH_SHIELDING,
	BATTLE_GARB,
	CRYSTAL_BALL,
	HEART_FIRE,
	HEART_ICE,
	HELMET_ANDURAN,
	HOLY_HAMMER,
	LEGENDARY_SCEPTER,
	MASTHEAD,
	SPHERE_NEGATION,
	STAFF_WIZARDRY,
	SWORD_BREAKER,
	SWORD_ANDURAN,
	SPADE_NECROMANCY,

	UNKNOWN
    };

    Artifact(u8 = UNKNOWN);

    bool operator== (const Spell &) const;
    bool operator== (const Artifact &) const;
    bool operator!= (const Artifact &) const;
    u8 operator() (void) const;
    u8 GetID(void) const;

    bool isUltimate(void) const;
    bool isValid(void) const;

    void Reset(void);

    u16 ExtraValue(void) const;
    u8 Level(void) const;
    u8 LoyaltyLevel(void) const;
    u8 Type(void) const;

    /* objnarti.icn */
    u8 IndexSprite(void) const;
    /* artfx.icn */
    u8 IndexSprite32(void) const;
    /* artifact.icn */
    u8 IndexSprite64(void) const;

    void SetSpell(u8);
    u8 GetSpell(void) const;

    const char* GetName(void) const;
    std::string GetDescription(void) const;

    static u8 Rand(level_t);
    static Artifact FromMP2IndexSprite(u8);
    static const char* GetScenario(const Artifact &);
    static void UpdateStats(const std::string &);

private:
    friend class Game::IO;

    u8 id;
    u8 ext;
};

u16 GoldInsteadArtifact(u8);

struct BagArtifacts : std::vector<Artifact>
{
    BagArtifacts();

    bool ContainSpell(const Spell &) const;
    bool isPresentArtifact(const Artifact &) const;
    bool PushArtifact(const Artifact &);
    bool isFull(void) const;
    bool MakeBattleGarb(void);
    bool ContainUltimateArtifact(void) const;

    void  RemoveScroll(const Artifact &);

    u8   CountArtifacts(void) const;
    u8   Count(const Artifact &) const;

    std::string String(void) const;
};

#endif
