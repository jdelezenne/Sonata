/*=============================================================================
SceneWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENEWRITER_H_
#define _SE_SCENEWRITER_H_

#include "Core/IO/Stream.h"
#include "Graphics/Scene/Scene.h"

namespace SonataEngine
{

/** Scene writer options. */
struct SceneWriterOptions
{
};

/** Scene writer. */
class SE_GRAPHICS_EXPORT SceneWriter
{
public:
	virtual bool SaveScene(Stream& stream, Scene* scene, SceneWriterOptions* options = NULL) = 0;
};

}

#endif 
