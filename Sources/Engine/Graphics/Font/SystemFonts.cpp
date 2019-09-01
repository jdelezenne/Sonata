/*=============================================================================
SystemFonts.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SystemFonts.h"

namespace SonataEngine
{

SystemFonts::FontFamilyList SystemFonts::_Families;

SystemFonts::SystemFonts()
{
	EnumerateFamilies();
}

SystemFonts::~SystemFonts()
{
}

int SystemFonts::GetFamilyCount() const
{
	return _Families.Count();
}

const FontFamily& SystemFonts::GetFamily(int index) const
{
	return _Families[index];
}

SystemFonts::FontFamilyList::Iterator SystemFonts::GetFamilyIterator() const
{
	return _Families.GetIterator();
}

}
