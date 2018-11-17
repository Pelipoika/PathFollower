#include "sp_callbacks.h"
#include "context.h"
#include "util.h"

void PFCallbacks::Call_Approach(const Vector& dst)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_Approach];
	if (func == nullptr) return;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	cell_t c_dst[3];
	VectorToCells(dst, c_dst);
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushArray(c_dst, 3);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_Approach: got error %d\n", err);
	}
}

void PFCallbacks::Call_OnMoveToSuccess(const Path *path)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_OnMoveToSuccess];
	if (func == nullptr) return;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushCell((cell_t)path);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_OnMoveToSuccess: got error %d\n", err);
	}
}

void PFCallbacks::Call_OnMoveToFailure(const Path *path, MoveToFailureType fail)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_OnMoveToFailure];
	if (func == nullptr) return;

	PluginBot *bot = this->GetPluginBot();

	cell_t result = 0;

	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushCell((cell_t)path);
	func->PushCell((cell_t)fail);
	int err = func->Execute(&result);

	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_OnMoveToFailure: got error %d\n", err);
	}
}

void PFCallbacks::Call_OnActorEmoted(CBaseCombatCharacter* who, int concept)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_OnActorEmoted];
	if (func == nullptr) return;

	PluginBot *bot = this->GetPluginBot();

	cell_t result = 0;

	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushCell(ENTINDEX(who));
	func->PushCell((cell_t)concept);
	int err = func->Execute(&result);

	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_OnActorEmoted: got error %d\n", err);
	}
}

bool PFCallbacks::Call_ClimbUpToLedge(const Vector& dst, const Vector& dir)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_ClimbUpToLedge];
	if (func == nullptr) return false;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	cell_t c_dst[3];
	VectorToCells(dst, c_dst);
	
	cell_t c_dir[3];
	VectorToCells(dir, c_dir);
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushArray(c_dst, 3);
	func->PushArray(c_dir, 3);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_ClimbUpToLedge: got error %d\n", err);
	}

	return (result != 0);
}

bool PFCallbacks::Call_IsEntityTraversable(CBaseEntity *ent, ILocomotion::TraverseWhenType when)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_IsEntityTraversable];
	if (func == nullptr) return false;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushCell(ENTINDEX(ent));
	func->PushCell((cell_t)when);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_IsEntityTraversable: got error %d\n", err);
	}
	
	return (result != 0);
}

float PFCallbacks::Call_GetPathCost(CNavArea *area, CNavArea *fromArea, float length)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_GetPathCost];
	if (func == nullptr) return -1.0f;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	func->PushCell((cell_t)area);
	func->PushCell((cell_t)fromArea);
	func->PushFloat(length);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_GetPathCost: got error %d\n", err);
	}
	
	return sp_ctof(result);
}

void PFCallbacks::Call_PathFailed()
{
	IPluginFunction *func = this->m_Callbacks[PFCB_PathFailed];
	if (func == nullptr) return;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_PathFailed: got error %d\n", err);
	}
}

void PFCallbacks::Call_PathSuccess()
{
	IPluginFunction *func = this->m_Callbacks[PFCB_PathSuccess];
	if (func == nullptr) return;
	
	PluginBot *bot = this->GetPluginBot();
	
	cell_t result = 0;
	
	func->PushCell(ENTINDEX(bot->GetEntity()));
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_PathSuccess: got error %d\n", err);
	}
}

void PFCallbacks::Call_EntityGone(int entidx)
{
	IPluginFunction *func = this->m_Callbacks[PFCB_EntityGone];
	if (func == nullptr) return;
	
	cell_t result = 0;
	
	func->PushCell(entidx);
	int err = func->Execute(&result);
	
	if (err != SP_ERROR_NONE) {
		Warning("PFCallbacks::Call_EntityGone: got error %d\n", err);
	}
}


PluginBot *PFCallbacks::GetPluginBot() const
{
	const PFContext *ctx = static_cast<const PFContext *>(this);
	return ctx->m_pPluginBot;
}
