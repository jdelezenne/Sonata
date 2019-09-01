/*=============================================================================
SampleLighting.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleLighting.h"

Application* theApp = Application::Instance();

ManagerPlugin* renderSystenPlugin;
ManagerPlugin* shaderSystenPlugin;

class GameCore : public Singleton<GameCore>
{
public:
	GameCore() :
		_Scene(NULL)
	{
	}

	bool Create()
	{
		LogHandler* consoleHandler = new TextStreamLogHandler(Console::Out());
		((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_None));

		Logger* logger = new Logger();
		logger->GetHandlers().Add(consoleHandler);
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

		_MainWindow = new Window();
		_MainWindow->Create(false, "Lighting Sample", 800, 600);

		theApp->AddWindow(_MainWindow);
		theApp->SetCurrentWindow(_MainWindow);

		_MainWindow->Sized.Add(new DefaultEventDelegateM<GameCore>(GameCore::Instance(), &GameCore::OnSize));

		RenderSystem::SetCurrent((RenderSystem*)renderSystenPlugin->CreateManager());

		RenderContextDescription desc;
		desc.Mode.SetWidth(800);
		desc.Mode.SetHeight(600);
		if (!RenderSystem::Current()->CreateRenderContext(_MainWindow, desc))
			return false;

		ShaderSystem::SetCurrent((ShaderSystem*)shaderSystenPlugin->CreateManager());

		InputMouse* mouse = (InputMouse*)InputSystem::Current()->GetInputDeviceByType(InputDeviceType_Mouse);
		InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->GetInputDeviceByType(InputDeviceType_Keyboard);

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

	void Destroy()
	{
		SE_DELETE(_Scene);
		SE_DELETE(_camera);
		SE_DELETE(_MainWindow);
	}

	bool CreateRenderSystem()
	{
		ManagerPlugin* gl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
		if (gl == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL plugin."));
			return false;
		}
/*
		ManagerPlugin* d3d8 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D8);
		if (d3d8 == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D8 plugin."));
			return false;
		}
*/
		ManagerPlugin* d3d9 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D9);
		if (d3d9 == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 plugin."));
			return false;
		}

		renderSystenPlugin = d3d9;

		Console::WriteLine("Select a render system:");
		Console::WriteLine("[0] Direct3D9");
		Console::WriteLine("[1] OpenGL");
		SEchar choice = Console::Read();
		if (choice == _T('0'))
		{
			renderSystenPlugin = d3d9;
		}
		else if (choice == _T('1'))
		{
			renderSystenPlugin = gl;
		}

		return true;
	}

	bool CreateShaderSystem()
	{
		ManagerPlugin* glsl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLSL);
		if (glsl == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL GLSL plugin."));
			return false;
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
			return false;
		}

		ManagerPlugin* glcg = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLCG);
		if (glcg == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL Cg plugin."));
			return false;
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

		shaderSystenPlugin = d3d9cg;

		Console::WriteLine("Select a shader system:");
		Console::WriteLine("[0] Direct3D9 Cg");
		Console::WriteLine("[1] OpenGL Cg");
		SEchar choice = Console::Read();
		if (choice == _T('0'))
		{
			shaderSystenPlugin = d3d9cg;
		}
		else if (choice == _T('1'))
		{
			shaderSystenPlugin = glcg;
		}

		return true;
	}

	bool CreateInputSystem()
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

		return true;
	}

	bool OnInit(Object* sender, const EventArgs& e)
	{
		RenderSystem::Current()->SetClearColor(Color32::Navy);

		_camera = new Camera();
		_camera->SetPerspective(45.0f,
			(real32)_MainWindow->GetClientWidth()/_MainWindow->GetClientHeight(),
			1.0f, 5000.0f);
		_camera->SetLocalPosition(Vector3(0, 0, 10));
		RenderSystem::Current()->SetCamera(_camera);

		_Scene = new Scene();
		_Scene->SetAmbientColor(Color32::Black);
		_Scene->SetRoot(new SceneNode());

		_GridShape.SetGridPlanes(GridPlane_ZX);
		_GridShape.SetColor(Color32::Yellow);
		_GridShape.Build();

		AxisShape* axisShape = new AxisShape();
		axisShape->SetAxisSize(10.0f);
		axisShape->Build();

		return true;
	}

	static bool OnExit(Object* sender, const EventArgs& e)
	{
		UI::UISystem::DestroyInstance();

		InputSystem::DestroyCurrent();
		RenderSystem::DestroyCurrent();

		return true;
	}

	bool OnSize(Object* sender, const EventArgs& e)
	{
		if (_camera != NULL)
			_camera->SetAspectRatio((real32)_MainWindow->GetClientWidth()/_MainWindow->GetClientHeight());

		return true;
	}

	bool OnUpdate(Object* sender, const UpdateEventArgs& e)
	{
		real64 elapsed = e.ElapsedFrameTime;

		InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
			GetInputDeviceByType(InputDeviceType_Keyboard);
		if (keyboard->IsKeyDown(Key_Escape))
		{
			Application::Instance()->Exit();
			return true;
		}

		if (keyboard->IsKeyDown(Key_Escape))
			Application::Instance()->Exit();

		return true;
	}

	bool OnBeginFrame(Object* sender, const UpdateEventArgs& e)
	{
		static Pen pen = Pen();
		pen.Width = 5;

		_Scene->Render();

		_GridShape.Render();
		_AxisViewport.Render();

		return true;
	}

protected:
	Window* _MainWindow;
	Camera* _camera;
	Scene* _Scene;
	AxisViewport _AxisViewport;
	GridShape _GridShape;
};

void EntryPoint()
{
	Engine::Instance();

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

	Console::WriteLine("Lighting Sample");
	Console::WriteLine("==============");

	// Register the events
	theApp->OnInit.Add(new DefaultEventDelegateM<GameCore>(GameCore::Instance(), &GameCore::OnInit));
	theApp->OnExit += new DefaultEventDelegateF(&GameCore::OnExit);
	theApp->OnUpdate.Add(new UpdateEventDelegateM<GameCore>(GameCore::Instance(), &GameCore::OnUpdate));
	theApp->OnBeginFrame.Add(new UpdateEventDelegateM<GameCore>(GameCore::Instance(), &GameCore::OnBeginFrame));

	try
	{
		if (GameCore::Instance()->Create())
		{
			// Run the application
			if (theApp->Create())
				theApp->Run();

			GameCore::Instance()->Destroy();
		}
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}
}
