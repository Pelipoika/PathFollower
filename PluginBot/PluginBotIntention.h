#ifndef _INCLUDE_PLUGINBOT_PLUGINBOTINTENTION_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOTINTENTION_H_


#include "../NextBot/NextBotIntentionInterface.h"
#include "PluginBotCommon.h"


class PluginBotIntention : public IIntention
{
public:
	PluginBotIntention(PluginBot *pluginbot) :	m_pPluginBot(pluginbot) {}
	virtual ~PluginBotIntention() {}
	
	/* INextBotEventResponder */
	virtual INextBotEventResponder *FirstContainedResponder() const override                                    { STUB_INTENT(); return nullptr; }
	virtual INextBotEventResponder *NextContainedResponder(INextBotEventResponder *prev) const override         { STUB_INTENT(); return nullptr; }
	virtual void OnLeaveGround(CBaseEntity *ent) override                                                       { STUB_INTENT(); }
	virtual void OnLandOnGround(CBaseEntity *ent) override                                                      { STUB_INTENT(); }
	virtual void OnContact(CBaseEntity *ent, CGameTrace *trace) override                                        { STUB_INTENT(); }
	virtual void OnMoveToSuccess(const Path *path) override                                                     { STUB_INTENT(); }
	virtual void OnMoveToFailure(const Path *path, MoveToFailureType fail) override                             { STUB_INTENT(); }
	virtual void OnStuck() override                                                                             { STUB_INTENT(); }
	virtual void OnUnStuck() override                                                                           { STUB_INTENT(); }
	virtual void OnPostureChanged() override                                                                    { STUB_INTENT(); }
	virtual void OnAnimationActivityComplete(int i1) override                                                   { STUB_INTENT(); }
	virtual void OnAnimationActivityInterrupted(int i1) override                                                { STUB_INTENT(); }
	virtual void OnAnimationEvent(animevent_t *a1) override                                                     { STUB_INTENT(); }
	virtual void OnIgnite() override                                                                            { STUB_INTENT(); }
	virtual void OnInjured(const CTakeDamageInfo& info) override                                                { STUB_INTENT(); }
	virtual void OnKilled(const CTakeDamageInfo& info) override                                                 { STUB_INTENT(); }
	virtual void OnOtherKilled(CBaseCombatCharacter *who, const CTakeDamageInfo& info) override                 { STUB_INTENT(); }
	virtual void OnSight(CBaseEntity *ent) override                                                             { STUB_INTENT(); }
	virtual void OnLostSight(CBaseEntity *ent) override                                                         { STUB_INTENT(); }
	virtual void OnSound(CBaseEntity *ent, const Vector& where, KeyValues *kv) override                         { STUB_INTENT(); }
	virtual void OnSpokeConcept(CBaseCombatCharacter *who, const char *concept, AI_Response *response) override { STUB_INTENT(); }
	virtual void OnWeaponFired(CBaseCombatCharacter *who, CBaseCombatWeapon *weapon) override                   { STUB_INTENT(); }
	virtual void OnNavAreaChanged(CNavArea *area1, CNavArea *area2) override                                    { STUB_INTENT(); }
	virtual void OnModelChanged() override                                                                      { STUB_INTENT(); }
	virtual void OnPickUp(CBaseEntity *ent, CBaseCombatCharacter *who) override                                 { STUB_INTENT(); }
	virtual void OnDrop(CBaseEntity *ent) override                                                              { STUB_INTENT(); }
	virtual void OnActorEmoted(CBaseCombatCharacter *who, int concept) override                                 { STUB_INTENT(); }
	virtual void OnCommandAttack(CBaseEntity *ent) override                                                     { STUB_INTENT(); }
	virtual void OnCommandApproach(const Vector& where, float f1) override                                      { STUB_INTENT(); }
	virtual void OnCommandApproach(CBaseEntity *ent) override                                                   { STUB_INTENT(); }
	virtual void OnCommandRetreat(CBaseEntity *ent, float f1) override                                          { STUB_INTENT(); }
	virtual void OnCommandPause(float f1) override                                                              { STUB_INTENT(); }
	virtual void OnCommandResume() override                                                                     { STUB_INTENT(); }
	virtual void OnCommandString(const char *cmd) override                                                      { STUB_INTENT(); }
	virtual void OnShoved(CBaseEntity *ent) override                                                            { STUB_INTENT(); }
	virtual void OnBlinded(CBaseEntity *ent) override                                                           { STUB_INTENT(); }
	virtual void OnTerritoryContested(int idx) override                                                         { STUB_INTENT(); }
	virtual void OnTerritoryCaptured(int idx) override                                                          { STUB_INTENT(); }
	virtual void OnTerritoryLost(int idx) override                                                              { STUB_INTENT(); }
	virtual void OnWin() override                                                                               { STUB_INTENT(); }
	virtual void OnLose() override                                                                              { STUB_INTENT(); }
	
	/* INextBotComponent */
	virtual void Reset() override             { STUB_INTENT(); }
	virtual void Update() override            { STUB_INTENT(); }
	virtual void Upkeep() override            { STUB_INTENT(); }
	virtual INextBot *GetBot() const override { return this->m_pPluginBot; }
	
	PluginBot *GetPluginBot() const { return this->m_pPluginBot; }
	
private:
	PluginBot *m_pPluginBot;
};


#endif // _INCLUDE_PLUGINBOT_PLUGINBOTINTENTION_H_
