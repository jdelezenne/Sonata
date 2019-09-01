/*=============================================================================
VertexLayout.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexLayout.h"

namespace SonataEngine
{

VertexLayout::VertexLayout() :
	RefObject()
{
}

VertexLayout::~VertexLayout()
{
}

int VertexLayout::GetElementCount() const
{
	return _elements.Count();
}

VertexLayout::VertexElementList::Iterator VertexLayout::GetElementIterator() const
{
	return _elements.GetIterator();
}

bool VertexLayout::AddElement(const VertexElement& element)
{
	_elements.Add(element);
	return true;
}

void VertexLayout::RemoveAllElements()
{
	_elements.Clear();
}

const VertexElement& VertexLayout::GetElement(int index) const
{
	return _elements[index];
}

void VertexLayout::SetElements(const VertexElement* elements, int count)
{
	_elements.Clear();
	_elements.SetCapacity(count);
	for (int i = 0; i < count; i++)
	{
		_elements.Add(elements[i]);
	}
}

uint16 VertexLayout::GetSize() const
{
	uint16 size = 0;
	int count = _elements.Count();
	for (int i = 0; i < count; i++)
	{
		size += _elements[i].GetSize();
	}

	return size;
}

const VertexElement* VertexLayout::GetElementBySemantic(VertexSemantic semantic, uint8 index) const
{
	int count = _elements.Count();
	for (int i = 0; i < count; i++)
	{
		if (_elements[i].GetVertexSemantic() == semantic && _elements[i].GetSemanticIndex() == index)
		{
			return &_elements[i];
		}
	}
	return NULL;
}

}
