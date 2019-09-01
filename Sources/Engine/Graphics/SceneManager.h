/*=============================================================================
SceneManager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENEMANAGER_H_
#define _SE_SCENEMANAGER_H_

#include "Graphics/Common.h"
#include "Graphics/Scene/Camera.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Scene/Scene.h"

namespace SonataEngine
{

class ModelNode;
class PointLight;
class DirectionalLight;
class SpotLight;
class ShaderMaterial;
class Texture;

struct SceneState
{
	real64 CurrentTime;
	real64 LastTime;
	real64 ElapsedTime;
	int32 FrameNumber;

	Matrix4 Projection;
	Matrix4 View;
	Matrix4 World;
	Matrix4 ViewProjection;
	Matrix4 WorldView;
	Matrix4 WorldViewProjection;

	bool IsStencilPass;
	bool IsZPass;
	Scene* Scene;
	Camera* Camera;

	Array<ModelNode*> AllModels;
	Array<ModelNode*> VisibleModels;

	Array<PointLight*> AllPointLights;
	Array<DirectionalLight*> AllDirectionalLights;
	Array<SpotLight*> AllSpotLights;

	Array<PointLight*> VisiblePointLights;
	Array<DirectionalLight*> VisibleDirectionalLights;
	Array<SpotLight*> VisibleSpotLights;

	Array<PointLight*> ActivePointLights;
	Array<DirectionalLight*> ActiveDirectionalLights;
	Array<SpotLight*> ActiveSpotLights;
};

/** Base class for the managers. */
class SE_GRAPHICS_EXPORT SceneManager : public Singleton<SceneManager>
{
protected:
	Scene* _scene;
	SceneState _sceneState;
	Camera* _camera;
	ShaderMaterial* _defaultShader;
	bool _frustumCulling;

public:
	/** @name Constructors / Destructor. */
	//@{
	SceneManager();

	virtual ~SceneManager();
	//@}

	/** Properties. */
	//@{
	SceneState* GetSceneState() { return &_sceneState; }

	void InitializeSceneState();
	void ClearSceneState();

	Scene* GetScene() const { return _scene; }
	void SetScene(Scene* value) { _scene = value; }

	Camera* GetCamera() const { return _camera; }
	void SetCamera(Camera* value) { _camera = value; }

	ShaderMaterial* GetDefaultShader() const { return _defaultShader; }
	void SetDefaultShader(ShaderMaterial* value) { _defaultShader = value; }
	//@}

	virtual void Update(const TimeValue& timeValue);

	virtual void Render();

	/** Renders the specified mesh. */
	virtual void RenderMesh(Mesh* mesh, Matrix4 world = Matrix4::Identity);

protected:
	void BuildVisibilityList();
	void RenderZPass();
	void RenderScene();
	void RenderShadowMap(SpotLight* spotLight, Texture* destTexture, const Matrix4& lightViewProjection);
	void RenderPostEffects();
};

}

#endif 
