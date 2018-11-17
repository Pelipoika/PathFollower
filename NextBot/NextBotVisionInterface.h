#ifndef _NEXT_BOT_VISION_INTERFACE_H_
#define _NEXT_BOT_VISION_INTERFACE_H_

#include "NextBotComponentInterface.h"
#include "NextBotKnownEntity.h"

class INextBotEntityFilter
{
public:
	virtual bool IsAllowed( CBaseEntity *entity ) const = 0;
};

#define TEAM_ANY -2

class IVision : public INextBotComponent
{
public:
	IVision() {};
	virtual ~IVision() {};

	enum FieldOfViewCheckType
	{
		USE_FOV, 
		DISREGARD_FOV
	};

	class IForEachKnownEntity
	{
	public:
		virtual bool Inspect(const CKnownEntity &known) = 0;
	};

	virtual bool ForEachKnownEntity(IForEachKnownEntity &func)                                                      = 0;
	virtual void CollectKnownEntities(CUtlVector< CKnownEntity > *knownVector)                                      = 0;
	virtual const CKnownEntity *GetPrimaryKnownThreat(bool onlyVisibleThreats = false) const                        = 0;
	virtual float GetTimeSinceVisible(int team) const                                                               = 0;
	virtual const CKnownEntity *GetClosestKnown(int team = TEAM_ANY) const                                          = 0;
	virtual int GetKnownCount(int team, bool onlyVisible = false, float rangeLimit = -1.0f) const                   = 0;
	virtual const CKnownEntity *GetClosestKnown(const INextBotEntityFilter &filter) const                           = 0;
	virtual const CKnownEntity *GetKnown(const CBaseEntity *entity) const                                           = 0;
	virtual void AddKnownEntity(CBaseEntity *entity)                                                                = 0;
	virtual void ForgetEntity(CBaseEntity *forgetMe)                                                                = 0;
	virtual void ForgetAllKnownEntities()                                                                           = 0;
	virtual void CollectPotentiallyVisibleEntities(CUtlVector< CBaseEntity * > *potentiallyVisible)                 = 0;
	virtual float GetMaxVisionRange() const                                                                         = 0;
	virtual float GetMinRecognizeTime() const                                                                       = 0;
	virtual bool IsAbleToSee(CBaseEntity *subject, FieldOfViewCheckType checkFOV, Vector *visibleSpot = NULL) const = 0;
	virtual bool IsAbleToSee(const Vector &pos, FieldOfViewCheckType checkFOV) const                                = 0;
	virtual bool IsIgnored(CBaseEntity *subject) const                                                              = 0;
	virtual bool IsVisibleEntityNoticed(CBaseEntity *subject) const                                                 = 0;
	virtual bool IsInFieldOfView(const Vector &pos) const                                                           = 0;
	virtual bool IsInFieldOfView(CBaseEntity *subject) const                                                        = 0;
	virtual float GetDefaultFieldOfView() const                                                                     = 0;
	virtual float GetFieldOfView() const                                                                            = 0;
	virtual void SetFieldOfView(float horizAngle)                                                                   = 0;
	virtual bool IsLineOfSightClear(const Vector &pos) const                                                        = 0;
	virtual bool IsLineOfSightClearToEntity(const CBaseEntity *subject, Vector *visibleSpot = NULL) const           = 0;
	virtual bool IsLookingAt(const Vector &pos, float cosTolerance = 0.95f) const                                   = 0;
	virtual bool IsLookingAt(const CBaseCombatCharacter *actor, float cosTolerance = 0.95f) const                   = 0;
};

#endif // _NEXT_BOT_VISION_INTERFACE_H_
