/*=============================================================================
TerrainTextureGenerator.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TerrainTextureGenerator.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/Terrain/Terrain.h"

namespace SonataEngine
{

TerrainTextureLayer::TerrainTextureLayer() :
	RefObject(),
	_Image(NULL),
	_Scale(Vector2::One),
	_Height(RangeReal32(0.0f, 1.0f)),
	_Slope(RangeReal32(0.0f, 90.0f))
{
}

TerrainTextureLayer::~TerrainTextureLayer()
{
}


TerrainTextureGenerator::TerrainTextureGenerator() :
	_Image(NULL),
	_Width(0),
	_Height(0),
	_Terrain(NULL),
	_HeightField(NULL)
{
}

TerrainTextureGenerator::~TerrainTextureGenerator()
{
	Destroy();
}

real32 GetWeight(real32 value, const RangeReal32& r)
{
	if (value < r.Min || value > r.Max)
	{
		return 0.0f;
	}

	real32 delta = r.Max - r.Min;
	real32 weight = value - r.Min;
	weight *= 1.0f / delta;
	weight -= 0.5f;
	weight *= 2.0f;
	weight = 1.0f - Math::Abs(weight);
	weight = Math::Clamp(weight, 0.0f, 1.0f);

	return weight;
}

bool TerrainTextureGenerator::Create()
{
	if (_Width == 0 || _Height == 0)
	{
		SEthrow(Exception("Invalid image size."));
		return false;
	}

	if (_Terrain == NULL)
	{
		SEthrow(Exception("No terrain specified."));
		return false;
	}

	if (_HeightField == NULL)
	{
		SEthrow(Exception("No height field specified."));
		return false;
	}

	if (_Layers.IsEmpty())
	{
		SEthrow(Exception("No layers specified."));
		return false;
	}

	Destroy();

	_Image = new Image();
	_Image->Create(PixelFormat_R8G8B8A8, _Width, _Height);

	int x, y;
	int i;
	int32 layerX, layerY;
	Color8 currentColor;
	Color32 totalColor;
	real32 fieldHeight;
	Vector3 fieldNormal;
	real32 altitudeWeight, slopeWeight, blend;

	// for each pixel of the texture
	for (y = 0; y < _Width; y++)
	{
		for (x = 0; x < _Height; x++)
		{
			totalColor = Color32::Black;

			// get the interpolated altitude and normal at this position
			fieldHeight = _HeightField->GetInterpolatedHeight(x, y);
			fieldNormal = _Terrain->GetTerrainRenderer()->GetInterpolatedNormal(x, y);

			// for each texture layer
			int count = _Layers.Count();
			for (i = 0; i < count; i++)
			{
				// check validity
				Image* layerImage = _Layers[i].GetImage();
				if (layerImage == NULL || layerImage->GetWidth() == 0 || layerImage->GetHeight() == 0)
					continue;

				// scale position
				layerX = _Layers[i].GetScale().X * (x % layerImage->GetWidth());
				layerY = _Layers[i].GetScale().Y * (y % layerImage->GetHeight());

				// get layer color
				currentColor = layerImage->GetRGB(layerX, layerY);

				// compute altitude weight and slope weight
				altitudeWeight = GetWeight(fieldHeight, _Layers[i].GetHeight());
				slopeWeight = GetWeight(fieldNormal.Y, RangeReal32(
					_Layers[i].GetSlope().Min/90.0f,
					_Layers[i].GetSlope().Max/90.0f));
				slopeWeight = 1.0f;

				// combine the weights
				blend = altitudeWeight * slopeWeight / count;

				// add new color
				totalColor.R += (real32)currentColor.R * blend;
				totalColor.G += (real32)currentColor.G * blend;
				totalColor.B += (real32)currentColor.B * blend;
			}

			// set new color to image
			totalColor.Clamp(0.0f, 255.0f);
			_Image->SetRGB(x, y, Color8((int8)totalColor.R, (int8)totalColor.G, (int8)totalColor.B));
		}
	}

	return true;
}

void TerrainTextureGenerator::Destroy()
{
	SE_DELETE(_Image);
}

}
