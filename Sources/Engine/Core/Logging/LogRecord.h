/*=============================================================================
LogRecord.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGRECORD_H_
#define _SE_LOGRECORD_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Logging/LogLevel.h"
#include "Core/System/DateTime.h"

namespace SonataEngine
{

/**
	@brief Log record.

	Provides the information concerning a logging request to the logging handlers.
*/
class SE_CORE_EXPORT LogRecord
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	LogRecord();

	/** Constructor. */
	LogRecord(const LogLevel& level, const String& message, const String& source = String::Empty);

	/** Copy constructor. */
	LogRecord(const LogRecord& value);
	//@}

	/** @name Properties. */
	//@{
	/** Gets the level of the record. */
	const LogLevel& GetLevel() const { return _Level; }

	/** Sets the level of the record. */
	void SetLevel(const LogLevel& value) { _Level = value; }

	/** Gets the message of the record. */
	String GetMessage() const { return _Message; }

	/** Sets the message of the record. */
	void SetMessage(String value) { _Message = value; }

	/** Gets the source of the record. */
	String GetSource() const { return _Source; }

	/** Sets the source of the record. */
	void SetSource(String value) { _Source = value; }

	/** Gets the date of the record. */
	const DateTime& GetDateTime() const { return _DateTime; }

	/** Sets the date of the record. */
	void SetDateTime(const DateTime& value) { _DateTime = value; }
	//@}

private:
	LogLevel _Level;
	String _Message;
	String _Source;
	DateTime _DateTime;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Renamed LogEvent to LogRecord.
	Added accessors.
=============================================================================*/
