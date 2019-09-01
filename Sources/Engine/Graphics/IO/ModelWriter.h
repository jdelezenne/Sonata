/*=============================================================================
ModelWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MODELWRITER_H_
#define _SE_MODELWRITER_H_

#include "Core/IO/Stream.h"
#include "Graphics/Model/Model.h"

namespace SonataEngine
{

/** Model writer options. */
struct ModelWriterOptions
{
};

/** Model writer. */
class SE_GRAPHICS_EXPORT ModelWriter
{
public:
	virtual bool SaveModel(Stream& stream, Model* model, ModelWriterOptions* options = NULL) = 0;
};

}

#endif 
