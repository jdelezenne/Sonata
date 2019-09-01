/*=============================================================================
PhysicsUI.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICSUI_H_
#define _SE_PHYSICSUI_H_

#include "Common.h"
#include "Simulation.h"

class PhysicsUI
{
public:
	PhysicsUI();
	virtual ~PhysicsUI();

	void Create();
	void Destroy();

	void RefreshScenes();
	void Initialize();
	void AddEntity(PhysicsEntity* entity);
	void RemoveEntity(PhysicsEntity* entity);

protected:
	void CreateUI();

	virtual bool OnToolBarClick(Object* sender, const EventArgs& e);
	virtual bool OnToolBarSceneClick(Object* sender, const EventArgs& e);
	virtual bool OnListBoxBodiesSelectedItemChanged(Object* sender, const EventArgs& e);
	virtual bool OnButtonAddBodyClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonRemoveBodyClick(Object* sender, const EventArgs& e);

protected:
	UI::Widget* _Screen;

	UI::PushButton* _ButtonPause;
	UI::PushButton* _ButtonSettings;
	UI::PushButton* _ButtonBodies;
	UI::PushButton* _ButtonProperties;
	UI::PushButton* _ButtonExit;

	UI::ListBox* _ListBoxBodies;
	UI::PushButton* _ButtonAddBody;
	UI::PushButton* _ButtonRemoveBody;

	UI::PropertyGrid* _PropertyGrid;
	UI::PropertyGridBinder* _PropertyGridBinder;

	UI::ComboBox* _ComboBoxScenes;
	UI::PushButton* _ButtonNew;
	UI::PushButton* _ButtonLoad;
	UI::PushButton* _ButtonSave;
};

#endif 
