/*=============================================================================
SystemFonts.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SYSTEMFONTS_H_
#define _SE_SYSTEMFONTS_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Font/Font.h"
#include "Graphics/Font/FontFamily.h"

namespace SonataEngine
{

/**
	System fonts.
*/
class SE_GRAPHICS_EXPORT SystemFonts : public Singleton<SystemFonts>
{
public:
	typedef Array<FontFamily> FontFamilyList;

	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	SystemFonts();

	/** Destructor. */
	virtual ~SystemFonts();
	//@}

	/** @name Families. */
	//@{
	int GetFamilyCount() const;
	const FontFamily& GetFamily(int index) const;
	FontFamilyList::Iterator GetFamilyIterator() const;
	//@}

protected:
	void EnumerateFamilies();

	static FontFamilyList _Families;

	String _FamilyName;
	FontStyle _FontStyle;
};

}

#endif 
