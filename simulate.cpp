#include "simulate.h"

std::map<int, PFContext> bots;

void SimulateFrame(const bool simulating)
{
	if (!simulating)
		return;

	//DevMsg("SimulateFrame\n");

	std::vector<int> obsolete;

	for (auto &pair : bots)
	{
		PFContext &ctx = pair.second;

		PluginBot *           nextbot = ctx.m_pPluginBot;
		CBaseCombatCharacter *entity  = nextbot->GetEntity();

		if (entity == nullptr)
		{
			obsolete.push_back(pair.first);
			//DevMsg("SimulateFrame: obsolete\n");
			continue;
		}

		if (ctx.m_bPathing)
		{
			if (ctx.m_ctRePath.IsElapsed())
			{
				ctx.RecalculatePath();
			}

			ctx.m_pPluginBot->GetEntity()->UpdateLastKnownArea();
			ctx.m_pPathFollower->Update(ctx.m_pPluginBot);
		}
	}

	for (const auto idx : obsolete)
	{
		RemoveBot(idx);
	}
}

void RemoveBot(const int idx)
{
	//g_pSM->LogMessage(myself, "Removing obsolete bot #%d", idx);

	bots.at(idx).Call_EntityGone(idx);
	bots.erase(idx);
}
