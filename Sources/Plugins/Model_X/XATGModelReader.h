/*=============================================================================
XATGModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XATGMODELREADER_H_
#define _SE_XATGMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_X
{

#define VOID void
typedef uint32 DWORD;

enum XATGObjectLoaderType
{
    XATG_NONE = 0,
    XATG_MESH,
    XATG_SKINNEDMESHINFLUENCES,
    XATG_VERTEXBUFFER,
    XATG_INDEXBUFFER,
    XATG_VERTEXDECLELEMENT,
    XATG_INDEXBUFFERSUBSET,
    XATG_FRAME,
    XATG_MODEL,
    XATG_MATERIAL,
    XATG_AMBIENTLIGHT,
    XATG_DIRECTIONALLIGHT,
    XATG_POINTLIGHT,
    XATG_SPOTLIGHT,
    XATG_CAMERA,
    XATG_ANIMATION,
};

class XATGParserContext
{
public:
	XATGObjectLoaderType    CurrentObjectType;
	VOID*                   CurrentObject;
	VOID*                   UserData;
	DWORD                   UserDataIndex;
	Bone*                   CurrentParentFrame;
	VOID*                   CurrentParentObject;
	DWORD                   CurrentParameterIndex;

	XATGParserContext() :
		CurrentObjectType(XATG_NONE),
		CurrentObject(NULL),
		UserData(NULL),
		CurrentParentFrame(NULL),
		CurrentParentObject(NULL)
	{
	}
};

/**
	Xbox Advanced Technology Group Model Reader.
*/
class XATGModelReader : public ModelReader
{
protected:
	Stream* _stream;
	String _fileName;
	String _path;
	XMLElement* _currentElement;
	String _currentBody;
	SEbyte* _binaryBlobData;
	XATGParserContext _context;

	Scene* _currentScene;
	Bone* _rootFrame;

public:
	XATGModelReader();
	virtual ~XATGModelReader();

	virtual Model* LoadModel(Stream& source, ModelReaderOptions* options = NULL);

protected:
	bool ReadModel();
	void ParseElement(XMLElement* element);
	void HandleElement();
	void ProcessElementData();
	void ProcessRootData();
	void ProcessMeshData();
	void ProcessFrameData();
	void ProcessModelData();
	void ProcessMaterialData();
	void ProcessLightData();
	void ProcessCameraData();
	void ProcessAnimationData();

	void Destroy();

	Model* CreateModel();
};

}

#endif 
