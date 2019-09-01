/*=============================================================================
Interpolator.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class SE_CORE_EXPORT T>
SE_INLINE Interpolator<T>::Interpolator() :
	_StartTime((real)0.0),
	_EndTime((real)0.0),
	_StartValue(T()),
	_EndValue(T()),
	_currentTime((real)0.0),
	_currentValue(T())
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE Interpolator<T>::Interpolator(real startTime, real endTime, const T& startValue, const T& endValue) :
	_StartTime(startTime),
	_EndTime(endTime),
	_StartValue(startValue),
	_EndValue(endValue),
	_currentTime(startTime),
	_currentValue(startValue)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE Interpolator<T>::Interpolator(const Interpolator& value) :
	_StartTime(_StartTime),
	_EndTime(_EndTime),
	_StartValue(_StartValue),
	_EndValue(_EndValue),
	_currentTime(_currentTime),
	_currentValue(_currentValue)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE const T& Interpolator<T>::operator[](real time) const
{
	return GetValueAt(index);
}

template <class SE_CORE_EXPORT T>
SE_INLINE void Interpolator<T>::Reset(real startTime, real endTime, const T& startValue, const T& endValue)
{
	_StartTime = startTime;
	_EndTime = endTime;
	_StartValue = startValue;
	_EndValue = endValue;

	_currentTime = startTime;
	_currentValue = startValue;
}

template <class SE_CORE_EXPORT T>
SE_INLINE void Interpolator<T>::Update(real time)
{
	_currentTime = time;
	_currentValue = GetValueAt(time);
}


template <class SE_CORE_EXPORT T>
SE_INLINE LinearInterpolator<T>::LinearInterpolator() :
	Interpolator(value)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE LinearInterpolator<T>::LinearInterpolator(real startTime, real endTime, const T& startValue, const T& endValue) :
	Interpolator(startTime, endTime, startValue, endValue)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE LinearInterpolator<T>::LinearInterpolator(const LinearInterpolator& value) :
	Interpolator(value)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE const T& LinearInterpolator<T>::GetValueAt(real time) const
{
	if (time == _currentTime)
		return _currentValue;

	if (time <= _StartTime)
	{
		return _StartValue;
	}
	else if (time >= _EndTime)
	{
		return _EndValue;
	}
	else
	{
		real delta = (real)((time - _StartTime) / GetTotalTime());

		// start * a + end * b
		return (_StartValue * (1 - delta) + _EndValue * delta);
	}
}


template <class SE_CORE_EXPORT T>
SE_INLINE QuadraticInterpolator<T>::QuadraticInterpolator() :
	Interpolator(value),
	_MidTime((real)0.0)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE QuadraticInterpolator<T>::QuadraticInterpolator(real startTime, real midTime, real endTime, const T& startValue, const T& endValue) :
	Interpolator(startTime, endTime, startValue, endValue),
	_MidTime(midTime)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE QuadraticInterpolator<T>::QuadraticInterpolator(const QuadraticInterpolator& value) :
	Interpolator(value),
	_MidTime(value._MidTime)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE const T& QuadraticInterpolator<T>::GetValueAt(real time) const
{
	if (time == _currentTime)
		return _currentValue;

	if (time <= _StartTime)
	{
		return _StartValue;
	}
	else if (time >= _EndTime)
	{
		return _EndValue;
	}
	else
	{
		real deltaB = (real)((time - _StartTime) / GetTotalTime());
		real deltaA = (real)1.0 - deltaB;

		// start * a^2 +  mid * 2ab + end * b^2
		return (_StartValue * deltaA * deltaA +
			_MidTime * deltaA * deltaB + _EndValue * deltaB * deltaB);
	}
}


template <class SE_CORE_EXPORT T>
SE_INLINE CubicInterpolator<T>::CubicInterpolator() :
	Interpolator(value),
	_MidTime1((real)0.0),
	_MidTime2((real)0.0)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE CubicInterpolator<T>::CubicInterpolator(real startTime, real midTime1, real midTime2, real endTime, const T& startValue, const T& endValue) :
	Interpolator(startTime, endTime, startValue, endValue),
	_MidTime1(midTime1),
	_MidTime2(midTime2)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE CubicInterpolator<T>::CubicInterpolator(const CubicInterpolator& value) :
	Interpolator(value),
	_MidTime1(value._MidTime1),
	_MidTime2(value._MidTime2)
{
}

template <class SE_CORE_EXPORT T>
SE_INLINE const T& CubicInterpolator<T>::GetValueAt(real time) const
{
	if (time == _currentTime)
		return _currentValue;

	if (time <= _StartTime)
	{
		return _StartValue;
	}
	else if (time >= _EndTime)
	{
		return _EndValue;
	}
	else
	{
		real delta = (real)((time - _StartTime) / GetTotalTime());
		T startValueCube = (_StartValue * _StartValue * _StartValue);
		T endValueCube = (_EndValue * _EndValue * _EndValue);

		real deltaB = (real)((time - _StartTime) / GetTotalTime());
		real deltaA = (real)1.0 - deltaB;

		// start * a^3 +  mid1 * 3(a^2)b +  mid2 * 3a(b^2) + end * b^3
		return (_StartValue * deltaA * deltaA * deltaA +
			_MidTime1 * (real)3.0 * deltaA * deltaA * deltaB +
			_MidTime2 * (real)3.0 * deltaA * deltaB * deltaB +
			_EndValue * deltaB * deltaB * deltaB);
	}
}
