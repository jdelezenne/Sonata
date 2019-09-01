/*=============================================================================
XMLReaderTinyXML.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <tinyxml.h>

#include "XMLReader.h"
#include "XML.h"
#include "Core/IO/FileSystem.h"

namespace SonataEngine
{

void XMLReader::ReadNode(XMLNode* node, void* parent)
{
	TiXmlNode* tiNode = (TiXmlNode*)parent;

	if (tiNode->Type() == TiXmlNode::ELEMENT)
	{
		TiXmlElement* tiElement = (TiXmlElement*)tiNode;
		TiXmlAttribute* tiAttribute = tiElement->FirstAttribute();

		while (tiAttribute != NULL)
		{
			((XMLElement*)node)->SetAttribute(tiAttribute->Name(), tiAttribute->Value());

			tiAttribute = tiAttribute->Next();
		}
	}

	if (!tiNode->NoChildren())
	{
		TiXmlNode* tiChild = tiNode->FirstChild();

		while (tiChild != NULL)
		{
			XMLNode* child = NULL;
			switch (tiChild->Type())
			{
			case TiXmlNode::ELEMENT:
				child = node->GetOwnerDocument()->CreateElement(tiChild->Value());
				break;
			case TiXmlNode::TEXT:
				child = node->GetOwnerDocument()->CreateTextNode(tiChild->Value());
				break;
			case TiXmlNode::COMMENT:
				child = node->GetOwnerDocument()->CreateComment(tiChild->Value());
				break;
			}

			if (child != NULL)
			{
				node->AppendChild(child);

				ReadNode(child, tiChild);
			}

			tiChild = tiChild->NextSibling();
		}
	}
}

XMLReader::XMLReader(const String& fileName) :
	_Document(NULL)
{
	TiXmlDocument* tiDocument = new TiXmlDocument();
	if (!tiDocument->LoadFile(FileSystem::Instance()->GetFullPath(fileName).Data()))
	{
		String error = tiDocument->ErrorDesc();
		delete tiDocument;
		throw XMLException(error);
	}

	TiXmlElement* tiElement = tiDocument->RootElement();

	SE_DELETE(_Document);
	_Document = new XMLDocument();

	ReadNode(_Document, tiDocument);

	delete tiDocument;
}

XMLReader::~XMLReader()
{
}

XMLReader* XMLReader::Create(const String& fileName)
{
	return new XMLReader(fileName);
}

}
