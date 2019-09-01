/*=============================================================================
D3D8VertexLayout.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8VertexLayout.h"

namespace SE_D3D8
{

D3D8VertexLayout::D3D8VertexLayout() :
	VertexLayout(),
	_D3DDeclaration(NULL)
{
}

D3D8VertexLayout::D3D8VertexLayout(DWORD* declaration) :
	VertexLayout(),
	_D3DDeclaration(declaration)
{
}

D3D8VertexLayout::~D3D8VertexLayout()
{
}

void D3D8VertexLayout::SetDeclaration(DWORD* value)
{
	_D3DDeclaration = value;
}

}
