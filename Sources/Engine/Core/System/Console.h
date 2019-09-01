/*=============================================================================
Console.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CONSOLE_H_
#define _SE_CONSOLE_H_

#include "Core/Common.h"
#include "Core/IO/ConsoleStream.h"
#include "Core/IO/TextStream.h"

namespace SonataEngine
{

/**
	@brief System Console.

	This class can be used to read from and write to the system console.
	Because there is only one console in a system, all the methods are static.
*/
class SE_CORE_EXPORT Console
{
private:
	static TextStream* _In;
	static TextStream* _Out;
	static TextStream* _Error;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Console();

	/** Destructor. */
	virtual ~Console();
	//@}

	static TextStream* In();

	static void SetIn(TextStream* value);

	static TextStream* Out();

	static void SetOut(TextStream* value);

	static TextStream* Error();

	static void SetError(TextStream* value);

	/** Reads the next character from the standard input stream. */
	static SEchar Read();

	/** Reads the next line of characters from the standard input stream. */
	static String ReadLine();

	template <class T>
	static void Write(const T& value)
	{
		Out()->Write(String::ToString(value));
	}

	static void Write(SEchar value);

	static void Write(const String& value);

	static void WriteLine();

	template <class T>
	static void WriteLine(const T& value)
	{
		Out()->WriteLine(String::ToString(value));
	}

	static void WriteLine(SEchar value);

	static void WriteLine(const String& value);
};

}

#endif 
