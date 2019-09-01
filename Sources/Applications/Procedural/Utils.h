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

Font* CreateFont(const String& fileName);

#endif 
