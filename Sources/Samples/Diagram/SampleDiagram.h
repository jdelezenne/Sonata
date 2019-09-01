/*=============================================================================
SampleDiagram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLEDIAGRAM_H_
#define _SAMPLEDIAGRAM_H_

#include <Core/Core.h>
#include <Core/Engine.h>
#include <Graphics/Graphics.h>
#include <Input/Input.h>
#include <UI/UI.h>
#include <EntryPoint.h>
#include <Plugins.h>

#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

using namespace SonataEngine;
using namespace SonataEngine::Input;

#ifdef SE_STATIC
#	include <RenderSystem_GL/GLRenderSystemLib.h>
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#endif

#endif 
