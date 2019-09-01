/*=============================================================================
XMLNode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLNODE_H_
#define _SE_XMLNODE_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Containers/Array.h"

namespace SonataEngine
{

class XMLDocument;
class XMLAttribute;
class XMLElement;

/** Types of XML nodes. */
enum XMLNodeType
{
	XMLNodeType_None = 0,
	XMLNodeType_Element = 1,
	XMLNodeType_Attribute = 2,
	XMLNodeType_Text = 3,
	XMLNodeType_CDATA = 4,
	XMLNodeType_EntityReference = 5,
	XMLNodeType_Entity = 6,
	XMLNodeType_ProcessingInstruction = 7,
	XMLNodeType_Comment = 8,
	XMLNodeType_Document = 9,
	XMLNodeType_DocumentType = 10,
	XMLNodeType_DocumentFragment = 11,
	XMLNodeType_Notation = 12
};

/** Represents a single node in the XML document. */
class SE_CORE_EXPORT XMLNode
{
public:
	typedef Array<XMLNode*> XMLNodeList;
	typedef Array<XMLAttribute*> XMLAttributeList;

	virtual ~XMLNode();

	/// Gets the qualified name of the node.
	virtual String GetName() const = 0;

	/// Gets the value of the node.
	virtual String GetValue() const;

	/// Sets the value of the node.
	virtual void SetValue(const String& value);

	/// Gets the type of the current node.
	virtual XMLNodeType GetNodeType() const = 0;

	/// Gets the parent of this node (for nodes that can have parents).
	virtual XMLNode* GetParentNode() const;

	/// Gets a value indicating whether this node has any child nodes.
	virtual bool GetHasChildNodes() const;

	/// Gets all the child nodes of the node.
	virtual const XMLNodeList GetChildNodes() const;

	/// Gets the first child of the node.
	virtual XMLNode* GetFirstChild() const;

	/// Gets the last child of the node. 
	virtual XMLNode* GetLastChild() const;

	/// Gets the node immediately preceding this node.
	virtual XMLNode* GetPreviousSibling() const;

	/// Gets the node immediately following this node.
	virtual XMLNode* GetNextSibling() const;

	/// Gets an the attributes of this node.
	virtual const XMLAttributeList GetAttributes() const;

	/// Gets the XmlDocument to which this node belongs.
	virtual XMLDocument* GetOwnerDocument() const;

	/// Creates a duplicate of this node.
	virtual XMLNode* Clone();

	/// Adds the specified node to the end of the list of child nodes for this node.
	virtual XMLNode* AppendChild(XMLNode* node);

	/// Adds the specified node to the beginning of the list of child nodes for this node.
	virtual XMLNode* PrependChild(XMLNode* node);

	/// Inserts the specified node immediately before the specified reference node.
	virtual XMLNode* InsertBefore(XMLNode* newChild, XMLNode* refChild);

	/// Inserts the specified node immediately after the specified reference node.
	virtual XMLNode* InsertAfter(XMLNode* newChild, XMLNode* refChild);

	/// Removes all the child nodes and/or attributes of the current node.
	virtual void RemoveAll();

	/// Removes specified child node.
	virtual XMLNode* RemoveChild(XMLNode* node);

	/// Gets the first child element with the specified name.
	virtual XMLElement* GetItem(const String& name);

protected:
	XMLNode();
	XMLNode* FindChild(XMLNodeType type) const;

protected:
	XMLNode* _Parent;
	XMLNodeList _ChildNodes;
	XMLDocument* _OwnerDocument;

	friend class XMLDocument;
	friend class XMLReader;
	friend class XMLWriter;
};

}

#endif 
