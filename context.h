#ifndef _INCLUDE_CONTEXT_H_
#define _INCLUDE_CONTEXT_H_


#include "extension.h"
#include "thunks.h"
#include "sp_callbacks.h"

#include "Nav/nav_area.h"
#include "NextBot/NextBotPathFollow.h"
#include "PluginBot/PluginBot.h"


class PFContext : public PFCallbacks
{
public:
	PFContext(IPlugin *plugin) :
		PFCallbacks(plugin) {}
	~PFContext()
	{
		if (this->m_pPathFollower != nullptr) delete this->m_pPathFollower;
		if (this->m_pPluginBot    != nullptr) delete this->m_pPluginBot;
	}
	
	IPathCost *GetPathCostFunctor() const;
	
	bool RecalculatePath();
	
	PluginBot    *m_pPluginBot    = nullptr;
	PathFollower *m_pPathFollower = nullptr;
	
	bool m_bPathing = false;
	
	bool m_bGoalVector = false;
	Vector m_vecGoal = vec3_origin;
	
	bool m_bGoalEntity = false;
	CHandle<CBaseCombatCharacter> m_hGoal;
	
	float m_flRePathInterval = 0.0f;
	CountdownTimer m_ctRePath;
};

inline void PluginBot::OnMoveToSuccess(const Path *path)
{
	this->GetContext()->m_pPluginBot->GetContext()->Call_OnMoveToSuccess(path);
	DevMsg("PluginBot::OnMoveToSuccess\n");
}

inline void PluginBot::OnMoveToFailure(const Path *path, MoveToFailureType fail)
{
	this->GetContext()->m_pPluginBot->GetContext()->Call_OnMoveToFailure(path, ::MoveToFailureType(fail));
	DevMsg("PluginBot::OnMoveToFailure\n");
}

inline void PluginBot::OnActorEmoted(CBaseCombatCharacter* who, int concept)
{
	this->GetContext()->m_pPluginBot->GetContext()->Call_OnActorEmoted(who, concept);
	DevMsg("PluginBot::OnActorEmoted\n");
}


#endif // _INCLUDE_CONTEXT_H_
