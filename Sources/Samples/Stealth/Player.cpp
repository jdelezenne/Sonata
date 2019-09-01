/*=============================================================================
Player.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Player.h"
#include "SampleStealth.h"

SE_IMPLEMENT_CLASS(Player);
SE_IMPLEMENT_REFLECTION(Player);

Player::Player() :
	super()
{
}

Player::~Player()
{
	if (_Character != NULL)
	{
		GameCore::Instance()->GetWorld()->GetMap()->_Actors.Remove(_Character);
		SE_DELETE(_Character);
	}
}

void Player::Update(real64 elapsed)
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	if (_Character == NULL)
		return;

	if (keyboard->IsKeyDown(Key_LeftShift) || keyboard->IsKeyDown(Key_RightShift))
		_Character->_currentSpeed = _Character->_RunSpeed;
	else
		_Character->_currentSpeed = _Character->_WalkSpeed;

	real32 move = _Character->_currentSpeed * elapsed;
	if (keyboard->IsKeyDown(Key_W))
	{
		Vector3 pos = _Character->_Position;
		pos += _Character->_Orientation.Rotate(Vector3::UnitY * move);
		GameCore::Instance()->GetWorld()->GetMap()->SetActorPosition(_Character, Vector2(pos.X, pos.Y));
	}

	if (keyboard->IsKeyDown(Key_S))
	{
		Vector3 pos = _Character->_Position;
		pos += _Character->_Orientation.Rotate(-Vector3::UnitY * move);
		GameCore::Instance()->GetWorld()->GetMap()->SetActorPosition(_Character, Vector2(pos.X, pos.Y));
	}

	if (keyboard->IsKeyDown(Key_A))
	{
		_Character->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, Math::Pi * 2.0 * elapsed);
	}

	if (keyboard->IsKeyDown(Key_D))
	{
		_Character->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi * 2.0 * elapsed);
	}

	Map* map = GameCore::Instance()->GetWorld()->GetMap();
	Actor* actor = map->GetActorOfType(ActorType_Goal);
	if (actor != NULL)
	{
		Matrix4 m0 = Matrix4(Matrix3::CreateFromQuaternion(_Character->_Orientation), _Character->_Position);
		AABB aabb0 = _Character->_ActorDef->_Model->GetBoundingBox();
		aabb0.Transform(m0);

		Matrix4 m1 = Matrix4(Matrix3::CreateFromQuaternion(actor->_Orientation), actor->_Position);
		AABB aabb1 = actor->_ActorDef->_Model->GetBoundingBox();
		aabb1.Transform(m1);
		if (aabb0.Intersects(aabb1))
		{
			Logger::Current()->Log(LogLevel::Information, _T("Player.Update"),
				_T("The player reached the goal <") + actor->_name + _T(">"));

			Respawn();
		}
	}
}

void Player::Draw()
{
}

void Player::Respawn()
{
	if (_Character == NULL)
		return;

	Map* map = GameCore::Instance()->GetWorld()->GetMap();
	Actor* actor = map->GetActorOfType(ActorType_Spawn);
	if (actor != NULL)
	{
		map->SetActorPosition(_Character, Vector2(actor->_Position.X, actor->_Position.Y));
	}
}