#include "context.h"

#include "PluginBot/PluginBotPathCost.h"
#include "NextBot/NextBotPathFollow.h"


class SPCustomPathCost : public IPathCost
{
public:
	SPCustomPathCost(PluginBot *bot) :
		m_pPluginBot(bot)
	{
		this->m_flStepHeight      = bot->GetLocomotionInterface()->GetStepHeight();
		this->m_flMaxJumpHeight   = bot->GetLocomotionInterface()->GetMaxJumpHeight();
		this->m_flDeathDropHeight = bot->GetLocomotionInterface()->GetDeathDropHeight();
	}
	
	virtual float operator()(CNavArea *area, CNavArea *fromArea, const CNavLadder *ladder = nullptr, const CFuncElevator *elevator = nullptr, float length = -1.0f) const override
	{
		/* initial area in path */
		if (fromArea == nullptr) return 0.0f;
		
		if (!this->m_pPluginBot->GetLocomotionInterface()->IsAreaTraversable(area)) return -1.0f;
		
		/* account for step height, max jump height, death drop height */
		float delta_z = fromArea->ComputeAdjacentConnectionHeightChange(area);
		if (delta_z >= this->m_flStepHeight) {
			if (delta_z >= this->m_flMaxJumpHeight) return -1.0f;
		} else {
			if (delta_z < -this->m_flDeathDropHeight) return -1.0f;
		}
		
		return this->m_pPluginBot->GetContext()->Call_GetPathCost(area, fromArea, length);
	}
	
private:
	PluginBot *m_pPluginBot;
	
	float m_flStepHeight;
	float m_flMaxJumpHeight;
	float m_flDeathDropHeight;
};


IPathCost *PFContext::GetPathCostFunctor() const
{
	if (this->IsCallbackRegistered(PFCB_GetPathCost)) {
		return new SPCustomPathCost(this->m_pPluginBot);
	} else {
		return new PluginBotPathCost(this->m_pPluginBot);
	}
}


bool PFContext::RecalculatePath()
{
	bool success = false;
	
	/* bail out if bot is obsolete */
	if (this->m_pPluginBot->GetEntity() == nullptr) {
		return false;
	}
	
	/* can't have both */
	assert(!this->m_bGoalVector || !this->m_bGoalEntity);
	
	this->m_pPluginBot->GetEntity()->UpdateLastKnownArea();
	
	if (this->m_bGoalVector) {
		IPathCost *cost_func = this->GetPathCostFunctor();
		success = this->m_pPathFollower->Compute(this->m_pPluginBot, this->m_vecGoal, *cost_func);
		delete cost_func;
		
		if (!success) {
			this->m_pPluginBot->GetContext()->Call_PathFailed();
			this->m_pPathFollower->Invalidate();
		}
		else{
			this->m_pPluginBot->GetContext()->Call_PathSuccess();
		}
	}
	
	if (this->m_bGoalEntity && this->m_hGoal != nullptr) {
		this->m_hGoal->UpdateLastKnownArea();
		
		IPathCost *cost_func = this->GetPathCostFunctor();
		success = this->m_pPathFollower->Compute(this->m_pPluginBot, this->m_hGoal, *cost_func);
		delete cost_func;
		
		if (!success) {
			this->m_pPluginBot->GetContext()->Call_PathFailed();
			this->m_pPathFollower->Invalidate();
		}
		else{
			this->m_pPluginBot->GetContext()->Call_PathSuccess();
		}
	}
	
	this->m_ctRePath.Start(this->m_flRePathInterval);
	
	return success;
}