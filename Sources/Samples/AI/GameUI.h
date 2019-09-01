/*=============================================================================
GameUI.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_GameUI_H_
#define _SE_GameUI_H_

#include "Common.h"
#include "Game.h"

class GameUI
{
public:
	GameUI();
	virtual ~GameUI();

	void Create();
	void Destroy();

	void SetEditor(bool value);

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

protected:
	void CreateUI();

	virtual bool OnToolBarClick(Object* sender, const EventArgs& e);
	virtual bool OnToolBarEditorClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxEntitiesSelectedItemChanged(Object* sender, const EventArgs& e);
	virtual bool OnTileMouseClick(Object* sender, const EventArgs& e);
	virtual bool OnUnitMouseClick(Object* sender, const EventArgs& e);

protected:
	UI::Widget* _Screen;

	UI::PushButton* _ButtonNew;
	UI::PushButton* _ButtonLoad;
	UI::PushButton* _ButtonSave;

	UI::PushButton* _ButtonPause;
	UI::PushButton* _ButtonSettings;
	UI::PushButton* _ButtonEntities;
	UI::PushButton* _ButtonProperties;
	UI::PushButton* _ButtonExit;

	UI::ListBox* _ListBoxEntities;

	UI::PropertyGrid* _PropertyGrid;
	UI::PropertyGridBinder* _PropertyGridBinder;
};

#endif 
