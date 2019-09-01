/*=============================================================================
VertexElement.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexElement.h"

namespace SonataEngine
{

VertexElement::VertexElement()
{
}

VertexElement::VertexElement(uint16 stream, uint16 offset, VertexFormat format, VertexSemantic semantic, uint8 semanticIndex) :
	_stream(stream),
	_offset(offset),
	_vertexFormat(format),
	_vertexSemantic(semantic),
	_semanticIndex(semanticIndex)
{
}

VertexElement::~VertexElement()
{
}

uint16 VertexElement::GetTypeSize(VertexFormat format)
{
	switch (format)
	{
	case VertexFormat_Float1:
		return sizeof(real32);
	case VertexFormat_Float2:
		return sizeof(real32)*2;
	case VertexFormat_Float3:
		return sizeof(real32)*3;
	case VertexFormat_Float4:
		return sizeof(real32)*4;
	case VertexFormat_Color:
		return sizeof(uint32);
	case VertexFormat_UByte4:
        return sizeof(uint8)*4;
	case VertexFormat_Short2:
		return sizeof(int16)*2;
	case VertexFormat_Short4:
		return sizeof(int16)*4;
	case VertexFormat_UByte4N:
        return sizeof(uint8)*4;
	}

	return 0;
}

uint16 VertexElement::GetTypeComponents(VertexFormat format)
{
	switch (format)
	{
	case VertexFormat_Float1:
		return 1;
	case VertexFormat_Float2:
		return 2;
	case VertexFormat_Float3:
		return 3;
	case VertexFormat_Float4:
		return 4;
	case VertexFormat_Color:
		return 1;
	case VertexFormat_UByte4:
        return 4;
	case VertexFormat_Short2:
		return 2;
	case VertexFormat_Short4:
		return 4;
	case VertexFormat_UByte4N:
        return 4;
	}

	return 0;
}

bool VertexElement::operator==(const VertexElement& other) const
{
	return (_stream == other.GetStream() &&
		_offset == other.GetOffset() &&
		_vertexFormat == other.GetVertexFormat() &&
		_vertexSemantic == other.GetVertexSemantic() &&
		_semanticIndex == other.GetSemanticIndex());

}

bool VertexElement::operator!=(const VertexElement& other) const
{
	return (!(*this == other));
}

}
