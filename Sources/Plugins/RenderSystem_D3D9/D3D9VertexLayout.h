/*=============================================================================
D3D9VertexLayout.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9VERTEXLAYOUT_H_
#define _SE_D3D9VERTEXLAYOUT_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 vertex layout. */
class D3D9VertexLayout : public VertexLayout
{
public:
	D3D9VertexLayout();
	D3D9VertexLayout(IDirect3DVertexDeclaration9* declaration);
	virtual ~D3D9VertexLayout();

	IDirect3DVertexDeclaration9* GetDeclaration() const { return _D3DDeclaration; }
	void SetDeclaration(IDirect3DVertexDeclaration9* value);

protected:
	IDirect3DVertexDeclaration9* _D3DDeclaration;
};

}

#endif
