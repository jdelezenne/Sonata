/*=============================================================================
SpecialActors.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SpecialActors.h"
#include "SampleStealth.h"

SE_IMPLEMENT_CLASS(PlaceableActor);
SE_IMPLEMENT_REFLECTION(PlaceableActor);

PlaceableActor::PlaceableActor() :
	super()
{
}


SE_IMPLEMENT_CLASS(SpawnActor);
SE_IMPLEMENT_REFLECTION(SpawnActor);

SpawnActor::SpawnActor() :
	super()
{
	_Design = true;
}


SE_IMPLEMENT_CLASS(GoalActor);
SE_IMPLEMENT_REFLECTION(GoalActor);

GoalActor::GoalActor() :
	super()
{
}


SE_IMPLEMENT_CLASS(WayPointActor);
SE_IMPLEMENT_REFLECTION(WayPointActor);

WayPointActor::WayPointActor() :
	super()
{
	_Design = true;

	_WayPointMode = WayPointMode_Normal;
	_Angle = 0.0f;
	_Wait = 0.0f;
}
