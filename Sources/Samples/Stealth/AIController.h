/*=============================================================================
AIController.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_AICONTROLLER_H_
#define _SE_AICONTROLLER_H_

#include "Common.h"
#include "GameController.h"

class CharacterActorState;

class AIController : public GameController
{
	SE_DECLARE_ABSTRACT(AIController, GameController);

	SE_BEGIN_REFLECTION(AIController);
	SE_END_REFLECTION(AIController);

public:
	AIController();
	virtual ~AIController();

	CharacterActorState* GetCharacterActorState() const { return _CharacterActorState; }
	CharacterActorState* GetNextActorState() const { return _NextActorState; }
	void SetCharacterActorState(const String& value);

	virtual void Update(real64 elapsed);
	virtual void Draw();

	void OnPlayerKilled();

protected:
	Mesh* CreateLOSMesh(real32 viewConeAngle, real32 viewDistance);
	void SetLOSMeshColor(const Color32& color);

	bool CheckLineOfSight();
	void ChangeActorState();

public:
	bool _TargetVisible;
	Vector3 _TargetLastVisiblePosition;
	real64 _TargetLostTime;

protected:
	CharacterActorState* _CharacterActorState;
	CharacterActorState* _NextActorState;
	String _NextActorStateName;

	MeshPtr _LOSMesh;
	real64 _LastLOSCheck;
};

#endif 
