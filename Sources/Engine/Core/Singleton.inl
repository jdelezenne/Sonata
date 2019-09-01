/*=============================================================================
Singleton.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T> 
Singleton<T>::Singleton()
{
}

template <class T> 
Singleton<T>::~Singleton()
{
}

template <class T> 
T* Singleton<T>::Instance()
{
	if (_instance == NULL)
	{
		_instance = new T;
	}

	SE_ASSERT(_instance != NULL);
	return _instance;
}

template <class T> 
void Singleton<T>::DestroyInstance()
{
	SE_DELETE(_instance);
}
