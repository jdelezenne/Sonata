/*=============================================================================
SampleUI.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLEUI_H_
#define _SAMPLEUI_H_

#include <Core/Core.h>
#include <Core/Engine.h>
#include <Graphics/Graphics.h>
#include <Input/Input.h>
#include <UI/UI.h>
#include <Plugins.h>

#include <Framework/Framework.h>

using namespace SonataEngine;
using namespace SonataEngine::Input;
using namespace Framework;

#ifdef SE_STATIC
#	include <RenderSystem_GL/GLRenderSystemLib.h>
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#	include <Model_OBJ/OBJModelPlugin.h>
#endif

#endif 
