/*=============================================================================
SampleTerrain.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLETERRAIN_H_
#define _SAMPLETERRAIN_H_

#include "Common.h"

#ifdef SE_STATIC
#	include <RenderSystem_GL/GLRenderSystemLib.h>
//#	include <RenderSystem_D3D8/D3D8RenderSystemLib.h>
#	include <RenderSystem_D3D9/D3D9RenderSystemLib.h>

#	include <ShaderSystem_GLCg/GLCgShaderSystemLib.h>
//#	include <ShaderSystem_D3D8Cg/D3D8CgShaderSystemLib.h>
#	include <ShaderSystem_D3D9Cg/D3D9CgShaderSystemLib.h>

#	include <InputSystem_DI8/DI8InputSystemLib.h>

#	include <Image_DevIL/DevILImagePlugin.h>
#	include <Model_OBJ/OBJModelPlugin.h>
#	include <Game_Westwood/WestwoodGamePlugin.h>

class UI::CheckBoxButton checkboxbutton;
class UI::StaticImage staticimage;
class UI::Label label;
class UI::Panel panel;
class UI::ProgressBar progressbar;
class UI::PropertyGrid propertygrid;
class UI::PushButton pushbutton;
class UI::RadioButton radiobutton;
class UI::Slider slider;
class UI::TextBox textbox;
class UI::Theme theme;
class UI::Window window;
#endif

#endif 
