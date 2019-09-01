/*=============================================================================
Singleton.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SINGLETON_H_
#define _SE_SINGLETON_H_

#include "Core/Common.h"

namespace SonataEngine
{

/**
	@brief Singleton.

	Interface for the Singleton pattern.
	The classes that implementent this interface have a single instance.
*/
template <class T> 
class Singleton
{
private:
	static T* _instance;

public:
	Singleton();

	virtual ~Singleton();

	/**
		Retrieves the pointer to the instance of this class.
		@return Instance of this class.
	*/
	static T* Instance();

	/** Destroys the pointer of the instance of this class.	*/
	static void DestroyInstance();
};

#include "Singleton.inl"

template <class T> T* Singleton<T>::_instance = NULL;

}

#endif
