/*=============================================================================
SampleStealth.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>

#ifdef SE_STATIC
#	include <RenderSystem_GL/GLRenderSystemLib.h>
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

#	include <ShaderSystem_GLCg/GLCgShaderSystemLib.h>
#	include <ShaderSystem_D3D9Cg/D3D9CgShaderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#	include <Model_OBJ/OBJModelPlugin.h>
#else
#	include <Plugins.h>
#endif

#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

#include "SampleStealth.h"
#include "Game.h"
#include "Utils.h"

Application* theApp = Application::Instance();

GameCore::GameCore() :
	_World(NULL)
{
	_Exit = false;
}

real64 GameCore::GetTime()
{
	return Application::Instance()->GetTime() * 0.001;
}

real64 GameCore::GetElapsedTime()
{
	return Application::Instance()->GetElapsedTime();
}

bool GameCore::Create()
{
#ifdef WIN32
	LogHandler* consoleHandler = new Win32ConsoleLogHandler();
	((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions(
		(LogOptions)(LogOptions_None));

	LogHandler* debugHandler = new Win32DebugLogHandler();
	((DefaultLogFormatter*)debugHandler->GetFormatter())->SetOptions(
		(LogOptions)(LogOptions_Source | LogOptions_Time));
#endif

	Logger* logger = new Logger();
	logger->GetHandlers().Add(consoleHandler);
	logger->GetHandlers().Add(debugHandler);
	Logger::SetCurrent(logger);

	FileSystem::Instance()->AddRootPath(_T("Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
#ifndef SE_STATIC
	PluginManager::Instance()->ParsePlugins(Environment::GetCurrentDirectory());
#endif
	PluginManager::Instance()->CreateAllPlugins();

	if (!CreateRenderSystem())
		return false;

	if (!CreateShaderSystem())
		return false;

	if (!CreateInputSystem())
		return false;

	return true;
}

void GameCore::Destroy()
{
}

bool GameCore::CreateRenderSystem()
{
	_MainWindow = new Window();
	_MainWindow->Create(NULL, "Stealth Sample", 800, 600, false);

	theApp->AddWindow(_MainWindow);
	theApp->SetCurrentWindow(_MainWindow);

	_MainWindow->Sized.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnSize));

	ManagerPlugin* gl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
	if (gl == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL plugin."));
		return false;
	}

	ManagerPlugin* d3d9 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D9);
	if (d3d9 == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 plugin."));
		return false;
	}

	RenderSystem::SetCurrent((RenderSystem*)d3d9->CreateManager());

	RenderContextDescription desc;
	desc.Mode.SetWidth(800);
	desc.Mode.SetHeight(600);
	if (!RenderSystem::Current()->CreateRenderContext(_MainWindow, desc))
		return false;

	return true;
}

bool GameCore::CreateShaderSystem()
{
	ManagerPlugin* glsl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLSL);
	if (glsl == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL GLSL plugin."));
		//return false;
	}
/*
	ManagerPlugin* d3d8 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D8);
	if (d3d8 == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D8 shader plugin."));
		return false;
	}
*/
	ManagerPlugin* d3d9hlsl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_HLSL9);
	if (d3d9hlsl == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 HLSL plugin."));
		//return false;
	}

	ManagerPlugin* glcg = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLCG);
	if (glcg == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL Cg plugin."));
		//return false;
	}
/*
	ManagerPlugin* d3d8cg = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D8CG);
	if (d3d8cg == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D8 Cg plugin."));
		return false;
	}
*/
	ManagerPlugin* d3d9cg = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D9CG);
	if (d3d9cg == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 Cg plugin."));
		return false;
	}

	ShaderSystem::SetCurrent((ShaderSystem*)d3d9cg->CreateManager());

	return true;
}

bool GameCore::CreateInputSystem()
{
	ManagerPlugin* di8 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_INPUTSYSTEM_DI8);
	if (di8 == NULL)
	{
		Logger::Current()->Log(_T("Failed to get the DirectInput8 plugin."));
		return false;
	}
	InputSystem* inputSystem = (InputSystem*)di8->CreateManager();
	InputSystem::SetCurrent(inputSystem);

	inputSystem->Create();

	InputMouse* mouse = (InputMouse*)inputSystem->GetInputDeviceByType(InputDeviceType_Mouse);
	InputKeyboard* keyboard = (InputKeyboard*)inputSystem->GetInputDeviceByType(InputDeviceType_Keyboard);

	if (mouse == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the mouse device."));
		return false;
	}

	if (keyboard == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the keyboard device."));
		return false;
	}

	mouse->Create(_MainWindow);
	keyboard->Create(_MainWindow);

	return true;
}

void GameCore::CreateWorld()
{
	_World = new GameWorld();

	_World->Create();

	String tileset = FileSystem::Instance()->GetFullPath("AI\\MGS\\MGSTileset.Xml", true);
	_World->LoadTileset(tileset);
}

bool GameCore::OnInit(Object* sender, const EventArgs& e)
{
	RenderSystem::Current()->SetClearColor(Color32::Navy);

	UI::UISystem::Instance()->Create();
	String fontName = FileSystem::Instance()->GetFullPath("defaultfont.dds");
	UI::UISystem::Instance()->SetDefaultFont(CreateFont(fontName));

	CreateWorld();

	_GameUI = new GameUI();
	_GameUI->Create();

	_World->NewMap(10, 10, 4);

	_World->SetEditor(true);

	AxisShape* axisShape = new AxisShape();
	axisShape->SetAxisSize(1.0f);
	axisShape->Build();

	_AxisViewport = new AxisViewport();
	_AxisViewport->SetShape(axisShape);

	return true;
}

void GameCore::Exit()
{
	_Exit = true;
}

bool GameCore::OnExit(Object* sender, const EventArgs& e)
{
	UI::UISystem::DestroyInstance();
	InputSystem::DestroyCurrent();
	RenderSystem::DestroyCurrent();

	return true;
}

bool GameCore::OnSize(Object* sender, const EventArgs& e)
{
	return true;
}

bool GameCore::OnUpdate(Object* sender, const UpdateEventArgs& e)
{
	real64 elapsed = e.ElapsedFrameTime;

	if (_Exit)
	{
		Application::Instance()->Exit();
		return true;
	}

	UI::UISystem::Instance()->Update(elapsed);

	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);

	PointInt ptMouse;
	mouse->GetMousePosition(ptMouse.X, ptMouse.Y);
	if (UI::UISystem::Instance()->GetFocusedWidget() &&
		!UI::UISystem::Instance()->GetFocusedWidget()->Contains(ptMouse))
	{
		UI::UISystem::Instance()->SetFocusedWidget(NULL);
	}

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	if (keyboard->IsKeyDown(Key_Escape))
	{
		Exit();
		return true;
	}

	if (_World != NULL)
		_World->Update(elapsed);

	return true;
}

bool GameCore::OnBeginFrame(Object* sender, const EventArgs& e)
{
	if (_World != NULL)
		_World->Render();

	return true;
}

bool GameCore::OnEndFrame(Object* sender, const EventArgs& e)
{
	RenderSystem* renderer = RenderSystem::Current();
	Camera* camera = SceneManager::Instance()->GetCamera();
	renderer->SetWorldTransform(Matrix4::Identity);
	_AxisViewport->SetCamera(renderer->GetCamera());
	_AxisViewport->Render();

	Viewport viewport = renderer->GetViewport();
	renderer->GetCamera()->SetOrthographic(0, 0,
		viewport.GetWidth(), viewport.GetHeight(),
		-1.0f, 1.0f);
	renderer->UpdateProjectionTransform();
	renderer->SetWorldTransform(Matrix4::Identity);
	renderer->SetCullMode(CullMode_None);
	renderer->SetLighting(false);
	DepthState depthState;
	depthState.Enable = false;
	renderer->SetDepthState(depthState);

	static real64 t = GameCore::GetTime();
	static int32 frames = 0;
	frames++;

	int32 fps = (real32)frames / (GameCore::GetTime() - t);
	//Text::DrawText(String::ToString(fps) + _T(" FPS"),
	//	UI::UISystem::Instance()->GetDefaultFont(), PointReal(5.0f, 5.0f), Color32::Red);
	UI::Label* labelFPS = (UI::Label*)UI::UISystem::Instance()->GetWidgetByName("labelFPS");
	if (labelFPS != NULL)
	{
		labelFPS->SetText(String::ToString(fps) + _T(" FPS"));
	}

	UI::UISystem::Instance()->Render();

	return true;
}

void EntryPoint()
{
	Console::WriteLine("Stealth Sample");
	Console::WriteLine("==============");

#ifdef SE_STATIC
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Debug"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Release"));
#	endif
#else
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\DebugDLL"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\ReleaseDLL"));
#	endif
#endif

	// Register the events
	theApp->OnInit += new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnInit);
	theApp->OnExit += new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnExit);
	theApp->OnUpdate += new EventMethodSlot<GameCore>(GameCore::Instance(),
		(EventMethodSlot<GameCore>::MethodDelegate)&GameCore::OnUpdate);
	theApp->OnBeginFrame += new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnBeginFrame);
	theApp->OnEndFrame += new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnEndFrame);

	try
	{
		if (GameCore::Instance()->Create())
		{
			// Run the application
			if (theApp->Create())
				theApp->Run();

			GameCore::DestroyInstance();
		}
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}
}
