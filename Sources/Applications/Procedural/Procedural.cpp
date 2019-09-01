/*=============================================================================
Procedural.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>

#ifdef SE_STATIC
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#endif

/*#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif*/

#include "Procedural.h"
#include "Utils.h"
#include "Operators/Operators.h"

Application* theApp = Application::Instance();

//RenderSystem* GL_RenderSystem = NULL;
RenderSystem* D3D9_RenderSystem = NULL;

AppCore::AppCore() :
	_MainWindow(NULL),
	_camera(NULL),
	_MainUI(NULL),
	_OperatorLibrary(NULL),
	_Workflow(NULL)
{
	_Exit = false;

	_Settings = new AppSettings();
}

bool AppCore::Create()
{
/*#ifdef WIN32
	LogHandler* consoleHandler = new Win32ConsoleLogHandler();
	((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions(
		(LogOptions)(LogOptions_None));

	LogHandler* debugHandler = new Win32DebugLogHandler();
	((DefaultLogFormatter*)debugHandler->GetFormatter())->SetOptions(
		(LogOptions)(LogOptions_Source | LogOptions_Time));
#endif*/

	Logger* logger = new Logger();
	//logger->GetHandlers().Add(consoleHandler);
	//logger->GetHandlers().Add(debugHandler);
	Logger::SetCurrent(logger);

	FileSystem::Instance()->AddRootPath(_T("Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
	PluginManager::Instance()->ParsePlugins(Environment::GetCurrentDirectory());
	PluginManager::Instance()->CreateAllPlugins();

	String path = FileSystem::Instance()->GetFullPath(_T("Procedural\\Operators.xml"));
	Environment::SetCurrentDirectory(Path::Combine(Path::GetDirectoryName(path), _T("Samples")));

	if (!CreateRenderSystem())
		return false;

	if (!CreateInputSystem())
		return false;

	return true;
}

void AppCore::Destroy()
{
	SE_DELETE(_Workflow);

	SE_DELETE(_MainUI);
	SE_DELETE(_camera);
	SE_DELETE(_MainWindow);

	SE_DELETE(_Settings);
}

bool AppCore::CreateRenderSystem()
{
	_MainWindow = new Window();
	_MainWindow->Create(NULL, "Procedural Designer", 800, 600, false);

	theApp->AddWindow(_MainWindow);
	theApp->SetCurrentWindow(_MainWindow);

	_MainWindow->Sized.Add(new EventMethodSlot<AppCore>(AppCore::Instance(), &AppCore::OnSize));

	/*ManagerPlugin* gl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
	if (gl == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL plugin."));
		return false;
	}*/

	ManagerPlugin* d3d9 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D9);
	if (d3d9 == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 plugin."));
		return false;
	}

	//GL_RenderSystem = (RenderSystem*)gl->CreateManager();
	D3D9_RenderSystem = (RenderSystem*)d3d9->CreateManager();
	RenderSystem::SetCurrent(D3D9_RenderSystem);

	OnCreate();

	return true;
}

bool AppCore::CreateInputSystem()
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

bool AppCore::OnCreate()
{
	RenderContextDescription desc;
	desc.Mode.SetWidth(800);
	desc.Mode.SetHeight(600);
	if (!RenderSystem::Current()->CreateRenderContext(_MainWindow, desc))
		return false;

	return true;
}

bool AppCore::OnReset()
{
	SE_DELETE(_camera);
	_camera = new Camera();
	_camera->SetPerspective(45.0f,
		(real32)_MainWindow->GetClientWidth()/_MainWindow->GetClientHeight(),
		1.0f, 5000.0f);

	RenderSystem::Current()->SetClearColor(Color32::Gray);
	RenderSystem::Current()->SetCamera(_camera);
	RenderSystem::Current()->SetDefaultLight(false);

	UI::UISystem::Instance()->Create();
	String fontName = FileSystem::Instance()->GetFullPath("defaultfont.dds");
	UI::UISystem::Instance()->SetDefaultFont(CreateFont(fontName));

	return true;
}

bool AppCore::OnInit(Object* sender, const EventArgs& e)
{
	OnReset();

	_MainUI = new MainUI();

	OpenOperatorLibrary("Operators.xml");

	_MainUI->Create();

	CreateDocument();

	return true;
}

bool AppCore::OnExit(Object* sender, const EventArgs& e)
{
	UI::UISystem::DestroyInstance();

	InputSystem::DestroyCurrent();
	RenderSystem::DestroyCurrent();

	return true;
}

bool AppCore::OnSize(Object* sender, const EventArgs& e)
{
	return true;
}

bool AppCore::OnUpdate(Object* sender, const EventArgs& e)
{
	real64 elapsed = (*((UpdateEventArgs*) &e)).ElapsedFrameTime;

	if (_Exit)
	{
		Application::Instance()->Exit();
		return true;
	}

	UI::UISystem::Instance()->Update(elapsed);

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	if (keyboard->IsKeyDown(Key_Escape))
		Application::Instance()->Exit();

	if (_Workflow != NULL)
	{
		_Workflow->Update();
	}

	return true;
}

bool AppCore::OnBeginFrame(Object* sender, const EventArgs& e)
{
	UI::UISystem::Instance()->Render();

	return true;
}

void AppCore::ExitApplication()
{
	_Exit = true;
}

void AppCore::OpenOperatorLibrary(const String& fileName)
{
	SE_DELETE(_OperatorLibrary);

	String path = FileSystem::Instance()->GetFullPath(Path::Combine("Procedural", fileName));
	File* file = new File(path);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_OperatorLibrary = (OperatorLibrary*)serializer->Deserialize(stream.Get());
		stream->Close();
	}

	delete serializer;
	delete file;
}

void AppCore::CreateDocument()
{
	SE_DELETE(_Workflow);

	_Workflow = new Workflow();
	_MainUI->Initialize();
}

void AppCore::OpenDocument(const String& fileName)
{
	SE_DELETE(_Workflow);

	File* file = new File(fileName);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_Workflow = (Workflow*)serializer->Deserialize(stream.Get());
		stream->Close();
	}

	delete serializer;
	delete file;

	if (_Workflow != NULL)
	{
		_Workflow->SetFileName(fileName);
		_MainUI->Initialize();
		_Workflow->InitializeOperators();
	}
}

void AppCore::SaveDocument(const String& fileName)
{
	if (_Workflow == NULL)
		return;

	File* file = new File(fileName);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Create, FileAccess_Write);
	if (stream != NULL)
	{
		serializer->Serialize(stream.Get(), _Workflow);
		stream->Close();
	}

	delete serializer;
	delete file;

	_Workflow->SetFileName(fileName);
}

void AppCore::ExportOperator(const String& fileName)
{
	if (_Workflow == NULL)
		return;

	if (_Workflow->GetSelectedOperator() == NULL)
		return;

	const Image* image = NULL;

	Operator* op = _Workflow->GetSelectedOperator();
	if (op->GetType()->IsSubclassOf(typeof(ColorOperatorBase)))
	{
		image = ((Operators::ColorOperatorBase*)op)->GetImage();
	}
	else if (op->GetType()->IsSubclassOf(typeof(FunctionOperatorBase)))
	{
		image = ((Operators::FunctionOperatorBase*)op)->GetImage();
	}

	if (image != NULL)
	{
		Resource* resource = new Resource();
		resource->SetResourceType(SE_ID_DATA_IMAGE);
		resource->SetSize(image->GetDataSize());
		resource->SetData((void*)image);
		ResourceHelper::SaveToFile(resource, fileName);
		delete resource;
	}
}

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

	Console::WriteLine("Procedural Designer");
	Console::WriteLine("===================");

	// Register the events
	theApp->OnInit += new EventMethodSlot<AppCore>(AppCore::Instance(), &AppCore::OnInit);
	theApp->OnExit += new EventMethodSlot<AppCore>(AppCore::Instance(), &AppCore::OnExit);
	theApp->OnUpdate += new EventMethodSlot<AppCore>(AppCore::Instance(),
		(EventMethodSlot<AppCore>::MethodDelegate)&AppCore::OnUpdate);
	theApp->OnBeginFrame += new EventMethodSlot<AppCore>(AppCore::Instance(), &AppCore::OnBeginFrame);

	try
	{
		if (AppCore::Instance()->Create())
		{
			// Run the application
			if (theApp->Create())
				theApp->Run();

			AppCore::Instance()->Destroy();
		}
		else
			Console::Read();
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}
}
