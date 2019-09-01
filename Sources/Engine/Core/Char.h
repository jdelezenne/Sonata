/*=============================================================================
Char.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CHAR_H_
#define _SE_CHAR_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Unicode Character. */
class SE_CORE_EXPORT Char
{
public:
	/** Minimum value for Char types. */
	static const Char MinValue;

	/** Maximum value for Char types. */
	static const Char MaxValue;

	/** Null character. */
	static const Char Null;

	/** Tabulation character. */
	static const Char Tab;

	/** Carriage return character. */
	static const Char Cr;

	/** Line feed character. */
	static const Char Lf;

	/** Space character. */
	static const Char Space;

	/** Quote character. */
	static const Char Quote;

	/** @name Constructors. */
	//@{
	Char();
	Char(const SEchar value);
	Char(const int value);
	Char(const Char& value);
	//@}

	operator const SEchar() const;
	operator SEchar();

	const SEchar& operator*() const;
	SEchar& operator*();

    static bool IsPrintable(const Char& c);

    static bool IsControl(const Char& c);

    static bool IsPunctuation(const Char& c);

    static bool IsWhiteSpace(const Char& c);

    static bool IsLetter(const Char& c);

    static bool IsDigit(const Char& c);

    static bool IsLetterOrDigit(const Char& c);

    static bool IsNumber(const Char& c);

    static bool IsSymbol(const Char& c);

	/** Indicates whether a Unicode character is categorized as a uppercase letter. */
	static bool IsUpper(const Char& c);

	/** Indicates whether a Unicode character is categorized as a lowercase letter. */
	static bool IsLower(const Char& c);

	/** Converts the value of a Unicode character to its uppercase equivalent. */
	static Char ToUpper(const Char& c);

	/** Converts the value of a Unicode character to its lowercase equivalent. */
	static Char ToLower(const Char& c);

protected:
	SEchar _value;
};

#include "Char.inl"

}

#endif
