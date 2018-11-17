#include "simulate.h"


std::map<int, PFContext> bots;


void SimulateFrame(bool simulating)
{
//	DevMsg("SimulateFrame\n");
	
	std::vector<int> obsolete;
	
	for (auto& pair : bots) {
		PFContext& ctx = pair.second;
		
		if (ctx.m_pPluginBot->GetEntity() == nullptr) {
			obsolete.push_back(pair.first);
		//	DevMsg("SimulateFrame: obsolete\n");
			continue;
		}
		
		if (ctx.m_bPathing) {
			if (ctx.m_ctRePath.IsElapsed()) {
				ctx.RecalculatePath();
			}
			
			ctx.m_pPluginBot->GetEntity()->UpdateLastKnownArea();
			ctx.m_pPathFollower->Update(ctx.m_pPluginBot);
		}
	}
	
	for (auto idx : obsolete) {
		RemoveBot(idx);
	}
}


void RemoveBot(int idx)
{
//	g_pSM->LogMessage(myself, "Removing obsolete bot #%d", idx);
	
	bots.at(idx).Call_EntityGone(idx);
	bots.erase(idx);
}
