/*=============================================================================
Interpolator.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INTERPOLATOR_H_
#define _SE_INTERPOLATOR_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

/** Base class for the interpolators.
	The GetValueAt() method must be overriden in derived classes.
*/
template <class T>
class Interpolator
{
public:
	/** @name Access Operators. */
	//@{
	/// Retrieves the current value at the specified time.
	const T& operator[](real time) const;
	//@}

	real GetStartTime() const { return _StartTime; }
	void SetStartTime(real value) { _StartTime = value; }

	real GetEndTime() const { return _EndTime; }
	void SetEndTime(real value) { _EndTime = value; }

	const T& GetStartValue() const { return _StartValue; }
	void SetStartValue(const T& value) { _StartValue = value; }

	const T& GetEndValue() const { return _EndValue; }
	void SetEndValue(const T& value) { _EndValue = value; }

	real GetTotalTime() const { return _EndTime - _StartTime; }
	void SetTotalTime(real value) { _EndTime = _StartTime + value; }

	real GetCurrentTime() const { return _currentTime; }
	void SetCurrentTime(real value) { _currentTime = value; }

	const T& GetCurrentValue() const { return _currentValue; }
	void SetCurrentValue(const T& value) { _currentValue = value; }

	/// Resets the times and values.
	void Reset(real startTime, real endTime, const T& startValue, const T& endValue);

	/// Updates the current value at the specified time.
	void Update(real time);

	/// Retrieves the current value at the specified time.
	virtual const T& GetValueAt(real time) const = 0;

protected:
	/** @name Constructors. */
	//@{
	Interpolator();
	Interpolator(real startTime, real endTime, const T& startValue, const T& endValue);
	Interpolator(const Interpolator& value);
	//@}

protected:
	real _StartTime;
	real _EndTime;
	T _StartValue;
	T _EndValue;
	real _currentTime;
	T _currentValue;
};


/** Linear interpolator. */
template <class T>
class LinearInterpolator : public Interpolator<T>
{
public:
	LinearInterpolator();
	LinearInterpolator(real startTime, real endTime, const T& startValue, const T& endValue);
	LinearInterpolator(const LinearInterpolator& value);

	virtual const T& GetValueAt(real time) const;
};


/** Quadratic interpolator. */
template <class T>
class QuadraticInterpolator : public Interpolator<T>
{
public:
	QuadraticInterpolator();
	QuadraticInterpolator(real startTime, real midTime, real endTime, const T& startValue, const T& endValue);
	QuadraticInterpolator(const QuadraticInterpolator& value);

	virtual const T& GetValueAt(real time) const;

protected:
	real _MidTime;
};


/** Cubic interpolator. */
template <class T>
class CubicInterpolator : public Interpolator<T>
{
public:
	CubicInterpolator();
	CubicInterpolator(real startTime, real midTime1, real midTime2, real endTime, const T& startValue, const T& endValue);
	CubicInterpolator(const CubicInterpolator& value);

	virtual const T& GetValueAt(real time) const;

protected:
	real _MidTime1;
	real _MidTime2;
};

#include "Interpolator.inl"

}

#endif
