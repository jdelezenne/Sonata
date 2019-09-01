/*=============================================================================
PointState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINTSTATE_H_
#define _SE_POINTSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Point state.
*/
class SE_GRAPHICS_EXPORT PointState
{
public:
	/** @name Point Sprite */
	//@{
	/// Controls how the computation of size for point sprites is handled.
	bool Enable;

	/// Specifies the size to use for point size computation in cases in which point size is not specified for each vertex.
	real32 Size;

	/// Specifies the minimum size to which point sprites can be set.
	real32 MinSize;

	/// Specifies the minimum size to which point sprites can be set.
	real32 MaxSize;

	/// Controls how the computation of size for point sprites is handled.
	bool ScaleEnable;

	/// Controls the distance-based size attenuation for point primitives.
	real32 ConstantScale;
	real32 LinearScale;
	real32 QuadraticScale;
	//@}

public:
	/** @name Constructor. */
	//@{
	PointState();
	//@}
};

SE_DECLARE_STRUCT(PointState);
SE_BEGIN_STRUCT(PointState);
	SE_StructField(Enable, bool);
	SE_StructField(Size, real32);
	SE_StructField(MinSize, real32);
	SE_StructField(MaxSize, real32);
	SE_StructField(ScaleEnable, bool);
	SE_StructField(ConstantScale, real32);
	SE_StructField(LinearScale, real32);
	SE_StructField(QuadraticScale, real32);
SE_END_STRUCT(PointState);

}

#endif
