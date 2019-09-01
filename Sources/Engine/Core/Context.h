/*=============================================================================
Context.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CONTEXT_H_
#define _SE_CONTEXT_H_

namespace SonataEngine
{

/** Interface for the Context.
	The classes that implementent this interface have a single instance.
	The difference with a singleton is that the instances needs to be created
	manually and that the current instance can be changed.
*/
template <class T> 
class Context
{
private:
	static T* _instance;

public:
	Context();

	virtual ~Context();

	/**
		Retrieves the pointer to the instance of this class.
		@return Instance of this class.
	*/
	static T* Current();

	/**
		Sets the pointer to the new active instance of this class.
		@param value Instance of this class.
	*/
	static void SetCurrent(T* value);

	/** Destroys the pointer of the current instance of this class.	*/
	static void DestroyCurrent();
};

#include "Context.inl"

template <class T> T* Context<T>::_instance = NULL;

}

#endif
