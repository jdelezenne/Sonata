/*=============================================================================
Win32Exception.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/Exception/Exception.h"
#include "Core/Exception/AccessViolationException.h"
#include "Core/Exception/IndexOutOfRangeException.h"
#include "Core/Exception/DivideByZeroException.h"
#include "Core/Exception/OutOfMemoryException.h"
#include "Core/Exception/OverflowException.h"
#include "Core/Exception/StackOverflowException.h"
#include "Core/Exception/UnderflowException.h"

namespace SonataEngine
{

static void my_translator(unsigned code, EXCEPTION_POINTERS *info)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		throw SonataEngine::AccessViolationException();
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		throw SonataEngine::IndexOutOfRangeException();
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		throw SonataEngine::DivideByZeroException();
		break;
	case EXCEPTION_FLT_OVERFLOW:
		throw SonataEngine::OverflowException();
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		throw SonataEngine::UnderflowException();
		break;
	case EXCEPTION_STACK_OVERFLOW:
		throw SonataEngine::StackOverflowException();
		break;
	default:
		throw SonataEngine::Exception();
		break;
	}
}

class W32SEH
{
public:
	W32SEH(EXCEPTION_POINTERS const& info) throw();
	static void Install() throw();
	virtual char const* What() const throw();
	void const* Where() const throw();

private:
	void const* _address;
};

W32SEH::W32SEH(EXCEPTION_POINTERS const& info) throw()
{
	EXCEPTION_RECORD const& exception = *(info.ExceptionRecord);
	_address = exception.ExceptionAddress;
}

void W32SEH::Install() throw()
{
	_set_se_translator(my_translator);
}

char const* W32SEH::What() const throw()
{
	return "Unspecified Structured Exception";
}

void const* W32SEH::Where() const throw()
{
	return _address;
}

}
