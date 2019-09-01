/*=============================================================================
TextStreamLogHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TextStreamLogHandler.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

TextStreamLogHandler::TextStreamLogHandler() :
	LogHandler(),
	_TextStream(NULL)
{
}

TextStreamLogHandler::TextStreamLogHandler(Stream* stream) :
	LogHandler()
{
	if (stream == NULL)
	{
		SEthrow(ArgumentNullException("stream"));
		return;
	}

	_TextStream = new TextStream(stream);
}

TextStreamLogHandler::TextStreamLogHandler(TextStream* textStream) :
	LogHandler()
{
	if (textStream == NULL)
	{
		SEthrow(ArgumentNullException("textStream"));
		return;
	}

	_TextStream = textStream;
}

TextStreamLogHandler::~TextStreamLogHandler()
{
	Close();

	SE_DELETE(_TextStream);
}

void TextStreamLogHandler::Close()
{
	if (_TextStream != NULL)
	{
		_TextStream->Close();
	}
}

void TextStreamLogHandler::Write(const String& message)
{
	if (_TextStream != NULL)
	{
		_TextStream->Write(message);
	}
}

}
