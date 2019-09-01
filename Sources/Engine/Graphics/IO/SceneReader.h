/*=============================================================================
SceneReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENEREADER_H_
#define _SE_SCENEREADER_H_

#include "Core/IO/Stream.h"
#include "Graphics/Scene/Scene.h"
#include "Graphics/System/HardwareBuffer.h"

namespace SonataEngine
{

/** Scene reader options. */
struct SceneReaderOptions
{
};

/** Scene reader. */
class SE_GRAPHICS_EXPORT SceneReader
{
public:
	virtual Scene* LoadScene(Stream& stream, SceneReaderOptions* options = NULL) = 0;
};

}

#endif 
