/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __SPELL_PROC_H
#define __SPELL_PROC_H

#include "StdAfx.h"

class SpellProc;

#define SPELL_PROC_FACTORY_FUNCTION(T) \
  public: \
	static SpellProc* Create() { return new T(); }

typedef SpellProc *(*spell_proc_factory_function)();

typedef HM_NAMESPACE::hash_map<uint32, spell_proc_factory_function>  SpellProcMap;

class SpellProc
{
public:
	~SpellProc()
	{
	}

	// Returns true if this spell can proc, false otherwise
	virtual bool CanProc(Unit *victim, SpellEntry *CastingSpell)
    {
        return true;
    }

	// Called when procFlags is to be compared.
	// Return true on success, false otherwise
	virtual bool CheckProcFlags(uint32 flag)
    {
	    if ( mProcFlags & flag )
		    return true;
	    else	
		    return false;
    }

	// Returns the chance of proc
	virtual uint32 CalcProcChance(Unit *victim, SpellEntry *CastingSpell)
    {
        return mProcChance;
    }

	// Called just after this object is created. Usefull for initialize object members
	virtual void Init(Object *obj)
	{
	}

	SpellEntry* mSpell;
	SpellEntry* mOrigSpell;
	Unit*  mTarget;
	uint64 mCaster;
	uint32 mProcChance;
	uint32 mProcFlags;
	uint32 mProcCharges;
	uint32 mLastTrigger;
	uint32 mProcType; //0=triggerspell/1=triggerclassspell
	uint32 mGroupRelation[3];
	bool mDeleted;
};

class SpellProcMgr: public Singleton < SpellProcMgr >
{
public:
	SpellProcMgr()
	{
		Setup();
	}

	~SpellProcMgr()
	{
	}

	SpellProc* NewSpellProc(Unit *target, uint32 spell_id, uint32 orig_spell_id, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, Object *obj);

	SpellProc* NewSpellProc(Unit *target, SpellEntry *spell, SpellEntry *orig_spell, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, Object *obj);

private:

	SpellProcMap mSpellProc;
	SpellProcMap mSpellProcNameHash;

	void AddById(uint32 spellId, spell_proc_factory_function spell_proc)
	{
		mSpellProc.insert(make_pair(spellId, spell_proc));
	}

	void AddByNameHash(uint32 name_hash, spell_proc_factory_function spell_proc)
	{
		mSpellProcNameHash.insert(make_pair(name_hash, spell_proc));
	}

	void Setup();

	void SetupItems();
	void SetupDeathKnight();
	void SetupDruid();
	void SetupHunter();
	void SetupMage();
	void SetupPaladin();
	void SetupPriest();
	void SetupRogue();
	void SetupShamman();
	void SetupWarlock();
	void SetupWarrior();
};

#define sSpellProcMgr SpellProcMgr::getSingleton()

#endif
