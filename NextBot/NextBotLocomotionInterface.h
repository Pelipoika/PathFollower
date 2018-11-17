#ifndef NEXTBOT_NEXTBOTLOCOMOTIONINTERFACE_H
#define NEXTBOT_NEXTBOTLOCOMOTIONINTERFACE_H

#include "NextBotComponentInterface.h"

class CBaseEntity;
class QAngle;
class Vector;
class CNavLadder;

class ILocomotion : public INextBotComponent
{
public:
	enum TraverseWhenType 
	{ 
		IMMEDIATELY,		// the entity will not block our motion - we'll carry right through
		EVENTUALLY			// the entity will block us until we spend effort to open/destroy it
	};

	ILocomotion() {}
	virtual ~ILocomotion() {}
	
	virtual void Approach(const Vector& dst, float f1)                                                                                   = 0;
	virtual void DriveTo(const Vector& dst)                                                                                              = 0;
	virtual bool ClimbUpToLedge(const Vector& dst, const Vector& dir, const CBaseEntity *ent)                                            = 0;
	virtual void JumpAcrossGap(const Vector& dst, const Vector& dir)                                                                     = 0;
	virtual void Jump()                                                                                                                  = 0;
	virtual bool IsClimbingOrJumping() const                                                                                             = 0;
	virtual bool IsClimbingUpToLedge() const                                                                                             = 0;
	virtual bool IsJumpingAcrossGap() const                                                                                              = 0;
	virtual bool IsScrambling() const                                                                                                    = 0;
	virtual void Run()                                                                                                                   = 0;
	virtual void Walk()                                                                                                                  = 0;
	virtual void Stop()                                                                                                                  = 0;
	virtual bool IsRunning() const                                                                                                       = 0;
	virtual void SetDesiredSpeed(float speed)                                                                                            = 0;
	virtual float GetDesiredSpeed() const                                                                                                = 0;
	virtual void SetSpeedLimit(float limit)                                                                                              = 0;
	virtual float GetSpeedLimit() const                                                                                                  = 0;
	virtual bool IsOnGround() const                                                                                                      = 0;
	virtual CBaseEntity *GetGround() const                                                                                               = 0;
	virtual const Vector& GetGroundNormal() const                                                                                        = 0;
	virtual float GetGroundSpeed() const                                                                                                 = 0;
	virtual Vector& GetGroundMotionVector() const                                                                                        = 0;
	virtual void ClimbLadder(const CNavLadder *ladder, const CNavArea *area)                                                             = 0;
	virtual void DescendLadder(const CNavLadder *ladder, const CNavArea *area)                                                           = 0;
	virtual bool IsUsingLadder() const                                                                                                   = 0;
	virtual bool IsAscendingOrDescendingLadder() const                                                                                   = 0;
	virtual bool IsAbleToAutoCenterOnLadder() const                                                                                      = 0;
	virtual void FaceTowards(const Vector& vec)                                                                                          = 0;
	virtual void SetDesiredLean(const QAngle& ang)                                                                                       = 0;
	virtual QAngle& GetDesiredLean() const                                                                                               = 0;
	virtual bool IsAbleToJumpAcrossGaps() const                                                                                          = 0;
	virtual bool IsAbleToClimb() const                                                                                                   = 0;
	virtual const Vector& GetFeet() const                                                                                                = 0;
	virtual float GetStepHeight() const                                                                                                  = 0;
	virtual float GetMaxJumpHeight() const                                                                                               = 0;
	virtual float GetDeathDropHeight() const                                                                                             = 0;
	virtual float GetRunSpeed() const                                                                                                    = 0;
	virtual float GetWalkSpeed() const                                                                                                   = 0;
	virtual float GetMaxAcceleration() const                                                                                             = 0;
	virtual float GetMaxDeceleration() const                                                                                             = 0;
	virtual Vector& GetVelocity() const                                                                                                  = 0;
	virtual float GetSpeed() const                                                                                                       = 0;
	virtual Vector& GetMotionVector() const                                                                                              = 0;
	virtual bool IsAreaTraversable(const CNavArea *area) const                                                                           = 0;
	virtual float GetTraversableSlopeLimit() const                                                                                       = 0;
	virtual bool IsPotentiallyTraversable(const Vector& from, const Vector& to, TraverseWhenType when, float *pFraction = nullptr) const = 0;
	virtual bool HasPotentialGap(const Vector& vec1, const Vector& vec2, float *pFraction = nullptr) const                               = 0;
	virtual bool IsGap(const Vector& from, const Vector& to) const                                                                       = 0;
	virtual bool IsEntityTraversable(CBaseEntity *ent, TraverseWhenType when) const                                                      = 0;
	virtual bool IsStuck() const                                                                                                         = 0;
	virtual float GetStuckDuration() const                                                                                               = 0;
	virtual void ClearStuckStatus(char const *msg)                                                                                       = 0;
	virtual bool IsAttemptingToMove() const                                                                                              = 0;
	virtual bool ShouldCollideWith(const CBaseEntity *ent) const                                                                         = 0;
	virtual void AdjustPosture(const Vector& vec)                                                                                        = 0;
	virtual void StuckMonitor()                                                                                                          = 0;
};


#endif
