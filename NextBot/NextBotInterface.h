#ifndef NEXTBOT_NEXTBOTINTERFACE_H
#define NEXTBOT_NEXTBOTINTERFACE_H


#include "NextBotEventResponderInterface.h"

#include "NextBotVisionInterface.h"

class NextBotCombatCharacter;
class PathFollower;
class ILocomotion;
class IBody;
class IIntention;

//class IVision;


class INextBot : public INextBotEventResponder
{
public:
	enum NextBotDebugType
	{
		DEBUG_NONE = 0x0000,
		DEBUG_ANY  = 0xffff,
		
		DEBUG_BEHAVIOR   = (1 << 0),
		DEBUG_LOOK_AT    = (1 << 1),
		DEBUG_PATH       = (1 << 2),
		DEBUG_ANIMATION  = (1 << 3),
		DEBUG_LOCOMOTION = (1 << 4),
		DEBUG_VISION     = (1 << 5),
		DEBUG_HEARING    = (1 << 6),
		DEBUG_EVENTS     = (1 << 7),
		DEBUG_ERRORS     = (1 << 8),
	};
	
	struct NextBotDebugLineType
	{
		NextBotDebugType type;
		char buf[0x100];
	};
	
	INextBot() {}
	virtual ~INextBot() {}
	
	virtual void Reset()                                                 = 0;
	virtual void Update()                                                = 0;
	virtual void Upkeep()                                                = 0;
	
	virtual bool IsRemovedOnReset() const                                = 0;
	
	virtual CBaseCombatCharacter *GetEntity() const                      = 0;
	virtual NextBotCombatCharacter *GetNextBotCombatCharacter() const    = 0;
	
	virtual ILocomotion *GetLocomotionInterface() const                  = 0;
	virtual IBody *GetBodyInterface() const                              = 0;
	virtual IIntention *GetIntentionInterface() const                    = 0;
	virtual IVision *GetVisionInterface() const                          = 0;
	
	virtual bool SetPosition(const Vector& pos)                          = 0;
	virtual const Vector& GetPosition() const                            = 0;
	
	virtual bool IsEnemy(const CBaseEntity *ent) const                   = 0;
	virtual bool IsFriend(const CBaseEntity *ent) const                  = 0;
	virtual bool IsSelf(const CBaseEntity *ent) const                    = 0;
	
	virtual bool IsAbleToClimbOnto(const CBaseEntity *ent) const         = 0;
	virtual bool IsAbleToBreak(const CBaseEntity *ent) const             = 0;
	virtual bool IsAbleToBlockMovementOf(const INextBot *nextbot) const  = 0;
	
	virtual bool ShouldTouch(const CBaseEntity *ent) const               = 0;
	
	virtual bool IsImmobile() const                                      = 0;
	virtual float GetImmobileDuration() const                            = 0;
	virtual void ClearImmobileStatus()                                   = 0;
	virtual float GetImmobileSpeedThreshold() const                      = 0;
	
	virtual const PathFollower *GetCurrentPath() const                   = 0;
	virtual void SetCurrentPath(const PathFollower *follower)            = 0;
	virtual void NotifyPathDestruction(const PathFollower *follower)     = 0;
	
	virtual bool IsRangeLessThan(CBaseEntity *ent, float dist) const     = 0;
	virtual bool IsRangeLessThan(const Vector& vec, float dist) const    = 0;
	virtual bool IsRangeGreaterThan(CBaseEntity *ent, float dist) const  = 0;
	virtual bool IsRangeGreaterThan(const Vector& vec, float dist) const = 0;
	
	virtual float GetRangeTo(CBaseEntity *ent) const                     = 0;
	virtual float GetRangeTo(const Vector& vec) const                    = 0;
	virtual float GetRangeSquaredTo(CBaseEntity *ent) const              = 0;
	virtual float GetRangeSquaredTo(const Vector& vec) const             = 0;
	
	virtual bool IsDebugging(unsigned int type) const                    = 0;
	virtual char *GetDebugIdentifier() const                             = 0;
	virtual bool IsDebugFilterMatch(const char *filter) const            = 0;
	virtual void DisplayDebugText(const char *text) const                = 0;
};


#endif
