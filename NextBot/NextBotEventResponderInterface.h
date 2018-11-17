#ifndef NEXTBOT_NEXTBOTEVENTRESPONDERINTERFACE_H
#define NEXTBOT_NEXTBOTEVENTRESPONDERINTERFACE_H


class KeyValues;
class Vector;
class CGameTrace;
struct animevent_t;
class AI_Response;
class CNavArea;
class CTakeDamageInfo;
class CBaseCombatCharacter;
class CBaseCombatWeapon;
class CBaseEntity;
class Path;


class INextBotEventResponder
{
public:
	enum MoveToFailureType
	{
		FAIL_INVALID_PATH = 0,
		FAIL_STUCK        = 1,
		FAIL_FELL_OFF     = 2,
	};
	
	INextBotEventResponder() {}
	virtual ~INextBotEventResponder() {}
	
	virtual INextBotEventResponder *FirstContainedResponder() const                                    = 0;
	virtual INextBotEventResponder *NextContainedResponder(INextBotEventResponder *prev) const         = 0;
	
	virtual void OnLeaveGround(CBaseEntity *ent)                                                       = 0;
	virtual void OnLandOnGround(CBaseEntity *ent)                                                      = 0;
	
	virtual void OnContact(CBaseEntity *ent, CGameTrace *trace)                                        = 0;
	
	virtual void OnMoveToSuccess(const Path *path)                                                     = 0;
	virtual void OnMoveToFailure(const Path *path, MoveToFailureType fail)                             = 0;
	
	virtual void OnStuck()                                                                             = 0;
	virtual void OnUnStuck()                                                                           = 0;
	
	virtual void OnPostureChanged()                                                                    = 0;
	virtual void OnAnimationActivityComplete(int i1)                                                   = 0;
	virtual void OnAnimationActivityInterrupted(int i1)                                                = 0;
	virtual void OnAnimationEvent(animevent_t *a1)                                                     = 0;
	
	virtual void OnIgnite()                                                                            = 0;
	virtual void OnInjured(const CTakeDamageInfo& info)                                                = 0;
	virtual void OnKilled(const CTakeDamageInfo& info)                                                 = 0;
	virtual void OnOtherKilled(CBaseCombatCharacter *who, const CTakeDamageInfo& info)                 = 0;
	
	virtual void OnSight(CBaseEntity *ent)                                                             = 0;
	virtual void OnLostSight(CBaseEntity *ent)                                                         = 0;
	
	virtual void OnSound(CBaseEntity *ent, const Vector& where, KeyValues *kv)                         = 0;
	virtual void OnSpokeConcept(CBaseCombatCharacter *who, const char *concept, AI_Response *response) = 0;
	virtual void OnWeaponFired(CBaseCombatCharacter *who, CBaseCombatWeapon *weapon)                   = 0;
	
	virtual void OnNavAreaChanged(CNavArea *area1, CNavArea *area2)                                    = 0;
	virtual void OnModelChanged()                                                                      = 0;
	virtual void OnPickUp(CBaseEntity *ent, CBaseCombatCharacter *who)                                 = 0;
	virtual void OnDrop(CBaseEntity *ent)                                                              = 0;
	virtual void OnActorEmoted(CBaseCombatCharacter *who, int concept)                                 = 0;
	
	virtual void OnCommandAttack(CBaseEntity *ent)                                                     = 0;
	virtual void OnCommandApproach(const Vector& where, float f1)                                      = 0;
	virtual void OnCommandApproach(CBaseEntity *ent)                                                   = 0;
	virtual void OnCommandRetreat(CBaseEntity *ent, float f1)                                          = 0;
	virtual void OnCommandPause(float f1)                                                              = 0;
	virtual void OnCommandResume()                                                                     = 0;
	virtual void OnCommandString(const char *cmd)                                                      = 0;
	
	virtual void OnShoved(CBaseEntity *ent)                                                            = 0;
	virtual void OnBlinded(CBaseEntity *ent)                                                           = 0;
	
	virtual void OnTerritoryContested(int idx)                                                         = 0;
	virtual void OnTerritoryCaptured(int idx)                                                          = 0;
	virtual void OnTerritoryLost(int idx)                                                              = 0;
	
	virtual void OnWin()                                                                               = 0;
	virtual void OnLose()                                                                              = 0;
};


#endif
