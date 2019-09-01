/*=============================================================================
Event.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE EventArgs::EventArgs()
{
}

SE_INLINE EventArgs::~EventArgs()
{
}


template <class T>
SE_INLINE EventFunctorSlot<T>::EventFunctorSlot(FunctorDelegate delegate)
{
	Delegate = delegate;
}

template <class T>
SE_INLINE bool EventFunctorSlot<T>::Invoke(Object* sender, const T& e)
{
	return (Delegate)(sender, e);
}


template <class T, class O>
SE_INLINE EventMethodSlot<T, O>::EventMethodSlot(O* slotClass, bool (O::*delegate)(Object*, const T&))
{
	SlotClass = slotClass;
	Delegate = delegate;
}

template <class T, class O>
SE_INLINE bool EventMethodSlot<T, O>::Invoke(Object* sender, const T& e)
{
	return (SlotClass->*Delegate)(sender, e);
}


template <class T>
SE_INLINE Event<T>::Event()
{
}

template <class T>
SE_INLINE Event<T>::Event(IEventSlot<T>* slot)
{
	_slots.AddLast(slot);
}

template <class T>
SE_INLINE Event<T>::~Event()
{
	EventSlotList::Iterator it = _slots.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

template <class T>
SE_INLINE bool Event<T>::Invoke(Object* sender, const T& e)
{
	EventSlotList::Iterator it = _slots.GetIterator();
	while (it.Next())
	{
		if (!(it.Current()->Invoke(sender, e)))
		{
			return false;
		}
	}

	return true;
}

template <class T>
SE_INLINE void Event<T>::Add(EventFunctorSlot<T>* slot)
{
	if (slot == NULL)
	{
		SEthrow(ArgumentNullException("slot"));
		return;
	}

	_slots.AddLast(slot);
}

template <class T>
SE_INLINE void Event<T>::Remove(EventFunctorSlot<T>* slot)
{
	if (slot == NULL)
	{
		SEthrow(ArgumentNullException("slot"));
		return;
	}

	_slots.Remove(slot);
}

template <class T>
SE_INLINE Event<T>& Event<T>::operator+=(EventFunctorSlot<T>* slot)
{
	Add(slot);
	return *this;
}

template <class T>
SE_INLINE Event<T>& Event<T>::operator-=(EventFunctorSlot<T>* slot)
{
	Remove(slot);
	return *this;
}
