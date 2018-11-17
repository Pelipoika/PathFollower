#ifndef NEXTBOT_PATH_NEXTBOTPATHFOLLOW_H
#define NEXTBOT_PATH_NEXTBOTPATHFOLLOW_H


#include "NextBotPath.h"


class PathFollower : public Path
{
public:
	PathFollower();
	virtual ~PathFollower();
	
	virtual void Update(INextBot *nextbot)                                                       = 0;
	virtual void SetMinLookAheadDistance(float dist)                                             = 0;
	virtual CBaseEntity *GetHindrance() const                                                    = 0;
	virtual bool IsDiscontinuityAhead(INextBot *nextbot, SegmentType type, float max_dist) const = 0;
	
#if 0
private:
	enum : size_t
	{
		PATHFOLLOWER_REAL_SIZE = 0x5000,
	};
	
	static void *operator new(size_t sz)
	{
		assert(sz == sizeof(PathFollower));
		return ::operator new(PATHFOLLOWER_REAL_SIZE);
	}
	static void *operator new[](size_t sz)
	{
		assert(sz % sizeof(PathFollower) == 0);
		return ::operator new[]((sz / sizeof(PathFollower)) * PATHFOLLOWER_REAL_SIZE);
	}
#endif
};


#endif
