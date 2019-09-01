/*=============================================================================
Manager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MANAGER_H_
#define _SE_MANAGER_H_

#include "Core/Common.h"
#include "Core/Object.h"
#include "Core/IProcess.h"

namespace SonataEngine
{

/** Base class for the managers (subsystems).
	The systems are the core objects of the engine. They are
	the rendering system, input system, audio system...
*/
class SE_CORE_EXPORT Manager : public RefObject, public IProcess
{
public:
	/** Destructor. */
	virtual ~Manager();

protected:
	/** Constructor. */
	Manager();

	/** Constructor. */
	Manager(const String& name);

	/** @name Properties. */
	//@{
	/**
		Retrieves the name of the manager.
		@return Name of the manager.
	*/
	String GetName() const { return _name; }

	/**
		Sets the name of the manager.
		@param value Name of the manager.
	*/
	void SetName(const String& value) { _name = value; }
	//@}

	virtual	bool Create() { return true; }
	virtual	void Destroy() {}
	virtual void Update(real64 elapsed) {}

protected:
	String _name;
};

}

#endif 
