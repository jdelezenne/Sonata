/*=============================================================================
Context.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T> 
Context<T>::Context()
{
}

template <class T> 
Context<T>::~Context()
{
}

template <class T> 
T* Context<T>::Current()
{
	return _instance;
}

template <class T> 
void Context<T>::SetCurrent(T* value)
{
	_instance = value;
}

template <class T> 
void Context<T>::DestroyCurrent()
{
	SE_DELETE(_instance);
}
