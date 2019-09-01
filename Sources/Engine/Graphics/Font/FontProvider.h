/*=============================================================================
FontProvider.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FONTPROVIDER_H_
#define _SE_FONTPROVIDER_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Font/Font.h"

namespace SonataEngine
{

/**
	Base class for the font providers.
*/
class SE_GRAPHICS_EXPORT FontProvider
{
public:
	/** @name Constructors / Destructor. */
	//@{
	FontProvider();
	virtual ~FontProvider();
	//@}

public:
	Font::GlythList Glyths;
};

}

#endif 
