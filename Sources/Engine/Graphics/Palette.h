/*=============================================================================
Palette.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PALETTE_H_
#define _SE_PALETTE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/** Color Palette.
	Defines an array of colors that make up a color palette.
*/
class SE_GRAPHICS_EXPORT Palette
{
public:
	/** Constructor. */
	Palette();

	/** Constructor with a color count. */
	Palette(int32 count);

	/** Destructor. */
	virtual ~Palette();

	/** Gets a Color structure at a given index. */
	const Color8& operator[](int index) const;

	/** Gets a Color structure at a given index. */
	Color8& operator[](int index);

	/** Gets an array of Color structures. */
	operator const Color8*() const;

	/** Gets an array of Color structures. */
	operator Color8*() const;

	/** Gets the number of Colors. */
	int32 GetCount() const { return _Count; }

	/** Gets a Color structure at a given index. */
	const Color8& GetEntry(int index) const;

	/** Gets a Color structure at a given index. */
	Color8& GetEntry(int index);

	/** Gets an array of Color structures. */
	const Color8* GetEntries() const;

	/** Gets an array of Color structures. */
	Color8* GetEntries();

protected:
	int32 _Count;
	Color8* _Entries;
};

#include "Palette.inl"

}

#endif 
