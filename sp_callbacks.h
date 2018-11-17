#ifndef _INCLUDE_SP_CALLBACKS_H_
#define _INCLUDE_SP_CALLBACKS_H_


#include "extension.h"
#include "thunks.h"

class CBaseEntity;
class Vector;
class PluginBot;
class CNavArea;
class CNavLadder;
class Path;

enum PFCallbackType
{
	PFCB_Approach            = 0,
	PFCB_IsEntityTraversable = 1,
	PFCB_GetPathCost         = 2,
	PFCB_PathFailed          = 3,
	PFCB_PathSuccess         = 4,
	PFCB_EntityGone          = 5,
	PFCB_ClimbUpToLedge      = 6,
	PFCB_OnMoveToSuccess     = 7,
	PFCB_OnMoveToFailure     = 8,
	PFCB_OnActorEmoted       = 9,
	
	PFCB_NumCallbacks,
};

enum MoveToFailureType;

class PFCallbacks
{
public:
	void RegisterCallback(PFCallbackType type, IPluginFunction *func)
	{
		assert(type >= 0 && type < PFCB_NumCallbacks);
		assert(func != nullptr);
		
		this->m_Callbacks[type] = func;
	}
	
	void UnRegisterCallback(PFCallbackType type)
	{
		assert(type >= 0 && type < PFCB_NumCallbacks);
		
		this->m_Callbacks[type] = nullptr;
	}
	
	bool IsCallbackRegistered(PFCallbackType type) const
	{
		assert(type >= 0 && type < PFCB_NumCallbacks);
		
		return (this->m_Callbacks[type] != nullptr);
	}
	
	IPlugin *GetPlugin() const { return this->m_pPlugin; }
	
	void  Call_Approach           (const Vector& dst);
	bool  Call_ClimbUpToLedge     (const Vector& dst, const Vector& dir);
	bool  Call_IsEntityTraversable(CBaseEntity *ent, ILocomotion::TraverseWhenType when);
	float Call_GetPathCost        (CNavArea *area, CNavArea *fromArea, float length);
	void  Call_PathFailed         ();
	void  Call_PathSuccess        ();
	void  Call_EntityGone         (int entidx);
	void  Call_OnMoveToSuccess    (const Path *path);
	void  Call_OnMoveToFailure    (const Path *path, MoveToFailureType fail);
	void  Call_OnActorEmoted      (CBaseCombatCharacter* who, int concept);
	
protected:
	PFCallbacks(IPlugin *plugin) :
		m_pPlugin(plugin) {}
	
private:
	PluginBot *GetPluginBot() const;
	
	IPlugin *m_pPlugin;
	IPluginFunction *m_Callbacks[PFCB_NumCallbacks] = {};
};


#endif // _INCLUDE_SP_CALLBACKS_H_
