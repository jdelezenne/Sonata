/*=============================================================================
CoreException.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AccessViolationException.h"
#include "IndexOutOfRangeException.h"
#include "DivideByZeroException.h"
#include "OutOfMemoryException.h"
#include "OverflowException.h"
#include "StackOverflowException.h"
#include "UnderflowException.h"

#include "ArgumentException.h"
#include "ArgumentNullException.h"
#include "ArgumentOutOfRangeException.h"
#include "ArithmeticException.h"
#include "IndexOutOfRangeException.h"
#include "InvalidOperationException.h"
#include "NotImplementedException.h"
#include "NotSupportedException.h"

namespace SonataEngine
{

String AccessViolationException::DefaultMessage = "The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";
String ArgumentException::DefaultMessage = "Value does not fall within the expected range.";
String ArgumentNullException::DefaultMessage = "Value cannot be null.";
String ArgumentOutOfRangeException::DefaultMessage = "Specified argument was out of the range of valid values.";
String ArithmeticException::DefaultMessage = "Overflow or underflow in the arithmetic operation.";
String DivideByZeroException::DefaultMessage = "Attempted to divide by zero.";
String IndexOutOfRangeException::DefaultMessage = "Index was outside the bounds of the array.";
String NotImplementedException::DefaultMessage = "The method or operation is not implemented.";
String NotSupportedException::DefaultMessage = "Specified method is not supported.";
String OutOfMemoryException::DefaultMessage = "Insufficient memory to continue the execution of the program.";
String OverflowException::DefaultMessage = "Arithmetic operation resulted in an overflow.";
String StackOverflowException::DefaultMessage = "Operation caused a stack overflow.";
String UnderflowException::DefaultMessage = "Arithmetic operation resulted in an underflow.";

}
