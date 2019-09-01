/*=============================================================================
XMLReaderXerces.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "XMLReader.h"
#include "XML.h"

namespace SonataEngine
{

void XMLReader::ReadNode(XMLNode* node, void* parent)
{
	xercesc::DOMNode* domNode = (xercesc::DOMNode*) parent;

	if (domNode->hasChildNodes())
	{
		xercesc::DOMNodeList* domNodeList = domNode->getChildNodes();
		int count = domNodeList->getLength();
		for (int i = 0; i < count; i++)
		{
			xercesc::DOMNode* domChild = domNodeList->item(i);

			String name = xercesc::XMLString::transcode(domChild->getNodeName());
			String value = xercesc::XMLString::transcode(domChild->getNodeValue());

			if (domChild->getNodeType() == xercesc::DOMNode::ATTRIBUTE_NODE)
			{
				((XMLElement*)node)->SetAttribute(name, value);
			}
			else
			{
				XMLNode* child = NULL;
				switch (domChild->getNodeType())
				{
				case xercesc::DOMNode::ELEMENT_NODE:
					child = node->GetOwnerDocument()->CreateElement(name);
					break;
				case xercesc::DOMNode::TEXT_NODE:
					child = node->GetOwnerDocument()->CreateTextNode(value);
					break;
				case xercesc::DOMNode::COMMENT_NODE:
					child = node->GetOwnerDocument()->CreateComment(value);
					break;
				}

				if (child != NULL)
				{
					node->AppendChild(child);

					ReadNode(child, domChild);
				}
			}
		}
	}
}

XMLReader::XMLReader(const String& fileName)
{
	try
	{
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& e)
	{
		char* message = xercesc::XMLString::transcode(e.getMessage());
		String msg = message;
		xercesc::XMLString::release(&message);
		throw XMLException(_T("<xerces> ") + msg);
	}

	xercesc::XercesDOMParser* parser = new xercesc::XercesDOMParser();
	xercesc::ErrorHandler* errHandler = (xercesc::ErrorHandler*) new xercesc::HandlerBase();
	parser->setErrorHandler(errHandler);
	parser->setCreateEntityReferenceNodes(false);

	try
	{
		parser->parse(fileName.Data());
	}
	catch (const xercesc::XMLException& e)
	{
		char* message = xercesc::XMLString::transcode(e.getMessage());
		String msg = message;
		xercesc::XMLString::release(&message);
		xercesc::XMLPlatformUtils::Terminate();
		throw XMLException(_T("<xerces> ") + msg);
	}
	catch (const xercesc::DOMException& e)
	{
		char* message = xercesc::XMLString::transcode(e.msg);
		String msg = message;
		xercesc::XMLString::release(&message);
		xercesc::XMLPlatformUtils::Terminate();
		throw XMLException(_T("<xerces> ") + msg);
	}
	catch (...)
	{
		xercesc::XMLPlatformUtils::Terminate();
		throw XMLException(_T("<xerces>"));
	}

	xercesc::DOMDocument* domDocument = parser->getDocument();
	xercesc::DOMElement* domElement = domDocument->getDocumentElement();

	SE_DELETE(_Document);
	_Document = new XMLDocument();
	XMLElement* element = new XMLElement();

	ReadNode(_Document, domDocument);

	XMLNode* node;
	foreach (node, _Document->_ChildNodes, XMLNode::XMLNodeList)
	{
		if (node->GetNodeType() == XMLNodeType_Element)
		{
			_Document->_DocumentElement = (XMLElement*)node;
			break;
		}
	}

	delete parser;
	delete errHandler;

	xercesc::XMLPlatformUtils::Terminate();
}

XMLReader::~XMLReader()
{
}

XMLReader* XMLReader::Create(const String& fileName)
{
	return new XMLReader(fileName);
}

}
