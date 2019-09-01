/*=============================================================================
HeightField.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HEIGHTFIELD_H_
#define _SE_HEIGHTFIELD_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/** Height field formats. */
enum HeightFieldFormat
{
	HeightFieldFormat_Int8,
	HeightFieldFormat_Int16,
	HeightFieldFormat_Int32,
	HeightFieldFormat_Real
};

/**
	@brief Terrain height field.
*/
class SE_GRAPHICS_EXPORT HeightField : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	HeightField();

	/** Destructor. */
	virtual ~HeightField();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the width of the height field. */
	int GetWidth() const { return _Width; }

	/** Gets the height of the height field. */
	int32 GetHeight() const { return _Height; }

	/** Gets the size of the height field. */
	int32 GetSize() const;

	/**
		Gets the format of the height field data.
		@remarks The data is always HeightFieldFormat.Real.
	*/
	HeightFieldFormat GetFormat() const { return _Format; }

	/** Gets the height field data. */
	real32* GetData() const { return _Data; }

	/**
		Gets the height at a given position of the height field.
		@param x The x-coordinate on the height field.
		@param y The y-coordinate on the height field.
		@return The height at a given position of the height field.
	*/
	real32 GetHeight(int32 x, int32 y) const;

	/**
		Sets the height at a given position of the height field.
		@param x The x-coordinate on the height field.
		@param y The y-coordinate on the height field.
		@param height The height to be set.
	*/
	void SetHeight(int32 x, int32 y, real32 height);

	/**
		Gets the minimum height of the height field.
		@return The minimum height of the height field.
	*/
	real32 GetMinimumHeight() const;

	/**
		Gets the maximum height of the height field.
		@return The maxnimum height of the height field.
	*/
	real32 GetMaximumHeight() const;

	/**
		Gets the interpolated height at a given position of the height field.
		@param x The x-coordinate on the height field.
		@param y The y-coordinate on the height field.
		@return The interpolated height at a given position of the height field.
	*/
	real32 GetInterpolatedHeight(real32 x, real32 y) const;
	//@}

	/**
		Creates a new height field.
		The data contained in this height field will be deleted.
		@param width The width of the height field.
		@param height The height of the height field.
		@param format The format of the height field data.
	*/
	void Create(int32 width, int32 height, HeightFieldFormat format);

	/**	Destroys the height field. */
	void Destroy();

	/**	Loads an height field from a raw data stream. */
	bool LoadFromRAW(Stream& stream, int32 width, int32 height, HeightFieldFormat format);

	/**	Saves the height field to a raw data stream. */
	bool SaveToRAW(Stream& stream, HeightFieldFormat format);

protected:
	int32 _Width;
	int32 _Height;
	HeightFieldFormat _Format;
	real32* _Data;
};

typedef SmartPtr<HeightField> HeightFieldPtr;

}

#endif 
