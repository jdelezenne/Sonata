/*=============================================================================
SampleAI.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLEAI_H_
#define _SAMPLEAI_H_

#include "Common.h"
#include "Game.h"
#include "GameUI.h"

class GameCore : public Singleton<GameCore>
{
public:
	GameCore();

	Window* GetMainWindow() const { return _MainWindow; }
	void SetMainWindow(Window* value) { _MainWindow = value; }

	GameWorld* GetWorld() const { return _World; }
	void SetWorld(GameWorld* value) { _World = value; }

	GameUI* GetGameUI() const { return _GameUI; }
	void SetGameUI(GameUI* value) { _GameUI = value; }

	bool Create();
	void Destroy();
	void Exit();

	bool OnInit(Object* sender, const EventArgs& e);
	static bool OnExit(Object* sender, const EventArgs& e);
	bool OnSize(Object* sender, const EventArgs& e);
	bool OnUpdate(Object* sender, const UpdateEventArgs* e);
	bool OnBeginFrame(Object* sender, const EventArgs& e);

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

protected:
	bool CreateRenderSystem();
	bool CreateShaderSystem();
	bool CreateInputSystem();
	void CreateWorld();

protected:
	bool _Exit;
	Window* _MainWindow;
	GameWorld* _World;
	AxisViewport* _AxisViewport;

	GameUI* _GameUI;
};


#endif 
