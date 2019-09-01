/*=============================================================================
IProcess.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IPROCESS_H_
#define _SE_IPROCESS_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the processes. */
class SE_CORE_EXPORT IProcess
{
public:
	/// Creates this Process.
	virtual	bool Create() = 0;

	/// Destroys this Process.
	virtual	void Destroy() = 0;

	/// Updates this Process.
	virtual void Update(real64 elapsed) = 0;
};

}

#endif 
