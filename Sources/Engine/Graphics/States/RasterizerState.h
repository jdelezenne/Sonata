/*=============================================================================
RasterizerState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RASTERIZERSTATE_H_
#define _SE_RASTERIZERSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Rasterizer state.
*/
class SE_GRAPHICS_EXPORT RasterizerState
{
public:
	/** @name Rasterizer. */
	//@{
	/** Fill mode. */
	FillMode FillMode;

	/** Cull mode. */
	CullMode CullMode;

	/** Retrieves or sets the drawing order.
		$remark
			True to render vertices counter clockwise, false for clockwise.
	*/
	bool FrontCounterClockwise;

	/** Enables or disables multisampling. */
	bool MultisampleEnable;

	/** Enables or disables line antialiasing.
		$remark
			Only applies if doing line drawing and MultisampleEnable is false.
	*/
	bool AntialiasedLineEnable;
	//@}

public:
	/** @name Constructor. */
	//@{
	RasterizerState();
	//@}
};

SE_DECLARE_STRUCT(RasterizerState);
SE_BEGIN_STRUCT(RasterizerState);
	SE_StructField(FillMode, FillMode);
	SE_StructField(CullMode, CullMode);
	SE_StructField(FrontCounterClockwise, bool);
	SE_StructField(MultisampleEnable, bool);
	SE_StructField(AntialiasedLineEnable, bool);
SE_END_STRUCT(RasterizerState);

}

#endif 
