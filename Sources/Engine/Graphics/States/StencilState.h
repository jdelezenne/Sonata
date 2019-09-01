/*=============================================================================
StencilState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STENCILSTATE_H_
#define _SE_STENCILSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Depth Stencil Operation.
*/
struct DepthStencilOperation
{
	/// Retrieves or sets the stencil operation to perform if the stencil test fails.
	StencilOperation Fail;

	/// Retrieves or sets the stencil operation to perform if the stencil test passes and the depth test (z-test) fails.
	StencilOperation DepthBufferFail;

	/// Retrieves or sets the stencil operation to perform if both the stencil test and the depth test (z-test) pass.
	StencilOperation Pass;

	/// Retrieves or sets the comparison function for the stencil test.
	ComparisonFunction Function;
};

SE_DECLARE_STRUCT(DepthStencilOperation);
SE_BEGIN_STRUCT(DepthStencilOperation);
	SE_StructField(Fail, StencilOperation);
	SE_StructField(DepthBufferFail, StencilOperation);
	SE_StructField(Pass, StencilOperation);
	SE_StructField(Function, ComparisonFunction);
SE_END_STRUCT(DepthStencilOperation);

/**
	Stencil state.
*/
class SE_GRAPHICS_EXPORT StencilState
{
public:
	/** @name Stencil Testing */
	//@{
	/// Retrieves or sets stencil enabling.
	bool Enable;

	/// Description of the depth stencil operation if the pixel is front-facing.
	DepthStencilOperation FrontFace;

	/// Description of the depth stencil operation if the pixel is back-facing.
	DepthStencilOperation BackFace;

	/// Specifies a reference value to use for the stencil test.
	int32 Reference;

	/// Retrieves or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
	int32 ReadMask;

	/// Retrieves or sets the write mask applied to values written into the stencil buffer.
	int32 WriteMask;
	//@}

public:
	/** @name Constructor. */
	//@{
	StencilState();
	//@}
};

SE_DECLARE_STRUCT(StencilState);
SE_BEGIN_STRUCT(StencilState);
	SE_StructField(Enable, bool);
	SE_StructField(FrontFace, DepthStencilOperation);
	SE_StructField(BackFace, DepthStencilOperation);
	SE_StructField(Reference, int32);
	SE_StructField(ReadMask, int32);
	SE_StructField(WriteMask, int32);
SE_END_STRUCT(StencilState);

}

#endif
