/*=============================================================================
LogFilter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LogFilter.h"

namespace SonataEngine
{

DefaultLogFilter::DefaultLogFilter() :
	_FilterMode(LogFilterMode_All),
	_SingleLevel(LogLevel::None),
	_ExcludedLevel(LogLevel::None),
	_MinimumLevel(LogLevel::None)
{
}

DefaultLogFilter::~DefaultLogFilter()
{
}

bool DefaultLogFilter::CanLog(const LogRecord& record) const
{
	if (_FilterMode == LogFilterMode_Single)
	{
		if (record.GetLevel().GetValue() != _SingleLevel.GetValue())
			return false;
	}
	else if (_FilterMode == LogFilterMode_Excluded)
	{
		if (record.GetLevel().GetValue() == _SingleLevel.GetValue())
			return false;
	}
	else if (_FilterMode == LogFilterMode_Minimum)
	{
		if (record.GetLevel().GetValue() < _MinimumLevel.GetValue())
			return false;
	}

#ifndef _DEBUG
	if (record.GetLevel().GetValue() == LogLevel::Debug.GetValue())
		return false;
#endif

	return true;
}

}
