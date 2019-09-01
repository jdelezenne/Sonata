/*=============================================================================
XMLNode.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLNode.h"
#include "Core/Exception/InvalidOperationException.h"

namespace SonataEngine
{

XMLNode::XMLNode() :
	_Parent(NULL),
	_OwnerDocument(NULL)
{
}

XMLNode::~XMLNode()
{
	XMLNode* node;
	foreach (node, _ChildNodes, XMLNode::XMLNodeList)
	{
		delete node;
	}
}

XMLNode* XMLNode::FindChild(XMLNodeType type) const
{
	XMLNode* child = GetFirstChild();

	while (child != NULL)
	{
		if (child->GetNodeType() == type)
		{
			return child;
		}

		child = child->GetNextSibling();
	}

	return NULL;
}

String XMLNode::GetValue() const
{
	return String::Empty;
}

void XMLNode::SetValue(const String& value)
{
	throw InvalidOperationException();
}

XMLNodeType XMLNode::GetNodeType() const
{
	return XMLNodeType_None;
}

XMLNode* XMLNode::GetParentNode() const
{
	return _Parent;
}

bool XMLNode::GetHasChildNodes() const
{
	return !_ChildNodes.IsEmpty();
}

const XMLNode::XMLNodeList XMLNode::GetChildNodes() const
{
	return _ChildNodes;
}

XMLNode* XMLNode::GetFirstChild() const
{
	if (_ChildNodes.IsEmpty())
		return NULL;
	else
		return _ChildNodes[0];
}

XMLNode* XMLNode::GetLastChild() const
{
	if (_ChildNodes.IsEmpty())
		return NULL;
	else
		return _ChildNodes[_ChildNodes.Count() - 1];
}

XMLNode* XMLNode::GetPreviousSibling() const
{
	XMLNode* previous = NULL;
	const XMLNode::XMLNodeList& nodes = _Parent->GetChildNodes();
	XMLNode::XMLNodeList::Iterator it = nodes.GetIterator();
	while (it.Next())
	{
		if (it.Current() == this)
		{
			return previous;
		}
		previous = it.Current();
	}

	return NULL;
}

XMLNode* XMLNode::GetNextSibling() const
{
	const XMLNode::XMLNodeList& nodes = _Parent->GetChildNodes();
	XMLNode::XMLNodeList::Iterator it = nodes.GetIterator();
	while (it.Next())
	{
		if (it.Current() == this)
		{
			if (!it.Next())
				return NULL;
			else
				return it.Current();
		}
	}

	return NULL;
}

XMLElement* XMLNode::GetItem(const String& name)
{
	XMLNode* child = GetFirstChild();
	while (child != NULL)
	{
		if (child->GetNodeType() == XMLNodeType_Element && child->GetName() == name)
			return (XMLElement*)child;
		child = child->GetNextSibling();
	}

	return NULL;
}

const XMLNode::XMLAttributeList XMLNode::GetAttributes() const
{
	return XMLNode::XMLAttributeList();
}

XMLDocument* XMLNode::GetOwnerDocument() const
{
	return _OwnerDocument;
}

XMLNode* XMLNode::Clone()
{
	//TODO
	return NULL;
}

XMLNode* XMLNode::AppendChild(XMLNode* node)
{
	_ChildNodes.Add(node);
	node->_Parent = this;
	return node;
}

XMLNode* XMLNode::PrependChild(XMLNode* node)
{
	_ChildNodes.Insert(0, node);
	node->_Parent = this;
	return node;
}

void XMLNode::RemoveAll()
{
	XMLNode* node;
	foreach (node, _ChildNodes, XMLNode::XMLNodeList)
		delete node;

	_ChildNodes.Clear();
}

XMLNode* XMLNode::InsertBefore(XMLNode* newChild, XMLNode* refChild)
{
	if (refChild == NULL)
	{
		_ChildNodes.Add(newChild);
	}
	else
	{
		int index = _ChildNodes.IndexOf(refChild);
		if (index != -1)
			_ChildNodes.Insert(index, newChild);
	}
	newChild->_Parent = this;
	return newChild;
}

XMLNode* XMLNode::InsertAfter(XMLNode* newChild, XMLNode* refChild)
{
	if (refChild == NULL)
	{
		_ChildNodes.Add(newChild);
	}
	else
	{
		int index = _ChildNodes.IndexOf(refChild);
		if (index != -1)
			_ChildNodes.Insert(index + 1, newChild);
	}
	newChild->_Parent = this;
	return newChild;
}

XMLNode* XMLNode::RemoveChild(XMLNode* node)
{
	_ChildNodes.Remove(node);
	node->_Parent = NULL;
	return node;
}

}
