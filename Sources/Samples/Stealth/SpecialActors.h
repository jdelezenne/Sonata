/*=============================================================================
SpecialActors.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_SPECIALACTORS_H_
#define _SE_SPECIALACTORS_H_

#include "Common.h"
#include "World.h"

class PlaceableActor : public Actor
{
	SE_DECLARE_CLASS(PlaceableActor, Actor);

	SE_BEGIN_REFLECTION(PlaceableActor);
	SE_END_REFLECTION(PlaceableActor);

public:
	PlaceableActor();

public:
};


class SpawnActor : public Actor
{
	SE_DECLARE_CLASS(SpawnActor, Actor);

	SE_BEGIN_REFLECTION(SpawnActor);
	SE_END_REFLECTION(SpawnActor);

public:
	SpawnActor();

public:
};


class GoalActor : public Actor
{
	SE_DECLARE_CLASS(GoalActor, Actor);

	SE_BEGIN_REFLECTION(GoalActor);
	SE_END_REFLECTION(GoalActor);

public:
	GoalActor();

public:
};


enum WayPointMode
{
	WayPointMode_Normal,
	WayPointMode_Reverse,
	WayPointMode_Loop
};

SE_BEGIN_ENUM(WayPointMode);
	SE_Enum(Normal);
	SE_Enum(Reverse);
	SE_Enum(Loop);
SE_END_ENUM(WayPointMode);

class WayPointActor : public Actor
{
	SE_DECLARE_CLASS(WayPointActor, Actor);

	SE_BEGIN_REFLECTION(WayPointActor);
		SE_Field(_WayPointMode, WayPointMode, Public);
		SE_Field(_Previous, String, Public);
		SE_Field(_Next, String, Public);
		SE_Field(_Angle, real32, Public);
		SE_Field(_Wait, real32, Public);
	SE_END_REFLECTION(WayPointActor);

public:
	WayPointActor();

public:
	WayPointMode _WayPointMode;
	String _Previous;
	String _Next;
	real32 _Angle;
	real32 _Wait;
};

#endif 
