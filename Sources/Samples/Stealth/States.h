/*=============================================================================
States.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_STATES_H_
#define _SE_STATES_H_

#include "Common.h"
#include "World.h"
#include "Actions.h"

class AIController;
class CharacterActor;

class State : public RefObject
{
	SE_DECLARE_ABSTRACT(State, RefObject);

	SE_BEGIN_REFLECTION(State);
	SE_END_REFLECTION(State);

public:
	State();
	virtual ~State();

	virtual void PreUpdate(real64 elapsed);
	virtual void Update(real64 elapsed);
	virtual void PostUpdate(real64 elapsed);

protected:
	real64 _ElapsedTime;
	bool _FirstUpdate;
};


class CharacterActorState : public State
{
	SE_DECLARE_ABSTRACT(CharacterActorState, State);

	SE_BEGIN_REFLECTION(CharacterActorState);
	SE_END_REFLECTION(CharacterActorState);

public:
	enum States
	{
		Idle,
		Alert,//
		Follow,
		Combat,
		GotoTargetLost,
		Search,
		Guard,
		Patrol,
		Dead//
	};

	CharacterActorState();
	virtual ~CharacterActorState();

	AIController* GetAI() const { return _AIController; }
	CharacterActor* GetCharacter() const { return _Character; }

	virtual void Initialize(AIController* ai);

	virtual States GetState() const = 0;

protected:
	AIController* _AIController;
	CharacterActor* _Character;
};


class CharacterActorStateIdle : public CharacterActorState
{
	SE_DECLARE_CLASS(CharacterActorStateIdle, CharacterActorState);

	SE_BEGIN_REFLECTION(CharacterActorStateIdle);
	SE_END_REFLECTION(CharacterActorStateIdle);

public:
	CharacterActorStateIdle();
	virtual ~CharacterActorStateIdle();

	virtual States GetState() const { return CharacterActorState::Idle; }

	virtual void Update(real64 elapsed);
};


class CharacterActorStateGotoBase : public CharacterActorState
{
	SE_DECLARE_ABSTRACT(CharacterActorStateGotoBase, CharacterActorState);

	SE_BEGIN_REFLECTION(CharacterActorStateGotoBase);
	SE_END_REFLECTION(CharacterActorStateGotoBase);

public:
	CharacterActorStateGotoBase();
	virtual ~CharacterActorStateGotoBase();

	void SetDestination(const Vector3& value) { _Destination = value; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);

protected:
	CharacterActorActionMove* _ActionMove;
	Vector3 _Destination;
};


class CharacterActorStateFollow : public CharacterActorStateGotoBase
{
	SE_DECLARE_CLASS(CharacterActorStateFollow, CharacterActorStateGotoBase);

	SE_BEGIN_REFLECTION(CharacterActorStateFollow);
	SE_END_REFLECTION(CharacterActorStateFollow);

public:
	CharacterActorStateFollow();
	virtual ~CharacterActorStateFollow();

	virtual States GetState() const { return CharacterActorState::Follow; }

	virtual void Initialize(AIController* ai);

	virtual void PreUpdate(real64 elapsed);
	virtual void Update(real64 elapsed);

protected:
	Actor* _Target;
};


class CharacterActorStateCombat : public CharacterActorState
{
	SE_DECLARE_CLASS(CharacterActorStateCombat, CharacterActorState);

	SE_BEGIN_REFLECTION(CharacterActorStateCombat);
	SE_END_REFLECTION(CharacterActorStateCombat);

public:
	CharacterActorStateCombat();
	virtual ~CharacterActorStateCombat();

	virtual States GetState() const { return CharacterActorState::Combat; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);

protected:
};


class CharacterActorStateGotoTargetLost : public CharacterActorStateGotoBase
{
	SE_DECLARE_CLASS(CharacterActorStateGotoTargetLost, CharacterActorStateGotoBase);

	SE_BEGIN_REFLECTION(CharacterActorStateGotoTargetLost);
	SE_END_REFLECTION(CharacterActorStateGotoTargetLost);

public:
	CharacterActorStateGotoTargetLost();
	virtual ~CharacterActorStateGotoTargetLost();

	virtual States GetState() const { return CharacterActorState::GotoTargetLost; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);
};


class CharacterActorStateSearch : public CharacterActorState
{
	SE_DECLARE_CLASS(CharacterActorStateSearch, CharacterActorState);

	SE_BEGIN_REFLECTION(CharacterActorStateSearch);
	SE_END_REFLECTION(CharacterActorStateSearch);

public:
	CharacterActorStateSearch();
	virtual ~CharacterActorStateSearch();

	virtual States GetState() const { return CharacterActorState::Search; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);

protected:
};


class CharacterActorStateGuard : public CharacterActorStateGotoBase
{
	SE_DECLARE_CLASS(CharacterActorStateGuard, CharacterActorStateGotoBase);

	SE_BEGIN_REFLECTION(CharacterActorStateGuard);
	SE_END_REFLECTION(CharacterActorStateGuard);

public:
	CharacterActorStateGuard();
	virtual ~CharacterActorStateGuard();

	virtual States GetState() const { return CharacterActorState::Guard; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);

protected:
};


class CharacterActorStatePatrol : public CharacterActorStateGotoBase
{
	SE_DECLARE_CLASS(CharacterActorStatePatrol, CharacterActorStateGotoBase);

	SE_BEGIN_REFLECTION(CharacterActorStatePatrol);
	SE_END_REFLECTION(CharacterActorStatePatrol);

public:
	CharacterActorStatePatrol();
	virtual ~CharacterActorStatePatrol();

	virtual States GetState() const { return CharacterActorState::Patrol; }

	virtual void Initialize(AIController* ai);

	virtual void Update(real64 elapsed);

protected:
	String _NextWayPoint;
	bool _WayPointReverse;
};

#endif 
