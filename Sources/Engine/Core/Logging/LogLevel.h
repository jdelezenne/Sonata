/*=============================================================================
LogLevel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGLEVEL_H_
#define _SE_LOGLEVEL_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

/**
	@brief Log level.

	Represents the level of a log record. Some levels are predefined.
*/
class SE_CORE_EXPORT LogLevel
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	LogLevel(const String& name, int32 value);

	/** Copy constructor. */
	LogLevel(const LogLevel& value);
	//@}

	/** @name Properties. */
	//@{
	/** Gets the name of the level. */
	const String& GetName() const { return _name; }

	/** Gets the value of the level. */
	int32 GetValue() const { return _Value; }
	//@}

public:
	static const LogLevel None;
	static const LogLevel Debug;
	static const LogLevel Information;
	static const LogLevel Notice;
	static const LogLevel Warning;
	static const LogLevel Error;
	static const LogLevel Critical;
	static const LogLevel Fatal;

private:
	String _name;
	int32 _Value;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Created. Replacement for the LogLevel enumeration.
=============================================================================*/
