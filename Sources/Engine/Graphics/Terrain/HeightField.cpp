/*=============================================================================
HeightField.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "HeightField.h"

namespace SonataEngine
{

HeightField::HeightField() :
	RefObject(),
	_Width(0),
	_Height(0),
	_Format(HeightFieldFormat_Int8),
	_Data(NULL)
{
}

HeightField::~HeightField()
{
	Destroy();
}

int32 HeightField::GetSize() const
{
	int32 size = _Width * _Height;

	if (_Format == HeightFieldFormat_Int8)
		size *= sizeof(int8);
	if (_Format == HeightFieldFormat_Int16)
		size *= sizeof(int16);
	if (_Format == HeightFieldFormat_Int32)
		size *= sizeof(int32);
	else if (_Format == HeightFieldFormat_Real)
		size *= sizeof(real32);

	return size;
}

real32 HeightField::GetHeight(int32 x, int32 y) const
{
	if (!_Data || x >= _Width || y >= _Height)
		return 0;

	return _Data[_Width * y + x];
}

void HeightField::SetHeight(int32 x, int32 y, real32 height)
{
	if (!_Data || x >= _Width || y >= _Height)
		return;

	_Data[_Width * y + x] = height;
}

real32 HeightField::GetMinimumHeight() const
{
	if (!_Data)
		return 0.0f;

	int i;
	int size = _Width * _Height;
	real32 minimum = _Data[0];

	for (i = 1; i < size; ++i)
	{
		if (_Data[i] < minimum)
			minimum = _Data[i];
	}

	return minimum;
}

real32 HeightField::GetMaximumHeight() const
{
	if (!_Data)
		return 0.0f;

	int i;
	int size = _Width * _Height;
	real32 maximum = _Data[0];

	for (i = 1; i < size; ++i)
	{
		if (_Data[i] > maximum)
			maximum = _Data[i];
	}

	return maximum;
}

real32 HeightField::GetInterpolatedHeight(real32 x, real32 y) const
{
	real32 fX = x;
	real32 fY = y;

	int32 iX0 = Math::Round(fX);
	int32 iY0 = Math::Round(fY);

	fX -= iX0;
	fY -= iY0;

	iX0 = Math::Clamp(iX0, 0, _Width-1);
	iY0 = Math::Clamp(iY0, 0, _Height-1);

	int32 iX1 = Math::Clamp(iX0+1, 0, _Width-1);
	int32 iY1 = Math::Clamp(iY0+1, 0, _Height-1);

	real32 h0 = GetHeight(iX0, iY0);
	real32 h1 = GetHeight(iX1, iY0);
	real32 h2 = GetHeight(iX0, iY1);
	real32 h3 = GetHeight(iX1, iY1);

	// interpolate the heights along the X and Y axis
	real32 avgY = (h1*fX) + (h0*(1.0f-fX));
	real32 avgX = (h3*fX) + (h2*(1.0f-fX));

	// interpolate the resulting heights
	return (avgX*fY) + (avgY*(1.0f-fY));
}

void HeightField::Create(int32 width, int32 height, HeightFieldFormat format)
{
	if (_Data)
		Destroy();

	_Width = width;
	_Height = height;
	_Format = format;
	_Data = new real32[_Width * _Height];
}

void HeightField::Destroy()
{
	_Width = 0;
	_Height = 0;
	_Format = HeightFieldFormat_Int8;
	SE_DELETE_ARRAY(_Data);
}

bool HeightField::LoadFromRAW(Stream& stream, int32 width, int32 height, HeightFieldFormat format)
{
	Create(width, height, format);

	int i;
	int32 size = _Width * _Height;
	int32 element;

	if (format == HeightFieldFormat_Real)
	{
		if (stream.Read((SEbyte*)_Data, size * sizeof(real32)) != size * sizeof(real32))
			return false;
	}
	else
	{
		if (format == HeightFieldFormat_Int8)
			element = sizeof(int8);
		else if (format == HeightFieldFormat_Int16)
			element = sizeof(int16);
		else if (format == HeightFieldFormat_Int32)
			element = sizeof(int32);
		else
			return false;

		SEbyte* data = new SEbyte[size * element];
		if (stream.Read((SEbyte*)data, size * element) != size * element)
		{
			delete[] data;
			return false;
		}

		for (i = 0; i < size; ++i)
		{
			if (format == HeightFieldFormat_Int8)
				_Data[i] = (real32)*(int8*)&data[i*element];
			else if (format == HeightFieldFormat_Int16)
				_Data[i] = (real32)*(int16*)&data[i*element];
			else if (format == HeightFieldFormat_Int32)
				_Data[i] = (real32)*(int32*)&data[i*element];
		}

		delete[] data;
	}

	return true;
}

bool HeightField::SaveToRAW(Stream& stream, HeightFieldFormat format)
{
	if (!_Data)
		return false;

	int32 size = GetSize();

	if (stream.Write((SEbyte*)_Data, size) != size)
		return false;

	return true;
}

}
