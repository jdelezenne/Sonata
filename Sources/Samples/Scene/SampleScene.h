/*=============================================================================
SampleScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLESCENE_H_
#define _SAMPLESCENE_H_

#include <EntryPoint.h>
#include "Common.h"
#include "SampleScene.h"

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


class SampleScene : public SceneApplication
{
public:
	SampleScene();
	virtual ~SampleScene();

	virtual bool CreateScene();
	virtual bool CreateCamera();

	virtual bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();
	virtual void OnDraw();

	virtual void UpdateCamera(real64 elapsed);

protected:
	FirstPersonCameraController* _cameraController;
	Array<Camera*> _cameras;
	int _currentCamera;
};

#endif 
