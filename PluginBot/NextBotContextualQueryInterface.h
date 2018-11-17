#ifndef NEXTBOT_NEXTBOTCONTEXTUALQUERYINTERFACE_H
#define NEXTBOT_NEXTBOTCONTEXTUALQUERYINTERFACE_H


class CKnownEntity;


class IContextualQuery
{
public:
	IContextualQuery() {}
	virtual ~IContextualQuery() {}
	
	virtual ThreeState_t ShouldPickUp(const INextBot *nextbot, CBaseEntity *it) const                     { return TRS_NONE; }
	virtual ThreeState_t ShouldHurry(const INextBot *nextbot) const                                       { return TRS_NONE; }
	virtual ThreeState_t ShouldRetreat(const INextBot *nextbot) const                                     { return TRS_NONE; }
	virtual ThreeState_t ShouldAttack(const INextBot *nextbot, const CKnownEntity *threat) const          { return TRS_NONE; }
	virtual ThreeState_t IsHindrance(const INextBot *nextbot, CBaseEntity *it) const                      { return TRS_NONE; }
	virtual Vector SelectTargetPoint(const INextBot *nextbot, const CBaseCombatCharacter *them) const     { return vec3_origin; }
	virtual ThreeState_t IsPositionAllowed(const INextBot *nextbot, const Vector& pos) const              { return TRS_NONE; }
	virtual const CKnownEntity *SelectMoreDangerousThreat(const INextBot *nextbot,
		const CBaseCombatCharacter *them, const CKnownEntity *threat1, const CKnownEntity *threat2) const { return nullptr; }
};


#endif
