#include "test.h"
#include "thunks.h"

#include "PluginBot/PluginBot.h"
#include "PluginBot/PluginBotPathCost.h"
#include "NextBot/NextBotPathFollow.h"

#include "Nav/nav.h"
#include "Nav/nav_ladder.h"
#include "Nav/nav_area.h"
#include "Nav/nav_mesh.h"
#include "Nav/nav_pathfind.h"


/* VS2013, what a piece of shit */
#if defined _MSC_VER
#define constexpr const
#endif


/* these need to eventually be configurable */
constexpr float        PATHFOLLOWER_LOOKAHEAD_DISTANCE =  300.0f;
constexpr float        LOCOMOTION_STEP_HEIGHT          =   18.0f;
constexpr float        LOCOMOTION_MAX_JUMP_HEIGHT      =   72.0f;
constexpr float        LOCOMOTION_DEATH_DROP_HEIGHT    = 1000.0f;
constexpr float        LOCOMOTION_RUN_SPEED            =  300.0f;
constexpr float        LOCOMOTION_WALK_SPEED           =  150.0f;
constexpr unsigned int BODY_SOLIDMASK                  = MASK_NPCSOLID;


PathFollower *s_PathFollower = nullptr;
PluginBot    *s_NextBot      = nullptr;


CON_COMMAND(pathfollower_new, "PathFollower: create a new PluginBot and PathFollower")
{
	if (args.ArgC() != 2) {
		Warning("Expected 1 argument: entindex for the PluginBot\n");
		return;
	}
	
	CBaseEntity *ent = servertools->GetBaseEntityByEntIndex(atoi(args[1]));
	if (ent == nullptr) {
		Warning("Could not find entity with index %s\n", args[1]);
		return;
	}
	
	if (s_PathFollower == nullptr) {
		s_PathFollower = New_PathFollower();
		
		s_PathFollower->SetMinLookAheadDistance(PATHFOLLOWER_LOOKAHEAD_DISTANCE);
	}
	if (s_NextBot == nullptr) {
		s_NextBot = new PluginBot(reinterpret_cast<CBaseCombatCharacter *>(ent));
		
		auto loco = static_cast<PluginBotLocomotion *>(s_NextBot->GetLocomotionInterface());
		loco->SetStepHeight     (LOCOMOTION_STEP_HEIGHT);
		loco->SetMaxJumpHeight  (LOCOMOTION_MAX_JUMP_HEIGHT);
		loco->SetDeathDropHeight(LOCOMOTION_DEATH_DROP_HEIGHT);
		loco->SetRunSpeed       (LOCOMOTION_RUN_SPEED);
		loco->SetWalkSpeed      (LOCOMOTION_WALK_SPEED);
		
		auto body = static_cast<PluginBotBody *>(s_NextBot->GetBodyInterface());
		body->SetSolidMask(BODY_SOLIDMASK);
	}
	
	DevMsg("Entity              @ 0x%08x\n", (uintptr_t)s_NextBot->GetEntity());
	DevMsg("PluginBot           @ 0x%08x\n", (uintptr_t)s_NextBot);
	DevMsg("PluginBotLocomotion @ 0x%08x\n", (uintptr_t)s_NextBot->GetLocomotionInterface());
	DevMsg("PluginBotBody       @ 0x%08x\n", (uintptr_t)s_NextBot->GetBodyInterface());
	DevMsg("PluginBotIntention  @ 0x%08x\n", (uintptr_t)s_NextBot->GetIntentionInterface());
}
CON_COMMAND(pathfollower_delete, "PathFollower: destroy an existing PluginBot and PathFollower")
{
	if (s_PathFollower != nullptr) {
		delete s_PathFollower;
		s_PathFollower = nullptr;
	}
	if (s_NextBot != nullptr) {
		delete s_NextBot;
		s_NextBot = nullptr;
	}
}


bool use_subject = false;


Vector vecGoal;
CON_COMMAND(pathfollower_goto_goal, "PathFollower: make an entity path toward a goal vector")
{
	if (s_PathFollower == nullptr || s_NextBot == nullptr) {
		Warning("You haven't created a PluginBot and PathFollower yet.\n");
		return;
	}
	
	if (args.ArgC() != 4) {
		Warning("Expected 3 arguments: x y z\n");
		return;
	}
	
	vecGoal.x = atof(args[1]);
	vecGoal.y = atof(args[2]);
	vecGoal.z = atof(args[3]);
	
	use_subject = false;
}


CHandle<CBaseCombatCharacter> hSubject;
CON_COMMAND(pathfollower_goto_subject, "PathFollower: make an entity path toward a subject entity")
{
	if (s_PathFollower == nullptr || s_NextBot == nullptr) {
		Warning("You haven't created a PluginBot and PathFollower yet.\n");
		return;
	}
	
	if (args.ArgC() != 2) {
		Warning("Expected 1 argument: entindex for the subject\n");
		return;
	}
	
	CBaseEntity *ent = servertools->GetBaseEntityByEntIndex(atoi(args[1]));
	if (ent == nullptr) {
		Warning("Could not find entity with index %s\n", args[1]);
		return;
	}
	
	hSubject = static_cast<CBaseCombatCharacter *>(ent);
	
	use_subject = true;
}


void GameFrameHook(bool simulating)
{
	DevMsg("GameFrameHook\n");
	
	if (s_PathFollower != nullptr && s_NextBot != nullptr) {
		if (s_NextBot->GetEntity() == nullptr) {
			Warning("PluginBot entity is no longer valid!\n");
			return;
		}
		
		// TODO: don't do this so often!
		static CountdownTimer ctRecomputePath;
		if (ctRecomputePath.IsElapsed()) {
			ctRecomputePath.Start(0.200f);
			
			/* non-CBasePlayers never call UpdateLastKnownArea */
			s_NextBot->GetEntity()->UpdateLastKnownArea();
			
			PluginBotPathCost cost_func;
			
			bool success;
			if (use_subject) {
				if (hSubject != nullptr) {
					/* non-CBasePlayers never call UpdateLastKnownArea */
					hSubject->UpdateLastKnownArea();
					
					success = s_PathFollower->Compute(s_NextBot, hSubject, cost_func);
				} else {
					Warning("Subject entity is no longer valid!\n");
					return;
				}
			} else {
				success = s_PathFollower->Compute(s_NextBot, vecGoal, cost_func);
			}
			
			if (success) {
				Msg("Path computation successful.\n");
			} else {
				Warning("Path computation failed!\n");
			}
		}
		
		s_PathFollower->Update(s_NextBot);
	}
}


bool updating = false;
CON_COMMAND(pathfollower_update_start, "PathFollower: start updating")
{
	if (updating) return;
	updating = true;
	
	g_pSM->AddGameFrameHook(&GameFrameHook);
}
CON_COMMAND(pathfollower_update_stop, "PathFollower: stop updating")
{
	if (!updating) return;
	updating = false;
	
	g_pSM->RemoveGameFrameHook(&GameFrameHook);
}
