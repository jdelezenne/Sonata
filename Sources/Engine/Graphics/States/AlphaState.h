/*=============================================================================
AlphaState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ALPHASTATE_H_
#define _SE_ALPHASTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

const int32 MaxRenderTargets = 8;

/**
	Alpha state.
*/
class SE_GRAPHICS_EXPORT AlphaState
{
public:
	/** @name Alpha Testing */
	//@{
	/// Retrieves or sets a render state that enables a per-pixel alpha test.
	bool TestEnable;

	/// Specifies a reference alpha value against which pixels are tested when alpha testing is enabled.
	uint8 Reference;

	/// Retrieves or sets the comparison function for the alpha test.
	ComparisonFunction Function;
	//@}

	/**  Blending */
	//@{
	/// Retrieves or sets a value that enables a per-channel write for the final render target.
	ColorFlag RenderTargetWriteMask[MaxRenderTargets];

	/// Enables or disables the blending operation for each rendertarget.
	bool BlendEnable[MaxRenderTargets];

	/// Retrieves or sets the pre-blend operation to do on the source's RGB values.
	BlendMode SourceBlend;

	/// Retrieves or sets the pre-blend operation to do on the destination's RGB values.
	BlendMode DestinationBlend;

	/// Retrieves or sets the blend operation applied to the RGB values.
	BlendOperation BlendOp;

	/// Retrieves or sets the pre-blend operation to do on the source's alpha values.
	BlendMode SourceBlendAlpha;

	/// Retrieves or sets the pre-blend operation to do on the destination's alpha values.
	BlendMode DestinationBlendAlpha;

	/// Retrieves or sets the blend operation applied to the alpha values.
	BlendOperation BlendOperationAlpha;

	/// Retrieves or sets a Color to use for a constant blend factor during alpha blending.
	Color32 BlendFactor;

	/** Determines which multisampling method to use when setting the pixel to the rendertarget. */
	bool AlphaToCoverageEnable;
	//@}

public:
	/** @name Constructor. */
	//@{
	AlphaState();
	//@}
};

SE_DECLARE_STRUCT(AlphaState);
SE_BEGIN_STRUCT(AlphaState);
	SE_StructField(TestEnable, bool);
	SE_StructField(Reference, uint8);
	SE_StructField(Function, ComparisonFunction);
	//SE_StructField(RenderTargetWriteMask, ColorFlag[MaxRenderTargets]);
	//SE_StructField(BlendEnable, bool[MaxRenderTargets]);
	SE_StructField(SourceBlend, BlendMode);
	SE_StructField(DestinationBlend, BlendMode);
	SE_StructField(BlendOp, BlendOperation);
	SE_StructField(SourceBlendAlpha, BlendMode);
	SE_StructField(DestinationBlendAlpha, BlendMode);
	SE_StructField(BlendOperationAlpha, BlendOperation);
	SE_StructField(BlendFactor, Color32);
	SE_StructField(AlphaToCoverageEnable, bool);
SE_END_STRUCT(AlphaState);

}

#endif
