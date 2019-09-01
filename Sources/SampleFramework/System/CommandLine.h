/*=============================================================================
CommandLine.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_COMMANDLINE_H_
#define _SFW_COMMANDLINE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

/**
	@brief Application command-line.
	Provides the list of arguments given to the application executable.
*/
class SE_SAMPLEFRAMEWORK_EXPORT CommandLine
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	CommandLine();

	/** Initializes a new CommandLine from a standard main entry point. */
	CommandLine(int argc, char** argv);

	/** Initializes a new CommandLine from a list of arguments. */
	CommandLine(const Array<String>& arguments);

	/** Destructor. */
	virtual ~CommandLine();
	//@}

	/** @name Properties. */
	//@{
	const Array<String>& GetArguments() const { return _Arguments; }
	//@}

protected:
	Array<String> _Arguments;
};


SE_INLINE CommandLine::CommandLine()
{
}

SE_INLINE CommandLine::CommandLine(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		_Arguments.Add(argv[i]);
	}
}

SE_INLINE CommandLine::CommandLine(const Array<String>& arguments)
{
	_Arguments = arguments;
}

SE_INLINE CommandLine::~CommandLine()
{
}

}

#endif 
