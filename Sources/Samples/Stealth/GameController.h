/*=============================================================================
GameController.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_GAMECONTROLLER_H_
#define _SE_GAMECONTROLLER_H_

#include "Common.h"
#include "Character.h"

class GameController : public RefObject
{
	SE_DECLARE_CLASS(GameController, Object);

	SE_BEGIN_REFLECTION(GameController);
	SE_END_REFLECTION(GameController);

public:
	GameController();
	virtual ~GameController();

	CharacterActor* GetCharacter() const { return _Character; }
	void SetCharacter(CharacterActor* value) { _Character = value; }

	virtual void Update(real64 elapsed);
	virtual void Draw();

protected:
	CharacterActor* _Character;
};

#endif 
