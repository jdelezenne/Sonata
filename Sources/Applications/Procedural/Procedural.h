/*=============================================================================
Procedural.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_PROCEDURAL_H_
#define _PROCEDURAL_PROCEDURAL_H_

#include "Common.h"
#include "MainUI.h"
#include "Workflow.h"

struct AppSettings
{
	AppSettings() :
		_ImageSize(SizeInt(512, 512)),
		_PreviewSize(SizeInt(64, 64))
	{
	}

	SizeInt _ImageSize;
	SizeInt _PreviewSize;
};

class AppCore : public Singleton<AppCore>
{
public:
	AppCore();

	bool Create();
	void Destroy();

	bool OnCreate();
	bool OnReset();

	bool OnInit(Object* sender, const EventArgs& e);
	bool OnExit(Object* sender, const EventArgs& e);
	bool OnSize(Object* sender, const EventArgs& e);
	bool OnUpdate(Object* sender, const EventArgs& e);
	bool OnBeginFrame(Object* sender, const EventArgs& e);

	Window* GetMainWindow() const { return _MainWindow; }
	MainUI* GetMainUI() const { return _MainUI; }
	OperatorLibrary* GetOperatorLibrary() const { return _OperatorLibrary; }
	Workflow* GetWorkflow() const { return _Workflow; }

	AppSettings* Settings() const { return _Settings; }

	void ExitApplication();
	void OpenOperatorLibrary(const String& fileName);
	void CreateDocument();
	void OpenDocument(const String& fileName);
	void SaveDocument(const String& fileName);
	void ExportOperator(const String& fileName);

protected:
	bool CreateRenderSystem();
	bool CreateInputSystem();

protected:
	bool _Exit;
	Window* _MainWindow;
	Camera* _camera;
	MainUI* _MainUI;
	OperatorLibrary* _OperatorLibrary;
	Workflow* _Workflow;
	AppSettings* _Settings;
};

#endif
