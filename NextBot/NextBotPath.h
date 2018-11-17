#ifndef NEXTBOT_PATH_NEXTBOTPATH_H
#define NEXTBOT_PATH_NEXTBOTPATH_H


#include "Nav/nav.h"


class CFuncElevator;
class CBaseCombatCharacter;
class INextBot;


class IPathCost
{
public:
	virtual float operator()(CNavArea *area, CNavArea *fromArea, const CNavLadder *ladder = nullptr, const CFuncElevator *elevator = nullptr, float length = -1.0f) const = 0;
};


class IPathOpenGoalSelector
{
public:
	virtual CNavArea *operator()(CNavArea *area, CNavArea *fromArea) const;
};


class Path
{
public:
	enum
	{
		MAX_SEGMENTS  = 256,
		MAX_ADJ_AREAS = 64,
	};
	
	enum SegmentType
	{
		SEG_GROUND      = 0, // ground movement
		SEG_FALL        = 1, // falling down
		SEG_CLIMBJUMP   = 2, // climbing up or jumping
		SEG_GAPJUMP     = 3, // gap jumping
		SEG_LADDER_UP   = 4, // ladder up
		SEG_LADDER_DOWN = 5, // ladder down
	};
	
	enum SeekType
	{
		SEEK_FROM_START  = 0,
		SEEK_FROM_CURSOR = 1,
		SEEK_MAX,
	};
	
	enum MoveCursorType
	{
		MOVECUR_ABS = 0,
		MOVECUR_REL = 1,
		MOVECUR_MAX,
	};
	
	enum ResultType {};
	
	struct Segment
	{
		const CNavArea *area;     // The navmesh area this segment occupies
		NavTraverseType how;      // The direction of travel to reach the end of this segment from the start, represented as a cardinal direction integer 0 to 3, or 9 for vertical movement
		Vector pos;               // The position of the end of this segment
		const CNavLadder *ladder; // The navmesh ladder this segment occupies, if any
		SegmentType type;         // The movement type of this segment, indicating how bots are expected to move along this segment
		Vector forward;           // The direction of travel to reach the end of this segment from the start, represented as a normalised vector
		float length;             // Length of this segment
		float distanceFromStart;  // Distance of this segment from the start of the path
		float curvature;
		Vector m_portalCenter;
		float m_portalHalfWidth;
	};
	
	struct CursorData;
	struct AdjacentArea;
	
	Path();
	virtual ~Path();
	
	virtual float GetLength() const                                                                                                                      = 0;
	
	virtual const Vector& GetPosition(float dist, const Segment *seg) const                                                                              = 0;
	virtual const Vector& GetClosestPosition(const Vector& near, const Segment *seg, float dist) const                                                   = 0;
	virtual const Vector& GetStartPosition() const                                                                                                       = 0;
	virtual const Vector& GetEndPosition() const                                                                                                         = 0;
	
	virtual CBaseEntity *GetSubject() const                                                                                                              = 0;
	virtual const Segment *GetCurrentGoal() const                                                                                                        = 0;
	virtual float GetAge() const                                                                                                                         = 0;
	
	virtual void MoveCursorToClosestPosition(const Vector& near, SeekType stype, float dist) const                                                       = 0;
	virtual void MoveCursorToStart()                                                                                                                     = 0;
	virtual void MoveCursorToEnd()                                                                                                                       = 0;
	virtual void MoveCursor(float dist, MoveCursorType mctype)                                                                                           = 0;
	
	virtual float GetCursorPosition() const                                                                                                              = 0;
	virtual const CursorData& GetCursorData() const                                                                                                      = 0;
	
	virtual bool IsValid() const                                                                                                                         = 0;
	virtual void Invalidate()                                                                                                                            = 0;
	
	virtual void Draw(const Segment *seg) const                                                                                                          = 0;
	virtual void DrawInterpolated(float from, float to)                                                                                                  = 0;
	
	virtual const Segment *FirstSegment() const                                                                                                          = 0;
	virtual const Segment *NextSegment(const Segment *seg) const                                                                                         = 0;
	virtual const Segment *PriorSegment(const Segment *seg) const                                                                                        = 0;
	virtual const Segment *LastSegment() const                                                                                                           = 0;
	
	virtual void OnPathChanged(INextBot *nextbot, ResultType rtype)                                                                                      = 0;
	
	virtual void Copy(INextBot *nextbot, const Path& that)                                                                                               = 0;
	
	virtual bool ComputeWithOpenGoal(INextBot *nextbot, const IPathCost& cost_func, const IPathOpenGoalSelector& sel, float f1 = 0.0f)                   = 0;
	virtual void ComputeAreaCrossing(INextBot *nextbot, const CNavArea *area, const Vector& from, const CNavArea *to, NavDirType dir, Vector *out) const = 0;
	
	template<typename PathCost> bool Compute(INextBot *nextbot, const Vector& goal, PathCost& cost_func, float maxPathLength = 0.0f, bool b1 = true);
	template<typename PathCost> bool Compute(INextBot *nextbot, CBaseCombatCharacter *subject, PathCost& cost_func, float maxPathLength = 0.0f, bool b1 = true);
};


#endif
