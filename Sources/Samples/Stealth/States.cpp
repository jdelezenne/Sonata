/*=============================================================================
States.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "States.h"
#include "SampleStealth.h"
#include "SpecialActors.h"

SE_IMPLEMENT_ABSTRACT(State);
SE_IMPLEMENT_REFLECTION(State);

State::State() :
	RefObject()
{
	_ElapsedTime = 0.0;
	_FirstUpdate = true;
}

State::~State()
{
}

void State::PreUpdate(real64 elapsed)
{
	_ElapsedTime += elapsed;
}

void State::Update(real64 elapsed)
{
}

void State::PostUpdate(real64 elapsed)
{
	_FirstUpdate = false;
}


SE_IMPLEMENT_ABSTRACT(CharacterActorState);
SE_IMPLEMENT_REFLECTION(CharacterActorState);

CharacterActorState::CharacterActorState() :
	super()
{
	_AIController = NULL;
	_Character = NULL;
}

CharacterActorState::~CharacterActorState()
{
}

void CharacterActorState::Initialize(AIController* ai)
{
	_AIController = ai;
	_Character = ai->GetCharacter();
}


SE_IMPLEMENT_CLASS(CharacterActorStateIdle);
SE_IMPLEMENT_REFLECTION(CharacterActorStateIdle);

CharacterActorStateIdle::CharacterActorStateIdle() :
	super()
{
}

CharacterActorStateIdle::~CharacterActorStateIdle()
{
}

void CharacterActorStateIdle::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		_AIController->SetCharacterActorState("Follow");

		CharacterActorStateFollow* state = (CharacterActorStateFollow*)_AIController->GetNextActorState();
		state->SetDestination(_AIController->_TargetLastVisiblePosition);
		return;
	}

	if (_Character->_WayPoint.IsEmpty())
		_AIController->SetCharacterActorState("Guard");
	else
		_AIController->SetCharacterActorState("Patrol");
}


SE_IMPLEMENT_ABSTRACT(CharacterActorStateGotoBase);
SE_IMPLEMENT_REFLECTION(CharacterActorStateGotoBase);

CharacterActorStateGotoBase::CharacterActorStateGotoBase() :
	super()
{
	_ActionMove = new CharacterActorActionMove();
	_Destination = Vector3::Zero;
}

void CharacterActorStateGotoBase::Initialize(AIController* ai)
{
	super::Initialize(ai);

	_Character->_currentSpeed = _Character->_WalkSpeed;
	_ActionMove->Initialize(_Character);
}

CharacterActorStateGotoBase::~CharacterActorStateGotoBase()
{
	SE_DELETE(_ActionMove);
}

void CharacterActorStateGotoBase::Update(real64 elapsed)
{
	super::Update(elapsed);

	_ActionMove->Update(elapsed);

	_Character->LookAt(_Destination);
}


SE_IMPLEMENT_CLASS(CharacterActorStateFollow);
SE_IMPLEMENT_REFLECTION(CharacterActorStateFollow);

CharacterActorStateFollow::CharacterActorStateFollow() :
	super()
{
	_Target = NULL;
}

void CharacterActorStateFollow::Initialize(AIController* ai)
{
	super::Initialize(ai);

	_Character->_currentSpeed = _Character->_RunSpeed;
}

CharacterActorStateFollow::~CharacterActorStateFollow()
{
}

void CharacterActorStateFollow::PreUpdate(real64 elapsed)
{
	super::PreUpdate(elapsed);

	if (_AIController->_TargetVisible)
	{
		SetDestination(_AIController->_TargetLastVisiblePosition);

		if (_ActionMove->GetStatus() != ActionStatus_Active)
		{
			_ActionMove->SetDestination(_Destination);
		}
	}
}

void CharacterActorStateFollow::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		Player* player = GameCore::Instance()->GetWorld()->GetPlayer();

		Matrix4 m0 = Matrix4(Matrix3::CreateFromQuaternion(_Character->_Orientation), _Character->_Position);
		AABB aabb0 = _Character->_ActorDef->_Model->GetBoundingBox();
		aabb0.Transform(m0);

		Matrix4 m1 = Matrix4(Matrix3::CreateFromQuaternion(player->GetCharacter()->_Orientation), player->GetCharacter()->_Position);
		AABB aabb1 = player->GetCharacter()->_ActorDef->_Model->GetBoundingBox();
		aabb1.Transform(m1);
		if (aabb0.Intersects(aabb1))
		{
			_AIController->SetCharacterActorState("Combat");
			return;
		}
	}
	else
	{
		real64 t = GameCore::GetTime();
		if (t - _AIController->_TargetLostTime > 0.5)
		{
			_AIController->SetCharacterActorState("GotoTargetLost");

			CharacterActorStateGotoTargetLost* state = (CharacterActorStateGotoTargetLost*)_AIController->GetNextActorState();
			state->SetDestination(_AIController->_TargetLastVisiblePosition);
			return;
		}
	}
}


SE_IMPLEMENT_CLASS(CharacterActorStateCombat);
SE_IMPLEMENT_REFLECTION(CharacterActorStateCombat);

CharacterActorStateCombat::CharacterActorStateCombat() :
	super()
{
}

void CharacterActorStateCombat::Initialize(AIController* ai)
{
	super::Initialize(ai);
}

CharacterActorStateCombat::~CharacterActorStateCombat()
{
}

void CharacterActorStateCombat::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_ElapsedTime > 1.0)
	{
		Logger::Current()->Log(LogLevel::Information, _T("CharacterActorStateCombat.Update"),
			_T("Character <") + _Character->_name + _T("> killed the player!"));

		//GameCore::Instance()->GetWorld()->EndGameRequest();
		GameCore::Instance()->GetWorld()->KillPlayer();
		_AIController->SetCharacterActorState("Idle");
		return;
	}
}


SE_IMPLEMENT_CLASS(CharacterActorStateGotoTargetLost);
SE_IMPLEMENT_REFLECTION(CharacterActorStateGotoTargetLost);

CharacterActorStateGotoTargetLost::CharacterActorStateGotoTargetLost() :
	super()
{
}

void CharacterActorStateGotoTargetLost::Initialize(AIController* ai)
{
	super::Initialize(ai);
}

CharacterActorStateGotoTargetLost::~CharacterActorStateGotoTargetLost()
{
}

void CharacterActorStateGotoTargetLost::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		_AIController->SetCharacterActorState("Follow");

		CharacterActorStateFollow* state = (CharacterActorStateFollow*)_AIController->GetNextActorState();
		state->SetDestination(_AIController->_TargetLastVisiblePosition);
		return;
	}

	if (_ActionMove->GetStatus() != ActionStatus_Active)
	{
		if (_ActionMove->GetStatus() == ActionStatus_Inactive)
		{
			_ActionMove->SetDestination(_Destination);
		}
		else if (_ActionMove->GetStatus() == ActionStatus_Done)
		{
			_AIController->SetCharacterActorState("Search");
			return;
		}
	}
}


SE_IMPLEMENT_CLASS(CharacterActorStateSearch);
SE_IMPLEMENT_REFLECTION(CharacterActorStateSearch);

CharacterActorStateSearch::CharacterActorStateSearch() :
	super()
{
}

void CharacterActorStateSearch::Initialize(AIController* ai)
{
	super::Initialize(ai);
}

CharacterActorStateSearch::~CharacterActorStateSearch()
{
}

void CharacterActorStateSearch::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		_AIController->SetCharacterActorState("Follow");

		CharacterActorStateFollow* state = (CharacterActorStateFollow*)_AIController->GetNextActorState();
		state->SetDestination(_AIController->_TargetLastVisiblePosition);
		return;
	}

	if (_ElapsedTime > 4.0)
	{
		_AIController->SetCharacterActorState("Idle");
		return;
	}

	if (_ElapsedTime < 2.0)
	{
		//_Character->LookAt(Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi/4.0/2.0) * Vector3::UnitY);
		_Character->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, Math::Pi/4.0 * 2.0 * elapsed);
	}
	else if (_ElapsedTime < 4.0)
	{
		//_Character->LookAt(Quaternion::FromAxisAngle(Vector3::UnitZ, Math::Pi/4.0/2.0) * Vector3::UnitY);
		_Character->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi/4.0 * 2.0 * elapsed);
	}
}


SE_IMPLEMENT_CLASS(CharacterActorStateGuard);
SE_IMPLEMENT_REFLECTION(CharacterActorStateGuard);

CharacterActorStateGuard::CharacterActorStateGuard() :
	super()
{
}

void CharacterActorStateGuard::Initialize(AIController* ai)
{
	super::Initialize(ai);
}

CharacterActorStateGuard::~CharacterActorStateGuard()
{
}

void CharacterActorStateGuard::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		_AIController->SetCharacterActorState("Follow");

		CharacterActorStateFollow* state = (CharacterActorStateFollow*)_AIController->GetNextActorState();
		state->SetDestination(_AIController->_TargetLastVisiblePosition);
		return;
	}
	else
	{
		SetDestination(_Character->_GuardPosition);
	}
}


SE_IMPLEMENT_CLASS(CharacterActorStatePatrol);
SE_IMPLEMENT_REFLECTION(CharacterActorStatePatrol);

CharacterActorStatePatrol::CharacterActorStatePatrol() :
	super()
{
	_WayPointReverse = false;
}

void CharacterActorStatePatrol::Initialize(AIController* ai)
{
	super::Initialize(ai);
}

CharacterActorStatePatrol::~CharacterActorStatePatrol()
{
}

void CharacterActorStatePatrol::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_AIController->_TargetVisible)
	{
		_AIController->SetCharacterActorState("Follow");

		CharacterActorStateFollow* state = (CharacterActorStateFollow*)_AIController->GetNextActorState();
		state->SetDestination(_AIController->_TargetLastVisiblePosition);
		return;
	}
	else
	{
		// If not active, we just entered to this state
		if (_ActionMove->GetStatus() != ActionStatus_Active)
		{
			// Check if a waypoint is assigned, if not assign the first one
			if (_NextWayPoint.IsEmpty())
				_NextWayPoint = _Character->_WayPoint;

			// Check if we have a checkpoint
			WayPointActor* waypoint = NULL;
			if (!_NextWayPoint.IsEmpty())
			{
				// Retrieve it from its name
				Map* map = GameCore::Instance()->GetWorld()->GetMap();
				Actor* actor = map->GetActorByName(_NextWayPoint);
				if (actor != NULL && actor->_ActorDef->_ActorType == ActorType_WayPoint)
					waypoint = (WayPointActor*)actor;
			}

			if (waypoint != NULL)
			{
				// We found it, move to it
				SetDestination(waypoint->_Position);
				_ActionMove->SetDestination(_Destination);

				// Get the next waypoint
				if (!_WayPointReverse)
				{
					if (!waypoint->_Next.IsEmpty())
					{
						_NextWayPoint = waypoint->_Next;
					}
					else
					{
						_NextWayPoint = waypoint->_Previous;
						_WayPointReverse = true;
					}
				}
				else
				{
					if (!waypoint->_Previous.IsEmpty())
					{
						_NextWayPoint = waypoint->_Previous;
					}
					else
					{
						_NextWayPoint = waypoint->_Next;
						_WayPointReverse = false;
					}
				}
			}
		}
	}
}
