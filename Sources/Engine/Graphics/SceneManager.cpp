/*=============================================================================
SceneManager.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SceneManager.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/Shader/ShaderSystem.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Model/Bone.h"
#include "Graphics/Scene/ModelNode.h"
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/SpotLight.h"
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

struct ModelDistance
{
	ModelNode* Model;
	real DistanceFromCamera;
};

real DistanceFromCamera(const Vector3& cameraPos, const Vector3& modelPos, real maxRadius)
{
	real distance = (modelPos - cameraPos).Length() - maxRadius;
	return Math::Abs(distance);
}

typedef BaseArray<ModelDistance> ModelDistanceList;

bool ModelSortFunction(const ModelDistance& left, const ModelDistance& right)
{
    return left.DistanceFromCamera < right.DistanceFromCamera;
}

SceneManager::SceneManager() :
	_scene(NULL),
	_camera(NULL),
	_frustumCulling(true)
{
	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->RasterizerState.FillMode = FillMode_WireFrame;
	pass->LightState.Lighting = true;
	_defaultShader = shader;

	InitializeSceneState();
}

SceneManager::~SceneManager()
{
}

void SceneManager::InitializeSceneState()
{
	_sceneState.CurrentTime = 0.0;
	_sceneState.LastTime = 0.0;
	_sceneState.ElapsedTime = 0.0;
	_sceneState.FrameNumber = 0;

	_sceneState.Projection = Matrix4::Identity;
	_sceneState.View = Matrix4::Identity;
	_sceneState.ViewProjection = Matrix4::Identity;
	_sceneState.World = Matrix4::Identity;
	_sceneState.WorldView = Matrix4::Identity;
	_sceneState.WorldViewProjection = Matrix4::Identity;

	ClearSceneState();
}

void SceneManager::ClearSceneState()
{
	_sceneState.Scene = NULL;
	_sceneState.Camera = NULL;
	_sceneState.AllModels.Clear();
	_sceneState.VisibleModels.Clear();
	_sceneState.AllPointLights.Clear();
	_sceneState.AllDirectionalLights.Clear();
	_sceneState.AllSpotLights.Clear();
	_sceneState.VisiblePointLights.Clear();
	_sceneState.VisibleDirectionalLights.Clear();
	_sceneState.VisibleSpotLights.Clear();
	_sceneState.ActivePointLights.Clear();
	_sceneState.ActiveDirectionalLights.Clear();
	_sceneState.ActiveSpotLights.Clear();
}

void SceneManager::Update(const TimeValue& timeValue)
{
	_sceneState.ElapsedTime = (real64)timeValue - _sceneState.CurrentTime;
	_sceneState.LastTime = _sceneState.CurrentTime;
	_sceneState.CurrentTime = timeValue;
	_sceneState.FrameNumber++;

    if (_scene != NULL)
    {
		_scene->Update(timeValue);
	}
}

void SceneManager::Render()
{
	ClearSceneState();

    if (_scene == NULL || _camera == NULL)
    {
		return;
	}

	_sceneState.Scene = _scene;
	_sceneState.Camera = _camera;
	_sceneState.Projection = _camera->GetProjection();
	_sceneState.View = _camera->GetView();
	_sceneState.ViewProjection = _sceneState.Projection * _sceneState.View;

	Scene::SceneObjectList::Iterator it = _scene->GetSceneObjectIterator();
	while (it.Next())
	{
		SceneObject* obj = it.Current();
		if (is(obj, ModelNode))
		{
			_sceneState.AllModels.Add((ModelNode*)obj);
		}
		else if (is(obj, PointLight))
		{
			_sceneState.AllPointLights.Add((PointLight*)obj);
		}
		else if (is(obj, DirectionalLight))
		{
			_sceneState.AllDirectionalLights.Add((DirectionalLight*)obj);
		}
		else if (is(obj, SpotLight))
		{
			_sceneState.AllSpotLights.Add((SpotLight*)obj);
		}
	}

	_scene->Render();

	RenderZPass();

	BuildVisibilityList();

	RenderScene();

	RenderPostEffects();
}

void SceneManager::RenderMesh(Mesh* mesh, Matrix4 world)
{
	_sceneState.World = world;
	_sceneState.WorldView = _sceneState.View * _sceneState.World;
	_sceneState.WorldViewProjection = _sceneState.ViewProjection * _sceneState.World;

	RenderSystem* renderSystem = RenderSystem::Current();
	RenderData renderData;

#if 0
	int meshPartCount = mesh->GetMeshPartCount();
	static Dictionary< ShaderMaterial*, Array<MeshPart*> > meshPartsByShaders;

	ShaderMaterial* lastShader = NULL;
	Mesh::MeshPartList::Iterator it = mesh->GetMeshPartIterator();
	while (it.Next())
	{
		MeshPart* meshPart = it.Current();
		ShaderMaterial* currentShader = meshPart->GetShader();
		if (currentShader == NULL)
		{
			if (_defaultShader != NULL)
				currentShader = _defaultShader;
			else
				continue;
		}

		if (currentShader != lastShader)
		{
			Array<MeshPart*> meshParts;
			meshParts.SetCapacity(meshPartCount);
			meshPartsByShaders.Add(currentShader, meshParts);
		}

		Array<MeshPart*>& meshParts = meshPartsByShaders.GetItem(currentShader);
		meshParts.Add(meshPart);

		lastShader = currentShader;
	}

	BaseArray<ShaderMaterial*> shaders = meshPartsByShaders.Keys();
	int shaderCount = shaders.Count();
	for (int i = 0; i < shaderCount; ++i)
	{
		ShaderMaterial* shader = shaders[i];
		shader->SetupMaterial(&_sceneState);
		shader->BeginMaterial();

		const Array<MeshPart*>& meshParts = meshPartsByShaders.GetItem(shader);
		int meshPartCount = meshParts.Count();

		int passCount = shader->GetPassCount();
		for (int j = 0; j < passCount; ++j)
		{
			shader->BeginPass(j);

			for (int k = 0; k < meshPartCount; ++k)
			{
				MeshPart* meshPart = meshParts[k];
				shader->SetupPass(&_sceneState, meshPart);
				shader->SetupGeometry(meshPart);

				meshPart->GetRenderData(renderData);
				renderSystem->Render(&renderData);
			}

			shader->EndPass();
		}

		shader->EndMaterial();
	}
#elif 0
	int meshPartCount = mesh->GetMeshPartCount();

	Array<MeshPart*> meshParts;
	meshParts.SetCapacity(meshPartCount);

	ShaderMaterial* lastShader = NULL;
	for (int i = 0; i < meshPartCount + 1; ++i)
	{
		MeshPart* current = NULL;
		ShaderMaterial* currentShader = NULL;
		if (i < meshPartCount)
		{
			current = mesh->GetMeshPart(i);
			currentShader = current->GetShader();
			if (currentShader == NULL)
			{
				if (_defaultShader != NULL)
					currentShader = _defaultShader;
				else
					continue;
			}

			if (lastShader == NULL || currentShader == lastShader)
			{
				meshParts.Add(current);

				if (lastShader == NULL)
					lastShader = currentShader;

				if (i < meshPartCount - 1)
					continue;
			}
		}

		if (lastShader == NULL)
			continue;

		ShaderMaterial* shader = lastShader;

		shader->SetupMaterial(&_sceneState);
		shader->BeginMaterial();

		int meshPartByShaderCount = meshParts.Count();

		int passCount = shader->GetPassCount();
		for (int j = 0; j < passCount; ++j)
		{
			shader->BeginPass(j);

			for (int k = 0; k < meshPartByShaderCount; ++k)
			{
				MeshPart* meshPart = meshParts[k];
				shader->SetupPass(&_sceneState, meshPart);
				shader->SetupGeometry(meshPart);

				meshPart->GetRenderData(renderData);
				renderSystem->Render(&renderData);
			}

			shader->EndPass();
		}

		shader->EndMaterial();

		meshParts.Clear();
		meshParts.Add(current);
		lastShader = currentShader;
	}
#else
	int meshPartCount = mesh->GetMeshPartCount();
	int shaderCount = mesh->GetShaderCount();

	for (int i = 0; i < shaderCount; ++i)
	{
		ShaderMaterial* shader = mesh->GetShader(i);

		shader->SetupMaterial(&_sceneState);
		shader->BeginMaterial();

		int passCount = shader->GetPassCount();
		for (int j = 0; j < passCount; ++j)
		{
			shader->BeginPass(j);
			shader->SetupPass(&_sceneState, NULL);

			for (int k = 0; k < meshPartCount; ++k)
			{
				MeshPart* meshPart = mesh->GetMeshPart(k);
			}
			for (int k = 0; k < meshPartCount; ++k)
			{
				MeshPart* meshPart = mesh->GetMeshPart(k);
				if (meshPart->GetShader() != shader)
					continue;

				meshPart->GetRenderData(renderData);
				renderSystem->Render(&renderData);
			}

			shader->EndPass();
		}

		shader->EndMaterial();
	}
#endif
}

void SceneManager::BuildVisibilityList()
{
	int i;
	const Frustum& viewFrustum = _camera->GetFrustum();

	int modelCount = _sceneState.AllModels.Count();

	ModelDistanceList modelSortList;
	modelSortList.SetCapacity(modelCount);

	ModelDistance md;
	for (i = 0; i < modelCount; ++i)
	{
		ModelNode* model = _sceneState.AllModels[i];

		if (!model->IsVisible())
		{
			continue;
		}

		const BoundingSphere& worldBound = model->GetWorldBoundingSphere();
		const Vector3 worldPosition = model->GetWorldPosition();

		Vector3 cameraPos = _camera->GetWorldPosition();

		md.Model = model;
		md.DistanceFromCamera = DistanceFromCamera(cameraPos, worldPosition, worldBound.Radius);

		if (_frustumCulling)
		{
			if (viewFrustum.Intersects(worldBound))
			{
				modelSortList.Add(md);
			}
		}
		else
		{
			modelSortList.Add(md);
		}
	}

	modelSortList.Sort(ModelSortFunction);

	int visibleModelCount = modelSortList.Count();
	for (i = 0; i < visibleModelCount; ++i)
	{
		_sceneState.VisibleModels.Add(modelSortList[i].Model);
	}

	int pointLightCount = _sceneState.AllPointLights.Count();
	int spotLightCount = _sceneState.AllSpotLights.Count();
	int directionalLightCount = _sceneState.AllDirectionalLights.Count();

	for (i = 0; i < pointLightCount; ++i)
	{
		PointLight* pointLight = _sceneState.AllPointLights[i];
		if (!pointLight->IsEnabled())
		{
			continue;
		}

		const BoundingSphere& lightBound = pointLight->GetWorldBoundingSphere();
		if (!viewFrustum.Intersects(lightBound))
		{
			continue;
		}

		_sceneState.VisiblePointLights.Add(pointLight);
	}

	for (i = 0; i < spotLightCount; ++i)
	{
		SpotLight* spotLight = _sceneState.AllSpotLights[i];
		if (!spotLight->IsEnabled())
		{
			continue;
		}

		const Frustum& lightBound = spotLight->GetLightFrustum();
		if (!viewFrustum.Intersects(lightBound))
		{
			continue;
		}

		_sceneState.VisibleSpotLights.Add(spotLight);
	}

	for (i = 0; i < directionalLightCount; ++i)
	{
		DirectionalLight* directionalLight = _sceneState.AllDirectionalLights[i];
		if (!directionalLight->IsEnabled())
		{
			continue;
		}

		_sceneState.VisibleDirectionalLights.Add(directionalLight);
	}
}

void SceneManager::RenderZPass()
{
}

void SceneManager::RenderScene()
{
	int i, j;
	const Frustum& viewFrustum = _camera->GetFrustum();
	int visibleModelCount = _sceneState.VisibleModels.Count();
	int pointLightCount = _sceneState.VisiblePointLights.Count();
	int spotLightCount = _sceneState.VisibleSpotLights.Count();
	int directionalLightCount = _sceneState.VisibleDirectionalLights.Count();

	for (i = 0; i < visibleModelCount; ++i)
	{
		ModelNode* model = _sceneState.VisibleModels[i];
		const BoundingSphere& worldBound = model->GetWorldBoundingSphere();

		_sceneState.ActivePointLights.Clear();
		_sceneState.ActiveDirectionalLights.Clear();
		_sceneState.ActiveSpotLights.Clear();

		for (j = 0; j < pointLightCount; ++j)
		{
			PointLight* pointLight = _sceneState.VisiblePointLights[j];
			const BoundingSphere& lightBound = pointLight->GetWorldBoundingSphere();
			if (!lightBound.Intersects(worldBound))
			{
				continue;
			}

			_sceneState.ActivePointLights.Add(pointLight);
		}

		for (j = 0; j < spotLightCount; ++j)
		{
			SpotLight* spotLight = _sceneState.VisibleSpotLights[j];
			const Frustum& lightBound = spotLight->GetLightFrustum();
			if (!lightBound.Intersects(worldBound))
			{
				continue;
			}

			_sceneState.ActiveSpotLights.Add(spotLight);
		}

		for (j = 0; j < directionalLightCount; ++j)
		{
			DirectionalLight* directionalLight = _sceneState.VisibleDirectionalLights[j];
			_sceneState.ActiveDirectionalLights.Add(directionalLight);
		}

		Model::MeshList::Iterator it = model->GetModel()->GetMeshIterator();
		while (it.Next())
		{
			Mesh* mesh = it.Current();
			Matrix4 transform = model->GetModelTransform();
			if (mesh->GetParentBone() != NULL)
			{
				transform = transform * mesh->GetParentBone()->GetGlobalTransform();
			}
			RenderMesh(mesh, transform);
		}
	}
}

void SceneManager::RenderShadowMap(SpotLight* spotLight, Texture* destTexture, const Matrix4& lightViewProjection)
{
}

void SceneManager::RenderPostEffects()
{
}

}
