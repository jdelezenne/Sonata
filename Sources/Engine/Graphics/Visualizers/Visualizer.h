/*=============================================================================
Visualizer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VISUALIZER_H_
#define _SE_VISUALIZER_H_

#include "Graphics/Scene/SceneObject.h"

namespace SonataEngine
{

/**
 * Visualizer Mesh.
 */
class Visualizer : public SceneObject
{
public:
	Visualizer();
	virtual ~Visualizer();

	virtual void Render();

protected:
};

}

#endif 
