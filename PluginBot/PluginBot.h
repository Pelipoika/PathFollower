#ifndef _INCLUDE_PLUGINBOT_PLUGINBOT_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOT_H_


#include "../thunks.h"

#include "../NextBot/NextBotInterface.h"
#include "PluginBotCommon.h"

class PFContext;

class PluginBotLocomotion;
class PluginBotBody;
class PluginBotIntention;
class PluginBotVision;


class PluginBot : public INextBot
{
public:
	PluginBot(PFContext *ctx, CBaseCombatCharacter *entity);
	virtual ~PluginBot();
	
	/* INextBotEventResponder */
	virtual INextBotEventResponder *FirstContainedResponder() const override                                    { return nullptr; }
	virtual INextBotEventResponder *NextContainedResponder(INextBotEventResponder *prev) const override         { return nullptr; }
	virtual void OnLeaveGround(CBaseEntity *ent) override                                                       { STUB_NEXTBOT(); }
	virtual void OnLandOnGround(CBaseEntity *ent) override                                                      { STUB_NEXTBOT(); }
	virtual void OnContact(CBaseEntity* ent, CGameTrace* trace) override                                        { STUB_NEXTBOT(); }
	virtual void OnMoveToSuccess(const Path *path) override;
	virtual void OnMoveToFailure(const Path *path, MoveToFailureType fail) override;
	virtual void OnStuck() override                                                                             { STUB_NEXTBOT(); }
	virtual void OnUnStuck() override                                                                           { STUB_NEXTBOT(); }
	virtual void OnPostureChanged() override                                                                    { STUB_NEXTBOT(); }
	virtual void OnAnimationActivityComplete(int i1) override                                                   { STUB_NEXTBOT(); }
	virtual void OnAnimationActivityInterrupted(int i1) override                                                { STUB_NEXTBOT(); }
	virtual void OnAnimationEvent(animevent_t *a1) override                                                     { STUB_NEXTBOT(); }
	virtual void OnIgnite() override                                                                            { STUB_NEXTBOT(); }
	virtual void OnInjured(const CTakeDamageInfo& info) override                                                { STUB_NEXTBOT(); }
	virtual void OnKilled(const CTakeDamageInfo& info) override                                                 { STUB_NEXTBOT(); }
	virtual void OnOtherKilled(CBaseCombatCharacter *who, const CTakeDamageInfo& info) override                 { STUB_NEXTBOT(); }
	virtual void OnSight(CBaseEntity *ent) override                                                             { STUB_NEXTBOT(); }
	virtual void OnLostSight(CBaseEntity *ent) override                                                         { STUB_NEXTBOT(); }
	virtual void OnSound(CBaseEntity *ent, const Vector& where, KeyValues *kv) override                         { STUB_NEXTBOT(); }
	virtual void OnSpokeConcept(CBaseCombatCharacter *who, const char *concept, AI_Response *response) override { STUB_NEXTBOT(); }
	virtual void OnWeaponFired(CBaseCombatCharacter *who, CBaseCombatWeapon *weapon) override                   { STUB_NEXTBOT(); }
	virtual void OnNavAreaChanged(CNavArea *area1, CNavArea *area2) override                                    { STUB_NEXTBOT(); }
	virtual void OnModelChanged() override                                                                      { STUB_NEXTBOT(); }
	virtual void OnPickUp(CBaseEntity *ent, CBaseCombatCharacter *who) override                                 { STUB_NEXTBOT(); }
	virtual void OnDrop(CBaseEntity *ent) override                                                              { STUB_NEXTBOT(); }
	virtual void OnActorEmoted(CBaseCombatCharacter* who, int concept) override;
	virtual void OnCommandAttack(CBaseEntity *ent) override                                                     { STUB_NEXTBOT(); }
	virtual void OnCommandApproach(const Vector& where, float f1) override                                      { STUB_NEXTBOT(); }
	virtual void OnCommandApproach(CBaseEntity *ent) override                                                   { STUB_NEXTBOT(); }
	virtual void OnCommandRetreat(CBaseEntity *ent, float f1) override                                          { STUB_NEXTBOT(); }
	virtual void OnCommandPause(float f1) override                                                              { STUB_NEXTBOT(); }
	virtual void OnCommandResume() override                                                                     { STUB_NEXTBOT(); }
	virtual void OnCommandString(const char *cmd) override                                                      { STUB_NEXTBOT(); }
	virtual void OnShoved(CBaseEntity *ent) override                                                            { STUB_NEXTBOT(); }
	virtual void OnBlinded(CBaseEntity *ent) override                                                           { STUB_NEXTBOT(); }
	virtual void OnTerritoryContested(int idx) override                                                         { STUB_NEXTBOT(); }
	virtual void OnTerritoryCaptured(int idx) override                                                          { STUB_NEXTBOT(); }
	virtual void OnTerritoryLost(int idx) override                                                              { STUB_NEXTBOT(); }
	virtual void OnWin() override                                                                               { STUB_NEXTBOT(); }
	virtual void OnLose() override                                                                              { STUB_NEXTBOT(); }
	
	/* INextBot */
	virtual void Reset() override                                                 {}
	virtual void Update() override                                                {}
	virtual void Upkeep() override                                                {}
	virtual bool IsRemovedOnReset() const override                                { return false; }
	virtual CBaseCombatCharacter *GetEntity() const override                      { return this->m_hEntity; }
	virtual NextBotCombatCharacter *GetNextBotCombatCharacter() const override    { STUB_NEXTBOT(); return nullptr; }
	virtual ILocomotion *GetLocomotionInterface() const override;
	virtual IBody *GetBodyInterface() const override;
	virtual IIntention *GetIntentionInterface() const override;
	virtual IVision *GetVisionInterface() const override                          { STUB_NEXTBOT(); return nullptr; }
	virtual bool SetPosition(const Vector& pos) override                          { STUB_NEXTBOT(); return false; }
	virtual const Vector& GetPosition() const override                            { return this->m_hEntity->GetAbsOrigin(); }
	virtual bool IsEnemy(const CBaseEntity *ent) const override;
	virtual bool IsFriend(const CBaseEntity *ent) const override;
	virtual bool IsSelf(const CBaseEntity *ent) const override;
	virtual bool IsAbleToClimbOnto(const CBaseEntity *ent) const override         { STUB_NEXTBOT(); return false; }
	virtual bool IsAbleToBreak(const CBaseEntity *ent) const override             { STUB_NEXTBOT(); return false; }
	virtual bool IsAbleToBlockMovementOf(const INextBot *nextbot) const override  { STUB_NEXTBOT(); return false; }
	virtual bool ShouldTouch(const CBaseEntity *ent) const override               { STUB_NEXTBOT(); return false; }
	virtual bool IsImmobile() const override                                      { STUB_NEXTBOT(); return false; }
	virtual float GetImmobileDuration() const override                            { STUB_NEXTBOT(); return 0.0f; }
	virtual void ClearImmobileStatus() override                                   { STUB_NEXTBOT(); }
	virtual float GetImmobileSpeedThreshold() const override                      { STUB_NEXTBOT(); return 0.0f; }
	virtual const PathFollower *GetCurrentPath() const override                   { return this->m_pCurrentPath; }
	virtual void SetCurrentPath(const PathFollower *follower) override            { this->m_pCurrentPath = follower; }
	virtual void NotifyPathDestruction(const PathFollower *follower) override     {}
	virtual bool IsRangeLessThan(CBaseEntity *ent, float dist) const override     { STUB_NEXTBOT(); return false; }
	virtual bool IsRangeLessThan(const Vector& vec, float dist) const override    { STUB_NEXTBOT(); return false; }
	virtual bool IsRangeGreaterThan(CBaseEntity *ent, float dist) const override  { STUB_NEXTBOT(); return false; }
	virtual bool IsRangeGreaterThan(const Vector& vec, float dist) const override { STUB_NEXTBOT(); return false; }
	virtual float GetRangeTo(CBaseEntity *ent) const override                     { STUB_NEXTBOT(); return 0.0f; }
	virtual float GetRangeTo(const Vector& vec) const override                    { STUB_NEXTBOT(); return 0.0f; }
	virtual float GetRangeSquaredTo(CBaseEntity *ent) const override              { STUB_NEXTBOT(); return 0.0f; }
	virtual float GetRangeSquaredTo(const Vector& vec) const override             { STUB_NEXTBOT(); return 0.0f; }
	virtual bool IsDebugging(unsigned int type) const override                    { return cv_pathfollower_debug.GetBool(); }
	virtual char *GetDebugIdentifier() const override                             { STUB_NEXTBOT(); return const_cast<char *>(""); }
	virtual bool IsDebugFilterMatch(const char *filter) const override            { STUB_NEXTBOT(); return false; }
	virtual void DisplayDebugText(const char *text) const override                {}
	
	PFContext *GetContext() const { return this->m_pContext; }
	
private:
	/* this stuff needs to be here so we can register with the NextBotManager */
	uint8_t pad_04[0x08];
	int m_iManagerIndex = -1;
	bool m_bScheduledForNextTick;
	int m_iLastUpdateTick = -999;
	uint8_t pad_18[0x04];
	
	PFContext *m_pContext;
	CHandle<CBaseCombatCharacter> m_hEntity;
	
	PluginBotLocomotion *m_pLocomotion = nullptr;
	PluginBotBody       *m_pBody       = nullptr;
	PluginBotIntention  *m_pIntention  = nullptr;
	
	const PathFollower *m_pCurrentPath = nullptr;
};


inline bool PluginBot::IsEnemy(const CBaseEntity *ent) const
{
	if (ent == nullptr) return false;
	return (this->GetEntity()->GetTeamNumber() != ent->GetTeamNumber());
}

inline bool PluginBot::IsFriend(const CBaseEntity *ent) const
{
	if (ent == nullptr) return false;
	return (this->GetEntity()->GetTeamNumber() == ent->GetTeamNumber());
}

inline bool PluginBot::IsSelf(const CBaseEntity *ent) const
{
	if (ent == nullptr) return false;
	return (ENTINDEX(this->GetEntity()) == ENTINDEX(ent));
}

#endif // _INCLUDE_PLUGINBOT_PLUGINBOT_H_