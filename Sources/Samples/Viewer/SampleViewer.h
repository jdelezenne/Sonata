/*=============================================================================
SampleViewer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLEVIEWER_H_
#define _SAMPLEVIEWER_H_

#include <EntryPoint.h>
#include "Common.h"
#include "SampleViewer.h"

#ifdef SE_STATIC
//#	include <RenderSystem_GL/GLRenderSystemLib.h>
//#	include <RenderSystem_D3D8/D3D8RenderSystemLib.h>
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

//#	include <ShaderSystem_GLCg/GLCgShaderSystemLib.h>
//#	include <ShaderSystem_D3D8Cg/D3D8CgShaderSystemLib.h>
#	include <ShaderSystem_D3D9Cg/D3D9CgShaderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>

#	include <Model_3DS/3DSModelPlugin.h>
#	include <Model_OBJ/OBJModelPlugin.h>
#	include <Model_X/XModelPlugin.h>

#	include <Game_Quake/QuakeGamePlugin.h>
#	include <Game_Unreal/UnrealGamePlugin.h>
#	include <Game_Westwood/WestwoodGamePlugin.h>
#endif


struct Pose3x4
{
	Vector4 M0;
	Vector4 M1;
	Vector4 M2;
};

class SampleViewer : public SceneApplication
{
public:
	SampleViewer();
	virtual ~SampleViewer();

	virtual bool CreateScene();
	virtual bool CreateCamera();

	virtual bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();
	virtual void OnDraw();

	void GetUnrealModel(const String& model, real scale, const String& body, const String& skin, const String& hair);
	virtual void UpdateCamera(real64 elapsed);

	void ApplyShader(Model* model, const String& shaderName);
	void ApplyShader(Mesh* mesh, const String& shaderName);
	void UpdateAnimationSet(AnimationSet* animationSet, int& sequenceIndex);

	void InitSkin();
	void InitSkin(MeshPart* meshPart);
	void UpdateSkin();
	void UpdateSkin(Mesh* mesh, ShaderMaterial* skinShader, Matrix4* boneMatrices);

protected:
	OrbitCameraController* _cameraController;
	bool _hasEffect;
	bool _isSkinned;
	int32 _skinIndex;
	int32 _morphIndex;
	Model* _model;
	ModelNode* _modelNode;
	ViewAxisShape _viewAxisShape;
	AxisShape _axisShape;
	GridShape _gridShape;
	BaseArray<SkeletonShape*> _skeletonShapes;
	BaseArray<ShaderMaterial*> _skinShaders;
	BaseList<EffectShader*> _shaders;
};

#endif 
