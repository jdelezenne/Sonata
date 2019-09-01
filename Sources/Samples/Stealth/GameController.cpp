/*=============================================================================
GameController.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "GameController.h"

SE_IMPLEMENT_ABSTRACT(GameController);
SE_IMPLEMENT_REFLECTION(GameController);

GameController::GameController() :
	RefObject()
{
	_Character = NULL;
}

GameController::~GameController()
{
}

void GameController::Update(real64 elapsed)
{
}

void GameController::Draw()
{
}
