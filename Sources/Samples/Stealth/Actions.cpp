/*=============================================================================
Actions.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Actions.h"
#include "SampleStealth.h"
#include "ActorPathManager.h"

SE_IMPLEMENT_ABSTRACT(CharacterActorAction);
SE_IMPLEMENT_REFLECTION(CharacterActorAction);

CharacterActorAction::CharacterActorAction() :
	super()
{
	_Character = NULL;
}

CharacterActorAction::~CharacterActorAction()
{
}

void CharacterActorAction::Initialize(CharacterActor* character)
{
	_Character = character;
}


SE_IMPLEMENT_CLASS(CharacterActorActionMove);
SE_IMPLEMENT_REFLECTION(CharacterActorActionMove);

CharacterActorActionMove::CharacterActorActionMove() :
	super()
{
	_ActionStatus = ActionStatus_Inactive;
	_Destination = Vector3::Zero;
}

CharacterActorActionMove::~CharacterActorActionMove()
{
}

void CharacterActorActionMove::SetDestination(const Vector3& value)
{
	_Destination = value;
	_Path.ClearPath();

	Map* map = GameCore::Instance()->GetWorld()->GetMap();
	ActorPathManager mgr;
	mgr.Initialize(PathfinderType_AStar);
	Vector2 startPos(_Character->_Position.X, _Character->_Position.Y);
	Vector2 targetPos(_Destination.X, _Destination.Y);
	if (mgr.FindPath(_Character, map->GetMapNodeAt(startPos),
		map->GetMapNodeAt(targetPos), &_Path))
	{
		_ActionStatus = ActionStatus_Active;
	}
}

void CharacterActorActionMove::Initialize(CharacterActor* character)
{
	super::Initialize(character);
}

void CharacterActorActionMove::Update(real64 elapsed)
{
	if (_ActionStatus != ActionStatus_Active)
		return;

	Map* map = GameCore::Instance()->GetWorld()->GetMap();
	if (_Path.IsPathComplete())
	{
		_ActionStatus = ActionStatus_Done;
	}
	else
	{
		PathNode* node = _Path.GetCurrentPathNode();

		Vector3 dest = Vector3(node->_Position.X, node->_Position.Y, _Character->_Position.Z);

		Vector3 move = dest - _Character->_Position;

		real remainingDist = move.Length();
		real moveDist = _Character->_currentSpeed * elapsed;

		if (remainingDist < moveDist)
		{
			moveDist = remainingDist;
			// node reached
			_Path.AdvancePathNode();
		}

		// Scale based on our movement distance
		move.Normalize();
		move *= moveDist;

		// Calculate our new position

		Vector3 newPos = _Character->_Position + move;

		// Move the character
		_Character->Move(newPos);

		// Face the character
		_Character->LookAt(newPos);
	}
}
