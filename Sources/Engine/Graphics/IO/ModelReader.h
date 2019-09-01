/*=============================================================================
ModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MODELREADER_H_
#define _SE_MODELREADER_H_

#include "Core/IO/Stream.h"
#include "Graphics/Model/Model.h"
#include "Graphics/System/HardwareBuffer.h"

namespace SonataEngine
{

enum ModelOptionsFlag
{
	ModelOptions_VertexUsage = (1<<0),
	ModelOptions_IndexUsage = (1<<1),
	ModelOptions_NoShader = (1<<2)
};

/** Model reader options. */
struct ModelReaderOptions
{
	HardwareBufferUsage VertexUsage;
	HardwareBufferUsage IndexUsage;
};

/** Model reader. */
class SE_GRAPHICS_EXPORT ModelReader
{
public:
	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL) = 0;
};

}

#endif 
