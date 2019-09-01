/*=============================================================================
SampleTerrain.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleTerrain.h"
#include "Utils.h"

#define RESET_UI 0

Application* theApp = Application::Instance();

ManagerPlugin* renderSystenPlugin;
ManagerPlugin* shaderSystenPlugin;

TerrainNode* terrainNode;
enum CameraMode
{
	CameraMode_Free=1,
	CameraMode_FirstPerson=2,
	CameraMode_LookAt=4
} myCameraMode;
FreeCameraController* myFCameraController = new FreeCameraController();
FirstPersonCameraController* myFPCameraController = new FirstPersonCameraController();
LookAtCameraController* myLACameraController = new LookAtCameraController();

struct Unit
{
	ModelNode* _Node;
};
Array<Unit*> gUnits;
Unit* gActiveUnit;

bool OnButtonPreviousClick(Object* sender, const EventArgs& e)
{
	int32 index = gUnits.IndexOf(gActiveUnit) - 1;
	if (index < 0) index = gUnits.Count() - 1;
	gActiveUnit = gUnits[index];
	myLACameraController->SetTarget(gActiveUnit->_Node);
	return true;
}

bool OnButtonNextClick(Object* sender, const EventArgs& e)
{
	int32 index = gUnits.IndexOf(gActiveUnit) + 1;
	if (index >= gUnits.Count()) index = 0;
	gActiveUnit = gUnits[index];
	myLACameraController->SetTarget(gActiveUnit->_Node);
	return true;
}

class GameCore : public Singleton<GameCore>
{
public:
	GameCore() :
		_Scene(NULL),
		_Model(NULL),
		_vp(NULL),
		_pp(NULL),
		_RootWidget(NULL)
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
		_MainWindow->Create(false, "Terrain Sample", 0, 0, 1024, 768, true, true, true, false, false);

		theApp->AddWindow(_MainWindow);
		theApp->SetCurrentWindow(_MainWindow);

		_MainWindow->Sized.Add(new DefaultEventDelegateM<GameCore>(GameCore::Instance(), &GameCore::OnSize));

		RenderSystem::SetCurrent((RenderSystem*)renderSystenPlugin->CreateManager());

		RenderContextDescription desc;
		desc.Mode.Width = 1024;
		desc.Mode.Height = 768;
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
		SE_DELETE(_RootWidget);
		SE_DELETE(_Scene);
		SE_DELETE(_Model);
		SE_DELETE(_camera);
		SE_DELETE(_MainWindow);
	}

	bool CreateRenderSystem()
	{
		ManagerPlugin* gl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
		if (gl == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL plugin."));
			//return false;
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

		/*Console::WriteLine("Select a render system:");
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
		}*/
		renderSystenPlugin = d3d9;

		return true;
	}

	bool CreateShaderSystem()
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
			return false;
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

		shaderSystenPlugin = d3d9cg;

		/*Console::WriteLine("Select a shader system:");
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
		}*/
		shaderSystenPlugin = d3d9cg;

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
		SceneManager::Instance()->SetCamera(_camera);

		UI::UISystem::Instance()->Create();
		String fontName = FileSystem::Instance()->GetFullPath("defaultfont.dds");
		UI::UISystem::Instance()->SetDefaultFont(CreateFont(fontName));

		myCameraMode = CameraMode_LookAt;
		myFCameraController->SetCamera(_camera);
		myFPCameraController->SetCamera(_camera);
		myLACameraController->SetCamera(_camera);

		AxisShape* axisShape = new AxisShape();
		axisShape->SetAxisSize(10.0f);
		axisShape->Build();
		_AxisViewport.SetShape(axisShape);
		_AxisViewport.SetCamera(_camera);

		//_GridShape.SetGridPlanes((GridPlane)(GridPlane_XY | GridPlane_YZ));
		_GridShape.SetGridPlanes((GridPlane)0);
		_GridShape.SetColor(Color32::Magenta);
		_GridShape.Build();
/*
		_QuadShape.Build();

		ShaderMaterial* quadShader = new Shader();
		ShaderTechnique* quadTechnique = new ShaderTechnique();
		ShaderPass* quadPass = new ShaderPass();
		quadPass->LightState.Lighting = false;
		quadPass->MaterialState.VertexColor = false;
		quadPass->AddSamplerState(new SamplerState());
		TextureState* textureState = new TextureState();
		_texture = CreateTexture(_T("env3.bmp"));
		textureState->SetTexture(_texture);
		quadPass->AddTextureState(textureState);
		quadTechnique->AddPass(quadPass);
		quadShader->AddTechnique(quadTechnique);
		_QuadShape.GetMesh()->SetShader(quadShader);
*/
		_ShapeShader = new Shader();
		ShaderTechnique* technique = new ShaderTechnique();

		ShaderPass* pass0 = new ShaderPass();
		pass0->RasterizerState.FillMode = FillMode_Solid;
		//pass0->DepthState.Enable = false;
		pass0->MaterialState.DiffuseColor = Color32::Gray;
		pass0->LightState.Lighting = true;
		technique->AddPass(pass0);

		ShaderPass* pass1 = new ShaderPass();
		pass1->RasterizerState.FillMode = FillMode_WireFrame;
		pass1->RasterizerState.CullMode = CullMode_None;
		pass1->DepthState.SlopeScaledDepthBias = 1.0f;
		pass1->MaterialState.DiffuseColor = Color32::Black;
		pass1->LightState.Lighting = false;
		technique->AddPass(pass1);

		_ShapeShader->AddTechnique(technique);

		ShaderMaterial* vshader;

		if (ShaderSystem::Current() == NULL)
			vshader = _ShapeShader;
		else
			vshader = CreateHWShader(_T("Shaders\\simplevs.cg"), _T("Shaders\\simpleps.cg"), _T("main"));

		//vshader = CreateHWShader(_T("Shaders\\simple.vsh"), _T("Shaders\\simple.psh"), _T("main"));
		//vshader = CreateHWShader(_T("Shaders\\simplevert.glsl"), _T("Shaders\\simplefrag.glsl"), _T("main"));
		_vp = vshader->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetVertexProgram();
		_pp = vshader->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetPixelProgram();

/*
		_BoxShape = new BoxShape(Box(Vector3(2,2,2), Vector3(2,2,2), Matrix3::Identity));
		_BoxShape->SetDivisions(20);
		if (_BoxShape->CreateMesh(NULL))
			_BoxShape->GetMesh()->SetShader(vshader);

		_SphereShape = new SphereShape(Sphere(Vector3(2,2,2), 2.0f));
		_SphereShape->SetSlices(20);
		_SphereShape->SetStacks(20);
		if (_SphereShape->CreateMesh(NULL))
			_SphereShape->GetMesh()->SetShader(vshader);

		_QuadShape.GetMesh()->SetShader(vshader);


		_ScreenQuadShape.Build();
*/



		_Scene = new Scene();
		_Scene->SetAmbientColor(Color32::Black);
		_Scene->SetRoot(new SceneNode());



		SkyBox* skyBox = new SkyBox();
		/*
		skyBox->SetPlaneTexture(SkyBoxPlane_Left, CreateTexture(_T("sky\\Terrain_negx_0001.png")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Right, CreateTexture(_T("sky\\Terrain_posx_0001.png")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Top, CreateTexture(_T("sky\\Terrain_posy_0001.png")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Bottom, CreateTexture(_T("sky\\Terrain_negy_0001.png")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Front, CreateTexture(_T("sky\\Terrain_posz_0001.png")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Back, CreateTexture(_T("sky\\Terrain_negz_0001.png")));
		*/
		skyBox->SetPlaneTexture(SkyBoxPlane_Left, CreateTexture(_T("Mountains\\negx.jpg")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Right, CreateTexture(_T("Mountains\\posx.jpg")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Top, CreateTexture(_T("Mountains\\posy.jpg")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Bottom, CreateTexture(_T("Mountains\\negy.jpg")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Front, CreateTexture(_T("Mountains\\posz.jpg")));
		skyBox->SetPlaneTexture(SkyBoxPlane_Back, CreateTexture(_T("Mountains\\negz.jpg")));
		//skyBox->SetDistance(2 * _camera->GetFarPlane() * Math::Sqrt(3) / 3);
		skyBox->SetDistance(1000.0);
		skyBox->Create();

		SkyDome* skyDome = new SkyDome();
		skyDome->SetRadius(500.0f);
		skyDome->SetTexture(CreateTexture(_T("Terrain\\sky6.jpg")));
		skyDome->Create();



		_Scene->SetSky(skyDome);

		FogState fogState;
		fogState.Enable = true;
		fogState.RangeEnable = false;
		fogState.Color = Color32(0.0f, 0.4f, 0.7f);
		fogState.Density = 0.2;
		fogState.Start = 200.0;
		fogState.End = 1000.0;
		fogState.VertexMode = FogMode_Linear;
		_Scene->SetFogState(fogState);



		_PlaneShape = new PlaneShape(Plane(Vector3::UnitY, 1.0f));
		_PlaneShape->SetDivisions(1);
		_PlaneShape->SetScale(Vector2::One * 5000.0f);
		ShapeDescription waterDesc;
		waterDesc.TextureCoordinates = true;
		if (_PlaneShape->CreateMesh(&waterDesc))
			_PlaneShape->GetMesh()->SetShader(vshader);

		Model* waterModel = new Model();
		waterModel->AddMesh(_PlaneShape->GetMesh());
		ModelNode* waterNode = new ModelNode();
		waterNode->SetModel(waterModel);
		_Scene->GetRoot()->AddChild(waterNode);



		HeightField* field = new HeightField();
		field->Create(257, 257, HeightFieldFormat_Real);
		//field->Create(9, 9, HeightFieldFormat_Real);

		Random* rnd = new RandomStd();
		rnd->GenerateSeed();

		HeightFieldGenerator gen;
		gen.SetHeightField(field);
		gen.SetRandom(rnd);
		//gen.ConstantHeight(0.0f);
		//gen.RandomHeight(-20.0f, 20.0f);
		gen.Fault(TerrainFaultMethod_Line, 128, 0, 255);

		delete rnd;

		HeightFieldFilter filter;
		filter.SetHeightField(field);
		filter.Erosion(0.5f);
		filter.Normalize();

		QuadTerrainRenderer* quadRenderer = new QuadTerrainRenderer();
		quadRenderer->SetDepth(4);
		StaticTerrainRenderer* staticRenderer = new StaticTerrainRenderer();
		//staticRenderer->SetTextureScale(Vector2(16, 16));
		staticRenderer->SetDetailTextureScale(Vector2(64, 64));

		TerrainRenderer* terrainRenderer = staticRenderer;

		_Terrain = new Terrain();
		_Terrain->SetHeightField(field);
		//_Terrain->SetFieldScale(Vector2(5.0f, 5.0f));
		_Terrain->SetFieldScale(Vector2(10.0f, 10.0f));
		_Terrain->SetHeightScale(255.0f);
		_Terrain->SetTerrainRenderer(terrainRenderer);

		terrainRenderer->SetTerrain(_Terrain);
		terrainRenderer->SetCamera(_camera);
		if (ShaderSystem::Current())
			terrainRenderer->SetShader(CreateTerrainShader_Splatting(_Terrain));
		else
			terrainRenderer->SetShader(CreateTerrainShader_DiffuseGenerate(_Terrain));

		_Terrain->Create();

		terrainNode = new TerrainNode();
		terrainNode->SetTerrain(_Terrain);
		terrainNode->SetLocalPosition(
			Vector3(-_Terrain->GetWidth()/2, 0, -_Terrain->GetHeight()/2));
		_Scene->GetRoot()->AddChild(terrainNode);

		DirectionalLight* light = new DirectionalLight();
		light->SetDirection(-Vector3::UnitY);
		_Terrain->GetTerrainRenderer()->GetShader()->GetTechniqueByIndex(0)->
			GetPassByIndex(0)->LightState.Lights.Add(light);

		SceneManager::Instance()->SetDefaultLight(false);

		ModelPtr models[8];
		models[0] = CreateModel("Generals\\CvHumVee.W3D");
		models[1] = CreateModel("Generals\\CVbackhoe.W3D");
		models[2] = CreateModel("Generals\\NVBtMstr.W3D");
		models[3] = CreateModel("Generals\\CVTruck.W3D");
		models[4] = CreateModel("Generals\\PTPalm01.w3d");
		models[5] = CreateModel("Generals\\PTPalm02.w3d");
		models[6] = CreateModel("Generals\\PTPalm03.w3d");
		models[7] = CreateModel("Generals\\PTPalm04.w3d");

		Model* cloneModel;
		for (int n = 0; n < 50; ++n)
		{
			int m = Math::Random(0, 7);
			cloneModel = models[m]->Clone();
			ModelNode* modelNode = new ModelNode();
			modelNode->SetModel(cloneModel);
			modelNode->SetLocalPosition(
				Vector3(Math::Random(-1024.0f, 1024.0f), 0, Math::Random(-1024.0f, 1024.0f)));
			modelNode->SetLocalOrientation(Quaternion::FromAxisAngle(
				Vector3::UnitY, Math::Random(0.0f, Math::Pi)));
			_Scene->GetRoot()->AddChild(modelNode);

			Vector3 modelPos = modelNode->GetLocalPosition() - terrainNode->GetLocalPosition();
			modelPos.Y = _Terrain->GetInterpolatedHeight(modelPos.X, modelPos.Z);
			modelNode->SetLocalPosition(modelPos + terrainNode->GetLocalPosition());

			Quaternion modelRot = modelNode->GetLocalOrientation();

			Vector3 normal = _Terrain->GetTerrainRenderer()->GetInterpolatedNormal(modelPos.X, modelPos.Z);
			real zangle = Math::Acos(Vector3::Dot(modelRot.Rotate(Vector3::UnitZ), normal));
			real xangle = Math::Acos(Vector3::Dot(modelRot.Rotate(Vector3::UnitX), normal));

			Quaternion qX = Quaternion::FromAxisAngle(Vector3::UnitX, Math::Pi/2-zangle);
			Quaternion qZ = Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi/2+xangle);
			modelNode->SetLocalOrientation(modelRot*qX*qZ);

			if (m < 4)
			{
				Unit* unit = new Unit();
				unit->_Node = modelNode;
				gUnits.Add(unit);
			}
		}

		gActiveUnit = gUnits[0];
		myLACameraController->SetTarget(gActiveUnit->_Node);
		myLACameraController->SetDistance(50);
		myLACameraController->SetElevation(20);
		myLACameraController->SetAngle(180);

/*
		_BoxShape = new BoxShape(Box(Vector3(2,2,2), Vector3(2,2,2), Matrix3::Identity));
		_BoxShape->SetDivisions(20);
		_BoxShape->CreateMesh(NULL);
		_BoxShape->GetMesh()->SetShader(_ShapeShader);

		Model* boxModel = new Model();

		MeshFrame* boxFrame0 = new MeshFrame();
		boxFrame0->AddMeshLOD(0.0f, _BoxShape->GetMesh());
		boxFrame0->SetTransform(Transform(Vector3::Zero, Vector3::One,
			Quaternion::FromAxisAngle(Vector3::UnitY, Math::Pi/4)));
		boxModel->AddMeshFrame(boxFrame0);

		MeshFrame* boxFrame11 = new MeshFrame();
		boxFrame11->AddMeshLOD(0.0f, _BoxShape->GetMesh());
		boxFrame11->SetTransform(Transform(Vector3(-5, -5, 0), Vector3::One, Quaternion::Identity));
		boxFrame0->AddChild(boxFrame11);
		MeshFrame* boxFrame12 = new MeshFrame();
		boxFrame12->AddMeshLOD(0.0f, _BoxShape->GetMesh());
		boxFrame12->SetTransform(Transform(Vector3(5, -5, 0), Vector3::One, Quaternion::Identity));
		boxFrame0->AddChild(boxFrame12);

		ModelNode* boxNode = new ModelNode();
		boxNode->SetModel(boxModel);
		boxNode->SetLocalPosition(Vector3(0, 150, 0));
		_Scene->GetRoot()->AddChild(boxNode);
*/


		File* fileXml = new File("sampleviewer.Xml");
		XMLSerializer* serializerXml  = new XMLSerializer();

#if RESET_UI
		CreateUI();

		FileStreamPtr streamXml = fileXml->Open(FileMode_Create);
		if (streamXml != NULL)
		{
			serializerXml->Serialize(streamXml.Get(), _RootWidget);
			streamXml->Close();
		}
#else
		FileStreamPtr streamXml = fileXml->Open(FileMode_Open, FileAccess_Read);
		if (streamXml != NULL)
		{
			_RootWidget = (UI::Panel*)serializerXml->Deserialize(streamXml.Get());
			streamXml->Close();
		}
#endif

		if (_RootWidget != NULL)
		{
			UI::UISystem::Instance()->SetCurrentWidget(_RootWidget);

			UI::PushButton* buttonPrevious = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("buttonPrevious");
			UI::PushButton* buttonNext = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("buttonNext");

			if (buttonPrevious != NULL)
				buttonPrevious->Click += new DefaultEventDelegateF(&OnButtonPreviousClick);
			if (buttonNext != NULL)
				buttonNext->Click += new DefaultEventDelegateF(&OnButtonNextClick);
		}

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

		UI::UISystem::Instance()->Update(elapsed);

		InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
			GetInputDeviceByType(InputDeviceType_Keyboard);
		if (keyboard->IsKeyDown(Key_Escape))
		{
			Application::Instance()->Exit();
			return true;
		}

		if (keyboard->IsKeyDown(Key_Escape))
			Application::Instance()->Exit();

		if (keyboard->IsKeyDown(Key_Subtract))
			_camera->SetFOV(Math::Max(_camera->GetFOV() - 0.2f, 15.0f));

		if (keyboard->IsKeyDown(Key_Add))
			_camera->SetFOV(Math::Min(_camera->GetFOV() + 0.2f, 90.0f));

		if (keyboard->IsKeyDown(Key_Multiply))
			_camera->SetFOV(45.0f);

		if (keyboard->IsKeyDown(Key_Tab) && keyboard->IsKeyToggled(Key_Tab))
		{
			int32 index = gUnits.IndexOf(gActiveUnit) + 1;
			if (index >= gUnits.Count()) index = 0;
			gActiveUnit = gUnits[index];
			myLACameraController->SetTarget(gActiveUnit->_Node);
		}


		SceneObject* node = myLACameraController->GetTarget();

		if (keyboard->IsKeyDown(Key_W))
		{
			node->TranslateLocal(Vector3::UnitZ * elapsed * 50.0);
		}
		if (keyboard->IsKeyDown(Key_S))
		{
			node->TranslateLocal(-Vector3::UnitZ * elapsed * 50.0);
		}
		if (keyboard->IsKeyDown(Key_A))
		{
			node->RotateLocal(Vector3::UnitY, elapsed * 5.0);
		}
		if (keyboard->IsKeyDown(Key_D))
		{
			node->RotateLocal(Vector3::UnitY, -elapsed * 5.0);
		}

		Vector3 target = node->GetLocalPosition();
		Vector3 pos = target - terrainNode->GetLocalPosition();
		real32 altitude = _Terrain->GetInterpolatedHeight(pos.X, pos.Z);
		pos.Y = altitude + 5.0f;
		node->SetLocalPosition(pos + terrainNode->GetLocalPosition());

		Vector3 modelPos = node->GetLocalPosition() - terrainNode->GetLocalPosition();
		modelPos.Y = _Terrain->GetInterpolatedHeight(modelPos.X, modelPos.Z);
		node->SetLocalPosition(modelPos + terrainNode->GetLocalPosition());

		Quaternion modelRot = node->GetLocalOrientation();

		Vector3 normal = _Terrain->GetTerrainRenderer()->GetInterpolatedNormal(modelPos.X, modelPos.Z);
		real zangle = acos(Vector3::Dot(modelRot.Rotate(Vector3::UnitZ), normal));
		real xangle = acos(Vector3::Dot(modelRot.Rotate(Vector3::UnitX), normal));

		Quaternion qX = Quaternion::FromAxisAngle(Vector3::UnitX, Math::Pi/2-zangle);
		Quaternion qZ = Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi/2+xangle);
		node->SetLocalOrientation(modelRot*qX*qZ);

		if (myCameraMode & CameraMode_Free)
		{
			if (keyboard->IsKeyDown(Key_W))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MovePZ));

			if (keyboard->IsKeyDown(Key_S))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MoveNZ));

			if (keyboard->IsKeyDown(Key_A))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MovePX));

			if (keyboard->IsKeyDown(Key_D))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MoveNX));

			if (keyboard->IsKeyDown(Key_Z))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MovePY));

			if (keyboard->IsKeyDown(Key_C))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_MoveNY));

			if (keyboard->IsKeyDown(Key_Up))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookPY));

			if (keyboard->IsKeyDown(Key_Down))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookNY));

			if (keyboard->IsKeyDown(Key_Left))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookPX));

			if (keyboard->IsKeyDown(Key_Right))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookNX));

			if (keyboard->IsKeyDown(Key_Q))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookPZ));

			if (keyboard->IsKeyDown(Key_E))
				myFCameraController->HandleMessage(Message(FreeCameraController::Message_LookNZ));
		}
		if (myCameraMode & CameraMode_FirstPerson)
		{
			if (keyboard->IsKeyDown(Key_Up))
			{
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookUp));
			}

			if (keyboard->IsKeyDown(Key_Down))
			{
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookDown));
			}

			if (keyboard->IsKeyDown(Key_Left))
			{
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookLeft));
			}

			if (keyboard->IsKeyDown(Key_Right))
			{
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_LookRight));
			}

			if (keyboard->IsKeyDown(Key_I))
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveForward));

			if (keyboard->IsKeyDown(Key_K))
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveBackward));

			if (keyboard->IsKeyDown(Key_J))
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveLeft));

			if (keyboard->IsKeyDown(Key_L))
				myFPCameraController->HandleMessage(Message(FirstPersonCameraController::Message_MoveRight));
		}
		if (myCameraMode & CameraMode_LookAt)
		{
			if (keyboard->IsKeyDown(Key_Up))
			{
				myLACameraController->SetElevation(myLACameraController->GetElevation() + 1);
			}

			if (keyboard->IsKeyDown(Key_Down))
			{
				myLACameraController->SetElevation(myLACameraController->GetElevation() - 1);
			}

			if (keyboard->IsKeyDown(Key_Left))
			{
				myLACameraController->SetAngle(myLACameraController->GetAngle() - 10);
			}

			if (keyboard->IsKeyDown(Key_Right))
			{
				myLACameraController->SetAngle(myLACameraController->GetAngle() + 10);
			}

			myLACameraController->HandleMessage(Message(Entity::Message_Update));
		}

		pos = _camera->GetLocalPosition() - terrainNode->GetLocalPosition();
		altitude = _Terrain->GetInterpolatedHeight(pos.X, pos.Z);
		if (pos.Y < altitude + 5.0f)
		{
			pos.Y = altitude + 5.0f;
			//_camera->SetLocalPosition(pos + terrainNode->GetLocalPosition());
		}

/*
		if (keyboard->IsKeyDown(Key_Q))
			_camera->Translate(_camera->GetUp() * mSpeed);

		if (keyboard->IsKeyDown(Key_Z))
			_camera->Translate(-_camera->GetUp() * mSpeed);
*/
		static real64 t = 0.0;
		t += Application::Instance()->GetElapsedTime();

		if (_vp != NULL)
		{
			static ShaderParameter* matViewProjection = new ShaderParameter();
			ShaderParameterType mat4Type;
			mat4Type._Class = ShaderVariableClass_Matrix_Rows;
			mat4Type._Type = ShaderVariableType_Float;
			mat4Type._Bytes = sizeof(Matrix4);
#if 1
			Matrix4 mat = Matrix4::Transpose(_camera->GetView()) * 
				Matrix4::Transpose(_camera->GetProjection());
#else
			Matrix4 mat = Matrix4::Transpose(_camera->GetView()*_camera->GetProjection());
#endif
			matViewProjection->SetParameterType(mat4Type);
			matViewProjection->SetRawValue((void*)&mat);
			_vp->SetParameter("matViewProjection", matViewProjection);
		}

		if (_pp != NULL)
		{
			static ShaderParameter* fTime = new ShaderParameter();
			ShaderParameterType floatType;
			floatType._Class = ShaderVariableClass_Scalar;
			floatType._Type = ShaderVariableType_Float;
			floatType._Bytes = sizeof(real32);
			real32 time = t;
			fTime->SetParameterType(floatType);
			fTime->SetRawValue((void*)&time);
			_pp->SetParameter("fTime", fTime);
		}

		/*if (_Terrain != NULL)
		{
			IShaderPass* pass = _Terrain->GetTerrainRenderer()->GetShader()->
				GetTechniqueByIndex(0)->GetPassByIndex(0);
			ShaderProgram* vs = pass->GetVertexProgram();

			if (vs != NULL)
			{
				static ShaderParameter* matViewProjection = new ShaderParameter();
				ShaderParameterType mat4Type;
				mat4Type._Class = ShaderVariableClass_Matrix_Rows;
				mat4Type._Type = ShaderVariableType_Float;
				mat4Type._Bytes = sizeof(Matrix4);
				Matrix4 mat = Matrix4::Transpose(_camera->GetView()) * 
					Matrix4::Transpose(_camera->GetProjection());
				//Matrix4 mat = Matrix4::Transpose(_camera->GetView()*_camera->GetProjection());
				matViewProjection->SetParameterType(mat4Type);
				matViewProjection->SetRawValue((void*)&mat);
				vs->SetParameter("matViewProjection", matViewProjection);
			}
		}*/

		return true;
	}

	bool OnBeginFrame(Object* sender, const UpdateEventArgs& e)
	{
		static Pen pen = Pen();
		pen.Width = 5;

		SceneManager::Instance()->SetCamera(_camera);

		RenderSystem::Current()->SetProjectionTransform(_camera->GetProjection());
		RenderSystem::Current()->SetViewTransform(_camera->GetView());
		RenderSystem::Current()->SetWorldTransform(Matrix4::Identity);

		_Scene->Render();

		//_AxisViewport.Render();

		//UI::UISystem::Instance()->Render();

		return true;
	}

protected:
	Window* _MainWindow;
	Camera* _camera;
	Scene* _Scene;
	Model* _Model;
	AxisViewport _AxisViewport;
	GridShape _GridShape;
	QuadShape _QuadShape;
	ScreenQuadShape _ScreenQuadShape;
	PlaneShape* _PlaneShape;
	BoxShape* _BoxShape;
	SphereShape* _SphereShape;
	ShaderMaterial* _ShapeShader;
	Texture* _texture;
	ShaderProgram* _vp;
	ShaderProgram* _pp;
	Terrain* _Terrain;
	UI::Widget* _RootWidget;
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

	Console::WriteLine("Terrain Sample");
	Console::WriteLine("=============");

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
