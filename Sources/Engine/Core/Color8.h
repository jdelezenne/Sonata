/*=============================================================================
Color8.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_COLOUR8_H_
#define _SE_COLOUR8_H_

#include "Core/Common.h"

namespace SonataEngine
{

/**
	@brief Color.

	A color described by 4 byte components in an RGBA order.
*/
class SE_CORE_EXPORT Color8
{
public:
	union
	{
		struct
		{
			uint8 R;
			uint8 G;
			uint8 B;
			uint8 A;
		};
		uint8 Data[4];
	};

public:
	/** @name Constructors */
	//@{
	/** Creates an empty value. */
	Color8();

	/** Creates a color from its components.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
	*/
	Color8(uint8 red, uint8 green, uint8 blue, uint8 alpha = 255);

	/** Creates a color from an RGBA value.
		@param value An RGBA value.
		@param isRGBA true if the value is an RGBA value, false if it is an ARGB value.
	*/
	Color8(uint32 value, bool isRGBA = false);

	/**
		Creates a color from an array of 4 color components in RGBA or ARGB order.
		@param value Array of 4 color components.
		@param isRGBA true if the value is an RGBA value, false if it is an ARGB value.
	*/
	Color8(const uint8* value, bool isRGBA = false);

	/**
		Copy constructor.
		@param color Color to copy.
	*/
	Color8(const Color8& value);
	//@}

	/**
		Sets new values for the components of this value.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
	*/
	void Set(uint8 red, uint8 green, uint8 blue, uint8 alpha = 255);

	/** Assignment Operator. */
	Color8& operator=(const Color8& value);

	/** Creates a color from Real32 values.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
		@return The value.
	*/
	static Color8 FromReal32(real32 red, real32 green, real32 blue, real32 alpha);

	/** Creates a color from an RGBA value.
		@param value An RGBA value.
		@return The value.
	*/
	static Color8 FromRGBA(uint32 value);

	/** Creates a color from an ARGB value.
		@param value An ARGB value.
		@return The value.
	*/
	static Color8 FromARGB(uint32 value);

	/**
		Gets the 32-bit RGBA value of this Color.
		@return The 32-bit RGBA value of this Color.
	*/
	uint32 ToRGBA() const;

	/**
		Gets the 32-bit ARGB value of this Color.
		@return The 32-bit ARGB value of this Color.
	*/
	uint32 ToARGB() const;


	/** @name Casting Operators. */
	//@{
	operator uint32() const;

	operator uint8*();
	operator const uint8*() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	Color8& operator+=(const Color8& value);
	Color8& operator-=(const Color8& value);
	Color8& operator*=(uint8 value);
	Color8& operator/=(uint8 value);
	//@}

	/** @name Unary Operators. */
	//@{
	Color8 operator+() const;
	Color8 operator-() const;
	//@}

	/** @name Binary Operators. */
	//@{
	Color8 operator+(const Color8& value) const;
	Color8 operator-(const Color8& value) const;
	Color8 operator*(uint8 value) const;
	Color8 operator/(uint8 value) const;

	bool operator==(const Color8& value) const;
	bool operator!=(const Color8& value) const;
	//@}

	friend Color8 operator*(real32, const Color8&);

	/** (255-r, 255-g, 255-b, a) */
	void Negative();
	static Color8 Negative(const Color8& value);

	void Add(const Color8& value);
	static Color8 Add(const Color8& left, const Color8& right);

	void Subtract(const Color8& value);
	static Color8 Subtract(const Color8& left, const Color8& right);

	void Clamp(uint8 min, uint8 max);
	static Color8 Clamp(const Color8& color, uint8 min, uint8 max);

	void Scale(uint8 s);
	static Color8 Scale(const Color8& color, uint8 s);

	/** (r1*r2, g1*g2, b1*b2, a1*a2). */
	static Color8 Modulate(const Color8& left, const Color8& right);

	/**
		Linear interpolation of R, G, B, and A.
		Out = value1 + amount * (value2 - value1).
	*/
	static Color8 Lerp(const Color8& value1, const Color8& value2, real amount);

	/**
		Interpolate r,g,b between desaturated color and value.
		DesaturatedColor + s(Color - DesaturatedColor)
	*/
	void AdjustSaturation(uint8 s);
	static Color8 AdjustSaturation(const Color8& color, uint8 s);

	/**
		Interpolate r,g,b between 50% grey and value.
		Grey + s(Color - Grey).
	*/
	void AdjustContrast(uint8 c);
	static Color8 AdjustContrast(const Color8& color, uint8 c);

private:
	/// Color Component Shifts.
	static const int32 ARGBAlphaShift = 24;
    static const int32 ARGBRedShift = 16;
    static const int32 ARGBGreenShift = 8;
    static const int32 ARGBBlueShift = 0;

    static const int32 RGBARedShift = 24;
    static const int32 RGBAGreenShift = 16;
    static const int32 RGBABlueShift = 8;
	static const int32 RGBAAlphaShift = 0;

public:
	/// From .NET Types
	static const Color8 Transparent;
	static const Color8 AliceBlue;
	static const Color8 AntiqueWhite;
	static const Color8 Aqua;
	static const Color8 Aquamarine;
	static const Color8 Azure;
	static const Color8 Beige;
	static const Color8 Bisque;
	static const Color8 Black;
	static const Color8 BlanchedAlmond;
	static const Color8 Blue;
	static const Color8 BlueViolet;
	static const Color8 Brown;
	static const Color8 BurlyWood;
	static const Color8 CadetBlue;
	static const Color8 Chartreuse;
	static const Color8 Chocolate;
	static const Color8 Coral;
	static const Color8 CornflowerBlue;
	static const Color8 Cornsilk;
	static const Color8 Crimson;
	static const Color8 Cyan;
	static const Color8 DarkBlue;
	static const Color8 DarkCyan;
	static const Color8 DarkGoldenrod;
	static const Color8 DarkGray;
	static const Color8 DarkGreen;
	static const Color8 DarkKhaki;
	static const Color8 DarkMagenta;
	static const Color8 DarkOliveGreen;
	static const Color8 DarkOrange;
	static const Color8 DarkOrchid;
	static const Color8 DarkRed;
	static const Color8 DarkSalmon;
	static const Color8 DarkSeaGreen;
	static const Color8 DarkSlateBlue;
	static const Color8 DarkSlateGray;
	static const Color8 DarkTurquoise;
	static const Color8 DarkViolet;
	static const Color8 DeepPink;
	static const Color8 DeepSkyBlue;
	static const Color8 DimGray;
	static const Color8 DodgerBlue;
	static const Color8 Firebrick;
	static const Color8 FloralWhite;
	static const Color8 ForestGreen;
	static const Color8 Fuchsia;
	static const Color8 Gainsboro;
	static const Color8 GhostWhite;
	static const Color8 Gold;
	static const Color8 Goldenrod;
	static const Color8 Gray;
	static const Color8 Green;
	static const Color8 GreenYellow;
	static const Color8 Honeydew;
	static const Color8 HotPink;
	static const Color8 IndianRed;
	static const Color8 Indigo;
	static const Color8 Ivory;
	static const Color8 Khaki;
	static const Color8 Lavender;
	static const Color8 LavenderBlush;
	static const Color8 LawnGreen;
	static const Color8 LemonChiffon;
	static const Color8 LightBlue;
	static const Color8 LightCoral;
	static const Color8 LightCyan;
	static const Color8 LightGoldenrodYellow;
	static const Color8 LightGray;
	static const Color8 LightGreen;
	static const Color8 LightPink;
	static const Color8 LightSalmon;
	static const Color8 LightSeaGreen;
	static const Color8 LightSkyBlue;
	static const Color8 LightSlateGray;
	static const Color8 LightSteelBlue;
	static const Color8 LightYellow;
	static const Color8 Lime;
	static const Color8 LimeGreen;
	static const Color8 Linen;
	static const Color8 Magenta;
	static const Color8 Maroon;
	static const Color8 MediumAquamarine;
	static const Color8 MediumBlue;
	static const Color8 MediumOrchid;
	static const Color8 MediumPurple;
	static const Color8 MediumSeaGreen;
	static const Color8 MediumSlateBlue;
	static const Color8 MediumSpringGreen;
	static const Color8 MediumTurquoise;
	static const Color8 MediumVioletRed;
	static const Color8 MidnightBlue;
	static const Color8 MintCream;
	static const Color8 MistyRose;
	static const Color8 Moccasin;
	static const Color8 NavajoWhite;
	static const Color8 Navy;
	static const Color8 OldLace;
	static const Color8 Olive;
	static const Color8 OliveDrab;
	static const Color8 Orange;
	static const Color8 OrangeRed;
	static const Color8 Orchid;
	static const Color8 PaleGoldenrod;
	static const Color8 PaleGreen;
	static const Color8 PaleTurquoise;
	static const Color8 PaleVioletRed;
	static const Color8 PapayaWhip;
	static const Color8 PeachPuff;
	static const Color8 Peru;
	static const Color8 Pink;
	static const Color8 Plum;
	static const Color8 PowderBlue;
	static const Color8 Purple;
	static const Color8 Red;
	static const Color8 RosyBrown;
	static const Color8 RoyalBlue;
	static const Color8 SaddleBrown;
	static const Color8 Salmon;
	static const Color8 SandyBrown;
	static const Color8 SeaGreen;
	static const Color8 SeaShell;
	static const Color8 Sienna;
	static const Color8 Silver;
	static const Color8 SkyBlue;
	static const Color8 SlateBlue;
	static const Color8 SlateGray;
	static const Color8 Snow;
	static const Color8 SpringGreen;
	static const Color8 SteelBlue;
	static const Color8 Tan;
	static const Color8 Teal;
	static const Color8 Thistle;
	static const Color8 Tomato;
	static const Color8 Turquoise;
	static const Color8 Violet;
	static const Color8 Wheat;
	static const Color8 White;
	static const Color8 WhiteSmoke;
	static const Color8 Yellow;
	static const Color8 YellowGreen;
};

#include "Color8.inl"

}

#endif
