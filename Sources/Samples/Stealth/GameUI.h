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
#include "World.h"

class GameUI
{
public:
	GameUI();
	virtual ~GameUI();

	void Create();
	void Destroy();

	UI::Widget* GetChildByName(const String& name) const;

	void Initialize(Map* map);
	void SetEditor(bool value);
	void RefreshLevels();
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	void SetSelectedObject(Object* obj);
	Tile* GetSelectedTile() const;
	ActorDef* GetSelectedActorDef() const;

protected:
	void CreateUI();

	virtual bool OnToolBarClick(Object* sender, const EventArgs& e);
	virtual bool OnTabPanelEditorPageChanged(Object* sender, const EventArgs& e);
	virtual bool OnTabPanelPropertiesPageChanged(Object* sender, const EventArgs& e);
	virtual bool OnToolBarEditorClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxEntitiesMouseClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxEntitiesMouseDoubleClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxTilesMouseDoubleClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxActorsMouseDoubleClick(Object* sender, const EventArgs& e);

protected:
	UI::Widget* _Screen;

	UI::ComboBox* _ComboBoxLevels;
	UI::ListBox* _ListBoxEntities;

	UI::PropertyGrid* _PropertyGrid;
	UI::Panel* _PropertyDialog;
	UI::PropertyGridBinder* _PropertyGridBinder;

	UI::ListBox* _ListBoxTiles;
	UI::ListBox* _ListBoxActors;
};

#endif 
