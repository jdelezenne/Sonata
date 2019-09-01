/*=============================================================================
SampleScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleScene.h"

SampleScene::SampleScene()
{
	_isFullscreen = true;
}

SampleScene::~SampleScene()
{
}

bool SampleScene::CreateScene()
{
	SceneApplication::CreateScene();

	SkyBox* skyBox = new SkyBox();
	skyBox->Create();
	skyBox->SetPlaneTexture(SkyBoxPlane_Left, GetTexture(_T("Textures\\Skies\\skybox1XN.bmp")));
	skyBox->SetPlaneTexture(SkyBoxPlane_Right, GetTexture(_T("Textures\\Skies\\skybox1XP.bmp")));
	skyBox->SetPlaneTexture(SkyBoxPlane_Top, GetTexture(_T("Textures\\Skies\\skybox1YN.bmp")));
	skyBox->SetPlaneTexture(SkyBoxPlane_Bottom, GetTexture(_T("Textures\\Skies\\skybox1YP.bmp")));
	skyBox->SetPlaneTexture(SkyBoxPlane_Front, GetTexture(_T("Textures\\Skies\\skybox1ZN.bmp")));
	skyBox->SetPlaneTexture(SkyBoxPlane_Back, GetTexture(_T("Textures\\Skies\\skybox1ZP.bmp")));
	skyBox->SetDistance(5000.0);
	_scene->SetSky(skyBox);

	FogState fogState;
	//fogState.Enable = true;
	fogState.RangeEnable = false;
	fogState.Color = Color32(0.0f, 0.4f, 0.7f);
	fogState.Density = 0.2;
	fogState.Start = 200.0;
	fogState.End = 1000.0;
	fogState.VertexMode = FogMode_Linear;
	_scene->SetFogState(fogState);

	return true;
}

bool SampleScene::CreateCamera()
{
	SceneApplication::CreateCamera();

	SceneManager::Instance()->SetCamera(_camera);

	_cameraController = new FirstPersonCameraController();
	_cameraController->SetCamera(_camera);

	return true;
}

bool SampleScene::OnInitialize()
{
	_renderSystem->SetClearColor(Color32::CornflowerBlue);

	UI::UISystem::Instance()->Create();
	String fontName = FileSystem::Instance()->GetFullPath("Fonts\\defaultfont.dds");
	UI::UISystem::Instance()->SetDefaultFont(GetFont(fontName));

	FileSystem::Instance()->AddRootPath("C:\\Games\\Demos\\Q3Ademo\\demoq3");
#if 0
	Scene* scene = GetScene("maps\\q3dm1.bsp");
#else
	FileSystem::Instance()->AddRootPath("C:\\Games\\Demos\\Q3Ademo\\packs");
	Scene* scene = GetScene("maps\\japanc_ta.bsp");
#endif

	if (scene == NULL)
	{
		return true;
	}

	scene->SetSky(_scene->GetSky());
	SceneManager::Instance()->SetScene(scene);

	Scene::SceneObjectList::Iterator it = scene->GetSceneObjectIterator();
	while (it.Next())
	{
		SceneObject* obj = it.Current();
		if (is(obj, Camera))
		{
			_cameras.Add((Camera*)obj);
		}
	}

	if (_cameras.Count() > 0)
	{
		_currentCamera = 0;
		SceneManager::Instance()->SetCamera(_cameras[0]);
		_cameraController->SetCamera(_cameras[0]);
	}

	return true;
}

void SampleScene::OnTerminate()
{
}

void SampleScene::OnIdle()
{
	real64 elapsed = (real64)_elapsedTime;

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	if (keyboard->IsKeyDown(Key_Escape))
	{
		Application::Current()->Exit();
		return;
	}

	if (_cameras.Count() > 0)
	{
		bool changedCamera = false;
		if (keyboard->IsKeyToggled(Key_Home) && keyboard->IsKeyDown(Key_Home))
		{
			if (_currentCamera == 0)
				_currentCamera = _cameras.Count() - 1;
			else
				_currentCamera--;
			changedCamera = true;
		}
		else if (keyboard->IsKeyToggled(Key_End) && keyboard->IsKeyDown(Key_End))
		{
			if (_currentCamera == _cameras.Count() - 1)
				_currentCamera = 0;
			else
				_currentCamera++;
			changedCamera = true;
		}

		if (changedCamera)
		{
			Camera* camera = _cameras[_currentCamera];
			SceneManager::Instance()->SetCamera(camera);
			_cameraController->SetCamera(camera);
			Logger::Current()->Log(String::Concat("Camera: ", camera->GetName()));
		}
	}

	SceneManager::Instance()->Update(_runTime);

	//UI::UISystem::Instance()->Update(elapsed);

	UpdateCamera(elapsed);
}

void SampleScene::OnDraw()
{
	RenderSystem* renderSystem = RenderSystem::Current();

	SceneManager::Instance()->Render();

	//UI::UISystem::Instance()->Render();
}

void SampleScene::UpdateCamera(real64 elapsed)
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	if (keyboard->IsKeyDown(Key_Subtract))
		_camera->SetFOV(Math::Max(_camera->GetFOV() - 0.2f, 15.0f));

	if (keyboard->IsKeyDown(Key_Add))
		_camera->SetFOV(Math::Min(_camera->GetFOV() + 0.2f, 135.0f));

	if (keyboard->IsKeyDown(Key_Multiply))
		_camera->SetFOV(45.0f);

	if (keyboard->IsKeyDown(Key_W))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveForward));
	}
	if (keyboard->IsKeyDown(Key_S))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveBackward));
	}
	if (keyboard->IsKeyDown(Key_A))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveLeft));
	}
	if (keyboard->IsKeyDown(Key_D))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveRight));
	}

	if (keyboard->IsKeyDown(Key_PageUp))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveUp));
	}

	if (keyboard->IsKeyDown(Key_PageDown))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveDown));
	}

	if (keyboard->IsKeyDown(Key_Up))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookUp));
	}

	if (keyboard->IsKeyDown(Key_Down))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookDown));
	}

	if (keyboard->IsKeyDown(Key_Left))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookLeft));
	}

	if (keyboard->IsKeyDown(Key_Right))
	{
		_cameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookRight));
	}

	_cameraController->HandleMessage(Message(Entity::Message_Update));
}

void EntryPoint()
{
	Console::WriteLine("Scene Sample");
	Console::WriteLine("============");

	try
	{
		SampleScene::SetCurrent(new SampleScene());
		Application* theApp = SampleScene::Current();

		if (!theApp->Create())
		{
			Console::Read();
		}
		else
		{
			theApp->Start();
		}
		theApp->Destroy();
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
		Console::Read();
	}
}
