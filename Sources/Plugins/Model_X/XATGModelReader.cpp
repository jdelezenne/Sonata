/*=============================================================================
XATGModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XATGModelReader.h"

namespace SE_X
{

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("XATGModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("XATGModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("XATGModelReader"), ##message##);

#define MATCH_ELEMENT_NAME(x) (String::Compare(_currentElement->GetName(), x) == 0)
#define MIN_SAFE_VERSION 1.6f

VOID ScrubFloatString(const String& strFloatString)
{
	SEchar* pChar = (SEchar*)strFloatString.Data();
	while (*pChar != L'\0')
	{
		if (*pChar == L'{' || *pChar == L'}' || *pChar == L',' || *pChar == L'\t')
			*pChar = L' ';
		pChar++;
	}
}

XATGModelReader::XATGModelReader() :
	ModelReader()
{
}

XATGModelReader::~XATGModelReader()
{
	Destroy();
}

Model* XATGModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		SE_LOGERROR(_T("No active renderer."));
		return NULL;
	}

	_stream = &stream;
	_fileName = ((FileStream*)_stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);

	if (!ReadModel())
	{
		return NULL;
	}

	Model* model = CreateModel();
	return model;
}

bool XATGModelReader::ReadModel()
{
	XMLReader reader(_fileName);
	SonataEngine::XMLDocument* document = reader.GetDocument();
	if (document == NULL)
	{
		return false;
	}

	_context.CurrentObjectType = XATG_NONE;
	_context.CurrentObject = NULL;

	XMLElement* rootElement = document->GetDocumentElement();
	ParseElement(rootElement);

	return true;
}

void XATGModelReader::ParseElement(XMLElement* element)
{
	_currentElement = element;

	XMLNode* node = _currentElement->GetFirstChild();
	if (node != NULL && node->GetNodeType() == XMLNodeType_Text)
	{
		_currentBody = ((XMLText*)node)->GetValue();
	}
	else
	{
		_currentBody = "";
	}

	HandleElement();

	XMLNode::XMLNodeList nodes = element->GetChildNodes();
	XMLNode::XMLNodeList::Iterator it = nodes.GetIterator();
	while (it.Next())
	{
		XMLNode* node = it.Current();
		if (node->GetNodeType() == XMLNodeType_Element)
		{
			ParseElement((XMLElement*)node);
		}
	}
}

void XATGModelReader::HandleElement()
{
	if (MATCH_ELEMENT_NAME("StaticMesh") || MATCH_ELEMENT_NAME("Mesh"))
	{
		_context.CurrentObjectType = XATG_MESH;
	}
	else if (MATCH_ELEMENT_NAME("Frame"))
	{
		_context.CurrentObjectType = XATG_FRAME;
	}
	else if (MATCH_ELEMENT_NAME("Model"))
	{
		_context.CurrentObjectType = XATG_MODEL;
	}
	else if (MATCH_ELEMENT_NAME("MaterialInstance"))
	{
		_context.CurrentObjectType = XATG_MATERIAL;
	}
	else if (MATCH_ELEMENT_NAME("AmbientLight"))
	{
		_context.CurrentObjectType = XATG_AMBIENTLIGHT;
	}
	else if (MATCH_ELEMENT_NAME("DirectionalLight"))
	{
		_context.CurrentObjectType = XATG_DIRECTIONALLIGHT;
	}
	else if (MATCH_ELEMENT_NAME("PointLight"))
	{
		_context.CurrentObjectType = XATG_POINTLIGHT;
	}
	else if (MATCH_ELEMENT_NAME("SpotLight"))
	{
		_context.CurrentObjectType = XATG_SPOTLIGHT;
	}
	else if (MATCH_ELEMENT_NAME("PerspectiveCamera"))
	{
		_context.CurrentObjectType = XATG_CAMERA;
	}
	else if (MATCH_ELEMENT_NAME("Animation"))
	{
		_context.CurrentObjectType = XATG_ANIMATION;
	}

	ProcessElementData();
}

void XATGModelReader::ProcessElementData()
{
	switch (_context.CurrentObjectType)
	{
	case XATG_NONE:
		ProcessRootData();
		break;
	case XATG_FRAME:
		ProcessFrameData();
		break;
	case XATG_MESH:
	case XATG_VERTEXBUFFER:
	case XATG_INDEXBUFFER:
	case XATG_VERTEXDECLELEMENT:
	case XATG_INDEXBUFFERSUBSET:
	case XATG_SKINNEDMESHINFLUENCES:
		ProcessMeshData();
		break;
	case XATG_MODEL:
		ProcessModelData();
		break;
	case XATG_MATERIAL:
		ProcessMaterialData();
		break;
	case XATG_AMBIENTLIGHT:
	case XATG_DIRECTIONALLIGHT:
	case XATG_POINTLIGHT:
	case XATG_SPOTLIGHT:
		ProcessLightData();
		break;
	case XATG_CAMERA:
		ProcessCameraData();
		break;
	case XATG_ANIMATION:
		ProcessAnimationData();
		break;
	default:
		SE_ASSERT(false);
		break;
	}
}

void XATGModelReader::ProcessRootData()
{
	if (MATCH_ELEMENT_NAME("XFileATG"))
	{
		String strVersion = _currentElement->GetAttribute("Version");
		if (!strVersion.IsEmpty())
		{
			real32 fVersion = strVersion.ToReal32();
			if (fVersion < MIN_SAFE_VERSION)
			{
				SE_LOGERROR("File version is out of date.");
			}
		}
		return;
	}
	else if (MATCH_ELEMENT_NAME("PhysicalMemoryFile"))
	{
		String fileName = Path::Combine(_path, _currentBody);
		File file(fileName);
		FileStreamPtr stream = file.Open(FileMode_Open, FileAccess_Read);
		if (stream == NULL)
		{
			SE_LOGERROR("Could not find physical memory blob file.");
			return;
		}
		uint32 length = stream->GetLength();
		SE_ASSERT(length > 0);
		_binaryBlobData = new SEbyte[length];
		stream->Read(_binaryBlobData, length);
		return;
	}
	else if (MATCH_ELEMENT_NAME("BundledResources"))
	{
		String fileName = Path::Combine(_path, _currentBody);
		//PackedResource* pPackedResource = new PackedResource();
		//pPackedResource->Create(fileName);
		//g_pCurrentScene->GetResourceDatabase()->AddBundledResources(pPackedResource);
		return;
	}
}

void XATGModelReader::ProcessMeshData()
{
}

void XATGModelReader::ProcessFrameData()
{
}

void XATGModelReader::ProcessModelData()
{
}

void XATGModelReader::ProcessMaterialData()
{
}

void XATGModelReader::ProcessLightData()
{
}

void XATGModelReader::ProcessCameraData()
{
}

void XATGModelReader::ProcessAnimationData()
{
}

void XATGModelReader::Destroy()
{
}

Model* XATGModelReader::CreateModel()
{
	Model* model = new Model();
	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	Bone* rootBone = skeleton->AddBone();
	skeleton->SetRootBone(rootBone);

	rootBone->Update();

	return model;
}

}
