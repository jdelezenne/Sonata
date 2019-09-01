/*=============================================================================
VertexLayout.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXLAYOUT_H_
#define _SE_VERTEXLAYOUT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/** Vertex Layout. */
class SE_GRAPHICS_EXPORT VertexLayout : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	VertexLayout();

	/** Destructor. */
	virtual ~VertexLayout();
	//@}

	/** Vertex elements. */
	//@{
	typedef BaseArray<VertexElement> VertexElementList;

	int GetElementCount() const;
	VertexElementList::Iterator GetElementIterator() const;
	bool AddElement(const VertexElement& element);
	void RemoveAllElements();
	const VertexElement& GetElement(int index) const;
	void SetElements(const VertexElement* elements, int count);
	//@}

	/** Retrieves the size of the vertex layout. */
	uint16 GetSize() const;

	const VertexElement* GetElementBySemantic(VertexSemantic semantic, uint8 index = 0) const;

protected:
	VertexElementList _elements;
};

typedef SmartPtr<VertexLayout> VertexLayoutPtr;

}

#endif
