/*=============================================================================
Console.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Console.h"
#include "Core/Exception/ArgumentNullException.h"
 
namespace SonataEngine
{

TextStream* Console::_In = NULL;
TextStream* Console::_Out = NULL;
TextStream* Console::_Error = NULL;

Console::Console()
{
}

Console::~Console()
{
	SE_DELETE(Console::_In);
	SE_DELETE(Console::_Out);
	SE_DELETE(Console::_Error);
}

TextStream* Console::In()
{
	if (Console::_In == NULL)
	{
		Console::_In = new TextStream(ConsoleStream::StandardInput);
	}

	return Console::_In;
}

void Console::SetIn(TextStream* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}

	Console::_In = value;
}

TextStream* Console::Out()
{
	if (Console::_Out == NULL)
	{
		Console::_Out = new TextStream(ConsoleStream::StandardOutput);
	}

	return Console::_Out;
}

void Console::SetOut(TextStream* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}

	_Out = value;
}

TextStream* Console::Error()
{
	if (Console::_Error == NULL)
	{
		Console::_Error = new TextStream(ConsoleStream::StandardError);
	}

	return Console::_Error;
}

void Console::SetError(TextStream* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}

	Console::_Error = value;
}

SEchar Console::Read()
{
	return In()->Read();
}

String Console::ReadLine()
{
	return In()->ReadLine();
}

void Console::Write(SEchar value)
{
	Out()->Write(value);
}

void Console::Write(const String& value)
{
	Out()->Write(value);
}

void Console::WriteLine()
{
	Out()->WriteLine();
}

void Console::WriteLine(SEchar value)
{
	Out()->WriteLine(value);
}

void Console::WriteLine(const String& value)
{
	Out()->WriteLine(value);
}


}
