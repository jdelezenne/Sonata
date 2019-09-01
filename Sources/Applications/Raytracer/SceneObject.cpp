/*=============================================================================
RTSceneObject.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SceneObject.h"
#include "Shader.h"

namespace Raytracer
{
	TraceResult::TraceResult() :
		_Hit(false),
		_Distance(0.0f)
	{
	}


	SE_IMPLEMENT_ABSTRACT(RTSceneObject);
	SE_IMPLEMENT_REFLECTION(RTSceneObject);

	RTSceneObject::RTSceneObject() :
		RefObject(),
		_Shader(NULL)
	{
	}

	RTSceneObject::~RTSceneObject()
	{
	}

	void RTSceneObject::OnSerialized(XMLSerializer* context, XMLElement* element)
	{
		super::OnSerialized(context, element);

		SonataEngine::XMLDocument* document = element->GetOwnerDocument();

		if (_Shader != NULL)
		{
			XMLElement* item = document->CreateElement("Shader");
			context->Serialize(document, item, _Shader, _Shader->GetType());
			element->AppendChild(item);
		}
	}

	void RTSceneObject::OnDeserialized(XMLSerializer* context, XMLElement* element)
	{
		super::OnDeserialized(context, element);

		SonataEngine::XMLDocument* document = element->GetOwnerDocument();

		XMLElement* item = (XMLElement*)element->GetItem("Shader");
		if (item != NULL)
		{
			XMLElement* type = (XMLElement*)item->GetFirstChild();
			_Shader = (RTShader*)context->Deserialize(document, type);
		}
	}
}
