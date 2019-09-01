/*=============================================================================
HeightFieldGenerator.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HEIGHTFIELDGENERATOR_H_
#define _SE_HEIGHTFIELDGENERATOR_H_

#include "Core/Core.h"
#include "Graphics/Terrain/HeightField.h"

namespace SonataEngine
{

enum TerrainFaultMethod
{
	TerrainFaultMethod_LineStep,
	TerrainFaultMethod_LineSin,
	TerrainFaultMethod_LineCos,
	TerrainFaultMethod_Circle,
	TerrainFaultMethod_Square,
	TerrainFaultMethod_Line
};

/**
	@brief Height field generator.

	Base class for height field generator implementations.
*/
class SE_GRAPHICS_EXPORT HeightFieldGenerator : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	HeightFieldGenerator();

	/** Destructor. */
	virtual ~HeightFieldGenerator();
	//@}

	HeightField* GetHeightField() const { return _HeightField; }
	void SetHeightField(HeightField* value) { _HeightField = value; }

	Random* GetRandom() const { return _Random; }
	void SetRandom(Random* value) { _Random = value; }

	/** Generates a height field with a constant height. */
	bool ConstantHeight(real32 height);

	/** Generates a random height field. */
	bool RandomHeight(real32 min, real32 max);

	/** Generates an height field using Fault Formation. */
	bool Fault(TerrainFaultMethod method, int32 iterations, int32 minDelta, int32 maxDelta);

	/** Generates an height field using Midpoint Displacement. */
	bool Midpoint(int32 scale);

protected:
	HeightField* _HeightField;
	Random* _Random;
};

}

#endif 
