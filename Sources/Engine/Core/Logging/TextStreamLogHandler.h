/*=============================================================================
TextStreamLogHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TEXTSTREAMLOGHANDLER_H_
#define _SE_TEXTSTREAMLOGHANDLER_H_

#include "Core/Common.h"
#include "Core/Logging/LogHandler.h"
#include "Core/IO/Stream.h"
#include "Core/IO/TextStream.h"

namespace SonataEngine
{

/**
	@brief Standard Stream log handler.

	Logs to the standard stream.
*/
class SE_CORE_EXPORT TextStreamLogHandler : public LogHandler
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	TextStreamLogHandler();

	/** Constructor. */
	TextStreamLogHandler(Stream* stream);

	/** Constructor. */
	TextStreamLogHandler(TextStream* textStream);

	/** Destructor. */
	virtual ~TextStreamLogHandler();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the text stream that receives the messages. */
	TextStream* GetTextStream() const { return _TextStream; }

	/** Sets the text stream that receives the messages. */
	void SetTextStream(TextStream* value) { _TextStream = value; }
	//@}

	virtual void Close();

	virtual void Write(const String& message);

protected:
	TextStream* _TextStream;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Created. Merged the Std and File log handlers into this TextStream
	handler.
=============================================================================*/
