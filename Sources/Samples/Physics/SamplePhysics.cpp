/*=============================================================================
SamplePhysics.cpp
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

//#	include <PhysicsSystem_NX/NXPhysicsSystemLib.h>
//#	include <PhysicsSystem_ODE/ODEPhysicsSystemLib.h>
//#	include <PhysicsSystem_RW/RWPhysicsSystemLib.h>
#	include <PhysicsSystem_Software/SoftwarePhysicsSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#	include <Model_OBJ/OBJModelPlugin.h>
#else
#	include <Plugins.h>
#endif

#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

#include "SamplePhysics.h"
#include "Simulation.h"
#include "Utils.h"

Application* theApp = Application::Instance();

GameCore::GameCore() :
	_World(NULL)
{
	_Exit = false;
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

	if (!CreatePhysicsSystem())
		return false;

	return true;
}

void GameCore::Destroy()
{
}

bool GameCore::CreateRenderSystem()
{
	_MainWindow = new Window();
	_MainWindow->Create(NULL, "Physics Sample", 800, 600, false);

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

bool GameCore::CreatePhysicsSystem()
{
	ManagerPlugin* nx = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_PHYSICSSYSTEM_NX);
	if (nx == NULL)
	{
		Logger::Current()->Log(_T("Failed to get the Ageia PhysX plugin."));
		//return false;
	}

	ManagerPlugin* ode = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_PHYSICSSYSTEM_ODE);
	if (ode == NULL)
	{
		Logger::Current()->Log(_T("Failed to get the ODE plugin."));
		//return false;
	}

	ManagerPlugin* rw = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_PHYSICSSYSTEM_RW);
	if (rw == NULL)
	{
		Logger::Current()->Log(_T("Failed to get the RenderWare Physics plugin."));
		//return false;
	}

	ManagerPlugin* software = (ManagerPlugin*)PluginManager::Instance()->
		GetPluginByID(SE_ID_PHYSICSSYSTEM_SOFTWARE);
	if (software == NULL)
	{
		Logger::Current()->Log(_T("Failed to get the Software Physics plugin."));
		return false;
	}

	PhysicsSystem* physicsSystem = (PhysicsSystem*)software->CreateManager();
	PhysicsSystem::SetCurrent(physicsSystem);

	PhysicsSystemDescription desc;
	physicsSystem->Create(desc);

	return true;
}

void GameCore::CreateWorld()
{
	NewWorld();

#if 0
	int i, j;
	PhysicsEntity* entity;

#define CREATE_SCENE 2

#if CREATE_SCENE == 1
	entity = _World->CreateEntity();
	entity->CreateSphere(Sphere(Vector3::Zero,
		Math::Random(0.5f, 2.0f)), Math::Random(5.0f, 15.0f));
	entity->SetPosition(Vector3(-2.0, 4.0, 0.0));
	entity->SetName("sphere0");
	entity->SetColor(GetRandomColor());
	AddEntity(entity);

	entity = _World->CreateEntity();
	entity->CreateSphere(Sphere(Vector3::Zero,
		Math::Random(0.5f, 2.0f)), Math::Random(5.0f, 15.0f));
	entity->SetPosition(Vector3(2.0, 4.0, 0.0));
	entity->SetName("sphere1");
	entity->SetColor(GetRandomColor());
	AddEntity(entity);
#elif CREATE_SCENE == 2
	for (j=0; j<4; j++)
	{
		for (i=0; i<5; i++)
		{
			entity = _World->CreateEntity();
			entity->CreateBox(Box(Vector3::Zero, Vector3(1.0, 1.0, 1.0), Matrix3::Identity), 5);
			entity->SetName("box" + String::ToString(i+j));
			entity->SetPosition(Vector3(j % 2 ? -2.0 : 2.0, 4.0+i*5, j % 3 ? -2.0 : 2.0));
			entity->SetColor(GetRandomColor());
			AddEntity(entity);

			entity = _World->CreateEntity();
			entity->CreateSphere(Sphere(Vector3::Zero, 1.0), 10);
			entity->SetName("sphere" + String::ToString(i+j));
			entity->SetPosition(Vector3(8.0+(j % 2 ? -2.0 : 2.0), 4.0+i*5, j % 3 ? -2.0 : 2.0));
			entity->SetColor(GetRandomColor());
			AddEntity(entity);
		}
	}
#elif CREATE_SCENE == 3
	for (j=0; j<5; j++)
	{
		for (i=j; i<5; i++)
		{
			entity = _World->CreateEntity();
			entity->CreateSphere(Sphere(Vector3::Zero, 1.0), 10);
			entity->SetName("sphere" + String::ToString(i+j));
			entity->SetPosition(Vector3(5+j-i*2.0, 4.0, -20+j*2.0));
			entity->SetColor((i==3&&j==2)?Color32::Black:(i+j)%2 ? Color32::Red : Color32::Yellow);
			AddEntity(entity);
		}
	}

	entity = _World->CreateEntity();
	entity->CreateSphere(Sphere(Vector3::Zero, 1.0), 10);
	entity->SetName("sphere");
	entity->SetPosition(Vector3(5+2-3*2.0, 4.0, 5+2*2.0));
	entity->SetColor(Color32::White);
	AddEntity(entity);
#endif

#endif
}

void GameCore::NewWorld()
{
	SE_DELETE(_World);
	_PhysicsUI->Initialize();

	_World = new PhysicsWorld();
	_World->Create();

	// Always create a default plane
	PhysicsEntity* entity = _World->CreateEntity();
	entity->CreatePlane(Plane(Vector3::UnitY, 0.0f));
	entity->SetName("plane");
	AddEntity(entity);
}

void GameCore::LoadWorld(const String& fileName)
{
	SE_DELETE(_World);
	_PhysicsUI->Initialize();

	File* file = new File(fileName);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_World = (PhysicsWorld*)serializer->Deserialize(stream.Get());
		stream->Close();
	}

	delete serializer;
	delete file;

	if (_World != NULL)
	{
		int count = _World->GetEntityCount();
		for (int i=0; i<count; i++)
		{
			PhysicsEntity* entity = _World->GetEntity(i);
			_PhysicsUI->AddEntity(entity);
		}
	}
}

void GameCore::SaveWorld(const String& fileName)
{
	if (_World != NULL)
	{
		File* file = new File(fileName);
		XMLSerializer* serializer  = new XMLSerializer();

		FileStreamPtr stream = file->Open(FileMode_Create);
		if (stream != NULL)
		{
			serializer->Serialize(stream.Get(), _World);
			stream->Close();
		}

		delete serializer;
		delete file;
	}
}

void GameCore::AddEntity(PhysicsEntity* entity)
{
	_PhysicsUI->AddEntity(entity);
	_World->AddEntity(entity);
}

void GameCore::RemoveEntity(PhysicsEntity* entity)
{
	_PhysicsUI->RemoveEntity(entity);
	_World->RemoveEntity(entity);
}

bool GameCore::OnInit(Object* sender, const EventArgs& e)
{
	RenderSystem::Current()->SetClearColor(Color32::Navy);

	UI::UISystem::Instance()->Create();
	String fontName = FileSystem::Instance()->GetFullPath("defaultfont.dds");
	UI::UISystem::Instance()->SetDefaultFont(CreateFont(fontName));

	_PhysicsUI = new PhysicsUI();
	_PhysicsUI->Create();

	CreateWorld();

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
	UI::UISystem::Instance()->Destroy();
	InputSystem::Current()->Destroy();
	RenderSystem::Current()->Destroy();

	return true;
}

bool GameCore::OnSize(Object* sender, const EventArgs& e)
{
	//if (_camera != NULL)
	//	_camera->SetAspectRatio((real32)_MainWindow->GetClientWidth()/_MainWindow->GetClientHeight());

	return true;
}

bool GameCore::OnUpdate(Object* sender, const UpdateEventArgs* e)
{
	real64 elapsed = e->ElapsedFrameTime;

	if (_Exit)
	{
		Application::Instance()->Exit();
		return true;
	}

	UI::UISystem::Instance()->Update(elapsed);

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

	RenderSystem* renderer = RenderSystem::Current();
	renderer->SetWorldTransform(Matrix4::Identity);
	_AxisViewport->SetCamera(renderer->GetCamera());
	_AxisViewport->Render();

	UI::UISystem::Instance()->Render();

	return true;
}

void EntryPoint()
{
	Console::WriteLine("Physics Sample");
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
	theApp->OnInit.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnInit));
	theApp->OnExit += new EventFunctorSlot(&GameCore::OnExit);
	theApp->OnUpdate.Add(new EventMethodSlot<GameCore>(GameCore::Instance(),
		(EventMethodSlot<GameCore>::MethodDelegate)&GameCore::OnUpdate));
	theApp->OnBeginFrame.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnBeginFrame));

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
