/*=============================================================================
SmartPtr.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T> 
SmartPtr<T>::SmartPtr() :
	_ptr(NULL)
{
}

template <class T> 
SmartPtr<T>::SmartPtr(const SmartPtr<T>& ptr) :
	_ptr(ptr._ptr)
{
	if (_ptr != NULL)
	{
		_ptr->AddRef();
	}
}

template <class T> 
SmartPtr<T>::SmartPtr(T* ptr) :
	_ptr(ptr)
{
	if (_ptr != NULL)
	{
		_ptr->AddRef();
	}
}

template <class T> 
SmartPtr<T>::~SmartPtr()
{
	if (_ptr != NULL)
	{
		_ptr->Release();
		_ptr = NULL;
	}
}

template <class T> 
SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& ptr)
{
	if (_ptr != ptr._ptr)
	{
		if (ptr._ptr != NULL)
		{
			ptr._ptr->AddRef();
		}
		if (_ptr != NULL)
		{
			_ptr->Release();
		}
	}

	_ptr = ptr._ptr;
	return *this;
}

template <class T> 
SmartPtr<T>& SmartPtr<T>::operator=(T* ptr)
{
	if (_ptr != ptr)
	{
		if (ptr != NULL)
		{
			ptr->AddRef();
		}
		if (_ptr != NULL)
		{
			_ptr->Release();
		}
	}

	_ptr = ptr;
	return *this;
}

template <class T> 
T* SmartPtr<T>::operator->() const
{
	return _ptr;
}

template <class T> 
T& SmartPtr<T>::operator*() const
{
	return *_ptr;
}

/*template <class T> 
SmartPtr<T>::operator const T*() const
{
	return _ptr;
}*/

template <class T> 
SmartPtr<T>::operator T*() const
{
	return _ptr;
}

template <class T> 
SmartPtr<T>::operator bool() const
{
	return (_ptr != NULL);
}

template <class T> 
bool SmartPtr<T>::operator!() const
{
	return (_ptr == NULL);
}

/*template <class T> 
bool SmartPtr<T>::operator==(const T* ptr) const
{
	return _ptr == ptr;
}

template <class T> 
bool SmartPtr<T>::operator==(T* ptr) const
{
	return _ptr == ptr;
}

template <class T> 
bool SmartPtr<T>::operator!=(const T* ptr) const
{
	return _ptr == ptr;
}

template <class T> 
bool SmartPtr<T>::operator!=(T* ptr) const
{
	return _ptr == ptr;
}*/

template <class T> 
T* SmartPtr<T>::Get() const
{
	return _ptr;
}


template <class TA, class TB>
bool operator==(const SmartPtr<TA>& left, const SmartPtr<TB>& right)
{
	return (left.Get() == right.Get());
}

template <class TA, class TB>
bool operator!=(const SmartPtr<TA>& left, const SmartPtr<TB>& right)
{
	return (left.Get() != right.Get());
}

/*template <class T>
bool operator==(const SmartPtr<T>& left, int right)	
{
	return !(bool)p1;	
}

template <class T>
bool operator!=(const SmartPtr<T>& left, int right)
{
	return (bool)p1;	
}

template <class T>
bool operator==(int left, const SmartPtr<T>& right)
{
	return !(bool)p1;	
}

template <class T>
bool operator!=(int left, const SmartPtr<T>& right)
{
	return (bool)p1;	
}*/
