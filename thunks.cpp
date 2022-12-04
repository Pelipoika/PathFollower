#include "thunks.h"
#include "Nav/nav_mesh.h"
#include "find_stuff.h"
#include "sdk/smsdk_ext.h"


#define TRY_LOAD_THUNK(signame, ptr) \
	if (!g_pGameConf->GetMemSig(signame, reinterpret_cast<void **>(&ptr)) || ptr == nullptr) { \
		g_pSM->LogError(myself, "Failed to locate memory address based on signature for \"" signame "\"!"); \
		success = false; \
	} else { \
		g_pSM->LogMessage(myself, "Found \"" signame "\" @ 0x%08x", *reinterpret_cast<uintptr_t *>(&ptr)); \
	}

#define TRY_LOAD_VTIDX(offname, vtidx) \
	if (!g_pGameConf->GetOffset(offname, &vtidx)) { \
		g_pSM->LogError(myself, "Failed to find vtable index for \"" offname "\"!"); \
		success = false; \
	} else { \
		g_pSM->LogMessage(myself, "Found \"" offname "\""); \
	}

#define TRY_LOAD_STOFF(offname, stoff) \
	if (!g_pGameConf->GetOffset(offname, &stoff)) { \
		g_pSM->LogError(myself, "Failed to find struct offset for \"" offname "\"!"); \
		success = false; \
	}


#define RETURN_SENDPROP_OFFSET(type, sv_class, name) \
	static ptrdiff_t offset = -1; \
	if (offset == -1) { \
		sm_sendprop_info_t info; \
		if (!gamehelpers->FindSendPropInfo(#sv_class, #name, &info)) { \
			g_pSM->LogError(myself, "Failed to locate offset for " #sv_class "::" #name "!"); \
			abort(); \
		} \
		offset = info.prop->GetOffset(); \
	} \
	return *reinterpret_cast<type *>((uintptr_t)this + offset);

/* special case for e.g. structs inside a class, and you don't want to dereference the pointer */
#define RETURN_SENDPROP_OFFSET_NODEREF(type, sv_class, name) \
	static ptrdiff_t offset = -1; \
	if (offset == -1) { \
		sm_sendprop_info_t info; \
		if (!gamehelpers->FindSendPropInfo(#sv_class, #name, &info)) { \
			g_pSM->LogError(myself, "Failed to locate offset for " #sv_class "::" #name "!"); \
			abort(); \
		} \
		offset = info.prop->GetOffset(); \
	} \
	return reinterpret_cast<type *>((uintptr_t)this + offset);


CNavMesh *TheNavMesh = nullptr;


namespace Thunks
{
	const Vector& (CBaseEntity:: *CBaseEntity_GetAbsOrigin)() const = nullptr;
	
//	void (CBaseCombatCharacter:: *CBaseCombatCharacter_UpdateLastKnownArea)() = nullptr;
	
	bool (Path:: *Path_Compute_goal)(INextBot *, const Vector&, IPathCost&, float, bool) = nullptr;
	bool (Path:: *Path_Compute_subject)(INextBot *, CBaseCombatCharacter *, IPathCost&, float, bool) = nullptr;
	
	void (PathFollower:: *PathFollower_ctor)() = nullptr;
	
	bool (ILocomotion:: *ILocomotion_IsPotentiallyTraversable)(const Vector&, const Vector&, ILocomotion::TraverseWhenType, float *) const = nullptr;
	bool (ILocomotion:: *ILocomotion_HasPotentialGap)(const Vector&, const Vector&, float *) const = nullptr;
	bool (ILocomotion:: *ILocomotion_IsGap)(const Vector&, const Vector&) const = nullptr;

	bool (ILocomotion:: *CTFBotLocomotion_IsAreaTraversable)(const CNavArea *) const = nullptr;
	
	NextBotManager& (*TheNextBots)() = nullptr;
	int (NextBotManager:: *NextBotManager_Register)(INextBot *) = nullptr;
	void (NextBotManager:: *NextBotManager_UnRegister)(INextBot *) = nullptr;
	
	CNavArea * (CNavMesh:: *CNavMesh_GetNavArea)(const Vector&, float) const = nullptr;
	CNavArea * (CNavMesh:: *CNavMesh_GetNearestNavArea)(const Vector&, bool, float, bool, bool, int) const = nullptr;
	
	int vtidx_CBaseEntity_MyCombatCharacterPointer = -1;
	
	int vtidx_CBaseCombatCharacter_UpdateLastKnownArea = -1;
	int vtidx_CBaseCombatCharacter_GetLastKnownArea = -1;
	
	bool Init()
	{
		bool success = true;
		
		TRY_LOAD_THUNK("CBaseEntity::GetAbsOrigin", CBaseEntity_GetAbsOrigin);
		
	//	TRY_LOAD_THUNK("CBaseCombatCharacter::UpdateLastKnownArea", CBaseCombatCharacter_UpdateLastKnownArea);
		
#if defined _WINDOWS
		{
			uintptr_t addr = Find_Path_Compute_IPathCost_goal();
			if (addr != 0) {
				*reinterpret_cast<uintptr_t *>(&Path_Compute_goal) = addr;
			} else {
				success = false;
			}
		}
		{
			uintptr_t addr = Find_Path_Compute_IPathCost_subject();
			if (addr != 0) {
				*reinterpret_cast<uintptr_t *>(&Path_Compute_subject) = addr;
			} else {
				success = false;
			}
		}
		
		{
			uintptr_t addr = Find_TheNavMesh();
			if (addr != 0) {
				*reinterpret_cast<uintptr_t *>(&TheNavMesh) = addr;
			} else {
				success = false;
			}
		}
#else
		TRY_LOAD_THUNK("Path::Compute<IPathCost> [goal]",    Path_Compute_goal);
		TRY_LOAD_THUNK("Path::Compute<IPathCost> [subject]", Path_Compute_subject);
		
		TRY_LOAD_THUNK("TheNavMesh", TheNavMesh);
#endif
		
		TRY_LOAD_THUNK("PathFollower::PathFollower [C1]", PathFollower_ctor);
		
		TRY_LOAD_THUNK("ILocomotion::IsPotentiallyTraversable", ILocomotion_IsPotentiallyTraversable);

		TRY_LOAD_THUNK("ILocomotion::HasPotentialGap", ILocomotion_HasPotentialGap);

		TRY_LOAD_THUNK("ILocomotion::IsGap", ILocomotion_IsGap);
		
		TRY_LOAD_THUNK("CTFBotLocomotion::IsAreaTraversable", CTFBotLocomotion_IsAreaTraversable);
		
		TRY_LOAD_THUNK("TheNextBots",                TheNextBots);
		TRY_LOAD_THUNK("NextBotManager::Register",   NextBotManager_Register);
		TRY_LOAD_THUNK("NextBotManager::UnRegister", NextBotManager_UnRegister);
		
		TRY_LOAD_THUNK("CNavMesh::GetNavArea [Vec]",        CNavMesh_GetNavArea);
		TRY_LOAD_THUNK("CNavMesh::GetNearestNavArea [Vec]", CNavMesh_GetNearestNavArea);
		
		TRY_LOAD_VTIDX("CBaseEntity::MyCombatCharacterPointer", vtidx_CBaseEntity_MyCombatCharacterPointer);

		TRY_LOAD_VTIDX("CBaseCombatCharacter::UpdateLastKnownArea", vtidx_CBaseCombatCharacter_UpdateLastKnownArea);
		TRY_LOAD_VTIDX("CBaseCombatCharacter::GetLastKnownArea", vtidx_CBaseCombatCharacter_GetLastKnownArea);		

		return success;
	}
}


ICollideable *CBaseEntity::CollisionProp()
{
	RETURN_SENDPROP_OFFSET_NODEREF(ICollideable, CBaseEntity, m_Collision);
}
IServerNetworkable *CBaseEntity::NetworkProp()
{
	return static_cast<IServerUnknown *>(this)->GetNetworkable();
}
const Vector& CBaseEntity::GetAbsOrigin() const
{
#if defined _WINDOWS
	return (this->*Thunks::CBaseEntity_GetAbsOrigin)();
#else
	static ptrdiff_t offset = -1;
	if (offset == -1)
	{
		sm_dataprop_info_t info;
		if (!gamehelpers->FindDataMapOffs("CBaseEntity", "m_vecAbsOrigin", &info))
		{
			g_pSM->LogError(myself, "Failed to locate offset for " "CBaseEntity" "::" "m_vecAbsOrigin" "!");
			abort();
		}
		offset = info.prop->GetOffset();
	}

	return (*reinterpret_cast<Vector *>((uintptr_t)this + offset));
#endif
}
int CBaseEntity::GetTeamNumber() const
{
	RETURN_SENDPROP_OFFSET(int, CBaseEntity, m_iTeamNum);
}
CBaseEntity *CBaseEntity::GetGroundEntity() const
{
	RETURN_SENDPROP_OFFSET(CBaseEntity *, CBasePlayer, m_hGroundEntity);
}
bool CBaseEntity::IsAlive()
{
	static ptrdiff_t offset = -1;
	if (offset == -1)
	{
		sm_sendprop_info_t info;
		if (!gamehelpers->FindSendPropInfo("CBaseEntity", "m_lifeState", &info))
		{
			g_pSM->LogError(myself, "Failed to locate offset for " "CBaseEntity" "::" "m_lifeState" "!");
			abort();
		}
		offset = info.prop->GetOffset();
	}

	return (*reinterpret_cast<int *>((uintptr_t)this + offset) == LIFE_ALIVE);
}
const char *CBaseEntity::GetClassname() const
{
	static char buf[1024];
	assert(servertools->GetKeyValue(const_cast<CBaseEntity *>(this), "classname", buf, sizeof(buf)));
	
	return buf;
}
bool CBaseEntity::IsCombatCharacter() const
{
	uintptr_t *vtable = *reinterpret_cast<uintptr_t **>(const_cast<CBaseEntity *>(this));
	auto MyCombatCharacterPointer = *reinterpret_cast<CBaseCombatCharacter * (CBaseEntity::**)() const>(vtable + Thunks::vtidx_CBaseEntity_MyCombatCharacterPointer);
	
	return ((this->*MyCombatCharacterPointer)() != nullptr);
}

CBaseCombatCharacter *CBaseEntity::MyCombatCharacterPointer()
{
	uintptr_t *vtable = *reinterpret_cast<uintptr_t **>(const_cast<CBaseEntity *>(this));
	auto MyCombatCharacterPointer = *reinterpret_cast<CBaseCombatCharacter * (CBaseEntity::**)() const>(vtable + Thunks::vtidx_CBaseEntity_MyCombatCharacterPointer);
	
	return (this->*MyCombatCharacterPointer)();
}

void CBaseCombatCharacter::UpdateLastKnownArea()
{
	uintptr_t *vtable = *reinterpret_cast<uintptr_t **>(const_cast<CBaseCombatCharacter *>(this));
	auto UpdateLastKnownArea = *reinterpret_cast<void (CBaseCombatCharacter::**)(void)>(vtable + Thunks::vtidx_CBaseCombatCharacter_UpdateLastKnownArea);

	(this->*UpdateLastKnownArea)();
}

CNavArea *CBaseCombatCharacter::GetLastKnownArea() 
{
DevMsg("CBaseCombatCharacter::GetLastKnownArea()\n");

	uintptr_t *vtable = *reinterpret_cast<uintptr_t **>(const_cast<CBaseCombatCharacter *>(this));
	auto GetLastKnownArea = *reinterpret_cast<CNavArea * (CBaseCombatCharacter::**)(void)>(vtable + Thunks::vtidx_CBaseCombatCharacter_GetLastKnownArea);

	return (this->*GetLastKnownArea)();
}

int ENTINDEX(const CBaseEntity *pEntity)
{
	if (pEntity == nullptr) return 0;
	
	return engine->IndexOfEdict(const_cast<CBaseEntity *>(pEntity)->NetworkProp()->GetEdict());
}


NextBotManager& TheNextBots()
{
	return Thunks::TheNextBots();
}
int NextBotManager::Register(INextBot *nextbot)
{
	return (this->*Thunks::NextBotManager_Register)(nextbot);
}
void NextBotManager::UnRegister(INextBot *nextbot)
{
	(this->*Thunks::NextBotManager_UnRegister)(nextbot);
}


CNavArea *CNavMesh::GetNavArea(const Vector& pos, float beneathLimit) const
{
	return (this->*Thunks::CNavMesh_GetNavArea)(pos, beneathLimit);
}
CNavArea *CNavMesh::GetNearestNavArea(const Vector& pos, bool anyZ, float maxDist, bool checkLOS, bool checkGround, int team) const
{
	return (this->*Thunks::CNavMesh_GetNearestNavArea)(pos, anyZ, maxDist, checkLOS, checkGround, team);
}
