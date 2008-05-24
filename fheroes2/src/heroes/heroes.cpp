/*************************************************************************** 
 *   Copyright (C) 2006 by Andrey Afletdinov                               * 
 *   afletdinov@mail.dc.baikal.ru                                          * 
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

#include <cmath>
#include <algorithm>
#include "artifact.h"
#include "world.h"
#include "castle.h"
#include "config.h"
#include "agg.h"
#include "monster.h"
#include "engine.h"
#include "payment.h"
#include "cursor.h"
#include "sprite.h"
#include "rand.h"
#include "gamearea.h"
#include "heroes.h"

Heroes::Heroes(heroes_t ht, Race::race_t rc, const std::string & str) : Skill::Primary(), spellCasted(false), name(str), experience(0), magic_point(0),
    move_point(0), army(HEROESMAXARMY), heroes(ht), race(rc), army_spread(true), enable_move(false), shipmaster(false),
    save_maps_general(MP2::OBJ_ZERO), path(*this), direction(Direction::RIGHT), sprite_index(18)
{
    // hero is freeman
    color = Color::GRAY;

    switch(race)
    {
	case Race::KNGT:
            attack      = DEFAULT_KNGT_ATTACK;
            defence     = DEFAULT_KNGT_DEFENCE;
            power       = DEFAULT_KNGT_POWER;
            knowledge   = DEFAULT_KNGT_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::BALLISTICS, Skill::Level::BASIC));
	    break;
	    
	case Race::BARB:
            attack              = DEFAULT_BARB_ATTACK;
            defence             = DEFAULT_BARB_DEFENCE;
            power               = DEFAULT_BARB_POWER;
            knowledge           = DEFAULT_BARB_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::PATHFINDING, Skill::Level::ADVANCED));
	    break;
	    
	case Race::SORC:
            attack              = DEFAULT_SORC_ATTACK;
            defence             = DEFAULT_SORC_DEFENCE;
            power               = DEFAULT_SORC_POWER;
            knowledge           = DEFAULT_SORC_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::NAVIGATION, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::BASIC));

	    artifacts.push_back(Artifact::MAGIC_BOOK);
	    break;
	    
	case Race::WRLK:
            attack              = DEFAULT_WRLK_ATTACK;
            defence             = DEFAULT_WRLK_DEFENCE;
            power               = DEFAULT_WRLK_POWER;
            knowledge           = DEFAULT_WRLK_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::SCOUTING, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::BASIC));

	    artifacts.push_back(Artifact::MAGIC_BOOK);
	    break;
	    
	case Race::WZRD:
            attack              = DEFAULT_WZRD_ATTACK;
            defence             = DEFAULT_WZRD_DEFENCE;
            power               = DEFAULT_WZRD_POWER;
            knowledge           = DEFAULT_WZRD_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::ADVANCED));

	    artifacts.push_back(Artifact::MAGIC_BOOK);
	    break;
	    
	case Race::NECR:
            attack              = DEFAULT_NECR_ATTACK;
            defence             = DEFAULT_NECR_DEFENCE;
            power               = DEFAULT_NECR_POWER;
            knowledge           = DEFAULT_NECR_KNOWLEDGE;

	    secondary_skills.push_back(Skill::Secondary(Skill::NECROMANCY, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::BASIC));

	    artifacts.push_back(Artifact::MAGIC_BOOK);
	    break;
	    
	default: Error::Warning("Heroes::Heroes: unknown race."); break;
    }
    
    // set default army
    const Monster::stats_t monster = Monster::GetStats(Monster::Monster(race, Castle::DWELLING_MONSTER1));

    army[0].Set(monster.monster, monster.grown);

    // set master primary skill to army
    std::vector<Army::Troops>::iterator it1 = army.begin();
    std::vector<Army::Troops>::const_iterator it2 = army.end();
    for(; it1 != it2; ++it1) (*it1).SetMasterSkill(this);

    // extra hero
    switch(heroes)
    {
        case ROLAND:
            attack    = 0;
            defence   = 1;
            power     = 4;
            knowledge = 5;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::ARCHERY, Skill::Level::BASIC));
    	    break;

        case CORLAGON:
            attack    = 5;
            defence   = 3;
            power     = 1;
            knowledge = 1;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::NECROMANCY, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::BALLISTICS, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::PATHFINDING, Skill::Level::BASIC));
    	    break;
        
        case ELIZA:
            attack    = 0;
            defence   = 1;
            power     = 2;
            knowledge = 6;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::NAVIGATION, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::ARCHERY, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::LUCK, Skill::Level::BASIC));
    	    break;

    	case ARCHIBALD:
            attack    = 1;
            defence   = 1;
            power     = 4;
            knowledge = 4;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::SCOUTING, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::ADVANCED));
    	    break;

    	case HALTON:
            attack    = 3;
            defence   = 3;
            power     = 3;
            knowledge = 2;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::BALLISTICS, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::DIPLOMACY, Skill::Level::BASIC));
    	    break;
    	
    	case BAX:
            attack    = 1;
            defence   = 1;
            power     = 4;
            knowledge = 3;

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::EXPERT));
	    secondary_skills.push_back(Skill::Secondary(Skill::NECROMANCY, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::NAVIGATION, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::PATHFINDING, Skill::Level::BASIC));
    	    break;

	case SOLMYR:
	case DRAKONIA:
	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::BASIC));
	    break;

	case DAINWIN:
	case ELDERIAN:
	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::SCOUTING, Skill::Level::BASIC));
	    break;

	case MOG:
	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::WISDOM, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::NECROMANCY, Skill::Level::ADVANCED));
	    break;
	
	case UNCLEIVAN:
	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::PATHFINDING, Skill::Level::ADVANCED));
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::BASIC));
	    break;
	
	case JOSEPH:
	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::LEADERSHIP, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::SCOUTING, Skill::Level::BASIC));
	    break;
	
	case GALLAVANT:
	    break;
	
	case CEALLACH:
	    break;
	
	case MARTINE:
	    break;

	case JARKONAS:
	    break;

    	case SANDYSANDY:
	    army[0].Set(Monster::BLACK_DRAGON, 2);
    	    army[1].Set(Monster::RED_DRAGON, 3);

	    secondary_skills.clear();
	    secondary_skills.push_back(Skill::Secondary(Skill::PATHFINDING, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::LOGISTICS, Skill::Level::BASIC));
	    secondary_skills.push_back(Skill::Secondary(Skill::MYSTICISM, Skill::Level::BASIC));

	    artifacts.push_back(Artifact::MEDAL_VALOR);
	    artifacts.push_back(Artifact::STEALTH_SHIELD);
	    artifacts.push_back(Artifact::DRAGON_SWORD);
	    artifacts.push_back(Artifact::RABBIT_FOOT);
	    artifacts.push_back(Artifact::ENDLESS_BAG_GOLD);

	    experience = 777;
	    break;

	default: break;
    }

    magic_point = GetMaxSpellPoints();
    move_point = GetMaxMovePoints();
}

void Heroes::LoadFromMP2(u16 map_index, const void *ptr, const Color::color_t cl)
{
    mp.x = map_index % world.w();
    mp.y = map_index / world.h();

    color = cl;

    const u8  *ptr8  = static_cast<const u8 *>(ptr);
    u16 byte16 = 0;
    u32 byte32 = 0;

    // unknown
    ++ptr8;

    // custom troops
    bool custom_troops = *ptr8;
    if(custom_troops)
    {
        ++ptr8;

        // monster1
        army[0].SetMonster(Monster::Monster(*ptr8));
        ++ptr8;

        // monster2
        army[1].SetMonster(Monster::Monster(*ptr8));
        ++ptr8;

        // monster3
        army[2].SetMonster(Monster::Monster(*ptr8));
        ++ptr8;

        // monster4
        army[3].SetMonster(Monster::Monster(*ptr8));
        ++ptr8;

        // monster5
        army[4].SetMonster(Monster::Monster(*ptr8));
        ++ptr8;

        // count1
        LOAD16(ptr8, byte16);
        army[0].SetCount(byte16);
        ++ptr8;
        ++ptr8;

        // count2
        LOAD16(ptr8, byte16);
        army[1].SetCount(byte16);
        ++ptr8;
        ++ptr8;

        // count3
        LOAD16(ptr8, byte16);
        army[2].SetCount(byte16);
        ++ptr8;
        ++ptr8;

        // count4
        LOAD16(ptr8, byte16);
        army[3].SetCount(byte16);
        ++ptr8;
        ++ptr8;

        // count5
        LOAD16(ptr8, byte16);
        army[4].SetCount(byte16);
        ++ptr8;
        ++ptr8;
    }
    else
    {
        ptr8 += 16;
    }

    // custom portrate
    ++ptr8;

    // index sprite portrate
    ++ptr8;

    // artifacts
    Artifact::artifact_t artifact = Artifact::UNKNOWN;

    // artifact 1
    if(Artifact::UNKNOWN != (artifact = Artifact::Artifact(*ptr8))) artifacts.push_back(artifact);
    ++ptr8;

    // artifact 2
    if(Artifact::UNKNOWN != (artifact = Artifact::Artifact(*ptr8))) artifacts.push_back(artifact);
    ++ptr8;

    // artifact 3
    if(Artifact::UNKNOWN != (artifact = Artifact::Artifact(*ptr8))) artifacts.push_back(artifact);
    ++ptr8;

    // unknown byte
    ++ptr8;

    // experience
    LOAD32(ptr8, byte32);
    experience = byte32;
    ptr8 += 4;


    // custom skill
    if(*ptr8)
    {
	++ptr8;

	secondary_skills.clear();

	// skills
	for(u8 ii = 0; ii < 8; ++ii)
	{
	    const Skill::secondary_t skill = Skill::Secondary::FromMP2(*(ptr8 + ii));
	    const Skill::Level::type_t lvl = Skill::Level::FromMP2(*(ptr8 + ii + 8));

	    if(Skill::UNKNOWN != skill && Skill::Level::NONE != lvl) secondary_skills.push_back(Skill::Secondary(skill, lvl));
	}
	ptr8 += 15;
    }
    else
    {
	++ptr8;

        ptr8 += 16;
    }
    
    // unknown
    ++ptr8;

    // custom name
    ++ptr8;

    //name hero
    if(*ptr8) name = std::string(reinterpret_cast<const char *>(ptr8));
    
    ptr8 += 13;
    
    // patrol
    ++ptr8;

    // count square
    ++ptr8;

    // end

    // save general object
    save_maps_general = MP2::OBJ_ZERO;

    // other param
    magic_point = GetMaxSpellPoints();
    move_point = GetMaxMovePoints();

    if(H2Config::Debug()) Error::Verbose("add heroes: " + name + ", color: " + Color::String(color) + ", race: " + Race::String(race));
}

bool Heroes::operator== (const Heroes & h) const
{
    return heroes == h.heroes;
}

u8 Heroes::GetMobilityIndexSprite(void) const
{
    // valid range (0 - 25)
    const double floor = std::floor(move_point / 100);

    return 25 >= floor ? static_cast<u8>(floor) : 25;
}

u8 Heroes::GetManaIndexSprite(void) const
{
    // valid range (0 - 25)

    u8 r = magic_point / 5;

    return 25 >= r ? r : 25;
}

u8 Heroes::GetAttack(void) const
{
    u8 result = attack;

    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    for(; it != artifacts.end(); ++it)

	switch(*it)
	{
	    case Artifact::SPIKED_HELM:
	    case Artifact::THUNDER_MACE:
	    case Artifact::GIANT_FLAIL:
		result += 1;
		break;

            case Artifact::SPIKED_SHIELD:
            case Artifact::POWER_AXE:
        	result += 2;
		break;
	    
	    case Artifact::DRAGON_SWORD:
	        result += 3;
	        break;
	
	    case Artifact::ULTIMATE_CROWN:
	        result += 4;
	        break;
	
	    case Artifact::ULTIMATE_SHIELD:
	        result += 6;
	        break;
	
	    case Artifact::ULTIMATE_SWORD:
	        result += 12;
	        break;
	
	    default:
	        break;
	}

    return result;
}

u8 Heroes::GetDefense(void) const
{
    u8 result = defence;

    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    for(; it != artifacts.end(); ++it)

	switch(*it)
	{
            case Artifact::SPIKED_HELM:
            case Artifact::ARMORED_GAUNTLETS:
            case Artifact::DEFENDER_HELM:
                result += 1;
                break;

            case Artifact::SPIKED_SHIELD:
            case Artifact::STEALTH_SHIELD:
                result += 2;
                break;

            case Artifact::DIVINE_BREASTPLATE:
                result += 3;
                break;

            case Artifact::ULTIMATE_CROWN:
                result += 4;
                break;

            case Artifact::ULTIMATE_SHIELD:
                result += 6;
                break;

            case Artifact::ULTIMATE_CLOAK:
                result += 12;
                break;

            default:
                break;
	}

    return result;
}

u8 Heroes::GetPower(void) const
{
    u8 result = power;

    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    for(; it != artifacts.end(); ++it)

	switch(*it)
	{
            case Artifact::WHITE_PEARL:
                result += 1;
                break;

            case Artifact::BLACK_PEARL:
            case Artifact::CASTER_BRACELET:
            case Artifact::MAGE_RING:
                result += 2;
                break;

            case Artifact::WITCHES_BROACH:
                result += 3;
                break;

            case Artifact::ULTIMATE_CROWN:
            case Artifact::ARCANE_NECKLACE:
                result += 4;
                break;

            case Artifact::ULTIMATE_STAFF:
                result += 6;
                break;

            case Artifact::ULTIMATE_WAND:
                result += 12;
                break;

            default:
                break;
	}

    return result;
}

u8 Heroes::GetKnowledge(void) const
{
    u8 result = knowledge;

    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    for(; it != artifacts.end(); ++it)

	switch(*it)
	{
            case Artifact::WHITE_PEARL:
                result += 1;
                break;

            case Artifact::BLACK_PEARL:
            case Artifact::MINOR_SCROLL:
                result += 2;
                break;

            case Artifact::MAJOR_SCROLL:
                result += 3;
                break;

            case Artifact::ULTIMATE_CROWN:
            case Artifact::SUPERIOR_SCROLL:
                result += 4;
                break;

            case Artifact::FOREMOST_SCROLL:
                result += 5;
                break;

            case Artifact::ULTIMATE_STAFF:
                result += 6;
                break;

            case Artifact::ULTIMATE_BOOK:
                result += 12;
                break;

            default:
                break;
	}

    return result;
}

u32 Heroes::GetExperience(void) const
{
    return experience;
}

u16 Heroes::GetSpellPoints(void) const
{
    return magic_point;
}

u16 Heroes::GetMovePoints(void) const
{
    return move_point;
}

u16 Heroes::GetMaxSpellPoints(void) const
{
    return 10 * GetKnowledge();
}

u16 Heroes::GetMaxMovePoints(void) const
{
    u16 point = DEFAULT_MOVE_POINTS * 100;

    // skill logistics
    switch(GetLevelSkill(Skill::LOGISTICS))
    {
	// bonus: 10%
	case Skill::Level::BASIC:       point += (point * 10) / 100; break;
        // bonus: 20%
        case Skill::Level::ADVANCED:    point += (point * 20) / 100; break;
        // bonus: 30%
        case Skill::Level::EXPERT:      point += (point * 30) / 100; break;

        default: break;
    }

    if(isShipMaster())
    {
	switch(GetLevelSkill(Skill::NAVIGATION))
	{
	    // bonus: 1/3
	    case Skill::Level::BASIC:       point += point / 3; break;
    	    // bonus: 2/3
    	    case Skill::Level::ADVANCED:    point += (point * 2) / 3; break;
    	    // bonus: double
    	    case Skill::Level::EXPERT:      point *= 2; break;

    	    default: break;
	}

        // artifact bonus: FIXME: SAILORS_ASTROLABE_MOBILITY 20% ?
        if(HasArtifact(Artifact::SAILORS_ASTROLABE_MOBILITY)) point += (point * 20) / 100;
    }
    else
    {
        // artifact bonus: FIXME: NOMAD_BOOTS_MOBILITY 20% ?
	if(HasArtifact(Artifact::NOMAD_BOOTS_MOBILITY)) point += (point * 20) / 100;

        // artifact bonus: FIXME: TRAVELER_BOOTS_MOBILITY 20% ?
	if(HasArtifact(Artifact::TRAVELER_BOOTS_MOBILITY)) point += (point * 20) / 100;
    }

    // artifact bonus: FIXME: TRUE_COMPASS_MOBILITY 20% ?
    if(HasArtifact(Artifact::TRUE_COMPASS_MOBILITY)) point += (point * 20) / 100;

    return point;
}

Morale::morale_t Heroes::GetMorale(void) const
{
    s8 result = morale;

    // bonus artifact
    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    for(; it != artifacts.end(); ++it)
	switch(*it)
	{
            case Artifact::MEDAL_VALOR:
            case Artifact::MEDAL_COURAGE:
            case Artifact::MEDAL_HONOR:
            case Artifact::MEDAL_DISTINCTION:
                result += 1;
                break;

            case Artifact::FIZBIN_MISFORTUNE:
                result -= 2;
                break;

            default:
		break;
    	}

    // bonus leadership
    switch(GetLevelSkill(Skill::LEADERSHIP))
    {
        case Skill::Level::EXPERT:
            result += 3;
            break;

        case Skill::Level::ADVANCED:
            result += 2;
            break;

        case Skill::Level::BASIC:
            result += 1;
            break;

        default:
            break;
    }
    
    const Castle* castle = inCastle();

    // bonus in castle
    if(castle)
    {
	// and tavern
	if(castle->isBuild(Castle::BUILD_TAVERN)) result += 1;

	// and barbarian coliseum
        if(Race::BARB == castle->GetRace() && castle->isBuild(Castle::BUILD_SPEC)) result += 2;
    }

    // object visited
    if(isVisited(MP2::OBJ_BUOY)) ++result;
    if(isVisited(MP2::OBJ_OASIS)) ++result;
    if(isVisited(MP2::OBJ_TEMPLE)) result += 2;
    if(isVisited(MP2::OBJ_GRAVEYARD)) --result;
    if(isVisited(MP2::OBJ_SHIPWRECK)) --result;
    if(isVisited(MP2::OBJ_DERELICTSHIP)) --result;

    // different race penalty
    std::vector<Army::Troops>::const_iterator it1_army = army.begin();
    std::vector<Army::Troops>::const_iterator it2_army = army.end();
    u8 count = 0;
    u8 count_kngt = 0;
    u8 count_barb = 0;
    u8 count_sorc = 0;
    u8 count_wrlk = 0;
    u8 count_wzrd = 0;
    u8 count_necr = 0;
    u8 count_bomg = 0;
    for(; it1_army != it2_army; ++it1_army) if(Monster::UNKNOWN != (*it1_army).Monster())
	switch(Monster::GetRace((*it1_army).Monster()))
	{
	    case Race::KNGT: ++count_kngt; break;
	    case Race::BARB: ++count_barb; break;
	    case Race::SORC: ++count_sorc; break;
	    case Race::WRLK: ++count_wrlk; break;
	    case Race::WZRD: ++count_wzrd; break;
	    case Race::NECR: ++count_necr; break;
	    case Race::BOMG: ++count_bomg; break;
	    default: break;
	}
    if(count_kngt) ++count;
    if(count_barb) ++count;
    if(count_sorc) ++count;
    if(count_wrlk) ++count;
    if(count_wzrd) ++count;
    if(count_necr) ++count;
    if(count_bomg) ++count;

    switch(count)
    {
	case 0: break;
	case 1: result += (count_necr ? 0 : 1); break;
	case 3: result -= 1; break;
	case 4: result -= 2; break;
	// over 4 different race
	default: result -=3; break;
    }

    // undead in life group
    if(count_necr && (count_kngt || count_barb || count_sorc || count_wrlk || count_wzrd || count_bomg)) --result;

    if(result < Morale::AWFUL)	return Morale::TREASON;
    else
    if(result < Morale::POOR)	return Morale::AWFUL;
    else
    if(result < Morale::NORMAL)	return Morale::POOR;
    else
    if(result < Morale::GOOD)	return Morale::NORMAL;
    else
    if(result < Morale::GREAT)	return Morale::GOOD;
    else
    if(result < Morale::BLOOD)	return Morale::GREAT;

    return Morale::BLOOD;
}

Luck::luck_t Heroes::GetLuck(void) const
{
    s8 result = luck;

    std::vector<Artifact::artifact_t>::const_iterator it = artifacts.begin();

    // bonus artifact
    for(; it != artifacts.end(); ++it)

	switch(*it)
	{
    	    case Artifact::RABBIT_FOOT:
            case Artifact::GOLDEN_HORSESHOE:
            case Artifact::GAMBLER_LUCKY_COIN:
            case Artifact::FOUR_LEAF_CLOVER:
	    	++result;
		break;

            default:
		break;
    	}

    // bonus luck
    switch(GetLevelSkill(Skill::LUCK))
    {
        case Skill::Level::EXPERT:
            result += 3;
            break;

        case Skill::Level::ADVANCED:
            result += 2;
            break;

        case Skill::Level::BASIC:
            result += 1;
            break;

        default:
            break;
    }

    // object visited
    if(isVisited(MP2::OBJ_FAERIERING)) ++result;
    if(isVisited(MP2::OBJ_FOUNTAIN)) ++result;
    if(isVisited(MP2::OBJ_IDOL)) ++result;

    // bonus in castle and sorceress rainbow
    const Castle* castle = inCastle();
    if(castle && Race::SORC == castle->GetRace() && castle->isBuild(Castle::BUILD_SPEC)) result += 2;


    if(result < Luck::AWFUL)	return Luck::CURSED;
    else
    if(result < Luck::BAD)	return Luck::AWFUL;
    else
    if(result < Luck::NORMAL)	return Luck::BAD;
    else
    if(result < Luck::GOOD)	return Luck::NORMAL;
    else
    if(result < Luck::GREAT)	return Luck::GOOD;
    else
    if(result < Luck::IRISH)	return Luck::GREAT;

    return Luck::IRISH;
}

// return valid count heroes army
u8 Heroes::GetCountArmy(void) const
{
    u8 result = 0;

    for(u8 ii = 0; ii < HEROESMAXARMY; ++ii) if(Army::isValid(army[ii])) ++result;

    return result;
}

/* recrut hero */
void Heroes::Recruit(const Color::color_t & cl, const Point & pt)
{
    color = cl;
    mp = pt;

    Maps::Tiles & tiles = world.GetTiles(mp);

    // save general object
    save_maps_general = tiles.GetObject();
    tiles.SetObject(MP2::OBJ_HEROES);
}

void Heroes::Recruit(const Castle & castle)
{
    Recruit(castle.GetColor(), castle.GetCenter());
}

void Heroes::ActionNewDay(void)
{
    // recovery move points
    move_point = GetMaxMovePoints();
    path.Rescan();

    // recovery spell points
    if(HasArtifact(Artifact::MAGIC_BOOK))
    {
	// everyday
	if(magic_point != GetMaxSpellPoints()) ++magic_point;

	// secondary skill
	switch(GetLevelSkill(Skill::MYSTICISM))
	{
	    case Skill::Level::BASIC:	if(magic_point != GetMaxSpellPoints()) magic_point += 1; break;
	    case Skill::Level::ADVANCED:if(magic_point != GetMaxSpellPoints()) magic_point += 2; break;
	    case Skill::Level::EXPERT:	if(magic_point != GetMaxSpellPoints()) magic_point += 3; break;

	    default: break;
	}
    }

    // remove day visit object
    std::remove_if(visit_object.begin(), visit_object.end(), Visit::IndexObject::isDayLife);
}

void Heroes::ActionNewWeek(void)
{
    // remove week visit object
    std::remove_if(visit_object.begin(), visit_object.end(), Visit::IndexObject::isWeekLife);
}

void Heroes::ActionNewMonth(void)
{
    // remove month visit object
    std::remove_if(visit_object.begin(), visit_object.end(), Visit::IndexObject::isMonthLife);
}


void Heroes::ActionAfterBattle(void)
{
    // remove month visit object
    std::remove_if(visit_object.begin(), visit_object.end(), Visit::IndexObject::isBattleLife);
}

u16 Heroes::FindPath(u16 dst_index)
{
    return path.Calculate(dst_index);
}

/* if hero in castle */
const Castle* Heroes::inCastle(void) const
{
    if(Color::GRAY == color) return false;

    const std::vector<Castle *> & castles = world.GetKingdom(color).GetCastles();
    
    std::vector<Castle *>::const_iterator it1 = castles.begin();
    std::vector<Castle *>::const_iterator it2 = castles.end();

    for(; it1 != it2; ++it1) if((**it1).GetCenter() == mp) return *it1;

    return NULL;
}

/* is visited cell */
bool Heroes::isVisited(const Maps::Tiles & tile, const Visit::type_t type) const
{
    if(Visit::GLOBAL == type) return world.GetKingdom(color).isVisited(tile);

    std::list<Visit::IndexObject>::const_iterator it1 = visit_object.begin();
    std::list<Visit::IndexObject>::const_iterator it2 = visit_object.end();

    const u16 & index = tile.GetIndex();
    const MP2::object_t object = (tile.GetObject() == MP2::OBJ_HEROES ? GetUnderObject() : tile.GetObject());
    //const MP2::object_t & object = tile.GetObject();

    for(; it1 != it2; ++it1) if(index == (*it1).first && object == (*it1).second) return true;

    return false;
}

/* return true if object visited */
bool Heroes::isVisited(const MP2::object_t & object, const Visit::type_t type) const
{
    if(Visit::GLOBAL == type) return world.GetKingdom(color).isVisited(object);

    std::list<Visit::IndexObject>::const_iterator it1 = visit_object.begin();
    std::list<Visit::IndexObject>::const_iterator it2 = visit_object.end();

    for(; it1 != it2; ++it1) if((*it1).second == object) return true;

    return false;
}

/* set visited cell */
void Heroes::SetVisited(const u16 index, const Visit::type_t type)
{
    const Maps::Tiles & tile = world.GetTiles(index);

    const MP2::object_t object = (tile.GetObject() == MP2::OBJ_HEROES ? GetUnderObject() : tile.GetObject());

    if(Visit::GLOBAL == type)
	world.GetKingdom(color).SetVisited(index, object);
    else
    if(isVisited(tile))
	return;
    else
    if(MP2::OBJ_ZERO != object) visit_object.push_front(Visit::IndexObject(index, object));
}

/* return true if artifact present */
bool Heroes::HasArtifact(const Artifact::artifact_t & art) const
{
    return artifacts.end() != std::find(artifacts.begin(), artifacts.end(), art);
}

/* return level hero */
u8 Heroes::GetLevel(void) const
{
    return GetLevelFromExperience(experience);
}

void Heroes::IncreaseExperience(const u16 exp)
{
    const u8 level_old = GetLevelFromExperience(experience);
    const u8 level_new = GetLevelFromExperience(experience + exp);

    for(u8 ii = 0; ii < level_new - level_old; ++ii) LevelUp();

    experience += exp;
}

/* calc level from exp */
u8 Heroes::GetLevelFromExperience(u32 exp)
{
    for(u8 lvl = 1; lvl < 255; ++ lvl) if(exp < GetExperienceFromLevel(lvl)) return lvl;

    return 0;
}

/* calc exp from level */
u32 Heroes::GetExperienceFromLevel(u8 lvl)
{
    switch(lvl)
    {
	case 0:		return 0;
	case 1:		return 1000;
	case 2:		return 2000;
	case 3:		return 3200;
	case 4:		return 4500;
	case 5:		return 6000;
	case 6:		return 7700;
	case 7:		return 9000;
	case 8: 	return 11000;
	case 9:		return 13200;
	case 10:	return 15500;
	case 11:	return 18500;
	case 12:	return 22100;
	case 13:	return 26400;
	case 14:	return 31600;
	case 15:	return 37800;
	case 16:	return 45300;
	case 17:	return 54200;
	case 18:	return 65000;


	// FIXME:	calculate alghoritm

	default:	return MAXU16;
    }

    return 0;
}

/* buy book */
bool Heroes::BuySpellBook(void)
{
    if(HasArtifact(Artifact::MAGIC_BOOK) || Color::GRAY == color) return false;

    PaymentConditions::BuySpellBook payment;
    Kingdom & kingdom = world.GetKingdom(color);

    if( ! kingdom.AllowPayment(payment)) return false;

    kingdom.OddFundsResource(payment);

    artifacts.push_back(Artifact::MAGIC_BOOK);
    
    return true;
}

/* add new spell to book from storage */
void Heroes::AppendSpellsToBook(const Spell::Storage & spells)
{
    spell_book.Appends(spells, GetLevelSkill(Skill::WISDOM));
}

void Heroes::AppendSpellToBook(const Spell::spell_t spell)
{
    spell_book.Append(spell, GetLevelSkill(Skill::WISDOM));
}

/* return true is move enable */
bool Heroes::isEnableMove(void) const
{
    return enable_move && path.EnableMove();
}

/* return true isn allow move to dst tile */
bool Heroes::isAllowMove(const u16 dst_index)
{
    return path.Calculate(dst_index);
}

/* set enable move */
void Heroes::SetMove(bool f)
{
    enable_move = f;
}

MP2::object_t Heroes::GetUnderObject(void) const
{
    return save_maps_general;
}

bool Heroes::isShipMaster(void) const
{
    return shipmaster;
}

void Heroes::SetShipMaster(bool f)
{
    shipmaster = f;
}

void Heroes::PlayWalkSound(void) const
{
    if(GetColor() != H2Config::MyColor()) return;

    M82::m82_t wav = M82::UNKNOWN;
    
    const u8 speed = 3;

    // play sound
    switch(world.GetTiles(mp).GetGround())
    {
        case Maps::Ground::WATER:	wav = (1 == speed ? M82::WSND00 : (2 == speed ? M82::WSND10 : M82::WSND20)); break;
        case Maps::Ground::GRASS:	wav = (1 == speed ? M82::WSND01 : (2 == speed ? M82::WSND11 : M82::WSND21)); break;
        case Maps::Ground::WASTELAND:	wav = (1 == speed ? M82::WSND02 : (2 == speed ? M82::WSND12 : M82::WSND22)); break;
        case Maps::Ground::SWAMP:
        case Maps::Ground::BEACH:	wav = (1 == speed ? M82::WSND03 : (2 == speed ? M82::WSND13 : M82::WSND23)); break;
        case Maps::Ground::LAVA:	wav = (1 == speed ? M82::WSND04 : (2 == speed ? M82::WSND14 : M82::WSND24)); break;
        case Maps::Ground::DESERT:
        case Maps::Ground::SNOW:	wav = (1 == speed ? M82::WSND05 : (2 == speed ? M82::WSND15 : M82::WSND25)); break;
        case Maps::Ground::DIRT:	wav = (1 == speed ? M82::WSND06 : (2 == speed ? M82::WSND16 : M82::WSND26)); break;

        default: return;
    }

    AGG::PlaySound(wav);
}

void Heroes::PlayPickupSound(void) const
{
    if(GetColor() != H2Config::MyColor()) return;

    M82::m82_t wav = M82::UNKNOWN;

    switch(Rand::Get(1, 7))
    {
	case 1:	wav = M82::PICKUP01; break;
	case 2:	wav = M82::PICKUP02; break;
	case 3:	wav = M82::PICKUP03; break;
	case 4:	wav = M82::PICKUP04; break;
	case 5:	wav = M82::PICKUP05; break;
	case 6:	wav = M82::PICKUP06; break;
	case 7:	wav = M82::PICKUP07; break;

	default: return;
    }

    AGG::PlaySound(wav);
}

bool Heroes::HasSecondarySkill(const Skill::secondary_t skill) const
{
    return Skill::Level::NONE != GetLevelSkill(skill);
}

Skill::Level::type_t Heroes::GetLevelSkill(const Skill::secondary_t skill) const
{
    std::vector<Skill::Secondary>::const_iterator it1 = secondary_skills.begin();
    std::vector<Skill::Secondary>::const_iterator it2 = secondary_skills.end();
    
    for(; it1 != it2; ++it1)
	if((*it1).Skill() == skill) return (*it1).Level();

    return Skill::Level::NONE;
}

void Heroes::LearnBasicSkill(const Skill::secondary_t skill)
{
    std::vector<Skill::Secondary>::iterator it1 = secondary_skills.begin();
    std::vector<Skill::Secondary>::const_iterator it2 = secondary_skills.end();

    // find_if
    for(; it1 != it2; ++it1) if((*it1).Skill() == skill) break;

    if(it1 != it2)
	(*it1).SetLevel(Skill::Level::BASIC);
    else
	secondary_skills.push_back(Skill::Secondary(skill, Skill::Level::BASIC));
}

void Heroes::Scoute(void)
{
    int scouting = SCOUTINGBASE + GetLevelSkill(Skill::SCOUTING);

    const Point & center = mp;

    for(s16 y = center.y - scouting; y <= center.y + scouting; ++y)
        for(s16 x = center.x - scouting; x <= center.x + scouting; ++x)
            if(Maps::isValidAbsPoint(x, y) &&  scouting + 2 >= std::abs(x - center.x) + std::abs(y - center.y))
                world.GetTiles(Maps::GetIndexFromAbsPoint(x, y)).ClearFog(color);
}

bool Heroes::PickupArtifact(const Artifact::artifact_t & art)
{
    if(HasArtifact(art) || HEROESMAXARTIFACT <= artifacts.size()) return false;
    
    artifacts.push_back(art);
    
    return true;
}

/* set cente from index maps */
void Heroes::SetCenter(const u16 index)
{
    mp.x = index % world.w();
    mp.y = index / world.h();
}

/* return route range in days */
u8 Heroes::GetRangeRouteDays(void) const
{
    if(path.isValid())
    {
	u8 result = 0;
	s32 total = path.TotalPenalty();
	const u16 max = GetMaxMovePoints();

	// current day
	if(move_point > total) return 1;
	++result;
	total -= move_point;

	// next days cast
	while(1)
	{
	    ++result;

	    if(max < total) total -= max;
	    else return result;
	}
    }

    return 0;
}

/* up level */
void Heroes::LevelUp(void)
{
    const u8 level = GetLevel();
    std::vector<u8> primary(4);

    // primary skill
    switch(GetRace())
    {
	case Race::BARB:
	    if(10 > level)
	    {
		primary[0] = 55; primary[1] = 35; primary[2] =  5; primary[3] =  5;
	    }
	    else
	    {
		primary[0] = 30; primary[1] = 30; primary[2] = 20; primary[3] = 20;
	    }
	    break;

	case Race::KNGT:
	    if(10 > level)
	    {
		primary[0] = 35; primary[1] = 45; primary[2] = 10; primary[3] = 10;
	    }
	    else
	    {
		primary[0] = 25; primary[1] = 25; primary[2] = 25; primary[3] = 25;
	    }
	    break;

	case Race::NECR:
	    if(10 > level)
	    {
		primary[0] = 15; primary[1] = 15; primary[2] = 35; primary[3] = 35;
	    }
	    else
	    {
		primary[0] = 25; primary[1] = 25; primary[2] = 25; primary[3] = 25;
	    }
	    break;

	case Race::SORC:
	    if(10 > level)
	    {
		primary[0] = 10; primary[1] = 10; primary[2] = 30; primary[3] = 50;
	    }
	    else
	    {
		primary[0] = 20; primary[1] = 20; primary[2] = 30; primary[3] = 30;
	    }
	    break;

	case Race::WRLK:
	    if(10 > level)
	    {
		primary[0] = 10; primary[1] = 10; primary[2] = 50; primary[3] = 30;
	    }
	    else
	    {
		primary[0] = 20; primary[1] = 20; primary[2] = 30; primary[3] = 30;
	    }
	    break;

	case Race::WZRD:
	    if(10 > level)
	    {
		primary[0] = 10; primary[1] = 10; primary[2] = 40; primary[3] = 40;
	    }
	    else
	    {
		primary[0] = 20; primary[1] = 20; primary[2] = 30; primary[3] = 30;
	    }
	    break;

	default: return;
    }

    // calculate in percents
    std::vector<u8>::const_iterator it1 = primary.begin();
    std::vector<u8>::const_iterator it2 = primary.end();

    const u8 value = Rand::Get(1, 100);
    u8 amount = 0;
    u8 res = 0;

    for(; it1 != it2; ++it1)
    {
        amount += *it1;
        ++res;

        if(value <= amount) break;
    }

    // select skill
    switch(res)
    {
	case 1:	++attack;
	case 2: ++defence;
	case 3: ++power;
	case 4: ++knowledge;

	default: break;
    }
    
    if(GetColor() == H2Config::MyColor())
    {
	AGG::PlaySound(M82::NWHEROLV);

	// show select dialog
    }
    else
    {
	// for AI
    }

    if(H2Config::Debug()) Error::Verbose("Heroes::LevelUp: for " + GetName());
}

/* apply penalty */
void Heroes::ApplyPenaltyMovement(void)
{
    if(path.isValid()) move_point -= path.GetFrontPenalty();
}
