#ifndef _INCLUDE_THUNKS_H_
#define _INCLUDE_THUNKS_H_


#include "extension.h"
#include "NextBot/NextBotLocomotionInterface.h"
#include <iservernetworkable.h>
#include <iserverentity.h>


class Path;
class PathFollower;
class INextBot;
class CBaseCombatCharacter;
class IPathCost;
class ILocomotion;


/* MSVC's member function pointer system needs to die in a fire */
#if defined _MSC_VER
#pragma pointers_to_members(full_generality, single_inheritance)
#endif
/* this pragma won't be necessary if compiling with '/vmg /vms' */


namespace Thunks
{
	extern bool (Path:: *Path_Compute_goal)(INextBot *, const Vector&, IPathCost&, float, bool);
	extern bool (Path:: *Path_Compute_subject)(INextBot *, CBaseCombatCharacter *, IPathCost&, float, bool);
	
	extern void (PathFollower:: *PathFollower_ctor)();
	
	extern bool (ILocomotion:: *ILocomotion_IsPotentiallyTraversable)(const Vector&, const Vector&, ILocomotion::TraverseWhenType, float *) const;

	extern bool (ILocomotion:: *ILocomotion_HasPotentialGap)(const Vector&, const Vector&, float *) const;
	
	extern bool (ILocomotion:: *ILocomotion_IsGap)(const Vector&, const Vector&) const;

	extern bool (ILocomotion:: *CTFBotLocomotion_IsAreaTraversable)(const CNavArea *) const;
	
	
	bool Init();
}


class CBaseEntity : public IServerEntity
{
public:
	IServerNetworkable *NetworkProp();
	ICollideable *CollisionProp();
	const Vector& GetAbsOrigin() const;
	int GetTeamNumber() const;
	CBaseEntity *GetGroundEntity() const;
	const char *GetClassname() const;
	bool IsCombatCharacter() const;
	
	CBaseCombatCharacter *MyCombatCharacterPointer();
	bool IsAlive();
};

class CBaseCombatCharacter : public CBaseEntity
{
public:
	CNavArea *GetLastKnownArea();
	void UpdateLastKnownArea();
};

class NextBotManager
{
public:
	int Register(INextBot *nextbot);
	void UnRegister(INextBot *nextbot);
};


int ENTINDEX(const CBaseEntity *pEntity);

NextBotManager& TheNextBots();


inline PathFollower *New_PathFollower()
{
	auto path = reinterpret_cast<PathFollower *>(::operator new(0x5000));
	(path->*Thunks::PathFollower_ctor)();
	
	return path;
}


#include "NextBot/NextBotPath.h"


template<typename PathCost> bool Path::Compute(INextBot *nextbot, const Vector& goal, PathCost& cost_func, float maxPathLength, bool b1)
{
	return (this->*Thunks::Path_Compute_goal)(nextbot, goal, cost_func, maxPathLength, b1);
}

template<typename PathCost> bool Path::Compute(INextBot *nextbot, CBaseCombatCharacter *subject, PathCost& cost_func, float maxPathLength, bool b1)
{
	return (this->*Thunks::Path_Compute_subject)(nextbot, subject, cost_func, maxPathLength, b1);
}


#endif // _INCLUDE_THUNKS_H_
