#include "PluginBot.h"

#include "PluginBotLocomotion.h"
#include "PluginBotBody.h"
#include "PluginBotIntention.h"

PluginBot::PluginBot(PFContext *ctx, CBaseCombatCharacter *entity) :
	m_pContext(ctx), m_hEntity(entity)
{
	this->m_pLocomotion = new PluginBotLocomotion(this);
	this->m_pBody       = new PluginBotBody      (this);
	this->m_pIntention  = new PluginBotIntention (this);

	this->m_iManagerIndex = TheNextBots().Register(this);
}

PluginBot::~PluginBot()
{
	TheNextBots().UnRegister(this);
	
	delete this->m_pLocomotion;
	delete this->m_pBody;
	delete this->m_pIntention;
}


ILocomotion *PluginBot::GetLocomotionInterface() const
{
	return this->m_pLocomotion;
}

IBody *PluginBot::GetBodyInterface() const
{
	return this->m_pBody;
}

IIntention *PluginBot::GetIntentionInterface() const
{
	return this->m_pIntention;
}