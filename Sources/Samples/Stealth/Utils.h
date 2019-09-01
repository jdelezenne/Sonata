/*=============================================================================
Utils.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include "Common.h"

#define IsKeyPushed(key) \
	(keyboard->IsKeyDown(key) && keyboard->IsKeyToggled(key))

Image* CreateImage(const String& fileName);

Texture* CreateTexture(const String& fileName);

Font* CreateFont(const String& fileName);;

Sprite* CreateSprite(const String& fileName);

Model* CreateModel(const String& fileName);

ShaderProgram* CreateVP(const String& fileName, const String& entryPoint);

ShaderProgram* CreatePP(const String& fileName, const String& entryPoint);

ShaderMaterial* CreateHWShader(const String& vertex, const String& pixel, const String& entryPoint);

#endif 
