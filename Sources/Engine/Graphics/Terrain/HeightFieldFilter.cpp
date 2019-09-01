/*=============================================================================
HeightFieldFilter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "HeightFieldFilter.h"

namespace SonataEngine
{

HeightFieldFilter::HeightFieldFilter() :
	RefObject()
{
}

HeightFieldFilter::~HeightFieldFilter()
{
}

bool HeightFieldFilter::Normalize()
{
	if (_HeightField == NULL)
		return false;

	real32 minimum = _HeightField->GetMinimumHeight();
	real32 maximum = _HeightField->GetMaximumHeight();

	if (Math::Equals(minimum, maximum, Math::Epsilon))
		return true;

	int32 size = _HeightField->GetWidth() * _HeightField->GetHeight();
	real32* data = _HeightField->GetData();

	for (int i = 0; i < size; ++i)
	{
		data[i] = (data[i] - minimum) / (maximum - minimum);
	}

	return true;
}

bool HeightFieldFilter::Scale(real32 scale)
{
	if (_HeightField == NULL)
		return false;

	int32 size = _HeightField->GetWidth() * _HeightField->GetHeight();
	real32* data = _HeightField->GetData();

	for (int i = 0; i < size; ++i)
	{
		data[i] = data[i] * scale;
	}

	return true;
}

bool HeightFieldFilter::Invert()
{
	if (_HeightField == NULL)
		return false;

	int32 size = _HeightField->GetWidth() * _HeightField->GetHeight();
	real32* data = _HeightField->GetData();

	for (int i = 0; i < size; ++i)
	{
		data[i] = 1.0f - data[i];
	}

	return true;
}

void HeightFieldFilter::_ErosionLine(real32* line, int32 stride, int32 count, real32 filter)
{
	float v = line[0];
	int j = stride;
	int i;

	//go through the height line and apply the erosion filter
	for (i = 0; i < count-1; i++)
	{
		line[j] = filter*v + (1-filter)*line[j];
		
		v = line[j];
		j += stride;
	}
}


void HeightFieldFilter::Erosion(real32 filter)
{
	if (_HeightField == NULL)
		return;

	int i;
	int32 width = _HeightField->GetWidth();
	int32 height = _HeightField->GetHeight();
	real32* data = _HeightField->GetData();

	//erode left to right
	for (i = 0; i < height; i++)
		_ErosionLine(&data[width*i], 1, width, filter);
	
	//erode right to left
	for (i = 0; i < height; i++)
		_ErosionLine(&data[width*i+width-1], -1, width, filter);

	//erode top to bottom
	for (i = 0; i < width; i++)
		_ErosionLine(&data[i], width, height, filter);

	//erode from bottom to top
	for (i = 0; i < width; i++)
		_ErosionLine(&data[height*(width-1)+i], -width, height, filter);
}

}
