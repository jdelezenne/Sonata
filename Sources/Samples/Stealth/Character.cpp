/*=============================================================================
Character.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Character.h"
#include "SampleStealth.h"

SE_IMPLEMENT_CLASS(CharacterActor);
SE_IMPLEMENT_REFLECTION(CharacterActor);

CharacterActor::CharacterActor() :
	super()
{
	_WalkSpeed = 64.0f;
	_RunSpeed = 96.0f;
	_currentSpeed = _WalkSpeed;
	_ViewConeAngle = 90.0f;
	_ViewDistance = 300.0f;
	_GuardPosition = Vector3::Zero;
	_GuardOrientation = Quaternion::Identity;
}

void CharacterActor::Update(real64 elapsed)
{
	super::Update(elapsed);
}
