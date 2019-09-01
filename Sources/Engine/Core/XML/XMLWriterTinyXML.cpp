/*=============================================================================
XMLWriterTinyXML.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <tinyxml.h>

#include "XMLWriter.h"
#include "XML.h"
#include "Core/IO/FileSystem.h"

namespace SonataEngine
{

void XMLWriter::WriteNode(XMLNode* node, void* parent)
{
	TiXmlNode* tiNode = (TiXmlNode*)parent;

	const XMLNode::XMLNodeList& nodes = node->GetChildNodes();
	XMLNode* child;
	foreach (child, nodes, XMLNode::XMLNodeList)
	{
		switch (child->GetNodeType())
		{
		case XMLNodeType_Element:
			{
			TiXmlElement tiChild(child->GetName().Data());

			const XMLNode::XMLAttributeList& attributes = child->GetAttributes();
			XMLAttribute* attribute;
			foreach (attribute, attributes, XMLNode::XMLAttributeList)
			{
				tiChild.SetAttribute(attribute->GetName().Data(), attribute->GetValue().Data());
			}

			WriteNode(child, &tiChild);
			tiNode->InsertEndChild(tiChild);
			}
			break;
		case XMLNodeType_Text:
			{
			TiXmlText tiChild(child->GetValue().Data());
			WriteNode(child, &tiChild);
			tiNode->InsertEndChild(tiChild);
			}
			break;
		case XMLNodeType_Comment:
			{
			TiXmlComment* tiChild = new TiXmlComment();
			tiChild->SetValue(child->GetValue().Data());
			WriteNode(child, &tiChild);
			tiNode->InsertEndChild(*tiChild);
			}
			break;
		}
	}
}

XMLWriter::XMLWriter(XMLDocument* document, const String& fileName)
{
	TiXmlDocument* tiDocument = new TiXmlDocument();

	WriteNode(document, tiDocument);

	if (!tiDocument->SaveFile(FileSystem::Instance()->GetFullPath(fileName).Data()))
	{
		delete tiDocument;
		throw XMLException();
	}

	delete tiDocument;
}

XMLWriter::~XMLWriter()
{
}

XMLWriter* XMLWriter::Create(XMLDocument* document, const String& fileName)
{
	return new XMLWriter(document, fileName);
}

}
