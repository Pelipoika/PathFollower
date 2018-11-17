#ifndef _INCLUDE_PLUGINBOT_PLUGINBOTPATHCOST_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOTPATHCOST_H_


#include "../Nav/nav_area.h"
#include "../NextBot/NextBotPath.h"
#include "PluginBotCommon.h"


/* similar to ShortestPathCost, but with checks for locomotion parameters */
class PluginBotPathCost : public IPathCost
{
public:
	PluginBotPathCost(PluginBot *bot) :
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
		
		float dist;
		if (length > 0.0f) {
			dist = length;
		} else {
			dist = (area->GetCenter() - fromArea->GetCenter()).Length();
		}
		
		/* account for step height, max jump height, death drop height */
		float delta_z = fromArea->ComputeAdjacentConnectionHeightChange(area);

		//TODO Fix ComputeAdjacentConnectionHeightChange sometimes giving weird values.
	//	DevMsg("PluginBotPathCost delta_z %f\n", delta_z);

		if (delta_z >= this->m_flStepHeight) {
			if (delta_z >= this->m_flMaxJumpHeight) return -1.0f;

			/* cost penalty for going up steps */
			dist *= 2.0f;
		} else {
			if (delta_z < -this->m_flDeathDropHeight) return -1.0f;
		}
		
		return dist + fromArea->GetCostSoFar();
	}
	
private:
	PluginBot *m_pPluginBot;
	
	float m_flStepHeight;
	float m_flMaxJumpHeight;
	float m_flDeathDropHeight;
};


#endif // _INCLUDE_PLUGINBOT_PLUGINBOTPATHCOST_H_
