/*=============================================================================
SceneApplication.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SceneApplication.h"
#include <Core/Engine.h>
#include <Plugins.h>

#ifdef WIN32
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

namespace SampleFramework
{

SceneApplication::SceneApplication() :
	Application(),
	_logMode(LogMode_Console)
{
	Engine::Instance();

#ifdef SE_STATIC
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Debug"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Release"));
#	endif
#else
#	if defined(WIN32)
#		ifdef _DEBUG
			Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Win32\\DebugDLL"));
#		else
			Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Win32\\ReleaseDLL"));
#		endif
#	elif defined(XBOX)
#		ifdef _DEBUG
			Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Xbox\\DebugDLL"));
#		else
			Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Xbox\\ReleaseDLL"));
#		endif
#	endif
#endif

	FileSystem::Instance()->AddRootPath(_T("Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\..\\Data"));
}

SceneApplication::~SceneApplication()
{
}

bool SceneApplication::CreateLogHandlers()
{
	Logger* logger = new Logger();
	Logger::SetCurrent(logger);

	if ((_logMode & LogMode_Debug) != 0)
	{
#ifdef WIN32
		logger->GetHandlers().Add(new Win32DebugLogHandler());
#endif
	}
	if ((_logMode & LogMode_Console) != 0)
	{
		LogHandler* consoleHandler = new TextStreamLogHandler(Console::Out());
		((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions((LogOptions)(LogOptions_None));
		logger->GetHandlers().Add(consoleHandler);
	}
	if ((_logMode & LogMode_File) != 0)
	{
		String name = String::Concat(Path::GetFileNameWithoutExtension(
			Environment::GetApplicationFileName()), _T(".log"));
		LogHandler* fileHandler = new TextStreamLogHandler(File(name).Open(FileMode_Create));
		((DefaultLogFormatter*)fileHandler->GetFormatter())->SetOptions((LogOptions)(LogOptions_None));
		logger->GetHandlers().Add(fileHandler);
	}

	return true;
}

bool SceneApplication::Create()
{
	if (_isReady)
	{
		return false;
	}

	if (!CreateLogHandlers())
		return false;

#ifndef SE_STATIC
	PluginManager::Instance()->ParsePlugins(Environment::GetCurrentDirectory());
#endif
	PluginManager::Instance()->CreateAllPlugins();

	if (!CreateRenderSystem())
		return false;

	RenderContextDescription desc;
	if (_mainWindow == NULL)
	{
		if (!CreateMainWindow())
			return false;
	}
	desc.Mode.Width = _mainWindow->GetClientWidth();
	desc.Mode.Height = _mainWindow->GetClientHeight();

	if (!RenderSystem::Current()->CreateRenderContext(_mainWindow, desc))
		return false;

	if (!CreateShaderSystem())
		return false;

	if (!CreateInputSystem())
		return false;

	if (!CreateAudioSystem())
		return false;

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

	mouse->Create(_mainWindow);
	keyboard->Create(_mainWindow);

	CreateScene();
	CreateCamera();
	CreateScreenCamera();

	return Application::Create();
}

bool SceneApplication::Destroy()
{
	return Application::Destroy();
}

bool SceneApplication::CreateMainWindow()
{
	_mainWindow = new Window();
	if (!_isFullscreen)
	{
		_mainWindow->Create("Sample Application", 0, 0, 1024, 768);
	}
	else
	{
		DisplayAdapter adapter;
		SystemDisplay::GetPrimaryDisplayAdapter(adapter);

		RenderContextDescription desc;
		SystemDisplay::GetDesktopDisplayMode(adapter, desc.Mode);
		_mainWindow->Create("Sample Application", 0, 0, desc.Mode.Width, desc.Mode.Height, WindowStyle_None);
	}
	return true;
}

bool SceneApplication::CreateRenderSystem()
{
	ManagerPlugin* renderPlugin = FindRenderSystemPlugin();
	if (renderPlugin == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the render system plugin."));
		return false;
	}

	_renderSystem = (RenderSystem*)renderPlugin->CreateManager();
	if (_renderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to create the render system."));
		return false;
	}

	_renderSystem->Create();
	RenderSystem::SetCurrent(_renderSystem);

	return true;
}

bool SceneApplication::CreateShaderSystem()
{
	ManagerPlugin* shaderPlugin = FindShaderSystemPlugin();
	if (shaderPlugin == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the shader system plugin."));
		return false;
	}

	_shaderSystem = (ShaderSystem*)shaderPlugin->CreateManager();
	if (_shaderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to create the shader system."));
		return false;
	}

	ShaderSystem::SetCurrent(_shaderSystem);

	return true;
}

bool SceneApplication::CreateInputSystem()
{
	ManagerPlugin* inputPlugin = FindInputSystemPlugin();
	if (inputPlugin == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the input system plugin."));
		return false;
	}

	_inputSystem = (InputSystem*)inputPlugin->CreateManager();
	if (_inputSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to create the input system."));
		return false;
	}

	_inputSystem->Create();
	InputSystem::SetCurrent(_inputSystem);

	return true;
}

bool SceneApplication::CreateAudioSystem()
{
	ManagerPlugin* audioPlugin = FindAudioSystemPlugin();
	if (audioPlugin == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to get the audio system plugin."));
		return false;
	}

	_audioSystem = (AudioSystem*)audioPlugin->CreateManager();
	if (_audioSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Failed to create the audio system."));
		return false;
	}

	AudioSystem::SetCurrent(_audioSystem);

	return true;
}

bool SceneApplication::CreateScene()
{
	_scene = new Scene();
	_scene->SetAmbientColor(Color32::Black);
	_scene->SetRoot(new SceneNode());

	SceneManager::Instance()->SetScene(_scene);

	return true;
}

bool SceneApplication::CreateCamera()
{
	int width = _mainWindow->GetClientWidth();
	int height = _mainWindow->GetClientHeight();

	_camera = new Camera();
	_camera->SetPerspective(45.0f,
		(real32)width / (real32)height,
		1.0f, 10000.0f);
	_camera->SetViewport(Viewport(0, 0, width, height));
	_camera->SetLocalPosition(Vector3(0, 5, 5));

	_camera->SetScene(_scene);
	SceneManager::Instance()->SetCamera(_camera);

	return true;
}

bool SceneApplication::CreateScreenCamera()
{
	_screenCamera = new Camera();
	_screenCamera->SetOrthographic(0, 0,
		_mainWindow->GetClientWidth(), _mainWindow->GetClientHeight(), 0.0f, 1.0f);

	return true;
}

Image* SceneApplication::GetImage(const String& name)
{
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(name);
	resource = ResourceManager::Instance()->Get(path);
	if (resource == NULL)
	{
		resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
	}
	if (resource == NULL)
		return NULL;

	return (Image*)resource->GetData();
}

Texture* SceneApplication::GetTexture(const String& name)
{
	Texture* texture;
	Image* image;

	image = SceneApplication::GetImage(name);

	if (!RenderSystem::Current()->CreateTexture(&texture))
		return NULL;

	if (!texture->Create(image, TextureUsage_Static))
	{
		SE_DELETE(texture);
		return NULL;
	}

	return texture;
}

Font* SceneApplication::GetFont(const String& name)
{
	String path = FileSystem::Instance()->GetFullPath(name);
	Resource* resImage = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
	if (resImage == NULL)
		return NULL;

	Image* fontImage = (Image*)resImage->GetData();
	if (fontImage == NULL)
		return NULL;

	Texture* texture;
	if (!RenderSystem::Current()->CreateTexture(&texture))
		return NULL;

	if (!texture->Create(fontImage, TextureUsage_Static))
	{
		SE_DELETE(texture);
		return NULL;
	}

	String xmlName = Path::Combine(Path::GetDirectoryName(path),
		Path::ChangeExtension(path, _T("xml")));
	XMLReader reader(xmlName);
	SonataEngine::XMLDocument* document = reader.GetDocument();
	if (document == NULL)
		return NULL;

	Font* font = new Font();
	font->SetTexture(texture);

	const XMLNode::XMLAttributeList& fontAttributes = document->GetDocumentElement()->GetAttributes();
	for (int i=0; i<fontAttributes.Count(); i++)
	{
		if (fontAttributes[i]->GetName().CompareTo("Spacing", true) == 0)
			font->SetSpacing(fontAttributes[i]->GetValue().ToInt32());
		else if (fontAttributes[i]->GetName().CompareTo("SpaceWidth", true) == 0)
			font->SetSpaceWidth(fontAttributes[i]->GetValue().ToInt32());
		else if (fontAttributes[i]->GetName().CompareTo("Height", true) == 0)
			font->SetHeight(fontAttributes[i]->GetValue().ToInt32());
	}

	XMLNode* glyths = document->GetDocumentElement()->GetFirstChild();
	XMLNode* glyth = glyths->GetFirstChild();
	while (glyth != NULL)
	{
		FontGlyth fontGlyth;

		const XMLNode::XMLAttributeList& glythAttributes = glyth->GetAttributes();
		for (int i=0; i<glythAttributes.Count(); i++)
		{
			try
			{
				if (glythAttributes[i]->GetName().CompareTo("Character", true) == 0)
					fontGlyth.Character = glythAttributes[i]->GetValue().ToChar();
				else if (glythAttributes[i]->GetName().CompareTo("X", true) == 0)
					fontGlyth.Rectangle.X = glythAttributes[i]->GetValue().ToInt32();
				else if (glythAttributes[i]->GetName().CompareTo("Y", true) == 0)
					fontGlyth.Rectangle.X = glythAttributes[i]->GetValue().ToInt32();
				else if (glythAttributes[i]->GetName().CompareTo("Width", true) == 0)
					fontGlyth.Rectangle.Width = glythAttributes[i]->GetValue().ToInt32();
			}
			catch (const FormatException&)
			{
			}
		}

		fontGlyth.Rectangle.Width -= fontGlyth.Rectangle.X;
		fontGlyth.Rectangle.Height = font->GetHeight();

		if (fontGlyth.Character != 0 && !fontGlyth.Rectangle.IsEmpty())
		{
			font->SetGlyth(fontGlyth.Character, fontGlyth);
		}

		glyth = glyth->GetNextSibling();
	}

	font->Build();

	delete fontImage;

	return font;
}

Sprite* SceneApplication::GetSprite(const String& name)
{
	Texture* image = SceneApplication::GetTexture(name);
	if (image == NULL)
		return NULL;

	Sprite* sprite = new Sprite();
	SpriteFrame* frame = new SpriteFrame();

	SizeInt imageSize = SizeInt(image->GetWidth(), image->GetHeight());
	frame->SetSourceRect(RectangleInt(PointInt(0, 0), imageSize));
	frame->SetTexture(image);

	sprite->AddSpriteFrame(frame);
	sprite->SetSize(Vector2(imageSize.Width, imageSize.Height));

	return sprite;
}

Scene* SceneApplication::GetScene(const String& name)
{
	Scene* scene;
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(name);
	resource = ResourceManager::Instance()->Get(path);
	if (resource == NULL)
	{
		resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_SCENE);
	}
	if (resource == NULL)
		return NULL;

	scene = (Scene*)resource->GetData();
	scene->SetName(Path::GetFileNameWithoutExtension(name));

	return scene;
}

Model* SceneApplication::GetModel(const String& name, real scale)
{
	Model* model;
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(name);
	resource = ResourceManager::Instance()->Get(path);
	if (resource == NULL)
	{
		resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_MODEL);
	}
	if (resource == NULL)
		return NULL;

	model = (Model*)resource->GetData();
	model->SetName(Path::GetFileNameWithoutExtension(name));

	if (scale != 1.0)
	{
		model->SetTransform(model->GetTransform() * Matrix4::CreateScale(Vector3(scale)));
	}

	return model;
}

EffectShader* SceneApplication::GetEffectShader(const String& name)
{
	EffectShader* effectShader;

	String path = FileSystem::Instance()->GetFullPath(name);
	effectShader = ShaderSystem::Current()->CreateEffectShader(path);

	return effectShader;
}

ManagerPlugin* SceneApplication::FindRenderSystemPlugin()
{
	ManagerPlugin* renderPlugin;

	renderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D9);
	if (renderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D9 render plugin."));
		return renderPlugin;
	}

	renderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D8);
	if (renderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D8 render plugin."));
		return renderPlugin;
	}

	renderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
	if (renderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the OpenGL render plugin."));
		return renderPlugin;
	}

	return NULL;
}

ManagerPlugin* SceneApplication::FindShaderSystemPlugin()
{
	ManagerPlugin* shaderPlugin;

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D9CG);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D9 Cg shader plugin."));
		return shaderPlugin;
	}

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_HLSL9);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D9 HLSL shader plugin."));
		return shaderPlugin;
	}

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D8CG);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D8 Cg shader plugin."));
		return shaderPlugin;
	}

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_D3D8);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Direct3D8 shader plugin."));
		return shaderPlugin;
	}

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLCG);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the OpenGL Cg shader plugin."));
		return shaderPlugin;
	}

	shaderPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_SHADERSYSTEM_GLSL);
	if (shaderPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the OpenGL GLSL shader plugin."));
		return shaderPlugin;
	}

	return NULL;
}

ManagerPlugin* SceneApplication::FindInputSystemPlugin()
{
	ManagerPlugin* inputPlugin;

	inputPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_INPUTSYSTEM_DI8);
	if (inputPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the DirectInput8 input plugin."));
		return inputPlugin;
	}

	inputPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_INPUTSYSTEM_WIN32);
	if (inputPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the Win32 input plugin."));
		return inputPlugin;
	}

	return NULL;
}

ManagerPlugin* SceneApplication::FindAudioSystemPlugin()
{
	ManagerPlugin* audioPlugin;

	audioPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_AUDIOSYSTEM_DS8);
	if (audioPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the DirectSound8 audio plugin."));
		return audioPlugin;
	}

	audioPlugin = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_AUDIOSYSTEM_AL);
	if (audioPlugin != NULL)
	{
		Logger::Current()->Log(LogLevel::Notice, _T("Found the OpenAL audio plugin."));
		return audioPlugin;
	}

	return NULL;
}

}
