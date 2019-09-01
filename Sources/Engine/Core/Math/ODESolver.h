/*=============================================================================
ODESolver.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ODESOLVER_H_
#define _SE_ODESOLVER_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

typedef void (*ODEFunction)(real t, const real* state, real* deltaState, const void* userData);

/** Base class for the numerical solvers for ordinary differential equations.
	The Update() method must be overriden in derived classes.
	The user derive function and user data must be supplied at each update.
*/
class SE_CORE_EXPORT ODESolver
{
public:
	/** Destructor. */
	virtual ~ODESolver();

	/** Updates the current state at the specified times.
		@return The delta time value.
	*/
	virtual real Update(const real* y0, real* y1, real t0, real t1) = 0;

protected:
	/** Constructor. */
	ODESolver(int32 dimension, ODEFunction derive, const void* userData);

protected:
	int32 _dimension;
	ODEFunction _fnDerive;
	const void* _userData;
};


/** Euler solver.
	@remark The fastest and least stable ODE solver.
*/
class SE_CORE_EXPORT ODESolverEuler : public ODESolver
{
public:
	ODESolverEuler(int32 dimension, ODEFunction fnDerive, const void* userData);

	virtual ~ODESolverEuler();

	virtual real Update(const real* y0, real* y1, real t0, real t1);

protected:
	real* _dy;
};


/** Midpoint solver.
	@remark Better than Euler but not as stable as Runge-Kutta.
*/
class SE_CORE_EXPORT ODESolverMidpoint : public ODESolver
{
public:
	ODESolverMidpoint(int32 dimension, ODEFunction fnDerive, const void* userData);

	virtual ~ODESolverMidpoint();

	virtual real Update(const real* y0, real* y1, real t0, real t1);

protected:
	real* _dy;
	real* _dtmp;
};


/** Runge-Kutta 4 solver.
	@remark Good stability / speed trade-off.
*/
class SE_CORE_EXPORT ODESolverRungeKutta4 : public ODESolver
{
public:
	ODESolverRungeKutta4(int32 dimension, ODEFunction fnDerive, const void* userData);

	virtual ~ODESolverRungeKutta4();

	virtual real Update(const real* y0, real* y1, real t0, real t1);

protected:
	real* _dy;
	real* _dtmp;
	real* _d1;
	real* _d2;
	real* _d3;
	real* _d4;
};

}

#endif
