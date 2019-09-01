/*=============================================================================
XMLWriterXerces.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>

#include "XMLWriter.h"
#include "XML.h"
#include "Core/String.h"

/// From Xerces Sample 'CreateDOMDocument.cpp'
class XStr
{
public :
	XStr(const char* toTranscode)
	{
		// Call the private transcoding method
		fUnicodeForm = xercesc::XMLString::transcode(toTranscode);
	}

	~XStr()
	{
		xercesc::XMLString::release(&fUnicodeForm);
	}

	const XMLCh* unicodeForm() const
	{
		return fUnicodeForm;
	}

private :
	XMLCh* fUnicodeForm;
};

#define MakeXStr(str) XStr(str.Data()).unicodeForm()

namespace SonataEngine
{

void XMLWriter::WriteNode(XMLNode* node, void* parent)
{
	xercesc::DOMNode* domNode = (xercesc::DOMNode*) parent;
	xercesc::DOMDocument* domDocument = domNode->getOwnerDocument();

	if (node->GetNodeType() == XMLNodeType_Element)
	{
		xercesc::DOMElement* domElement = domDocument->createElement(MakeXStr(node->GetName()));

		XMLAttribute* attribute;
		foreach (attribute, node->GetAttributes(), XMLNode::XMLAttributeList)
		{
			domElement->setAttribute(MakeXStr(attribute->GetName()), MakeXStr(attribute->GetValue()));
		}
	}

	XMLNode* child;
	foreach (child, node->GetChildNodes(), XMLNode::XMLNodeList)
	{
		xercesc::DOMNode* domChild = NULL;
		switch (child->GetNodeType())
		{
		case XMLNodeType_Element:
			domChild = domDocument->createElement(MakeXStr(child->GetName()));
			break;
		case XMLNodeType_Text:
			domChild = domDocument->createTextNode(MakeXStr(child->GetValue()));
			break;
		case XMLNodeType_Comment:
			domChild = domDocument->createComment(MakeXStr(child->GetValue()));
			break;
		}

		if (domChild != NULL)
		{
			domNode->appendChild(domChild);
			WriteNode(child, domChild);
		}
	}
}

XMLWriter::XMLWriter(XMLDocument* document, const String& fileName)
{
	XMLElement* element = document->GetDocumentElement();
	if (element == NULL)
		return;

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

	try
	{
		xercesc::DOMImplementation* impl =
			xercesc::DOMImplementationRegistry::getDOMImplementation(MakeXStr(((String)_T("LS"))));
		xercesc::DOMWriter* theSerializer = ((xercesc::DOMImplementationLS*)impl)->createDOMWriter();

		xercesc::DOMDocument* domDocument =
			impl->createDocument(0, MakeXStr(element->GetName()), 0);

		WriteNode(element, domDocument->getDocumentElement());

		xercesc::XMLFormatTarget* formTarget = new xercesc::LocalFileFormatTarget(fileName.Data());

		theSerializer->writeNode(formTarget, *domDocument);

		delete theSerializer;
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

	xercesc::XMLPlatformUtils::Terminate();
}

XMLWriter::~XMLWriter()
{
}

XMLWriter* XMLWriter::Create(XMLDocument* document, const String& fileName)
{
	return new XMLWriter(document, fileName);
}

}
