/*=============================================================================
QuakeBSPSceneReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEBSPSCENEREADER_H_
#define _SE_QUAKEBSPSCENEREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

#include "QuakeBSPScene.h"

namespace SE_Quake
{

class BSPFile;

/**
	Quake 3 BSP Maps.

	Resources:
		- Rendering Quake 3 Maps
			Morgan McGuire
			http://graphics.cs.brown.edu/games/quake/quake3.html
		- Quake 3 BSP Collision Detection
			Nathan Ostgard
			http://www.devmaster.net/articles/quake3collision
*/
class QuakeBSPSceneReader : public SceneReader
{
protected:
	String _fileName;
	String _path;
	Stream* _stream;
	BSPFile* _bspFile;

public:
	QuakeBSPSceneReader();
	virtual ~QuakeBSPSceneReader();

	virtual Scene* LoadScene(Stream& stream, SceneReaderOptions* options = NULL);

protected:
	void Destroy();
	bool ReadBSP();
	Scene* CreateScene();
};

}

#endif 
