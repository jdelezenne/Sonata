/*=============================================================================
RefCounter.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE RefCounter::RefCounter()
{
	_reference = 0;
}

SE_INLINE RefCounter::~RefCounter()
{
	SE_ASSERT(_reference == 0);
}

SE_INLINE uint32 RefCounter::GetRefCounter() const
{
	return _reference;
}

SE_INLINE bool RefCounter::IsFree() const
{
	SE_ASSERT(_reference >= 0);

	return _reference == 0;
}

SE_INLINE uint32 RefCounter::AddRef()
{
	_reference++;

	return _reference;
}

SE_INLINE uint32 RefCounter::Release()
{
	_reference--;
	SE_ASSERT(_reference >= 0);

	if (_reference == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return _reference;
	}
}
