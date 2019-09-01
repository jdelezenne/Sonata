/*=============================================================================
Win32SystemFonts.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Graphics/Font/SystemFonts.h"

namespace SonataEngine
{

typedef SystemFonts Win32SystemFonts;

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam)
{
	String familyName = lpelfe->elfLogFont.lfFaceName;
	int32 height, width, weight;
	bool italic, underline, strikeout, fixed, ttf;

	if (familyName[0] == '@')
		return 1;

	SystemFonts::FontFamilyList* families = (SystemFonts::FontFamilyList*)lParam;
	SystemFonts::FontFamilyList::Iterator it = families->GetIterator();
	while (it.Next())
	{
		if (it.Current().GetFaceName() == familyName)
			return 1;
	}

	weight = lpelfe->elfLogFont.lfWeight;
	italic = lpelfe->elfLogFont.lfItalic != 0;
	underline = lpelfe->elfLogFont.lfUnderline != 0;
	strikeout = lpelfe->elfLogFont.lfStrikeOut != 0;
	fixed = (lpntme->ntmTm.tmPitchAndFamily & TMPF_FIXED_PITCH) == 0;
	ttf = (lpntme->ntmTm.tmPitchAndFamily & TMPF_TRUETYPE) != 0;
	height = lpelfe->elfLogFont.lfHeight;
	width = lpelfe->elfLogFont.lfWidth;

	int style = 0;
	if (weight >= FW_BOLD)
		style |= FontStyle_Bold;
	if (italic)
		style |= FontStyle_Italic;
	if (underline)
		style |= FontStyle_Underline;
	if (strikeout)
		style |= FontStyle_Strikeout;

	FontFamily family(familyName, (FontStyle)style, weight, height, width);
	families->Add(family);

	// continue enumeration
	return 1;
}

void Win32SystemFonts::EnumerateFamilies()
{
	HDC hDC = ::GetDC(NULL);
	LOGFONT lf;

	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0] = 0;
	lf.lfPitchAndFamily = 0;

	::EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&_Families, 0);

	::ReleaseDC(NULL, hDC);
}

}
