/*=============================================================================
Color32.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_COLOUR32_H_
#define _SE_COLOUR32_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Color models. */
enum ColorType
{
	/// RGB (Red Green Blue).
	ColorType_RGB,

	/// HSV (Hue Saturation Value).
	ColorType_HSV,

	/// CMY (Cyan Magenta Yellow).
	ColorType_CMY,

	/// CMYK (Cyan Magenta Yellow blacK).
	ColorType_CMYK
};

/**
	@brief Color.

	A color described by 4 floating-point components in an RGBA order.
*/
class SE_CORE_EXPORT Color32
{
public:
	union
	{
		struct
		{
			real32 R;
			real32 G;
			real32 B;
			real32 A;
		};
		real32 Data[4];
	};

public:
	/** @name Constructors */
	//@{
	/** Creates an empty value. */
	Color32();

	/** Creates a color from its components.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
	*/
	Color32(real32 red, real32 green, real32 blue, real32 alpha = 1.0f);

	/** Creates a color from an RGBA or ARGB value.
		@param value An RGBA or ARGB value.
		@param isRGBA true if the value is an RGBA value, false if it is an ARGB value.
	*/
	Color32(uint32 value, bool isRGBA = false);

	/** Creates a color from an array of 4 color components in RGBA or ARGB order.
		@param value Array of 4 color components.
		@param isRGBA true if the value is an RGBA value, false if it is an ARGB value.
	*/
	Color32(const real32* value, bool isRGBA = false);

	/** Copy constructor.
		@param color Color to copy.
	*/
	Color32(const Color32& color);
	//@}

	/** Sets new values for the components of this value.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
	*/
	void Set(real32 red, real32 green, real32 blue, real32 alpha = 1.0f);

	/** Assignment Operator. */
	Color32& operator=(const Color32& color);

	/** Creates a color from UInt8 values.
		@param red Red component.
		@param green Green component.
		@param blue Blue component.
		@param alpha Alpha component.
		@return The value.
	*/
	static Color32 FromUInt8(uint8 red, uint8 green, uint8 blue, uint8 alpha);

	/** Creates a color from an RGBA value.
		@param value An RGBA value.
		@return The value.
	*/
	static Color32 FromRGBA(uint32 value);

	/** Creates a color from an ARGB value.
		@param value An ARGB value.
		@return The value.
	*/
	static Color32 FromARGB(uint32 value);

	/** Gets the 32-bit RGBA value of this Color.
		@return The 32-bit RGBA value of this Color.
	*/
	uint32 ToRGBA() const;

	/** Gets the 32-bit ARGB value of this Color.
		@return The 32-bit ARGB value of this Color.
	*/
	uint32 ToARGB() const;

	/** @name Casting Operators. */
	//@{
	operator uint32() const;

	operator real32*();
	operator const real32*() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	Color32& operator+=(const Color32& value);
	Color32& operator-=(const Color32& value);
	Color32& operator*=(real32 value);
	Color32& operator/=(real32 value);
	//@}

	/** @name Unary Operators. */
	//@{
	Color32 operator+() const;
	Color32 operator-() const;
	//@}

	/** @name Binary Operators. */
	//@{
	Color32 operator+(const Color32& value) const;
	Color32 operator-(const Color32& value) const;
	Color32 operator*(const Color32& value) const;
	Color32 operator*(real32 value) const;
	Color32 operator/(real32 value) const;

	bool operator==(const Color32& value) const;
	bool operator!=(const Color32& value) const;
	//@}

	friend Color32 operator*(real32 left, const Color32& right);

	/** (1-r, 1-g, 1-b, a) */
	void Negative();
	static Color32 Negative(const Color32& value);

	void Add(const Color32& value);
	static Color32 Add(const Color32& left, const Color32& right);

	void Subtract(const Color32& value);
	static Color32 Subtract(const Color32& left, const Color32& right);

	void Clamp(real32 min, real32 max);
	static Color32 Clamp(const Color32& color, real32 min, real32 max);

	void Scale(real32 s);
	static Color32 Scale(const Color32& color, real32 s);

	/** (r1*r2, g1*g2, b1*b2, a1*a2). */
	static Color32 Modulate(const Color32& left, const Color32& right);

	/**
		Linear interpolation of R, G, B, and A.
		Out = value1 + amount * (value2 - value1).
	*/
	static Color32 Lerp(const Color32& value1, const Color32& value2, real amount);

	/**
		Interpolate r,g,b between desaturated color and value.
		DesaturatedColor + s(Color - DesaturatedColor)
	*/
	void AdjustSaturation(real32 s);
	static Color32 AdjustSaturation(const Color32& color, real32 s);

	/**
		Interpolate r,g,b between 50% grey and value.
		Grey + s(Color - Grey).
	*/
	void AdjustContrast(real32 c);
	static Color32 AdjustContrast(const Color32& color, real32 c);

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
	static const Color32 Transparent;
	static const Color32 AliceBlue;
	static const Color32 AntiqueWhite;
	static const Color32 Aqua;
	static const Color32 Aquamarine;
	static const Color32 Azure;
	static const Color32 Beige;
	static const Color32 Bisque;
	static const Color32 Black;
	static const Color32 BlanchedAlmond;
	static const Color32 Blue;
	static const Color32 BlueViolet;
	static const Color32 Brown;
	static const Color32 BurlyWood;
	static const Color32 CadetBlue;
	static const Color32 Chartreuse;
	static const Color32 Chocolate;
	static const Color32 Coral;
	static const Color32 CornflowerBlue;
	static const Color32 Cornsilk;
	static const Color32 Crimson;
	static const Color32 Cyan;
	static const Color32 DarkBlue;
	static const Color32 DarkCyan;
	static const Color32 DarkGoldenrod;
	static const Color32 DarkGray;
	static const Color32 DarkGreen;
	static const Color32 DarkKhaki;
	static const Color32 DarkMagenta;
	static const Color32 DarkOliveGreen;
	static const Color32 DarkOrange;
	static const Color32 DarkOrchid;
	static const Color32 DarkRed;
	static const Color32 DarkSalmon;
	static const Color32 DarkSeaGreen;
	static const Color32 DarkSlateBlue;
	static const Color32 DarkSlateGray;
	static const Color32 DarkTurquoise;
	static const Color32 DarkViolet;
	static const Color32 DeepPink;
	static const Color32 DeepSkyBlue;
	static const Color32 DimGray;
	static const Color32 DodgerBlue;
	static const Color32 Firebrick;
	static const Color32 FloralWhite;
	static const Color32 ForestGreen;
	static const Color32 Fuchsia;
	static const Color32 Gainsboro;
	static const Color32 GhostWhite;
	static const Color32 Gold;
	static const Color32 Goldenrod;
	static const Color32 Gray;
	static const Color32 Green;
	static const Color32 GreenYellow;
	static const Color32 Honeydew;
	static const Color32 HotPink;
	static const Color32 IndianRed;
	static const Color32 Indigo;
	static const Color32 Ivory;
	static const Color32 Khaki;
	static const Color32 Lavender;
	static const Color32 LavenderBlush;
	static const Color32 LawnGreen;
	static const Color32 LemonChiffon;
	static const Color32 LightBlue;
	static const Color32 LightCoral;
	static const Color32 LightCyan;
	static const Color32 LightGoldenrodYellow;
	static const Color32 LightGray;
	static const Color32 LightGreen;
	static const Color32 LightPink;
	static const Color32 LightSalmon;
	static const Color32 LightSeaGreen;
	static const Color32 LightSkyBlue;
	static const Color32 LightSlateGray;
	static const Color32 LightSteelBlue;
	static const Color32 LightYellow;
	static const Color32 Lime;
	static const Color32 LimeGreen;
	static const Color32 Linen;
	static const Color32 Magenta;
	static const Color32 Maroon;
	static const Color32 MediumAquamarine;
	static const Color32 MediumBlue;
	static const Color32 MediumOrchid;
	static const Color32 MediumPurple;
	static const Color32 MediumSeaGreen;
	static const Color32 MediumSlateBlue;
	static const Color32 MediumSpringGreen;
	static const Color32 MediumTurquoise;
	static const Color32 MediumVioletRed;
	static const Color32 MidnightBlue;
	static const Color32 MintCream;
	static const Color32 MistyRose;
	static const Color32 Moccasin;
	static const Color32 NavajoWhite;
	static const Color32 Navy;
	static const Color32 OldLace;
	static const Color32 Olive;
	static const Color32 OliveDrab;
	static const Color32 Orange;
	static const Color32 OrangeRed;
	static const Color32 Orchid;
	static const Color32 PaleGoldenrod;
	static const Color32 PaleGreen;
	static const Color32 PaleTurquoise;
	static const Color32 PaleVioletRed;
	static const Color32 PapayaWhip;
	static const Color32 PeachPuff;
	static const Color32 Peru;
	static const Color32 Pink;
	static const Color32 Plum;
	static const Color32 PowderBlue;
	static const Color32 Purple;
	static const Color32 Red;
	static const Color32 RosyBrown;
	static const Color32 RoyalBlue;
	static const Color32 SaddleBrown;
	static const Color32 Salmon;
	static const Color32 SandyBrown;
	static const Color32 SeaGreen;
	static const Color32 SeaShell;
	static const Color32 Sienna;
	static const Color32 Silver;
	static const Color32 SkyBlue;
	static const Color32 SlateBlue;
	static const Color32 SlateGray;
	static const Color32 Snow;
	static const Color32 SpringGreen;
	static const Color32 SteelBlue;
	static const Color32 Tan;
	static const Color32 Teal;
	static const Color32 Thistle;
	static const Color32 Tomato;
	static const Color32 Turquoise;
	static const Color32 Violet;
	static const Color32 Wheat;
	static const Color32 White;
	static const Color32 WhiteSmoke;
	static const Color32 Yellow;
	static const Color32 YellowGreen;
};

#include "Color32.inl"

typedef Color32 Color;

}

#endif
