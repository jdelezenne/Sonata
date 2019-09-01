/*=============================================================================
HeightFieldFilter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HEIGHTFIELDFILTER_H_
#define _SE_HEIGHTFIELDFILTER_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Terrain/HeightField.h"

namespace SonataEngine
{

/**
	@brief Terrain height field filter.

	Base class for filter implementations.
*/
class SE_GRAPHICS_EXPORT HeightFieldFilter : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	HeightFieldFilter();

	/** Destructor. */
	virtual ~HeightFieldFilter();
	//@}

	HeightField* GetHeightField() const { return _HeightField; }
	void SetHeightField(HeightField* value) { _HeightField = value; }

	/** Normalizes the heights of an height field. */
	bool Normalize();

	/** Scales the heights of an height field. */
	bool Scale(real32 scale);

	/** Inverts the heights of an height field. */
	bool Invert();

	void Erosion(real32 filter);

protected:
	void _ErosionLine(real32* line, int32 stride, int32 count, real32 filter);

protected:
	HeightField* _HeightField;
};

}

#endif 
