#ifndef _INCLUDE_PLUGINBOT_PLUGINBOTLOCOMOTION_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOTLOCOMOTION_H_


#include "../NextBot/NextBotLocomotionInterface.h"
#include "PluginBotCommon.h"
#include "thunks.h"
#include "context.h"


class PluginBotLocomotion : public ILocomotion
{
public:
	PluginBotLocomotion(PluginBot *pluginbot) :
		m_pPluginBot(pluginbot) {};
	virtual ~PluginBotLocomotion() {};
	
	enum : bool
	{
		ABLE_TO_CLIMB    = true,
		ABLE_TO_GAP_JUMP = false,
	};
	
	/* INextBotEventResponder */
	virtual INextBotEventResponder *FirstContainedResponder() const override                                    { STUB_LOCO(); return nullptr; }
	virtual INextBotEventResponder *NextContainedResponder(INextBotEventResponder *prev) const override         { STUB_LOCO(); return nullptr; }
	virtual void OnLeaveGround(CBaseEntity *ent) override                                                       { STUB_LOCO(); }
	virtual void OnLandOnGround(CBaseEntity *ent) override                                                      { STUB_LOCO(); }
	virtual void OnContact(CBaseEntity* ent, CGameTrace* trace) override                                        { STUB_LOCO(); }
	virtual void OnMoveToSuccess(const Path *path) override                                                     { STUB_LOCO(); }
	virtual void OnMoveToFailure(const Path *path, MoveToFailureType fail) override                             { STUB_LOCO(); }
	virtual void OnStuck() override                                                                             { STUB_LOCO(); }
	virtual void OnUnStuck() override                                                                           { STUB_LOCO(); }
	virtual void OnPostureChanged() override                                                                    { STUB_LOCO(); }
	virtual void OnAnimationActivityComplete(int i1) override                                                   { STUB_LOCO(); }
	virtual void OnAnimationActivityInterrupted(int i1) override                                                { STUB_LOCO(); }
	virtual void OnAnimationEvent(animevent_t *a1) override                                                     { STUB_LOCO(); }
	virtual void OnIgnite() override                                                                            { STUB_LOCO(); }
	virtual void OnInjured(const CTakeDamageInfo& info) override                                                { STUB_LOCO(); }
	virtual void OnKilled(const CTakeDamageInfo& info) override                                                 { STUB_LOCO(); }
	virtual void OnOtherKilled(CBaseCombatCharacter *who, const CTakeDamageInfo& info) override                 { STUB_LOCO(); }
	virtual void OnSight(CBaseEntity *ent) override                                                             { STUB_LOCO(); }
	virtual void OnLostSight(CBaseEntity *ent) override                                                         { STUB_LOCO(); }
	virtual void OnSound(CBaseEntity *ent, const Vector& where, KeyValues *kv) override                         { STUB_LOCO(); }
	virtual void OnSpokeConcept(CBaseCombatCharacter *who, const char *concept, AI_Response *response) override { STUB_LOCO(); }
	virtual void OnWeaponFired(CBaseCombatCharacter *who, CBaseCombatWeapon *weapon) override                   { STUB_LOCO(); }
	virtual void OnNavAreaChanged(CNavArea *area1, CNavArea *area2) override                                    { STUB_LOCO(); }
	virtual void OnModelChanged() override                                                                      { STUB_LOCO(); }
	virtual void OnPickUp(CBaseEntity *ent, CBaseCombatCharacter *who) override                                 { STUB_LOCO(); }
	virtual void OnDrop(CBaseEntity *ent) override                                                              { STUB_LOCO(); }
	virtual void OnActorEmoted(CBaseCombatCharacter *who, int concept) override                                 { STUB_LOCO(); }
	virtual void OnCommandAttack(CBaseEntity *ent) override                                                     { STUB_LOCO(); }
	virtual void OnCommandApproach(const Vector& where, float f1) override                                      { STUB_LOCO(); }
	virtual void OnCommandApproach(CBaseEntity *ent) override                                                   { STUB_LOCO(); }
	virtual void OnCommandRetreat(CBaseEntity *ent, float f1) override                                          { STUB_LOCO(); }
	virtual void OnCommandPause(float f1) override                                                              { STUB_LOCO(); }
	virtual void OnCommandResume() override                                                                     { STUB_LOCO(); }
	virtual void OnCommandString(const char *cmd) override                                                      { STUB_LOCO(); }
	virtual void OnShoved(CBaseEntity *ent) override                                                            { STUB_LOCO(); }
	virtual void OnBlinded(CBaseEntity *ent) override                                                           { STUB_LOCO(); }
	virtual void OnTerritoryContested(int idx) override                                                         { STUB_LOCO(); }
	virtual void OnTerritoryCaptured(int idx) override                                                          { STUB_LOCO(); }
	virtual void OnTerritoryLost(int idx) override                                                              { STUB_LOCO(); }
	virtual void OnWin() override                                                                               { STUB_LOCO(); }
	virtual void OnLose() override                                                                              { STUB_LOCO(); }
	
	/* INextBotComponent */
	virtual void Reset() override             { STUB_LOCO(); }
	virtual void Update() override            { STUB_LOCO(); }
	virtual void Upkeep() override            { STUB_LOCO(); }
	virtual INextBot *GetBot() const override { return this->m_pPluginBot; }
	
	/* ILocomotion */
	virtual void Approach(const Vector& dst, float f1) override;
	virtual void DriveTo(const Vector& dst) override                                                                { STUB_LOCO(); }
	virtual bool ClimbUpToLedge(const Vector& dst, const Vector& dir, const CBaseEntity *ent) override;
	virtual void JumpAcrossGap(const Vector& dst, const Vector& dir) override                                       { STUB_LOCO(); }
	virtual void Jump() override                                                                                    { STUB_LOCO(); }
	virtual bool IsClimbingOrJumping() const override                                                               { return !this->IsOnGround(); }
	virtual bool IsClimbingUpToLedge() const override                                                               { return false; }
	virtual bool IsJumpingAcrossGap() const override                                                                { return false; }
	virtual bool IsScrambling() const override                                                                      { STUB_LOCO(); return false; }
	virtual void Run() override                                                                                     { STUB_LOCO(); }
	virtual void Walk() override                                                                                    { STUB_LOCO(); }
	virtual void Stop() override                                                                                    { STUB_LOCO(); }
	virtual bool IsRunning() const override                                                                         { return false; }
	virtual void SetDesiredSpeed(float speed) override                                                              { this->m_flDesiredSpeed = speed; }
	virtual float GetDesiredSpeed() const override                                                                  { return this->m_flDesiredSpeed; }
	virtual void SetSpeedLimit(float limit) override                                                                { this->m_flSpeedLimit = limit; }
	virtual float GetSpeedLimit() const override                                                                    { return this->m_flSpeedLimit; }
	virtual bool IsOnGround() const override                                                                        { return (this->GetGround() != nullptr); }
	virtual CBaseEntity *GetGround() const override                                                                 { return this->GetBot()->GetEntity()->GetGroundEntity(); }
	virtual const Vector& GetGroundNormal() const override                                                          { static const Vector vecUp(0.0f, 0.0f, 1.0f); return vecUp; }
	virtual float GetGroundSpeed() const override                                                                   { STUB_LOCO(); return 0.0f; }
	virtual Vector& GetGroundMotionVector() const override                                                          { STUB_LOCO(); return vecStub; }
	virtual void ClimbLadder(const CNavLadder *ladder, const CNavArea *area) override                               { STUB_LOCO(); }
	virtual void DescendLadder(const CNavLadder *ladder, const CNavArea *area) override                             { STUB_LOCO(); }
	virtual bool IsUsingLadder() const override                                                                     { return false; }
	virtual bool IsAscendingOrDescendingLadder() const override                                                     { return false; }
	virtual bool IsAbleToAutoCenterOnLadder() const override                                                        { return false; }
	virtual void FaceTowards(const Vector& vec) override;
	virtual void SetDesiredLean(const QAngle& ang) override                                                         { STUB_LOCO(); }
	virtual QAngle& GetDesiredLean() const override                                                                 { STUB_LOCO(); return angStub; }
	virtual bool IsAbleToJumpAcrossGaps() const override                                                            { return ABLE_TO_GAP_JUMP; }
	virtual bool IsAbleToClimb() const override                                                                     { return ABLE_TO_CLIMB; }
	virtual const Vector& GetFeet() const override                                                                  { return this->GetBot()->GetEntity()->GetAbsOrigin(); }
	virtual float GetStepHeight() const override                                                                    { return this->m_flStepHeight; }
	virtual float GetMaxJumpHeight() const override                                                                 { return this->m_flMaxJumpHeight; }
	virtual float GetDeathDropHeight() const override                                                               { return this->m_flDeathDropHeight; }
	virtual float GetRunSpeed() const override                                                                      { return this->m_flRunSpeed; }
	virtual float GetWalkSpeed() const override                                                                     { return this->m_flWalkSpeed; }
	virtual float GetMaxAcceleration() const override                                                               { STUB_LOCO(); return 0.0f; }
	virtual float GetMaxDeceleration() const override                                                               { STUB_LOCO(); return 0.0f; }
	virtual Vector& GetVelocity() const override                                                                    { STUB_LOCO(); return vecStub; }
	virtual float GetSpeed() const override                                                                         { STUB_LOCO(); return 0.0f; }
	virtual Vector& GetMotionVector() const override                                                                { STUB_LOCO(); return vecStub; }
	virtual bool IsAreaTraversable(const CNavArea *area) const override;
	virtual float GetTraversableSlopeLimit() const override                                                         { return this->m_flSlopeLimit; }
	virtual bool IsPotentiallyTraversable(const Vector& from, const Vector& to, TraverseWhenType when, float *pFraction = nullptr) const override;
	virtual bool HasPotentialGap(const Vector& vec1, const Vector& vec2, float *pFraction = nullptr) const override;
	virtual bool IsGap(const Vector& pos, const Vector& forward) const override;
	virtual bool IsEntityTraversable(CBaseEntity *ent, TraverseWhenType when) const override;
	virtual bool IsStuck() const override                                                                           { STUB_LOCO(); return false; }
	virtual float GetStuckDuration() const override                                                                 { STUB_LOCO(); return 0.0f; }
	virtual void ClearStuckStatus(char const *msg) override                                                         { STUB_LOCO(); }
	virtual bool IsAttemptingToMove() const override                                                                { STUB_LOCO(); return false; }
	virtual bool ShouldCollideWith(const CBaseEntity *ent) const override                                           { STUB_LOCO(); return true; }
	virtual void AdjustPosture(const Vector& vec) override                                                          { STUB_LOCO(); }
	virtual void StuckMonitor() override                                                                            { STUB_LOCO(); }
	
	PluginBot *GetPluginBot() const { return this->m_pPluginBot; }
	
	void SetStepHeight     (float val) { this->m_flStepHeight      = val; }
	void SetMaxJumpHeight  (float val) { this->m_flMaxJumpHeight   = val; }
	void SetDeathDropHeight(float val) { this->m_flDeathDropHeight = val; }
	void SetRunSpeed       (float val) { this->m_flRunSpeed        = val; }
	void SetWalkSpeed      (float val) { this->m_flWalkSpeed       = val; }
	void SetSlopeLimit     (float val) { this->m_flSlopeLimit      = val; }
	
private:
	PluginBot *m_pPluginBot;
	
	float m_flStepHeight      = 0.0f;
	float m_flMaxJumpHeight   = 0.0f;
	float m_flDeathDropHeight = 0.0f;
	float m_flRunSpeed        = 0.0f;
	float m_flWalkSpeed       = 0.0f;
	float m_flSlopeLimit      = 0.0f;
	
	float m_flDesiredSpeed = 0.0f;
	float m_flSpeedLimit   = 0.0f;
};


inline bool PluginBotLocomotion::IsAreaTraversable(const CNavArea *area) const
{
	/* this is totally safe, all the function does is call GetTeamNumber() on the bot entity */
	return (this->*Thunks::CTFBotLocomotion_IsAreaTraversable)(area);
}


inline bool PluginBotLocomotion::IsPotentiallyTraversable(const Vector& from, const Vector& to, TraverseWhenType when, float *pFraction) const
{
	return (this->*Thunks::ILocomotion_IsPotentiallyTraversable)(from, to, when, pFraction);
}

inline bool PluginBotLocomotion::HasPotentialGap(const Vector& vec1, const Vector& vec2, float *pFraction) const
{
	return (this->*Thunks::ILocomotion_HasPotentialGap)(vec1, vec2, pFraction);
}

inline bool PluginBotLocomotion::IsGap(const Vector& pos, const Vector& forward) const
{
	return (this->*Thunks::ILocomotion_IsGap)(pos, forward);
}

inline void PluginBotLocomotion::Approach(const Vector& vec, float f1)
{
//	DevMsg("Approach [ %.1f %.1f %.1f ] @ %.2f\n", vec.x, vec.y, vec.z, f1);
	
	this->GetPluginBot()->GetContext()->Call_Approach(vec);
}

inline bool PluginBotLocomotion::ClimbUpToLedge(const Vector& dst, const Vector& dir, const CBaseEntity *ent)
{
	return (this->GetPluginBot()->GetContext()->Call_ClimbUpToLedge(dst, dir));
}

inline void PluginBotLocomotion::FaceTowards(const Vector& vec)
{
//	DevMsg("FaceTowards [ %.1f %.1f %.1f ]\n", vec.x, vec.y, vec.z);
//	
//	CallFwd_PF_FaceTowards(this->GetPluginBot(), vec);
}

inline bool PluginBotLocomotion::IsEntityTraversable(CBaseEntity *ent, TraverseWhenType when) const
{
	return this->GetPluginBot()->GetContext()->Call_IsEntityTraversable(ent, when);
}


#endif // _INCLUDE_PLUGINBOT_PLUGINBOTLOCOMOTION_H_
