#ifndef NEXTBOT_NEXTBOTBODYINTERFACE_H
#define NEXTBOT_NEXTBOTBODYINTERFACE_H


#include "NextBotComponentInterface.h"
#include <ai_activity.h>


class INextBotReply
{
public:
	enum FailureReason
	{
		FAIL_REJECTED      = 0, // AimHeadTowards denied the aim
		FAIL_PREEMPTED     = 1, // a higher priority aim preempted our aim
		FAIL_UNIMPLEMENTED = 2, // subclass didn't override IBody::AimHeadTowards
	};
	
	virtual void OnSuccess(INextBot *nextbot) {}
	virtual void OnFail(INextBot *nextbot, FailureReason reason) {}
};


class IBody : public INextBotComponent
{
public:
	enum LookAtPriorityType
	{
		PRI_BORING      = 0,
		PRI_INTERESTING = 1,
		PRI_IMPORTANT   = 2,
		PRI_CRITICAL    = 3,
		PRI_OVERRIDE    = 4,
	};
	
	enum PostureType
	{
		POSTURE_STAND  = 0,
		POSTURE_CROUCH = 1,
		POSTURE_SIT    = 2,
		POSTURE_LIE    = 4,
	};
	
	enum ArousalType
	{
		ALERT_YES = 0,
		ALERT_NO  = 1,
	};
	
	IBody() {}
	virtual ~IBody() {}
	
	virtual bool SetPosition(const Vector& pos)                                                                                           = 0;
	virtual Vector& GetEyePosition() const                                                                                                = 0;
	virtual Vector& GetViewVector() const                                                                                                 = 0;
	virtual void AimHeadTowards(const Vector& vec, LookAtPriorityType priority, float duration, INextBotReply *reply, const char *reason) = 0;
	virtual void AimHeadTowards(CBaseEntity *ent, LookAtPriorityType priority, float duration, INextBotReply *reply, const char *reason)  = 0;
	virtual bool IsHeadAimingOnTarget() const                                                                                             = 0;
	virtual bool IsHeadSteady() const                                                                                                     = 0;
	virtual float GetHeadSteadyDuration() const                                                                                           = 0;
	virtual float GetHeadAimSubjectLeadTime() const                                                                                       = 0;
	virtual float GetHeadAimTrackingInterval() const                                                                                      = 0;
	virtual void ClearPendingAimReply()                                                                                                   = 0;
	virtual float GetMaxHeadAngularVelocity() const                                                                                       = 0;
	virtual bool StartActivity(Activity a1, unsigned int i1)                                                                              = 0;
	virtual int SelectAnimationSequence(Activity a1) const                                                                                = 0;
	virtual Activity GetActivity() const                                                                                                  = 0;
	virtual bool IsActivity(Activity a1) const                                                                                            = 0;
	virtual bool HasActivityType(unsigned int i1) const                                                                                   = 0;
	virtual void SetDesiredPosture(PostureType posture)                                                                                   = 0;
	virtual PostureType GetDesiredPosture() const                                                                                         = 0;
	virtual bool IsDesiredPosture(PostureType posture) const                                                                              = 0;
	virtual bool IsInDesiredPosture() const                                                                                               = 0;
	virtual PostureType GetActualPosture() const                                                                                          = 0;
	virtual bool IsActualPosture(PostureType posture) const                                                                               = 0;
	virtual bool IsPostureMobile() const                                                                                                  = 0;
	virtual bool IsPostureChanging() const                                                                                                = 0;
	virtual void SetArousal(ArousalType arousal)                                                                                          = 0;
	virtual ArousalType GetArousal() const                                                                                                = 0;
	virtual bool IsArousal(ArousalType arousal) const                                                                                     = 0;
	virtual float GetHullWidth() const                                                                                                    = 0;
	virtual float GetHullHeight() const                                                                                                   = 0;
	virtual float GetStandHullHeight() const                                                                                              = 0;
	virtual float GetCrouchHullHeight() const                                                                                             = 0;
	virtual const Vector& GetHullMins() const                                                                                             = 0;
	virtual const Vector& GetHullMaxs() const                                                                                             = 0;
	virtual unsigned int GetSolidMask() const                                                                                             = 0;
	virtual int GetCollisionGroup() const                                                                                                 = 0;
};


#endif
