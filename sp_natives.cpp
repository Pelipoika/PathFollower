#include "sp_natives.h"
#include "thunks.h"
#include "simulate.h"
#include "util.h"

#include "Nav/nav_mesh.h"

#include "PluginBot/PluginBotLocomotion.h"
#include "PluginBot/PluginBotBody.h"
#include "PluginBot/PluginBotPathCost.h"

static cell_t PF_UpdateLastKnownArea(IPluginContext *pCtx, const cell_t *params)
{
	int goal_entidx = params[1];
	
//	DevMsg("PF_UpdateLastKnownArea: %d\n", goal_entidx);
	
	CBaseEntity *goal_ent = servertools->GetBaseEntityByEntIndex(goal_entidx);
	
	if (goal_ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", goal_entidx);
		return 0;
	}
	
	if (!goal_ent->IsCombatCharacter()) {
		pCtx->ReportError("Entity #%d (classname \"%s\") isn't a derived type of CBaseCombatCharacter!",
			goal_entidx, goal_ent->GetClassname());
		return 0;
	}
	
	auto goal_bcc = static_cast<CBaseCombatCharacter *>(goal_ent);
	goal_bcc->UpdateLastKnownArea();
	
	return 0;
}

static cell_t PF_GetLastKnownArea(IPluginContext *pCtx, const cell_t *params)
{
	int goal_entidx = params[1];
	
//	DevMsg("PF_GetLastKnownArea: %d\n", goal_entidx);
	
	CBaseEntity *goal_ent = servertools->GetBaseEntityByEntIndex(goal_entidx);
	
	if (goal_ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", goal_entidx);
		return 0;
	}
	
	if (!goal_ent->IsCombatCharacter()) {
		pCtx->ReportError("Entity #%d (classname \"%s\") isn't a derived type of CBaseCombatCharacter!",
			goal_entidx, goal_ent->GetClassname());
		return 0;
	}
	
	auto goal_bcc = static_cast<CBaseCombatCharacter *>(goal_ent);
	return (cell_t)goal_bcc->GetLastKnownArea();
}

static cell_t PF_Create(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx               = params[1];
	float step_height            = sp_ctof(params[2]);
	float max_jump_height        = sp_ctof(params[3]);
	float death_drop_height      = sp_ctof(params[4]);
	float slope_limit            = sp_ctof(params[5]);
	unsigned int body_solid_mask = params[6];
	float look_ahead_distance    = sp_ctof(params[7]);
	float repath_interval        = sp_ctof(params[8]);
	float hull_scale_xy          = sp_ctof(params[9]);
	float hull_scale_z           = sp_ctof(params[10]);
	
//	DevMsg("PF_Create: %d %f %f %f %f %08x %f %f\n",
//		bot_entidx, step_height, max_jump_height, death_drop_height, slope_limit,
//		body_solid_mask, look_ahead_distance, repath_interval);
	
	if (bots.find(bot_entidx) != bots.end()) {
		pCtx->ReportError("PathFollower already registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	CBaseEntity *bot_ent = servertools->GetBaseEntityByEntIndex(bot_entidx);
	
	if (bot_ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", bot_entidx);
		return 0;
	}
	
	if (!bot_ent->IsCombatCharacter()) {
		pCtx->ReportError("Entity #%d (classname \"%s\") isn't a derived type of CBaseCombatCharacter!",
			bot_entidx, bot_ent->GetClassname());
		return 0;
	}
	
	IPlugin *pPlugin = plsys->FindPluginByContext(pCtx->GetContext());
	if (pPlugin == nullptr) {
		pCtx->ReportError("Failed to find an IPlugin pointer for the calling plugin!");
		return 0;
	}
	
	PFContext& ctx = (*(bots.emplace(bot_entidx, PFContext(pPlugin)).first)).second;
	ctx.m_pPluginBot       = new PluginBot(&ctx, static_cast<CBaseCombatCharacter *>(bot_ent));
	ctx.m_pPathFollower    = New_PathFollower();
	ctx.m_flRePathInterval = repath_interval;
	
	ctx.m_pPathFollower->SetMinLookAheadDistance(look_ahead_distance);
	
	auto loco = static_cast<PluginBotLocomotion *>(ctx.m_pPluginBot->GetLocomotionInterface());
	loco->SetStepHeight     (step_height);
	loco->SetMaxJumpHeight  (max_jump_height);
	loco->SetDeathDropHeight(death_drop_height);
	loco->SetSlopeLimit     (slope_limit);
	
	auto body = static_cast<PluginBotBody *>(ctx.m_pPluginBot->GetBodyInterface());
	body->SetSolidMask(body_solid_mask);
	body->SetHullScaleX(hull_scale_xy);
	body->SetHullScaleY(hull_scale_xy);
	body->SetHullScaleZ(hull_scale_z);
	
	return 0;
}

static cell_t PF_Destroy(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_Destroy: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	auto it = bots.find(bot_entidx);
	bots.erase(it);
	
	return 0;
}

static cell_t PF_Exists(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_Exists: %d\n", bot_entidx);
	
	return (bots.find(bot_entidx) != bots.end());
}


static cell_t PF_SetGoalVector(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx =           params[1];
	Vector vec     = GetVector(params[2], pCtx);
	
//	DevMsg("PF_SetGoalVector: %d %f %f %f\n", bot_entidx, vec.x, vec.y, vec.z);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.m_bGoalVector = true;
	ctx.m_bGoalEntity = false;
	ctx.m_vecGoal     = vec;
	
	return 0;
}

static cell_t PF_GetGoalVector(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];

	//	DevMsg("PF_SetGoalVector: %d %f %f %f\n", bot_entidx, vec.x, vec.y, vec.z);

	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}

	PFContext& ctx = bots.at(bot_entidx);
	SetVector(params[2], pCtx, ctx.m_vecGoal);

	return 0;
}

static cell_t PF_SetGoalEntity(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx  = params[1];
	int goal_entidx = params[2];
	
//	DevMsg("PF_SetGoalEntity: %d %d\n", bot_entidx, goal_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	CBaseEntity *goal_ent = servertools->GetBaseEntityByEntIndex(goal_entidx);
	
	if (goal_ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", goal_entidx);
		return 0;
	}
	
	if (!goal_ent->IsCombatCharacter()) {
		pCtx->ReportError("Entity #%d (classname \"%s\") isn't a derived type of CBaseCombatCharacter!",
			goal_entidx, goal_ent->GetClassname());
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.m_bGoalVector = false;
	ctx.m_bGoalEntity = true;
	ctx.m_hGoal       = static_cast<CBaseCombatCharacter *>(goal_ent);
	
	return 0;
}


static cell_t PF_IsPathToVectorPossible(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx =           params[1];
	Vector vec     = GetVector(params[2], pCtx);
	
//	DevMsg("PF_IsPathToVectorPossible: %d %f %f %f\n", bot_entidx, vec.x, vec.y, vec.z);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return false;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	if (ctx.m_pPluginBot->GetEntity() == nullptr) return false;
	
	ctx.m_pPluginBot->GetEntity()->UpdateLastKnownArea();
	
	PathFollower *temp_path = New_PathFollower();

	//Not using custom pathcost func because this functions needs to be raped alot and asking SP stuff is too slow.
	IPathCost *cost_func = new PluginBotPathCost(ctx.m_pPluginBot);//ctx.GetPathCostFunctor();
	
	bool success = temp_path->Compute(ctx.m_pPluginBot, vec, *cost_func);
	
	cell_t *pLength;
	int err;
	if ((err = pCtx->LocalToPhysAddr(params[3], &pLength)) == SP_ERROR_NONE)
	{
		*pLength = sp_ftoc(temp_path->GetLength());
	}

	delete cost_func;
	delete temp_path;
	
	return success;
}

static cell_t PF_IsPathToEntityPossible(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx  = params[1];
	int goal_entidx = params[2];
	
//	DevMsg("PF_IsPathToEntityPossible: %d %d\n", bot_entidx, goal_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return false;
	}
	
	CBaseEntity *goal_ent = servertools->GetBaseEntityByEntIndex(goal_entidx);
	
	if (goal_ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", goal_entidx);
		return false;
	}
	
	if (!goal_ent->IsCombatCharacter()) {
		pCtx->ReportError("Entity #%d (classname \"%s\") isn't a derived type of CBaseCombatCharacter!",
			goal_entidx, goal_ent->GetClassname());
		return false;
	}
	
	auto goal_bcc = static_cast<CBaseCombatCharacter *>(goal_ent);
	
	PFContext& ctx = bots.at(bot_entidx);
	if (ctx.m_pPluginBot->GetEntity() == nullptr) return false;
	
	ctx.m_pPluginBot->GetEntity()->UpdateLastKnownArea();
	goal_bcc->UpdateLastKnownArea();
	
	PathFollower *temp_path = New_PathFollower();
	//Not using custom pathcost func because this functions needs to be raped alot and asking SP stuff is too slow.
	IPathCost *cost_func = new PluginBotPathCost(ctx.m_pPluginBot);//ctx.GetPathCostFunctor();
	
	bool success = temp_path->Compute(ctx.m_pPluginBot, goal_bcc, *cost_func);
	
	cell_t *pLength;
	int err;
	if ((err = pCtx->LocalToPhysAddr(params[3], &pLength)) == SP_ERROR_NONE)
	{
		*pLength = sp_ftoc(temp_path->GetLength());
	}
	
	delete cost_func;
	delete temp_path;
	
	return success;
}

static cell_t PF_StartPathing(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_StartPathing: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.m_bPathing = true;
	
	return 0;
}

static cell_t PF_StopPathing(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_StopPathing: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.m_bPathing = false;
	
	return 0;
}


static cell_t PF_GetFutureSegment(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	int num        = params[2];
	
//	DevMsg("PF_GetFutureSegment: %d %d\n", bot_entidx, num);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	const Path::Segment *seg = ctx.m_pPathFollower->GetCurrentGoal();
	for (int i = 0; i < num; ++i) {
		seg = ctx.m_pPathFollower->NextSegment(seg);
	}
	
	if (seg == nullptr) {
		return false;
	}
	
	SetVector(params[3], pCtx, seg->pos);
	
	return true;
}

static cell_t PF_IsDiscontinuityAhead(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx         = params[1];
	Path::SegmentType type = (Path::SegmentType)params[2];
	float range            = sp_ctof(params[3]);
	
//	DevMsg("PF_IsDiscontinuityAhead: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return false;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	if (ctx.m_pPluginBot == nullptr) 
		return false;

	const auto m_goal = ctx.m_pPathFollower->GetCurrentGoal();

	if ( m_goal )
	{
		const auto current = ctx.m_pPathFollower->PriorSegment( m_goal );
		if ( current && current->type == type )
		{
			// we're on the discontinuity now
			return true;
		}

		float rangeSoFar = ( m_goal->pos - ctx.m_pPluginBot->GetLocomotionInterface()->GetFeet() ).Length();

		for( auto s = m_goal; s; s = ctx.m_pPathFollower->NextSegment( s ) )
		{
			if ( rangeSoFar >= range )
			{
				break;
			}

			if ( s->type == type )
			{
				return true;
			}

			rangeSoFar += s->length;
		}
	}

	return false;
}

static cell_t PF_IsPotentiallyTraversable(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx        = params[1];
	Vector from           = GetVector(params[2], pCtx);
	Vector to             = GetVector(params[3], pCtx);
	ILocomotion::TraverseWhenType when = (ILocomotion::TraverseWhenType)(params[4]);

//	DevMsg("PF_IsPotentiallyTraversable: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) 
	{
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	if (ctx.m_pPluginBot == nullptr) 
		return 0;
	
	float fraction;
	bool bResult = ctx.m_pPluginBot->GetLocomotionInterface()->IsPotentiallyTraversable(from, to, when, &fraction);

	cell_t *pFraction;
	int err;
	if ((err = pCtx->LocalToPhysAddr(params[5], &pFraction)) == SP_ERROR_NONE)
	{
		*pFraction = sp_ftoc(fraction);
	}

	return bResult;
}

static cell_t PF_HasPotentialGap(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx        = params[1];
	Vector from           = GetVector(params[2], pCtx);
	Vector to             = GetVector(params[3], pCtx);

//	DevMsg("PF_HasPotentialGap: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) 
	{
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	if (ctx.m_pPluginBot == nullptr) 
		return 0;
	
	float fraction;
	bool bResult = ctx.m_pPluginBot->GetLocomotionInterface()->HasPotentialGap(from, to, &fraction);

	cell_t *pFraction;
	int err;
	if ((err = pCtx->LocalToPhysAddr(params[4], &pFraction)) == SP_ERROR_NONE)
	{
		*pFraction = sp_ftoc(fraction);
	}

	return bResult;
}

static cell_t PF_GetPtr_PluginBot(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_GetPtr_PluginBot: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	return (cell_t)ctx.m_pPluginBot;
}

static cell_t PF_GetPtr_PluginBotLocomotion(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_GetPtr_PluginBotLocomotion: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	if (ctx.m_pPluginBot == nullptr) return (cell_t)nullptr;
	return (cell_t)ctx.m_pPluginBot->GetLocomotionInterface();
}

static cell_t PF_GetPtr_PluginBotBody(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_GetPtr_PluginBotBody: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	if (ctx.m_pPluginBot == nullptr) return (cell_t)nullptr;
	return (cell_t)ctx.m_pPluginBot->GetBodyInterface();
}

static cell_t PF_GetPtr_PluginBotIntention(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx = params[1];
	
//	DevMsg("PF_GetPtr_PluginBotIntention: %d\n", bot_entidx);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	
	if (ctx.m_pPluginBot == nullptr) return (cell_t)nullptr;
	return (cell_t)ctx.m_pPluginBot->GetIntentionInterface();
}

static cell_t PF_IsEntityACombatCharacter(IPluginContext *pCtx, const cell_t *params)
{
	int entidx = params[1];
	
//	DevMsg("PF_IsEntityACombatCharacter: %d\n", entidx);
	
	CBaseEntity *ent = servertools->GetBaseEntityByEntIndex(entidx);
	
	if (ent == nullptr) {
		pCtx->ReportError("Entity #%d doesn't seem to actually exist!", entidx);
		return 0;
	}
	
	return (ent->IsCombatCharacter());
}


static cell_t PF_EnableCallback(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx      =                 params[1];
	auto cb_type        = (PFCallbackType)params[2];
	funcid_t cb_func_id =                 params[3];
	
//	DevMsg("PF_EnableCallback: %d %d %" PRIu32 "\n", bot_entidx, cb_type, cb_func_id);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	if (cb_type < 0 || cb_type >= PFCB_NumCallbacks) {
		pCtx->ReportError("PFCallbackType %d isn't valid!", cb_type);
		return 0;
	}
	
	IPluginFunction *cb_func = pCtx->GetFunctionById(cb_func_id);
	if (cb_func == nullptr) {
		pCtx->ReportError("Invalid or nonexistent callback function specified!");
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.RegisterCallback(cb_type, cb_func);
	
	return 0;
}

static cell_t PF_DisableCallback(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx =                 params[1];
	auto cb_type   = (PFCallbackType)params[2];
	
//	DevMsg("PF_DisableCallback: %d %d\n", bot_entidx, cb_type);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	if (cb_type < 0 || cb_type >= PFCB_NumCallbacks) {
		pCtx->ReportError("PFCallbackType %d isn't valid!", cb_type);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	ctx.UnRegisterCallback(cb_type);
	
	return 0;
}

static cell_t PF_IsCallbackEnabled(IPluginContext *pCtx, const cell_t *params)
{
	int bot_entidx =                 params[1];
	auto cb_type   = (PFCallbackType)params[2];
	
//	DevMsg("PF_IsCallbackEnabled: %d %d\n", bot_entidx, cb_type);
	
	if (bots.find(bot_entidx) == bots.end()) {
		pCtx->ReportError("PathFollower not yet registered for entity #%d!", bot_entidx);
		return 0;
	}
	
	if (cb_type < 0 || cb_type >= PFCB_NumCallbacks) {
		pCtx->ReportError("PFCallbackType %d isn't valid!", cb_type);
		return 0;
	}
	
	PFContext& ctx = bots.at(bot_entidx);
	return ctx.IsCallbackRegistered(cb_type);
}


static cell_t NavArea_GetPlayerCount(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	int team  =                   params[2];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return area->GetPlayerCount(team);
}

static cell_t NavArea_HasAttributes(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	int bits  =                   params[2];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return area->HasAttributes(bits);
}

static cell_t NavArea_GetID(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return area->GetID();
}

static cell_t NavArea_GetZ(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	float x = sp_ctof(params[2]);
	float y = sp_ctof(params[3]);
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return sp_ftoc(area->GetZ(x, y));
}

static cell_t NavArea_ComputeNormal(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];

	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}

	bool alternate = (params[3] != 0);

	Vector normal;
	area->ComputeNormal(&normal, alternate);

	SetVector(params[2], pCtx, normal);

	return 0;
}

static cell_t NavArea_GetCorner(IPluginContext *pCtx, const cell_t *params)
{
	auto area            = (const CNavArea *)params[1];
	NavCornerType corner = (NavCornerType)params[3];

	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	SetVector(params[2], pCtx, area->GetCorner(corner));

	return 0;
}

static cell_t NavArea_GetCenter(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	SetVector(params[2], pCtx, area->GetCenter());
	
	return 0;
}

static cell_t NavArea_GetAdjacentCount(IPluginContext *pCtx, const cell_t *params)
{
	auto area      = (const CNavArea *)params[1];
	NavDirType dir = (NavDirType)params[2];

	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return area->GetAdjacentCount(dir);
}

static cell_t NavArea_GetAdjacentArea(IPluginContext *pCtx, const cell_t *params)
{
	auto area      = (const CNavArea *)params[1];
	NavDirType dir = (NavDirType)params[2];
	int index      = params[3];

	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}

	return (cell_t)area->GetAdjacentArea(dir, index);
}

static cell_t NavArea_GetRandomPoint(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	SetVector(params[2], pCtx, area->GetRandomPoint());
	
	return 0;
}

static cell_t NavArea_GetCostSoFar(IPluginContext *pCtx, const cell_t *params)
{
	auto area = (const CNavArea *)params[1];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	return sp_ftoc(area->GetCostSoFar());
}

static cell_t NavArea_ComputeAdjacentConnectionHeightChange(IPluginContext *pCtx, const cell_t *params)
{
	auto area            = (const CNavArea *)params[1];
	auto destinationArea = (const CNavArea *)params[2];
	
	if (area == nullptr) {
		pCtx->ReportError("NavArea (this) is null!");
		return 0;
	}
	
	if (destinationArea == nullptr) {
		pCtx->ReportError("NavArea (destinationArea) is null!");
		return 0;
	}
	
	return sp_ftoc(area->ComputeAdjacentConnectionHeightChange(destinationArea));
}


static cell_t NavMesh_GetNavArea_Vec(IPluginContext *pCtx, const cell_t *params)
{
	auto mesh          = (const CNavMesh *)params[1];
	Vector pos         =         GetVector(params[2], pCtx);
	float beneathLimit =           sp_ctof(params[3]);
	
	return (cell_t)TheNavMesh->GetNavArea(pos, beneathLimit);
}

static cell_t NavMesh_GetNearestNavArea_Vec(IPluginContext *pCtx, const cell_t *params)
{
	auto mesh        = (const CNavMesh *)params[1];
	Vector pos       =         GetVector(params[2], pCtx);
	bool anyZ        =                  (params[3] != 0);
	float maxDist    =           sp_ctof(params[4]);
	bool checkLOS    =                  (params[5] != 0);
	bool checkGround =                  (params[6] != 0);
	int team         =                   params[7];
	
	return (cell_t)TheNavMesh->GetNearestNavArea(pos, anyZ, maxDist, checkLOS, checkGround, team);
}

#if 0
static cell_t NavMesh_CollectSurroundingAreas(IPluginContext *pCtx, const cell_t *params)
{
	auto mesh                 = (const CNavMesh *)params[1];
	auto startArea            =       (CNavArea *)params[4];
	float travelDistanceLimit =           sp_ctof(params[5]);
	float maxStepUpLimit      =           sp_ctof(params[6]);
	float maxDropDownLimit    =           sp_ctof(params[7]);
	
	CUtlVector<CNavArea *> nearbyAreaVector;
	CollectSurroundingAreas(&nearbyAreaVector, startArea, travelDistanceLimit, maxStepUpLimit, maxDropDownLimit);
	
	int nearbyAreasMax = params[2];
	cell_t *nearbyAreas; pCtx->LocalToPhysAddr(params[3], &nearbyAreas);
	
	for (int i = 0; i < nearbyAreaVector.Count() && i < nearbyAreasMax; ++i) {
		nearbyAreas[i] = (cell_t)nearbyAreaVector[i];
	}
	
	return Min(nearbyAreaVector.Count(), nearbyAreasMax);
}
#endif

const sp_nativeinfo_t g_Natives[] = {
	{ "PF_Create",                                     &PF_Create                                     },
	{ "PF_Destroy",                                    &PF_Destroy                                    },
	{ "PF_Exists",                                     &PF_Exists                                     },
	
	{ "PF_SetGoalVector",                              &PF_SetGoalVector                              },
	{ "PF_GetGoalVector",                              &PF_GetGoalVector                              },
	{ "PF_SetGoalEntity",                              &PF_SetGoalEntity                              },
	
	{ "PF_IsPathToVectorPossible",                     &PF_IsPathToVectorPossible                     },
	{ "PF_IsPathToEntityPossible",                     &PF_IsPathToEntityPossible                     },
	
	{ "PF_StartPathing",                               &PF_StartPathing                               },
	{ "PF_StopPathing",                                &PF_StopPathing                                },
	
	{ "PF_GetFutureSegment",                           &PF_GetFutureSegment                           },
	
	{ "PF_GetPtr_PluginBot",                           &PF_GetPtr_PluginBot                           },
	{ "PF_GetPtr_PluginBotLocomotion",                 &PF_GetPtr_PluginBotLocomotion                 },
	{ "PF_GetPtr_PluginBotBody",                       &PF_GetPtr_PluginBotBody                       },
	{ "PF_GetPtr_PluginBotIntention",                  &PF_GetPtr_PluginBotIntention                  },
	
	{ "PF_IsEntityACombatCharacter",                   &PF_IsEntityACombatCharacter                   },

	{ "PF_UpdateLastKnownArea",                        &PF_UpdateLastKnownArea                        },
	{ "PF_GetLastKnownArea",                           &PF_GetLastKnownArea                           },

	{ "PF_IsPotentiallyTraversable",                   &PF_IsPotentiallyTraversable                   },
	{ "PF_HasPotentialGap",                            &PF_HasPotentialGap                            },
	{ "PF_IsDiscontinuityAhead",                       &PF_IsDiscontinuityAhead                       },

	{ "PF_EnableCallback",                             &PF_EnableCallback                             },
	{ "PF_DisableCallback",                            &PF_DisableCallback                            },
	{ "PF_IsCallbackEnabled",                          &PF_IsCallbackEnabled                          },
	
	{ "NavArea.GetID",                                 &NavArea_GetID                                 },
	{ "NavArea.GetPlayerCount",                        &NavArea_GetPlayerCount                        },
	{ "NavArea.HasAttributes",                         &NavArea_HasAttributes                         },
	{ "NavArea.GetCenter",                             &NavArea_GetCenter                             },
	{ "NavArea.GetRandomPoint",                        &NavArea_GetRandomPoint                        },
	{ "NavArea.GetZ",                                  &NavArea_GetZ                                  },
	{ "NavArea.GetCostSoFar",                          &NavArea_GetCostSoFar                          },
	{ "NavArea.ComputeAdjacentConnectionHeightChange", &NavArea_ComputeAdjacentConnectionHeightChange },
	{ "NavArea.ComputeNormal",                         &NavArea_ComputeNormal                         },
	{ "NavArea.GetCorner",                             &NavArea_GetCorner                             },
	{ "NavArea.GetAdjacentCount",                      &NavArea_GetAdjacentCount                      },
	{ "NavArea.GetAdjacentArea",                       &NavArea_GetAdjacentArea                       },

	{ "NavMesh.GetNavArea_Vec",                        &NavMesh_GetNavArea_Vec                        },
	{ "NavMesh.GetNearestNavArea_Vec",                 &NavMesh_GetNearestNavArea_Vec                 },
//	{ "NavMesh.CollectSurroundingAreas",               &NavMesh_CollectSurroundingAreas               },

	{ nullptr,                                         nullptr                                        },
};
