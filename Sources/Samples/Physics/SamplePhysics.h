/*=============================================================================
SamplePhysics.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLEPHYSICS_H_
#define _SAMPLEPHYSICS_H_

#include "Common.h"
#include "Simulation.h"
#include "PhysicsUI.h"

class GameCore : public Singleton<GameCore>
{
public:
	GameCore();

	Window* GetMainWindow() const { return _MainWindow; }
	void SetMainWindow(Window* value) { _MainWindow = value; }

	PhysicsWorld* GetWorld() const { return _World; }
	void SetWorld(PhysicsWorld* value) { _World = value; }

	bool Create();
	void Destroy();
	void Exit();

	bool OnInit(Object* sender, const EventArgs& e);
	static bool OnExit(Object* sender, const EventArgs& e);
	bool OnSize(Object* sender, const EventArgs& e);
	bool OnUpdate(Object* sender, const UpdateEventArgs* e);
	bool OnBeginFrame(Object* sender, const EventArgs& e);

	void NewWorld();
	void LoadWorld(const String& fileName);
	void SaveWorld(const String& fileName);

	void AddEntity(PhysicsEntity* entity);
	void RemoveEntity(PhysicsEntity* entity);

protected:
	bool CreateRenderSystem();
	bool CreateShaderSystem();
	bool CreateInputSystem();
	bool CreatePhysicsSystem();
	void CreateWorld();

protected:
	bool _Exit;
	Window* _MainWindow;
	PhysicsWorld* _World;
	AxisViewport* _AxisViewport;

	PhysicsUI* _PhysicsUI;
};


#endif 
