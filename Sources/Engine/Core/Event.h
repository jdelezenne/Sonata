/*=============================================================================
Event.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EVENT_H_
#define _SE_EVENT_H_

#include "Core/Common.h"
#include "Core/Containers/List.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

class Object;

/** Base class for classes containing event data.
*/
class SE_CORE_EXPORT EventArgs
{
public:
	static const EventArgs& Empty;

	EventArgs();
	virtual ~EventArgs();
};

template <class T>
class IEventSlot
{
public:
	virtual bool Invoke(Object* sender, const T& e) = 0;
};

template <class T>
class EventFunctorSlot : public IEventSlot<T>
{
public:
	typedef bool (*FunctorDelegate) (Object*, const T&);

	EventFunctorSlot(FunctorDelegate delegate);

	virtual bool Invoke(Object* sender, const T& e);

	FunctorDelegate Delegate;
};

template <class T, class O>
class EventMethodSlot : public IEventSlot<T>
{
public:
	typedef bool (O::*MethodDelegate)(Object*, const T&);

	EventMethodSlot(O* slotClass, bool (O::*delegate)(Object*, const T&));

	virtual bool Invoke(Object* sender, const T& e);

	O* SlotClass;
	MethodDelegate Delegate;
};

/**
	@brief Event.
	Represents an event, which is a signal sent by an object.
	Event slots can be attached to an event in order to handle its invocation.
	When an event is invoked, all the slots attached to it are called.
*/
template <class T>
class Event
{
	typedef List<IEventSlot<T>*> EventSlotList;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Event();

	/** Constructor. */
	Event(IEventSlot<T>* slot);

	/** Destructor. */
	virtual ~Event();
	//@}

	/** @name Callbacks. */
	//@{
	/** Calls the callback functions.
		@param sender The source of the event.
		@param e A Parameter that contains the event data.
		@return FALSE if the event has been cancelled, TRUE otherwise.
	*/
	bool Invoke(Object* sender, const T& e);

	/** Adds a callback function to this event.
		@param delegate Function to add to this event.
	*/
	void Add(EventFunctorSlot<T>* slot);
	template <class O>
	void Add(EventMethodSlot<T, O>* slot)
	{
		if (slot == NULL)
		{
			SEthrow(ArgumentNullException("slot"));
			return;
		}

		_slots.AddLast(slot);
	}

	/** Removes a callback function from this event.
		@param slot Function to remove from this event.
	*/
	void Remove(EventFunctorSlot<T>* slot);
	template <class O>
	void Remove(EventMethodSlot<T, O>* slot)
	{
		if (slot == NULL)
		{
			SEthrow(ArgumentNullException("slot"));
			return;
		}

		_slots.Remove(slot);
	}
	//@}

	/** @name Operators. */
	//@{
	Event<T>& operator+=(EventFunctorSlot<T>* slot);
	template <class O>
	Event<T>& operator+=(EventMethodSlot<T, O>* slot)
	{
		Add(slot);
		return *this;
	}

	Event<T>& operator-=(EventFunctorSlot<T>* slot);
	template <class O>
	Event<T>& operator-=(EventMethodSlot<T, O>* slot)
	{
		Remove(slot);
		return *this;
	}
	//@}

protected:
	EventSlotList _slots;
};

#define SE_DEFINE_EVENT(eventname, eventargs) \
	typedef EventFunctorSlot<eventargs> eventname##EventDelegateF; \
	template <class O> \
	class eventname##EventDelegateM : public EventMethodSlot<eventargs, O> \
	{ \
	public: \
		eventname##EventDelegateM(O* slotClass, bool (O::*delegate)(Object*, const eventargs&)) \
			: EventMethodSlot(slotClass, delegate) \
		{ \
		} \
	}; \
	typedef Event<eventargs> eventname##Event;

#define SE_DEFINE_EVENTARGS(eventname) \
	SE_DEFINE_EVENT(eventname, eventname##EventArgs);

SE_DEFINE_EVENT(Default, EventArgs);

#include "Event.inl"

}

#endif 
