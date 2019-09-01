/*=============================================================================
SampleViewer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleViewer.h"

//#define SKINNING_HW

static const int maxBoneCount = 60;

SampleViewer::SampleViewer() :
	_hasEffect(false),
	_isSkinned(false),
	_skinIndex(-1),
	_morphIndex(-1)
{
}

SampleViewer::~SampleViewer()
{
}

bool SampleViewer::CreateScene()
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

	return true;
}

bool SampleViewer::CreateCamera()
{
	SceneApplication::CreateCamera();

	SceneManager::Instance()->SetCamera(_camera);

	_cameraController = new OrbitCameraController();
	_cameraController->SetCamera(_camera);

	return true;
}

bool SampleViewer::OnInitialize()
{
	_renderSystem->SetClearColor(Color32::CornflowerBlue);

	UI::UISystem::Instance()->Create();
	String fontName = FileSystem::Instance()->GetFullPath("Fonts\\defaultfont.dds");
	UI::UISystem::Instance()->SetDefaultFont(GetFont(fontName));

	_axisShape.SetAxisSize(10.0f);
	_axisShape.Build();
	_viewAxisShape.SetShape(&_axisShape);
	_viewAxisShape.SetCamera(_camera);

	_gridShape.SetGridPlanes(GridPlane_ZX);
	_gridShape.SetColor(Color32::Magenta);
	_gridShape.Build();

	_hasEffect = false;
	_isSkinned = false;

#if 1
	//_model = GetModel("Models\\Cup\\cup.obj");
	//_model = GetModel("Models\\arrow.x");
	//_model = GetModel("Models\\teapot.x");
	//_model = GetModel("Models\\tiger.x");
	_model = GetModel("Models\\bigship.x");
	//_model = GetModel("Models\\ChairScene\\room.x");
	//_model = GetModel("Models\\camera.x");
	//_model = GetModel("Models\\Crypt\\crypt.sdkmesh");
	//_model = GetModel("Models\\Quake\\player.mdl");
	//_model = GetModel("Models\\Warrior\\warrior.sdkmesh");
	//_model = GetModel("Models\\Quake\\soldier\\tris.md2");
	//_model = GetModel("Models\\Quake\\sarge\\head.md3");
#elif 1
	_model = GetModel("Models\\Dwarf\\dwarf.x");
	_hasEffect = true;
#elif 1
	_model = GetModel("Models\\Tiny\\tiny.x", 0.1);
	//_model = GetModel("Models\\Soldier\\soldier.sdkmesh");
	_isSkinned = true;
#else
	GetUnrealModel("Models\\UDN\\Male.PSK", 0.1,
		"Models\\UDN\\cloth.tga", "Models\\UDN\\skin.tga", "Models\\UDN\\hair.tga");
	_isSkinned = true;
#endif

	if (_model == NULL)
	{
		return true;
	}

	InitSkin();

	_modelNode = new ModelNode();
	_modelNode->SetModel(_model);
	_scene->GetRoot()->AddChild(_modelNode);

	_scene->AddObject(_modelNode);

	_cameraController->SetTarget(_modelNode);
	_cameraController->SetDistance(50.0);
	_cameraController->SetElevationAngle(20.0);
	_cameraController->SetAzimuthAngle(180.0);

#if 1
	ApplyShader(_model, "Shaders\\Cartoon.fx");
#endif

	return true;
}

void SampleViewer::ApplyShader(Model* model, const String& shaderName)
{
	Model::MeshList::Iterator it = model->GetMeshIterator();
	while (it.Next())
	{
		ApplyShader(it.Current(), shaderName);
	}
}

void SampleViewer::ApplyShader(Mesh* mesh, const String& shaderName)
{
	MeshPart* meshPart = NULL;
	FFPPass* oldPass = NULL;
	Mesh::MeshPartList::Iterator it = mesh->GetMeshPartIterator();
	while (it.Next())
	{
		meshPart = it.Current();

		EffectShader* effectShader = GetEffectShader(shaderName);
		if (effectShader != NULL)
		{
			_shaders.AddLast(effectShader);

#if !defined(SKINNING_HW)
			if (!_hasEffect)
			{
				DefaultMaterial* oldShader = NULL;
				oldShader = (DefaultMaterial*)meshPart->GetShader();
				if (oldShader != NULL)
				{
					FFPTechnique* oldTechnique = (FFPTechnique*)oldShader->GetTechnique();
					oldPass = (FFPPass*)oldTechnique->GetPassByIndex(0);
				}
			}
#endif

			EffectMaterial* shader = new EffectMaterial();
			shader->SetEffectShader(effectShader);
			meshPart->SetShader(shader);

#if !defined(SKINNING_HW)
			if (!_hasEffect)
			{
				if (oldPass != NULL && oldPass->GetSamplerStateCount() > 0)
				{
					if (oldPass->GetSamplerStateByIndex(0)->GetTexture() != NULL)
					{
						EffectParameter* diffuseTextureParam = effectShader->GetParameterByName("DiffuseTexture");
						diffuseTextureParam->SetValue(((TexturePtr)oldPass->GetSamplerStateByIndex(0)->GetTexture()).Get());
						shader->SetTechnique("CartoonTextured");
					}
					else
					{
						EffectParameter* diffuseColorParam = effectShader->GetParameterByName("DiffuseColor");
						diffuseColorParam->SetValue(&oldPass->MaterialState.DiffuseColor);

						shader->SetTechnique("Cartoon");
					}
				}
			}
			else
			{
				EffectParameter* diffuseColorParam = effectShader->GetParameterByName("DiffuseColor");
				diffuseColorParam->SetValue((void*)&Color32::Yellow);
				shader->SetTechnique("Cartoon");
			}
		}
#endif

		EffectParameter* rampTextureParam = effectShader->GetParameterByName("RampTexture");
		rampTextureParam->SetValue(GetTexture("Shaders\\toonshade.dds"));
	}
}

void SampleViewer::OnTerminate()
{
}

void SampleViewer::UpdateAnimationSet(AnimationSet* animationSet, int& sequenceIndex)
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	if (keyboard->IsKeyToggled(Key_Space) || keyboard->IsKeyToggled(Key_LeftBracket) || keyboard->IsKeyToggled(Key_RightBracket))
	{
		if (sequenceIndex != -1)
		{
			animationSet->GetAnimationSequenceByIndex(sequenceIndex)->Stop();
		}
	}

	if (keyboard->IsKeyToggled(Key_Space))
	{
		sequenceIndex = -1;
	}
	else if (keyboard->IsKeyToggled(Key_LeftBracket))
	{
		sequenceIndex--;
		if (sequenceIndex < 0)
		{
			sequenceIndex = animationSet->GetAnimationSequenceCount() - 1;
		}
		animationSet->GetAnimationSequenceByIndex(sequenceIndex)->SetLooping(true);
		animationSet->GetAnimationSequenceByIndex(sequenceIndex)->Start();
	}
	else if (keyboard->IsKeyToggled(Key_RightBracket))
	{
		sequenceIndex++;
		if (sequenceIndex >= animationSet->GetAnimationSequenceCount())
		{
			sequenceIndex = 0;
		}
		animationSet->GetAnimationSequenceByIndex(sequenceIndex)->SetLooping(true);
		animationSet->GetAnimationSequenceByIndex(sequenceIndex)->Start();
	}

	if (keyboard->IsKeyToggled(Key_Space) || keyboard->IsKeyToggled(Key_LeftBracket) || keyboard->IsKeyToggled(Key_RightBracket))
	{
		if (sequenceIndex == -1)
		{
			Logger::Current()->Log("<Rest>");
		}
		else
		{
			Logger::Current()->Log(animationSet->GetAnimationSequenceByIndex(sequenceIndex)->GetName());
		}
	}
}

void SampleViewer::OnIdle()
{
	real64 elapsed = (real64)_elapsedTime;

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	if (keyboard->IsKeyDown(Key_Escape))
	{
		Application::Current()->Exit();
		return;
	}

	if (_model != NULL)
	{
		AnimationSet* animationSet;
		if (_model->GetSkeleton() != NULL)
		{
			animationSet = _model->GetSkeleton()->GetAnimationSet();
			if (animationSet != NULL)
			{
				UpdateAnimationSet(animationSet, _skinIndex);
			}
		}

		animationSet = _model->GetAnimationSet();
		if (animationSet != NULL)
		{
			UpdateAnimationSet(animationSet, _morphIndex);
		}
	}

	SceneManager::Instance()->Update(_runTime);

	UI::UISystem::Instance()->Update(elapsed);

	UpdateCamera(elapsed);

	if (_model != NULL)
	{
		UpdateSkin();
	}

	SceneState* sceneState = SceneManager::Instance()->GetSceneState();

	BaseList<EffectShader*>::Iterator it = _shaders.GetIterator();
	while (it.Next())
	{
		EffectShader* effectShader = it.Current();
		Matrix4 world = _modelNode->GetWorldTransform() * _model->GetTransform();
		Matrix4 viewProj = _camera->GetProjection() * _camera->GetView();

		Matrix4 worldView = _camera->GetView() * world;
		EffectParameter* worldViewParam = effectShader->GetParameterByName("WorldView");
		worldViewParam->SetValue(&worldView);

		Matrix4 worldViewProj = viewProj * world;
		EffectParameter* worldViewProjParam = effectShader->GetParameterByName("WorldViewProj");
		worldViewProjParam->SetValue(&worldViewProj);

		Vector3 lightDirection = Quaternion::Rotate(_camera->GetWorldOrientation(), Vector3::Forward);
		EffectParameter* lightDirectionParam = effectShader->GetParameterByName("LightDirection");
		lightDirectionParam->SetValue(&lightDirection);
	}
}

void SampleViewer::OnDraw()
{
	SceneManager::Instance()->Render();

	for (int i = 0; i < _skeletonShapes.Count(); i++)
	{
		if (_skeletonShapes[i]->GetMesh() != NULL)
		{
			SceneManager::Instance()->RenderMesh(_skeletonShapes[i]->GetMesh(),
				_modelNode->GetWorldTransform() * _model->GetTransform());
		}
	}

	SceneManager::Instance()->RenderMesh(_axisShape.GetMesh());

	_viewAxisShape.Render();

	UI::UISystem::Instance()->Render();
}

void SampleViewer::GetUnrealModel(const String& model, real scale, const String& body, const String& skin, const String& hair)
{
	_model = GetModel(model, scale);
	if (_model == NULL)
		return;

	_isSkinned = true;

	Mesh* mesh = _model->GetMeshByIndex(0);
	if (mesh == NULL)
		return;

	if (mesh->GetMeshPartCount() > 2)
	{
		DefaultMaterial* effect = (DefaultMaterial*)mesh->GetMeshPart(2)->GetShader();
		((FFPPass*)effect->GetTechnique()->GetPassByIndex(0))->GetSamplerStateByIndex(0)->SetTexture(GetTexture(hair));
	}
	if (mesh->GetMeshPartCount() > 1)
	{
		DefaultMaterial* effect = (DefaultMaterial*)mesh->GetMeshPart(1)->GetShader();
		((FFPPass*)effect->GetTechnique()->GetPassByIndex(0))->GetSamplerStateByIndex(0)->SetTexture(GetTexture(skin));
	}
	if (mesh->GetMeshPartCount() > 0)
	{
		DefaultMaterial* effect = (DefaultMaterial*)mesh->GetMeshPart(0)->GetShader();
		((FFPPass*)effect->GetTechnique()->GetPassByIndex(0))->GetSamplerStateByIndex(0)->SetTexture(GetTexture(body));
	}

	//_model->GetSkeleton()->GetRootBone()->SetLocalOrientation(
	//	Quaternion::CreateFromAxisAngle(Vector3::UnitY, -Math::Pi / 2) *
	//	_model->GetSkeleton()->GetRootBone()->GetLocalOrientation());
}

void SampleViewer::UpdateCamera(real64 elapsed)
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	if (keyboard->IsKeyDown(Key_Subtract))
		_camera->SetFOV(Math::Max(_camera->GetFOV() - 0.2f, 15.0f));

	if (keyboard->IsKeyDown(Key_Add))
		_camera->SetFOV(Math::Min(_camera->GetFOV() + 0.2f, 135.0f));

	if (keyboard->IsKeyDown(Key_Multiply))
		_camera->SetFOV(45.0f);

	SceneObject* node = _cameraController->GetTarget();

	if (keyboard->IsKeyDown(Key_W))
	{
		node->RotateLocal(Vector3::UnitX, -Math::Pi * 0.5 * elapsed);
	}
	if (keyboard->IsKeyDown(Key_S))
	{
		node->RotateLocal(Vector3::UnitX, Math::Pi * 0.5 * elapsed);
	}
	if (keyboard->IsKeyDown(Key_A))
	{
		node->RotateLocal(Vector3::UnitZ, Math::Pi * 0.5 * elapsed);
	}
	if (keyboard->IsKeyDown(Key_D))
	{
		node->RotateLocal(Vector3::UnitZ, -Math::Pi * 0.5 * elapsed);
	}
	if (keyboard->IsKeyDown(Key_Q))
	{
		node->RotateLocal(Vector3::UnitY, -Math::Pi * 0.5 * elapsed);
	}
	if (keyboard->IsKeyDown(Key_E))
	{
		node->RotateLocal(Vector3::UnitY, Math::Pi * 0.5 * elapsed);
	}

	if (keyboard->IsKeyDown(Key_PageUp))
	{
		_cameraController->SetDistance(_cameraController->GetDistance() - 1);
	}

	if (keyboard->IsKeyDown(Key_PageDown))
	{
		_cameraController->SetDistance(_cameraController->GetDistance() + 1);
	}

	if (keyboard->IsKeyDown(Key_Up))
	{
		_cameraController->SetElevationAngle(_cameraController->GetElevationAngle() + 5);
	}

	if (keyboard->IsKeyDown(Key_Down))
	{
		_cameraController->SetElevationAngle(_cameraController->GetElevationAngle() - 5);
	}

	if (keyboard->IsKeyDown(Key_Left))
	{
		_cameraController->SetAzimuthAngle(_cameraController->GetAzimuthAngle() - 5);
	}

	if (keyboard->IsKeyDown(Key_Right))
	{
		_cameraController->SetAzimuthAngle(_cameraController->GetAzimuthAngle() + 5);
	}

	_cameraController->HandleMessage(Message(Entity::Message_Update));
}

void SampleViewer::InitSkin()
{
	if (_isSkinned)
	{
		if (_model->GetSkeleton() != NULL)
		{
			Bone* rootBone = _model->GetSkeleton()->GetRootBone();

			for (int i = 0; i < _model->GetMeshCount(); ++i)
			{
				if (_model->GetMeshByIndex(i)->GetSkin() != NULL)
				{
					SkeletonShape* skeletonShape = new SkeletonShape();
					skeletonShape->SetSkeleton(_model->GetSkeleton());
					skeletonShape->SetSkin(_model->GetMeshByIndex(i)->GetSkin());
					skeletonShape->SetColor(Color32::Red);
					skeletonShape->Build();
					_skeletonShapes.Add(skeletonShape);
				}
			}
		}
	}

#ifdef SKINNING_HW
	InitSkin();

	for (int i = 0; i < _model->GetMeshCount(); ++i)
	{
		Mesh* mesh = _model->GetMeshByIndex(i);
		for (int j = 0; j < mesh->GetMeshPartCount(); ++j)
		{
			InitSkin(mesh->GetMeshPart(j));
		}
	}
#endif
}

void SampleViewer::InitSkin(MeshPart* meshPart)
{
	if (_hasEffect)
	{
		return;
	}

	DefaultMaterial* oldShader = NULL;
	FFPPass* oldPass = NULL;
	oldShader = (DefaultMaterial*)meshPart->GetShader();
	if (oldShader != NULL)
	{
		FFPTechnique* oldTechnique = (FFPTechnique*)oldShader->GetTechnique();
		oldPass = (FFPPass*)oldTechnique->GetPassByIndex(0);
	}

	EffectShader* effectShader = GetEffectShader("Shaders\\Skinning.fx");
	EffectMaterial* shader = new EffectMaterial();
	shader->SetEffectShader(effectShader);
	meshPart->SetShader(shader);

	EffectParameter* diffuseTextureParam = effectShader->GetParameterByName("DiffuseTexture");
	if (oldPass != NULL && oldPass->GetSamplerStateCount() > 0)
	{
		diffuseTextureParam->SetValue(((TexturePtr)oldPass->GetSamplerStateByIndex(0)->GetTexture()).Get());
	}
	if (_isSkinned)
	{
		shader->SetTechnique("Skin");
	}
	else
	{
		shader->SetTechnique("Transform");
	}
	_skinShaders.Add(shader);
}

void SampleViewer::UpdateSkin()
{
	if (_isSkinned)
	{
		for (int i = 0; i < _skeletonShapes.Count(); i++)
		{
			_skeletonShapes[i]->Update();
		}
	}

#ifdef SKINNING_HW
	if (_hasEffect)
	{
		return;
	}

	Skeleton* skeleton = _model->GetSkeleton();
	Matrix4* boneMatrices = NULL;

	for (int i = 0; i < _model->GetMeshCount(); ++i)
	{
		Mesh* mesh = _model->GetMeshByIndex(i);
		Skin* skin = mesh->GetSkin();
		if (_isSkinned && skin != NULL)
		{
			int boneCount = Math::Min(maxBoneCount, skin->SkinVertices.Count());
			boneMatrices = new Matrix4[maxBoneCount];
			for (int j = 0; j < boneCount; ++j)
			{
				Bone* bone = skeleton->GetBoneByIndex(skin->SkinVertices[j].BoneIndex);
				boneMatrices[j] = bone->GetSkinTransform();
				//boneMatrices[j].M0 = matrix.GetRow(0);
				//boneMatrices[j].M1 = matrix.GetRow(1);
				//boneMatrices[j].M2 = matrix.GetRow(2);
			}
		}

		for (int j = 0; j < mesh->GetMeshPartCount(); ++j)
		{
			UpdateSkin_HW(mesh, mesh->GetMeshPart(j)->GetShader(), boneMatrices);
		}

		SE_DELETE_ARRAY(boneMatrices);
	}
#endif
}

void SampleViewer::UpdateSkin(Mesh* mesh, ShaderMaterial* skinShader, Matrix4* boneMatrices)
{
	EffectShader* effect = ((EffectMaterial*)skinShader)->GetEffectShader();
	EffectTechnique* technique = effect->GetTechniqueByIndex(0);
	EffectPass* pass = technique->GetPassByIndex(0);

	Matrix4 world = _modelNode->GetWorldTransform() * _model->GetTransform();
	Matrix4 worldViewProj = world * _camera->GetView() * _camera->GetProjection();
	EffectParameter* worldViewProjParam = effect->GetParameterByName("WorldViewProj");
	worldViewProjParam->SetValue(&worldViewProj);

	if (boneMatrices != NULL)
	{
		EffectParameter* boneMatricesParam = effect->GetParameterByName("BoneMatrices");
		boneMatricesParam->SetValue(boneMatrices);
	}

	Vector3 objCameraPosition = _camera->GetWorldPosition();
	EffectParameter* objCameraPositionParam = effect->GetParameterByName("ObjCameraPosition");
	objCameraPositionParam->SetValue(&objCameraPosition);

	Color32 materialDiffuseColor = Color32::Red;
	EffectParameter* materialDiffuseColorParam = effect->GetParameterByName("MaterialDiffuseColor");
	materialDiffuseColorParam->SetValue(&materialDiffuseColor);

	Color32 materialSpecularColor = Color32::White;
	EffectParameter* materialSpecularColorParam = effect->GetParameterByName("MaterialSpecularColor");
	materialSpecularColorParam->SetValue(&materialSpecularColor);

	Vector3 directionalLightDirection = -Quaternion::Rotate(_camera->GetWorldOrientation(), Vector3::Forward);
	EffectParameter* directionalLightDirectionParam = effect->GetParameterByName("DirectionalLightDirection");
	directionalLightDirectionParam->SetValue(&directionalLightDirection);

	Color32 directionalLightColor = Color32::White;
	EffectParameter* directionalLightColorParam = effect->GetParameterByName("DirectionalLightColor");
	directionalLightColorParam->SetValue(&directionalLightColor);

	Color32 ambientLightColor = Color32::Black;
	EffectParameter* ambientLightColorParam = effect->GetParameterByName("AmbientLightColor");
	ambientLightColorParam->SetValue(&ambientLightColor);
}

void EntryPoint()
{
	Console::WriteLine("Viewer Sample");
	Console::WriteLine("=============");

	try
	{
		SampleViewer::SetCurrent(new SampleViewer());
		Application* theApp = SampleViewer::Current();

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
