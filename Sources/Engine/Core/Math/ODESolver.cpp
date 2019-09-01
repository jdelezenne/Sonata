/*=============================================================================
ODESolver.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODESolver.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

ODESolver::ODESolver(int32 dimension, ODEFunction fnDerive, const void* userData) :
	_dimension(dimension),
	_fnDerive(fnDerive),
	_userData(userData)
{
}

ODESolver::~ODESolver()
{
}


ODESolverEuler::ODESolverEuler(int32 dimension, ODEFunction fnDerive, const void* userData) :
	ODESolver(dimension, fnDerive, userData)
{
	if (dimension < 0)
	{
		SEthrow(ArgumentNullException("dimension"));
		return;
	}

	_dy = new real[dimension];
}

ODESolverEuler::~ODESolverEuler()
{
	SE_DELETE_ARRAY(_dy);
}

real ODESolverEuler::Update(const real* y0, real* y1, real t0, real t1)
{
	real delta;
	int i;

	delta = t1 - t0;

	_fnDerive(t0, y0, _dy, _userData);
	for (i = 0; i < _dimension; i++)
	{
		y1[i] = y0[i] + delta * _dy[i];
	}

	return delta;
}


ODESolverMidpoint::ODESolverMidpoint(int32 dimension, ODEFunction fnDerive, const void* userData) :
	ODESolver(dimension, fnDerive, userData)
{
	if (dimension < 0)
	{
		SEthrow(ArgumentNullException("dimension"));
		return;
	}

	_dy = new real[dimension];
	_dtmp = new real[dimension];
}

ODESolverMidpoint::~ODESolverMidpoint()
{
	SE_DELETE_ARRAY(_dy);
	SE_DELETE_ARRAY(_dtmp);
}

real ODESolverMidpoint::Update(const real* y0, real* y1, real t0, real t1)
{
	real delta, halfDelta;
	int i;

	delta = t1 - t0;
	halfDelta = delta * (real)0.5;

	// first step
	_fnDerive(t0, y0, _dy, _userData);
	for (i = 0; i < _dimension; i++)
	{
		_dtmp[i] = y0[i] + halfDelta * _dy[i];
	}

	// second step
	_fnDerive(t0 + halfDelta, _dtmp, _dy, _userData);
	for (i = 0; i < _dimension; i++)
	{
		y1[i] = y0[i] + delta * _dy[i];
	}

	return delta;
}


ODESolverRungeKutta4::ODESolverRungeKutta4(int32 dimension, ODEFunction fnDerive, const void* userData) :
	ODESolver(dimension, fnDerive, userData)
{
	if (dimension < 0)
	{
		SEthrow(ArgumentNullException("dimension"));
		return;
	}

	_dy = new real[dimension];
	_dtmp = new real[dimension];
	_d1 = new real[dimension];
	_d2 = new real[dimension];
	_d3 = new real[dimension];
	_d4 = new real[dimension];
}

ODESolverRungeKutta4::~ODESolverRungeKutta4()
{
	SE_DELETE_ARRAY(_dy);
	SE_DELETE_ARRAY(_dtmp);
	SE_DELETE_ARRAY(_d1);
	SE_DELETE_ARRAY(_d2);
	SE_DELETE_ARRAY(_d3);
	SE_DELETE_ARRAY(_d4);
}

real ODESolverRungeKutta4::Update(const real* y0, real* y1, real t0, real t1)
{
	real delta, halfDelta, sixthDelta;
	int i;

	delta = t1 - t0;
	halfDelta = delta * (real)0.5;
	sixthDelta = delta * (real)(1.0/6.0);

	// first step
	_fnDerive(t0, y0, _d1, _userData);
	for (i = 0; i < _dimension; i++)
	{
		_dtmp[i] = y0[i] + halfDelta * _d1[i];
	}

	// second step
	_fnDerive(t0 + halfDelta, _dtmp, _d2, _userData);
	for (i = 0; i < _dimension; i++)
	{
		_dtmp[i] = y0[i] + halfDelta * _d2[i];
	}

	// third step
	_fnDerive(t0 + halfDelta, _dtmp, _d3, _userData);
	for (i = 0; i < _dimension; i++)
	{
		_dtmp[i] = y0[i] + delta * _d3[i];
	}

	// fourth step
	_fnDerive(t0 + delta, _dtmp, _d4, _userData);
	for (i = 0; i < _dimension; i++)
	{
		y1[i] = y0[i] + sixthDelta * (_d1[i] + (real)2.0 * (_d2[i] + _d3[i]) + _d4[i]);
	}

	return delta;
}

}
