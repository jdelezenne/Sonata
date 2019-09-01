/*=============================================================================
Character.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_CHARACTER_H_
#define _SE_CHARACTER_H_

#include "Common.h"
#include "World.h"

class CharacterActor : public Actor
{
	SE_DECLARE_CLASS(CharacterActor, Actor);

	SE_BEGIN_REFLECTION(CharacterActor);
		SE_Field(_WalkSpeed, real32, Public);
		SE_Field(_RunSpeed, real32, Public);
		SE_Field(_ViewConeAngle, real32, Public);
		SE_Field(_ViewDistance, real32, Public);
		SE_Field(_WayPoint, String, Public);
	SE_END_REFLECTION(CharacterActor);

public:
	CharacterActor();

	virtual void Update(real64 elapsed);

public:
	real32 _WalkSpeed;
	real32 _RunSpeed;
	real32 _currentSpeed;
	real32 _ViewConeAngle;
	real32 _ViewDistance;
	Vector3 _GuardPosition;
	Quaternion _GuardOrientation;
	String _WayPoint;
};

#endif 
