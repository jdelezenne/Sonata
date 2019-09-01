/*=============================================================================
D3D8VertexLayout.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8VERTEXLAYOUT_H_
#define _SE_D3D8VERTEXLAYOUT_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 vertex layout. */
class D3D8VertexLayout : public VertexLayout
{
public:
	D3D8VertexLayout();
	D3D8VertexLayout(DWORD* declaration);
	virtual ~D3D8VertexLayout();

	DWORD* GetDeclaration() const { return _D3DDeclaration; }
	void SetDeclaration(DWORD* value);

protected:
	DWORD* _D3DDeclaration;
};

}

#endif
