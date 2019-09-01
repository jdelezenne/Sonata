/*=============================================================================
TerrainTextureGenerator.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TERRAINTEXTUREGENERATOR_H_
#define _SE_TERRAINTEXTUREGENERATOR_H_

#include "Core/Core.h"
#include "Graphics/Image.h"
#include "Graphics/Terrain/Terrain.h"
#include "Graphics/Terrain/HeightField.h"

namespace SonataEngine
{

/**
	@brief Terrain texture layer.

	A TerrainTextureGenerator object generates a blended texture from a set of
	TerrainTextureLayer objects. The properties of each layer determines
	its blending factor in the final texture.
*/
class SE_GRAPHICS_EXPORT TerrainTextureLayer : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	TerrainTextureLayer();

	/** Destructor. */
	virtual ~TerrainTextureLayer();
	//@}

	/** @name Properties. */
	//@{
	Image* GetImage() const { return _Image; }
	void SetImage(Image* value) { _Image = value; }

	const Vector2& GetScale() const { return _Scale; }
	void SetScale(const Vector2& value) { _Scale = value; }

	//0-255
	const RangeReal32& GetHeight() const { return _Height; }
	void SetHeight(const RangeReal32& value) { _Height = value; }

	//0-90
	const RangeReal32& GetSlope() const { return _Slope; }
	void SetSlope(const RangeReal32& value) { _Slope = value; }
	//@}

protected:
	Image* _Image;
	Vector2 _Scale;
	RangeReal32 _Height;
	RangeReal32 _Slope;
};

/**
	@brief Terrain texture generator.

	Generates a blended texture from an height field and a set of texture layers.
*/
class SE_GRAPHICS_EXPORT TerrainTextureGenerator
{
public:
	typedef Array<TerrainTextureLayer> TerrainTextureLayerList;

	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	TerrainTextureGenerator();

	/** Destructor. */
	virtual ~TerrainTextureGenerator();
	//@}

	/** @name Properties. */
	//@{
	Image* GetImage() const { return _Image; }

	int32 GetWidth() const { return _Width; }
	void SetWidth(int32 value) { _Width = value; }

	int32 GetHeight() const { return _Height; }
	void SetHeight(int32 value) { _Height = value; }

	Terrain* GetTerrain() const { return _Terrain; }
	void SetTerrain(Terrain* value) { _Terrain = value; }

	HeightField* GetHeightField() const { return _HeightField; }
	void SetHeightField(HeightField* value) { _HeightField = value; }

	const TerrainTextureLayerList& GetLayers() const { return _Layers; }
	void SetLayers(const TerrainTextureLayerList& value) { _Layers = value; }
	//@}

	/**
		Creates the image from the current layers.
		@remarks The current image will be destroyed.
		@return true if successful; otherwise, false.
	*/
	virtual bool Create();

	/** Destroys the current image. */
	void Destroy();

protected:
	Image* _Image;
	int32 _Width;
	int32 _Height;
	Terrain* _Terrain;
	HeightField* _HeightField;
	TerrainTextureLayerList _Layers;
};

}

#endif 
