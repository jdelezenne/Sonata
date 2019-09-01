/*=============================================================================
RefCounter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_REFCOUNTER_H_
#define _SE_REFCOUNTER_H_

#include "Core/Common.h"
#include "Core/IRefCounter.h"

namespace SonataEngine
{

/**
	@brief Reference counter object.

	Referenced object.
*/
class SE_CORE_EXPORT RefCounter : public IRefCounter
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	RefCounter();

	/** Destructor. */
	virtual ~RefCounter();
	//@}

	/** @name Reference */
	//@{
	/**
		Gets the reference value.
		@return Reference value.
		@see IsFree
	*/
	virtual uint32 GetRefCounter() const;

	/**
		Returns whether the counter is free.
		@return true if the counter is free; otherwise, false.
		@see GetRefCounter
	*/
	virtual bool IsFree() const;


	/**
		Increments the counter.
		@return New counter reference value.
		@see IsFree
	*/
	virtual uint32 AddRef();

	/**
		Decrements the counter.
		@return New counter reference value.
		@see IsFree
	*/
	virtual uint32 Release();

protected:
	uint32 _reference;
};

#include "RefCounter.inl"

}

#endif
