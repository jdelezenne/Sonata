/*=============================================================================
D3D9VertexLayout.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9VertexLayout.h"

namespace SE_D3D9
{

D3D9VertexLayout::D3D9VertexLayout() :
	VertexLayout(),
	_D3DDeclaration(NULL)
{
}

D3D9VertexLayout::D3D9VertexLayout(IDirect3DVertexDeclaration9* declaration) :
	VertexLayout(),
	_D3DDeclaration(declaration)
{
}

D3D9VertexLayout::~D3D9VertexLayout()
{
	SE_RELEASE(_D3DDeclaration);
}

void D3D9VertexLayout::SetDeclaration(IDirect3DVertexDeclaration9* value)
{
	SE_RELEASE(_D3DDeclaration);

	_D3DDeclaration = value;
}

}
