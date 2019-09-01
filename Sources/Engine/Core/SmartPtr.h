/*=============================================================================
SmartPtr.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SMARTPTR_H_
#define _SE_SMARTPTR_H_

namespace SonataEngine
{

/**
	@brief Smart pointer.
	
	Referenced pointer.
*/
template <class T>
class SmartPtr
{
protected:
	T* _ptr;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Default constructor. */
	SmartPtr();

	/** Copy constructor. */
	SmartPtr(const SmartPtr<T>& ptr);

	/**
		Constructor.
		@param ptr A native pointer.
	*/
	SmartPtr(T* ptr);

	/** Destructor. */
	virtual ~SmartPtr();
	//@}

	SmartPtr<T>& operator=(const SmartPtr<T>& ptr);

	SmartPtr<T>& operator=(T* ptr);

	T* operator->() const;

	T& operator*() const;

	/*operator const T*() const;*/

	operator T*() const;

	operator bool() const;

	bool operator!() const;

	/*bool operator==(const T* ptr) const;

	bool operator==(T* ptr) const;

	bool operator!=(const T* ptr) const;

	bool operator!=(T* ptr) const;*/

	T* Get() const;
};

#include "SmartPtr.inl"

#define SEPointer(classname) \
	typedef SmartPtr<classname> classname##Ptr

}

#endif
