/*=============================================================================
HeightFieldGenerator.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "HeightFieldGenerator.h"
#include "Core/System/Memory.h"

namespace SonataEngine
{

HeightFieldGenerator::HeightFieldGenerator() :
	RefObject(),
	_HeightField(NULL),
	_Random(NULL)
{
}

HeightFieldGenerator::~HeightFieldGenerator()
{
}

bool HeightFieldGenerator::ConstantHeight(real32 height)
{
	if (_HeightField == NULL)
		return false;

	real32* data = _HeightField->GetData();
	int32 size = _HeightField->GetWidth() * _HeightField->GetHeight();

	for (int i = 0; i < size; ++i)
	{
		data[i] = height;
	}

	return true;
}

bool HeightFieldGenerator::RandomHeight(real32 min, real32 max)
{
	if (_HeightField == NULL)
		return false;

	if (_Random == NULL)
		return false;

	real32* data = _HeightField->GetData();
	int32 size = _HeightField->GetWidth() * _HeightField->GetHeight();

	for (int i = 0; i < size; ++i)
	{
		data[i] = _Random->RandomReal(min, max);
	}

	return true;
}

bool HeightFieldGenerator::Fault(TerrainFaultMethod method, int32 iterations, int32 minDelta, int32 maxDelta)
{
	// Adapted from Gems #1
	int x, y;
	real32 range;
	int32 x1, y1;
	int32 x2, y2;
	int dirX, dirY;

	if (_HeightField == NULL)
		return false;

	if (_Random == NULL)
		return false;

	if (iterations <= 0)
		return false;

	real32* data = _HeightField->GetData();
	int32 width = _HeightField->GetWidth();
	int32 height = _HeightField->GetHeight();

	// Clear the height field
	Memory::Zero(data, width * height * sizeof(real32));

	_Random->SetSeed((int32)((real64)TimeValue::GetTime() * 1000.0));

	for (int i = 0; i < iterations; ++i)
	{
		// Compute the height range
		range = (real32)(maxDelta - ((maxDelta-minDelta )*i) / iterations);

		// Create a random line by picking two random points
		x1 = _Random->RandomInt(width);
		y1 = _Random->RandomInt(height);
		
		// Check to make sure that the points are not the same
		do
		{
			x2 = _Random->RandomInt(width);
			y2 = _Random->RandomInt(height);
		} while (x2 == x1 && y2 == y1);

		dirX = x2 - x1;
		dirY = y2 - y1;

		// Loop through the points in the height field
		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				// Raise of lower the height depending on the position of the current point
				// from the side of the line.
				if (((x - x1) * dirY - (y - y1) * dirX) > 0)
					data[y*width+x] += range;
			}
		}
	}

	return true;
}

bool HeightFieldGenerator::Midpoint(int32 scale)
{
	// Adapted from Gems #1
	int i,j,ni,nj,mi,mj,pmi,pmj,rectSize;

	if (_HeightField == NULL)
		return false;

	if (_Random == NULL)
		return false;

	if (_HeightField->GetWidth() != _HeightField->GetHeight())
	{
		Logger::Current()->Log(LogLevel::Error, _T("HeightFieldGenerator.Midpoint"),
			_T("The height field must be squared."));
		return false;
	}

	real32* data = _HeightField->GetData();
	int32 size = _HeightField->GetWidth();

	// Clear the height field
	Memory::Zero(data, size * size * sizeof(real32));

	_Random->SetSeed((int32)((real64)TimeValue::GetTime() * 1000.0));

	data[0] = 0;
	rectSize = size;
	real32 dh = (real32)rectSize/2,r = (real32)Math::Pow(2,-1*scale);

	while (rectSize > 0)
	{
		for (i=0; i<size; i+=rectSize)
		{
			for (j=0; j<size; j+=rectSize)
			{
				ni = (i+rectSize)%size;
				nj = (j+rectSize)%size;

				mi = (i+rectSize/2);
				mj = (j+rectSize/2);

				data[mi+mj*size] = (data[i+j*size] +
					data[ni+j*size] +
					data[i+nj*size] +
					data[ni+nj*size])/4 +
					_Random->RandomReal(-dh/2,dh/2);
			}
		}

		for (i=0; i<size; i+=rectSize)
		{
			for (j=0; j<size; j+=rectSize)
			{
				ni = (i+rectSize)%size;
				nj = (j+rectSize)%size;

				mi = (i+rectSize/2);
				mj = (j+rectSize/2);

				pmi = (i-rectSize/2+size)%size;
				pmj = (j-rectSize/2+size)%size;

				// top
				data[mi+j*size] = (data[i+j*size] +
					data[ni+j*size] +
					data[mi+pmj*size] +
					data[mi+mj*size])/4 +
					_Random->RandomReal(-dh/2,dh/2);

				
				// left
				data[i+mj*size] = (data[i+j*size] +
					data[i+nj*size] +
					data[pmi+mj*size] +
					data[mi+mj*size])/4 +
					_Random->RandomReal(-dh/2,dh/2);
			}
		}

		rectSize /= 2;
		dh *= r;
	}

	return true;
}

}
