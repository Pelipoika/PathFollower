#ifndef _INCLUDE_PLUGINBOT_PLUGINBOTBODY_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOTBODY_H_


#include "../NextBot/NextBotBodyInterface.h"
#include "PluginBotCommon.h"


class PluginBotBody : public IBody
{
public:
	PluginBotBody(PluginBot *pluginbot) :
		m_pPluginBot(pluginbot) {}
	virtual ~PluginBotBody() {}
	
	/* INextBotEventResponder */
	virtual INextBotEventResponder *FirstContainedResponder() const override                                    { STUB_BODY(); return nullptr; }
	virtual INextBotEventResponder *NextContainedResponder(INextBotEventResponder *prev) const override         { STUB_BODY(); return nullptr; }
	virtual void OnLeaveGround(CBaseEntity *ent) override                                                       { STUB_BODY(); }
	virtual void OnLandOnGround(CBaseEntity *ent) override                                                      { STUB_BODY(); }
	virtual void OnContact(CBaseEntity *ent, CGameTrace *trace) override                                        { STUB_BODY(); }
	virtual void OnMoveToSuccess(const Path *path) override                                                     { STUB_BODY(); }
	virtual void OnMoveToFailure(const Path *path, MoveToFailureType fail) override                             { STUB_BODY(); }
	virtual void OnStuck() override                                                                             { STUB_BODY(); }
	virtual void OnUnStuck() override                                                                           { STUB_BODY(); }
	virtual void OnPostureChanged() override                                                                    { STUB_BODY(); }
	virtual void OnAnimationActivityComplete(int i1) override                                                   { STUB_BODY(); }
	virtual void OnAnimationActivityInterrupted(int i1) override                                                { STUB_BODY(); }
	virtual void OnAnimationEvent(animevent_t *a1) override                                                     { STUB_BODY(); }
	virtual void OnIgnite() override                                                                            { STUB_BODY(); }
	virtual void OnInjured(const CTakeDamageInfo& info) override                                                { STUB_BODY(); }
	virtual void OnKilled(const CTakeDamageInfo& info) override                                                 { STUB_BODY(); }
	virtual void OnOtherKilled(CBaseCombatCharacter *who, const CTakeDamageInfo& info) override                 { STUB_BODY(); }
	virtual void OnSight(CBaseEntity *ent) override                                                             { STUB_BODY(); }
	virtual void OnLostSight(CBaseEntity *ent) override                                                         { STUB_BODY(); }
	virtual void OnSound(CBaseEntity *ent, const Vector& where, KeyValues *kv) override                         { STUB_BODY(); }
	virtual void OnSpokeConcept(CBaseCombatCharacter *who, const char *concept, AI_Response *response) override { STUB_BODY(); }
	virtual void OnWeaponFired(CBaseCombatCharacter *who, CBaseCombatWeapon *weapon) override                   { STUB_BODY(); }
	virtual void OnNavAreaChanged(CNavArea *area1, CNavArea *area2) override                                    { STUB_BODY(); }
	virtual void OnModelChanged() override                                                                      { STUB_BODY(); }
	virtual void OnPickUp(CBaseEntity *ent, CBaseCombatCharacter *who) override                                 { STUB_BODY(); }
	virtual void OnDrop(CBaseEntity *ent) override                                                              { STUB_BODY(); }
	virtual void OnActorEmoted(CBaseCombatCharacter *who, int concept) override                                 { STUB_BODY(); }
	virtual void OnCommandAttack(CBaseEntity *ent) override                                                     { STUB_BODY(); }
	virtual void OnCommandApproach(const Vector& where, float f1) override                                      { STUB_BODY(); }
	virtual void OnCommandApproach(CBaseEntity *ent) override                                                   { STUB_BODY(); }
	virtual void OnCommandRetreat(CBaseEntity *ent, float f1) override                                          { STUB_BODY(); }
	virtual void OnCommandPause(float f1) override                                                              { STUB_BODY(); }
	virtual void OnCommandResume() override                                                                     { STUB_BODY(); }
	virtual void OnCommandString(const char *cmd) override                                                      { STUB_BODY(); }
	virtual void OnShoved(CBaseEntity *ent) override                                                            { STUB_BODY(); }
	virtual void OnBlinded(CBaseEntity *ent) override                                                           { STUB_BODY(); }
	virtual void OnTerritoryContested(int idx) override                                                         { STUB_BODY(); }
	virtual void OnTerritoryCaptured(int idx) override                                                          { STUB_BODY(); }
	virtual void OnTerritoryLost(int idx) override                                                              { STUB_BODY(); }
	virtual void OnWin() override                                                                               { STUB_BODY(); }
	virtual void OnLose() override                                                                              { STUB_BODY(); }
	
	/* INextBotComponent */
	virtual void Reset() override                { STUB_BODY(); }
	virtual void Update() override               { STUB_BODY(); }
	virtual void Upkeep() override               { STUB_BODY(); }
	virtual INextBot *GetBot() const override    { return this->m_pPluginBot; }
	virtual void *GetScriptDesc() const override { return nullptr; };

	/* IBody */
	virtual bool SetPosition(const Vector& pos) override                                                                                           { STUB_BODY(); return false; }
	virtual Vector& GetEyePosition() const override                                                                                                { STUB_BODY(); return vecStub; }
	virtual Vector& GetViewVector() const override                                                                                                 { STUB_BODY(); return vecStub; }
	virtual void AimHeadTowards(const Vector& vec, LookAtPriorityType priority, float duration, INextBotReply *reply, const char *reason) override { STUB_BODY(); }
	virtual void AimHeadTowards(CBaseEntity *ent, LookAtPriorityType priority, float duration, INextBotReply *reply, const char *reason) override  { STUB_BODY(); }
	virtual bool IsHeadAimingOnTarget() const override                                                                                             { STUB_BODY(); return false; }
	virtual bool IsHeadSteady() const override                                                                                                     { STUB_BODY(); return false; }
	virtual float GetHeadSteadyDuration() const override                                                                                           { STUB_BODY(); return 0.0f; }
	virtual float GetHeadAimSubjectLeadTime() const override                                                                                       { STUB_BODY(); return 0.0f; }
	virtual float GetHeadAimTrackingInterval() const override                                                                                      { STUB_BODY(); return 0.0f; }
	virtual void ClearPendingAimReply() override                                                                                                   { STUB_BODY(); }
	virtual float GetMaxHeadAngularVelocity() const override                                                                                       { STUB_BODY(); return 0.0f; }
	virtual bool StartActivity(Activity a1, unsigned int i1) override                                                                              { STUB_BODY(); return false; }
	virtual int SelectAnimationSequence(Activity a1) const override                                                                                { STUB_BODY(); return 0; }
	virtual Activity GetActivity() const override                                                                                                  { STUB_BODY(); return ACT_INVALID; }
	virtual bool IsActivity(Activity a1) const override                                                                                            { STUB_BODY(); return false; }
	virtual bool HasActivityType(unsigned int i1) const override                                                                                   { STUB_BODY(); return false; }
	virtual void SetDesiredPosture(PostureType posture) override                                                                                   { STUB_BODY(); }
	virtual PostureType GetDesiredPosture() const override                                                                                         { STUB_BODY(); return POSTURE_STAND; }
	virtual bool IsDesiredPosture(PostureType posture) const override                                                                              { STUB_BODY(); return false; }
	virtual bool IsInDesiredPosture() const override                                                                                               { STUB_BODY(); return false; }
	virtual PostureType GetActualPosture() const override                                                                                          { STUB_BODY(); return POSTURE_STAND; }
	virtual bool IsActualPosture(PostureType posture) const override                                                                               { STUB_BODY(); return false; }
	virtual bool IsPostureMobile() const override                                                                                                  { STUB_BODY(); return false; }
	virtual bool IsPostureChanging() const override                                                                                                { STUB_BODY(); return false; }
	virtual void SetArousal(ArousalType arousal) override                                                                                          { STUB_BODY(); }
	virtual ArousalType GetArousal() const override                                                                                                { STUB_BODY(); return ALERT_YES; }
	virtual bool IsArousal(ArousalType arousal) const override                                                                                     { STUB_BODY(); return false; }
	virtual float GetHullWidth() const override;
	virtual float GetHullHeight() const override;
	virtual float GetStandHullHeight() const override;
	virtual float GetCrouchHullHeight() const override;
	virtual const Vector& GetHullMins() const override;
	virtual const Vector& GetHullMaxs() const override;
	virtual unsigned int GetSolidMask() const override                                                                                             { return this->m_nSolidMask; }
	virtual int GetCollisionGroup() const override                                                                                                 { STUB_BODY(); return 0; }
	
	PluginBot *GetPluginBot() const { return this->m_pPluginBot; }
	
	void SetSolidMask(unsigned int mask) { this->m_nSolidMask = mask; }
	
	void SetHullScaleX(float scale_x) { this->m_flHullScaleX = scale_x; }
	void SetHullScaleY(float scale_y) { this->m_flHullScaleY = scale_y; }
	void SetHullScaleZ(float scale_z) { this->m_flHullScaleZ = scale_z; }
	
private:
	PluginBot *m_pPluginBot;
	
	unsigned int m_nSolidMask = CONTENTS_SOLID;
	
	float m_flHullScaleX = 1.0f;
	float m_flHullScaleY = 1.0f;
	float m_flHullScaleZ = 1.0f;
};


inline float PluginBotBody::GetHullWidth() const
{
	return (this->GetHullMaxs().x - this->GetHullMins().x);
}

inline float PluginBotBody::GetHullHeight() const
{
	return (this->GetHullMaxs().z - this->GetHullMins().z);
}

inline float PluginBotBody::GetStandHullHeight() const
{
	return this->GetHullHeight();
}

inline float PluginBotBody::GetCrouchHullHeight() const
{
	return this->GetHullHeight();
}

inline const Vector& PluginBotBody::GetHullMins() const
{
	static Vector temp;
	
	temp = this->GetPluginBot()->GetEntity()->CollisionProp()->OBBMins();
	temp.x *= this->m_flHullScaleX;
	temp.y *= this->m_flHullScaleY;
	temp.z *= this->m_flHullScaleZ;
	
	return temp;
}

inline const Vector& PluginBotBody::GetHullMaxs() const
{
	static Vector temp;
	
	temp = this->GetPluginBot()->GetEntity()->CollisionProp()->OBBMaxs();
	temp.x *= this->m_flHullScaleX;
	temp.y *= this->m_flHullScaleY;
	temp.z *= this->m_flHullScaleZ;
	
	return temp;
}


#endif // _INCLUDE_PLUGINBOT_PLUGINBOTBODY_H_
