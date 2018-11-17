# PathFollower
SourcePawn interface for NextBot's PathFollower

[Example usages](https://github.com/Pelipoika/TF2_NextBot/) 

# WINDOWS ONLY
> In more recent builds of the TF2 dedicated server, the compiler has apparently been more aggressive about inlining some of these functions. To the point where there simply is no standalone version of the function in the final binary. It's just been integrated into all of the functions that called it, and then the compiler decided that there was no reason to keep a standalone, un-inlined version of the function around.

> For CBaseEntity::GetAbsOrigin, fortunately that's pretty easy, you can just reimplement the function yourself based on looking at the SDK2013 source code. (Ignore any Assert statements; those are excluded from release builds.) So to make CBaseEntity::GetAbsOrigin yourself, you do the following:
> 1. Check m_iEFlags for EFL_DIRTY_ABSTRANSFORM.
> 2. If that flag was set, then you need to call CBaseEntity::CalcAbsolutePosition. Otherwise, don't bother.
> 3. Return m_vecAbsOrigin.

> Path::Compute (both versions of it) is a far bigger, more complicated function and is therefore, unfortunately, not really feasible to reimplement manually.

> The Path::Compute situation is unfortunate; I know it's been that way for a while, and I'm not sure that there's really much to do about it. I can't remember, but it may have been that those two functions stopped having uninlined versions generated in the binary when Valve did their Toolchain Update for TF2; I guess the newer version of GCC they're using is more aggressive about function inlining, or they may be using different optimization flags than they were previously, or something. :shrug:

# Natives

## PathFollower.inc
```sourcepawn
enum TraverseWhenType 
{ 
	IMMEDIATELY,		// the entity will not block our motion - we'll carry right through
	EVENTUALLY			// the entity will block us until we spend effort to open/destroy it
};

enum SegmentType
{
	ON_GROUND,
	DROP_DOWN,
	CLIMB_UP,
	JUMP_OVER_GAP,
	LADDER_UP,
	LADDER_DOWN,
	
	NUM_SEGMENT_TYPES
};

enum MoveToFailureType
{
	FAIL_INVALID_PATH = 0,
	FAIL_STUCK        = 1,
	FAIL_FELL_OFF     = 2,
};

//struct PathFollowerSettings
//{
//	public float StepHeight;        /**< TFBots use 18.0   */
//	public float MaxJumpHeight;     /**< TFBots use 72.0   */
//	public float DeathDropHeight;   /**< TFBots use 1000.0 */
//	
//	public float SlopeLimit;        /**< TFBots use 0.6 */
//	
//	public int BodySolidMask;       /**< Use one of the MASK_ values from sdktools_trace.inc (MASK_SOLID, MASK_PLAYERSOLID, MASK_NPCSOLID, etc) */
//	
//	public float LookAheadDistance; /**< How far along the path the bot will look, to shortcut a bit; TFBots use 300.0 */
//	
//	public float RePathInterval;    /**< How often to completely recalculate the path, in seconds; recommended value is somewhere between 0.05 and 1.00 seconds */
//};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// NATIVES ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Create a PathFollower for an entity (see above for some reasonable parameter values)
native void PF_Create(int bot_entidx, float step_height, float max_jump_height, float death_drop_height, float slope_limit, int body_solid_mask, float look_ahead_distance, float repath_interval, float hull_scale_xy = 1.0, float hull_scale_z = 1.0);
// Destroy a PathFollower previously created for an entity
native void PF_Destroy(int bot_entidx);
// Check whether a PathFollower has already been created for an entity
native bool PF_Exists(int bot_entidx);


// Set the PathFollower's goal to a vector coordinate
native void PF_SetGoalVector(int bot_entidx, const float vec[3]);
// Get the PathFollower's goal to a float array
native void PF_GetGoalVector(int bot_entidx, float out[3]);

// Set the PathFollower's goal to an entity
// - The goal entity must be a CBaseCombatCharacter-derived entity class
// - You can call PF_IsEntityACombatCharacter to determine ahead of time whether a goal entity is valid
native void PF_SetGoalEntity(int bot_entidx, int goal_entidx);

// You can change a PathFollower's goal as many times as you want
// You can also switch between vector and entity goals without limitation
// (But obviously you can't have both at once; the latest one you set sticks)


// Determine whether it would hypothetically be possible to build a path to the given vector
// - Returns true if the path can be constructed, false if it's not possible
// - The paths length to the tested position is returned as a param (length)
native bool PF_IsPathToVectorPossible(int bot_entidx, const float vec[3], float &length = -1.0);
// Determine whether it would hypothetically be possible to build a path to the given entity
// - Returns true if the path can be constructed, false if it's not possible
// - The goal entity needs to be a combat character (same situation as with PF_SetGoalEntity)
// - The paths length to the tested entity is returned as a param (length)
native bool PF_IsPathToEntityPossible(int bot_entidx, int goal_entidx, float &length = -1.0);

// Start doing path computations and updates
native void PF_StartPathing(int bot_entidx);
// Stop doing path computations and updates
native void PF_StopPathing(int bot_entidx);

// Ask for the coordinates of a path segment beyond the immediate goal segment
// - The num parameter determines which segment to get: 0 => current goal segment, 1 => the one after that, etc
// - If a segment that far ahead doesn't exist, the native will return false
native bool PF_GetFutureSegment(int bot_entidx, int num, float vec[3]);

// Request a pointer to the PluginBot object associated with a particular entity (Address_Null if none exists)
native Address PF_GetPtr_PluginBot(int bot_entidx);
// Request a pointer to the PluginBotLocomotion object associated with a particular entity (Address_Null if none exists)
native Address PF_GetPtr_PluginBotLocomotion(int bot_entidx);
// Request a pointer to the PluginBotBody object associated with a particular entity (Address_Null if none exists)
native Address PF_GetPtr_PluginBotBody(int bot_entidx);
// Request a pointer to the PluginBotIntention object associated with a particular entity (Address_Null if none exists)
native Address PF_GetPtr_PluginBotIntention(int bot_entidx);

// Query whether a particular entity is a derivative of CBaseCombatCharacter or not
native bool PF_IsEntityACombatCharacter(int entidx);

// Update an entitys last known area
native void PF_UpdateLastKnownArea(int entidx);
// Get an entitys last known area
native NavArea PF_GetLastKnownArea(int entidx);

// Return true if this locomotor could potentially move along the line given.
// - If false is returned, fraction of walkable ray is returned in 'fraction'
native bool PF_IsPotentiallyTraversable(int bot_entidx, float from[3], float to[3], TraverseWhenType when, float &fraction = -1.0);
// Return true if there is a the given discontinuity ahead in the path within the given range.
// - (-1 = entire remaining path)
native bool PF_IsDiscontinuityAhead(int bot_entidx, SegmentType type, float &range);
 // Return true if there is a possible "gap" that will need to be jumped over
 // - If true is returned, fraction of ray before gap is returned in 'fraction'
native bool PF_HasPotentialGap(int bot_entidx, float from[3], float desiredTo[3], float &fraction = 1.0);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CALLBACKS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum PFCallbackType
{
	PFCB_Approach            = 0,
	PFCB_IsEntityTraversable = 1,
	PFCB_GetPathCost         = 2,
	PFCB_PathFailed          = 3,
	PFCB_PathSuccess         = 4,
	PFCB_EntityGone          = 5,
	PFCB_ClimbUpToLedge      = 6,
	PFCB_OnMoveToSuccess     = 7,
	PFCB_OnMoveToFailure     = 8,
	PFCB_OnActorEmoted       = 9,
};

typeset PFCallback
{
	function void(int bot_entidx, const float dst[3]);
	// Approach
	// Called when the PathFollower wants to tell you where to go next
	// - This seamlessly integrates obstacle avoidance as well as path lookahead (shortcutting)
	
	
	function bool(int bot_entidx, const float dst[3], const float dir[3]);
	// ClimbUpToLedge
	// Called when the PathFollower needs to jump up onto a ledge
	// - The dst vector contains the location of the closest point at the edge of the ledge
	// - The dir vector is a 3-dimensional (x,y,z) unit vector (length = 1) indicating the direction that the bot should jump to reach that point
	// - Your own code is responsible for doing the actual jumping itself
	
	
	function bool(int bot_entidx, int other_entidx, TraverseWhenType when);
	// IsEntityTraversable
	// Called to ask whether an entity in the way of the path can be passed through
	// - The default value should be false
	// - The NextBot implementation of this function handles "prop_door*", "func_door*", and breakable "func_brush" entities
	
	
	function float(int bot_entidx, NavArea area, NavArea from_area, float length);
	// GetPathCost
	// User-provided path cost functor
	// - If this forward isn't implemented, the PathFollower will use a default cost functor based on ShortestPathCost
	
	
	function void(int bot_entidx);
	// PathFailed
	// Called to inform you that the PathFollower cannot find a path to the current goal
	
	function void(int bot_entidx);
	// PathSucces
	// Called to inform you that the PathFollower successfully found a path to the current goal
	
	function void(int bot_entidx);
	// EntityGone
	// Called to inform you that the entity associated with a PathFollower went away
	// - If you get this callback, then you won't have to call PF_Destroy for the entity

	function void(int bot_entidx, Address path);
	// OnMoveToSuccess
	// Called when an bot reaches its MoveTo goal

	function void(int bot_entidx, Address path, MoveToFailureType fail)
	// OnMoveToFailure
	// Called when an bot fails to reach a MoveTo goal

	function void(int bot_entidx, int who, int concept)
	// OnActorEmoted
	// Called when "emoter" does an "emote" (ie: manual voice command, etc)
};

// Enable a callback function for a particular bot
// - If already enabled, calling this again overwrites the old callback function with the new one
native void PF_EnableCallback(int bot_entidx, PFCallbackType type, PFCallback func);
// Disable a callback function for a particular bot
native void PF_DisableCallback(int bot_entidx, PFCallbackType type);
// Check whether a callback function is enabled for a particular bot
native bool PF_IsCallbackEnabled(int bot_entidx, PFCallbackType type);
```

## PathFollower_Nav.inc
```sourcepawn
methodmap NavArea
{
	//Return this area's unique ID
	public native int GetID();

	//Returns the center most vector point for the given CNavArea.
	public native void GetCenter(float out[3]);

	//Returns a random point on the nav area.
	public native void GetRandomPoint(float out[3]);

	//Return Z of area at (x,y) of 'pos'
	public native float GetZ(float x, float y);
	
	//Internal function used when generating a path from one point to another in PathFollower::Compute; The cost so far.
	public native float GetCostSoFar();
	
	//Return height change between edges of adjacent nav areas (not actual underlying ground)
	public native float ComputeAdjacentConnectionHeightChange(NavArea destinationArea);

	//Returns true if the given CNavArea has this attribute flag set.
	public native bool HasAttributes(NavAttributeType bits);

	//Return number of players of given team currently within this area (team of zero means any/all)
	//BUG counts spectators.
	//BUG weird
	//I think you want to subtract 1 from GetClientTeam for this
	public native int GetPlayerCount(int teamID = TEAM_ANY);

	//Computes the area's normal based on m_nwCorner.  If 'alternate' is specified, m_seCorner is used instead.
	public native void ComputeNormal(float out[3], bool alternate = false);

	//Returns the vector position of the corner for the given CNavArea.
	public native void GetCorner(float out[3], NavCornerType corner);
	
	//Return true if area is visible from the given eyepoint, returns visible spot in visSpot param.
	public bool IsVisible(float eye[3], float visSpot[3] = NULL_VECTOR);

	//Returns the amount of CNavAreas that have a connection ( one or two way ) from this CNavArea in given direction.
	public native int GetAdjacentCount(NavDirType dir);

	//Return the i'th adjacent area in the given direction
	public native NavArea GetAdjacentArea(NavDirType dir, int i);
};

enum NavMesh
{
	TheNavMesh
};

methodmap NavMesh
{
	public native NavArea GetNavArea_Vec(const float pos[3], float beneathLimit = 120.0);
	
	public native NavArea GetNearestNavArea_Vec(const float pos[3], bool anyZ = false, float maxDist = 10000.0, bool checkLOS = false, bool checkGround = true, int team = TEAM_ANY);
};

```
