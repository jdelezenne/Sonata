/*=============================================================================
Actions.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_ACTIONS_H_
#define _SE_ACTIONS_H_

#include "Common.h"
#include "World.h"

class CharacterActor;

enum ActionStatus
{
	ActionStatus_Inactive,
	ActionStatus_Active,
	ActionStatus_Done
};

class CharacterActorAction : public RefObject
{
	SE_DECLARE_ABSTRACT(CharacterActorAction, RefObject);

	SE_BEGIN_REFLECTION(CharacterActorAction);
	SE_END_REFLECTION(CharacterActorAction);

public:
	enum Actions
	{
		Action_Move
	};

	CharacterActorAction();
	virtual ~CharacterActorAction();

	virtual void Update(real64 elapsed) = 0;

	CharacterActor* GetCharacter() const { return _Character; }
	ActionStatus GetStatus() const { return _ActionStatus; }

	virtual void Initialize(CharacterActor* character);

	virtual Actions GetAction() const = 0;

protected:
	CharacterActor* _Character;
	ActionStatus _ActionStatus;
};


class CharacterActorActionMove : public CharacterActorAction
{
	SE_DECLARE_CLASS(CharacterActorActionMove, CharacterActorAction);

	SE_BEGIN_REFLECTION(CharacterActorActionMove);
	SE_END_REFLECTION(CharacterActorActionMove);

public:
	CharacterActorActionMove();
	virtual ~CharacterActorActionMove();

	virtual Actions GetAction() const { return CharacterActorAction::Action_Move; }

	void SetDestination(const Vector3& value);

	virtual void Initialize(CharacterActor* character);

	virtual void Update(real64 elapsed);

protected:
	Vector3 _Destination;
	ActorPath _Path;
};

#endif 
