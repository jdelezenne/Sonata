/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <Core/Core.h>
#include <Core/Engine.h>
#include <Graphics/Graphics.h>
#include <Input/Input.h>
#include <AI/AI.h>
#include <UI/UI.h>
#include <Plugins.h>

#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

#include <Graphics/Font/Font.h>
#include <Framework/Framework.h>

using namespace SonataEngine;
using namespace SonataEngine::Input;
using namespace Framework;

#endif 
