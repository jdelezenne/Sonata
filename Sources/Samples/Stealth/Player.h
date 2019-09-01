/*=============================================================================
Player.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_PLAYER_H_
#define _SE_PLAYER_H_

#include "Common.h"
#include "GameController.h"
#include "World.h"

class Player : public GameController
{
	SE_DECLARE_CLASS(Player, GameController);

	SE_BEGIN_REFLECTION(Player);
	SE_END_REFLECTION(Player);

public:
	Player();
	virtual ~Player();

	virtual void Update(real64 elapsed);
	virtual void Draw();

	void Respawn();

public:
};

#endif 
