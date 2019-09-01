/*=============================================================================
Color32.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Color32.h"
#include "Core/Object.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

const Color32 Color32::Transparent = Color32::FromUInt8(255, 255, 255, 0);
const Color32 Color32::AliceBlue = Color32::FromUInt8(240, 248, 255, 255);
const Color32 Color32::AntiqueWhite = Color32::FromUInt8(250, 235, 215, 255);
const Color32 Color32::Aqua = Color32::FromUInt8(0, 255, 255, 255);
const Color32 Color32::Aquamarine = Color32::FromUInt8(127, 255, 212, 255);
const Color32 Color32::Azure = Color32::FromUInt8(240, 255, 255, 255);
const Color32 Color32::Beige = Color32::FromUInt8(245, 245, 220, 255);
const Color32 Color32::Bisque = Color32::FromUInt8(255, 228, 196, 255);
const Color32 Color32::Black = Color32::FromUInt8(0, 0, 0, 255);
const Color32 Color32::BlanchedAlmond = Color32::FromUInt8(255, 235, 205, 255);
const Color32 Color32::Blue = Color32::FromUInt8(0, 0, 255, 255);
const Color32 Color32::BlueViolet = Color32::FromUInt8(138, 43, 226, 255);
const Color32 Color32::Brown = Color32::FromUInt8(165, 42, 42, 255);
const Color32 Color32::BurlyWood = Color32::FromUInt8(222, 184, 135, 255);
const Color32 Color32::CadetBlue = Color32::FromUInt8(95, 158, 160, 255);
const Color32 Color32::Chartreuse = Color32::FromUInt8(127, 255, 0, 255);
const Color32 Color32::Chocolate = Color32::FromUInt8(210, 105, 30, 255);
const Color32 Color32::Coral = Color32::FromUInt8(255, 127, 80, 255);
const Color32 Color32::CornflowerBlue = Color32::FromUInt8(100, 149, 237, 255);
const Color32 Color32::Cornsilk = Color32::FromUInt8(255, 248, 220, 255);
const Color32 Color32::Crimson = Color32::FromUInt8(220, 20, 60, 255);
const Color32 Color32::Cyan = Color32::FromUInt8(0, 255, 255, 255);
const Color32 Color32::DarkBlue = Color32::FromUInt8(0, 0, 139, 255);
const Color32 Color32::DarkCyan = Color32::FromUInt8(0, 139, 139, 255);
const Color32 Color32::DarkGoldenrod = Color32::FromUInt8(184, 134, 11, 255);
const Color32 Color32::DarkGray = Color32::FromUInt8(169, 169, 169, 255);
const Color32 Color32::DarkGreen = Color32::FromUInt8(0, 100, 0, 255);
const Color32 Color32::DarkKhaki = Color32::FromUInt8(189, 183, 107, 255);
const Color32 Color32::DarkMagenta = Color32::FromUInt8(139, 0, 139, 255);
const Color32 Color32::DarkOliveGreen = Color32::FromUInt8(85, 107, 47, 255);
const Color32 Color32::DarkOrange = Color32::FromUInt8(255, 140, 0, 255);
const Color32 Color32::DarkOrchid = Color32::FromUInt8(153, 50, 204, 255);
const Color32 Color32::DarkRed = Color32::FromUInt8(139, 0, 0, 255);
const Color32 Color32::DarkSalmon = Color32::FromUInt8(233, 150, 122, 255);
const Color32 Color32::DarkSeaGreen = Color32::FromUInt8(143, 188, 139, 255);
const Color32 Color32::DarkSlateBlue = Color32::FromUInt8(72, 61, 139, 255);
const Color32 Color32::DarkSlateGray = Color32::FromUInt8(47, 79, 79, 255);
const Color32 Color32::DarkTurquoise = Color32::FromUInt8(0, 206, 209, 255);
const Color32 Color32::DarkViolet = Color32::FromUInt8(148, 0, 211, 255);
const Color32 Color32::DeepPink = Color32::FromUInt8(255, 20, 147, 255);
const Color32 Color32::DeepSkyBlue = Color32::FromUInt8(0, 191, 255, 255);
const Color32 Color32::DimGray = Color32::FromUInt8(105, 105, 105, 255);
const Color32 Color32::DodgerBlue = Color32::FromUInt8(30, 144, 255, 255);
const Color32 Color32::Firebrick = Color32::FromUInt8(178, 34, 34, 255);
const Color32 Color32::FloralWhite = Color32::FromUInt8(255, 250, 240, 255);
const Color32 Color32::ForestGreen = Color32::FromUInt8(34, 139, 34, 255);
const Color32 Color32::Fuchsia = Color32::FromUInt8(255, 0, 255, 255);
const Color32 Color32::Gainsboro = Color32::FromUInt8(220, 220, 220, 255);
const Color32 Color32::GhostWhite = Color32::FromUInt8(248, 248, 255, 255);
const Color32 Color32::Gold = Color32::FromUInt8(255, 215, 0, 255);
const Color32 Color32::Goldenrod = Color32::FromUInt8(218, 165, 32, 255);
const Color32 Color32::Gray = Color32::FromUInt8(128, 128, 128, 255);
const Color32 Color32::Green = Color32::FromUInt8(0, 128, 0, 255);
const Color32 Color32::GreenYellow = Color32::FromUInt8(173, 255, 47, 255);
const Color32 Color32::Honeydew = Color32::FromUInt8(240, 255, 240, 255);
const Color32 Color32::HotPink = Color32::FromUInt8(255, 105, 180, 255);
const Color32 Color32::IndianRed = Color32::FromUInt8(205, 92, 92, 255);
const Color32 Color32::Indigo = Color32::FromUInt8(75, 0, 130, 255);
const Color32 Color32::Ivory = Color32::FromUInt8(255, 255, 240, 255);
const Color32 Color32::Khaki = Color32::FromUInt8(240, 230, 140, 255);
const Color32 Color32::Lavender = Color32::FromUInt8(230, 230, 250, 255);
const Color32 Color32::LavenderBlush = Color32::FromUInt8(255, 240, 245, 255);
const Color32 Color32::LawnGreen = Color32::FromUInt8(124, 252, 0, 255);
const Color32 Color32::LemonChiffon = Color32::FromUInt8(255, 250, 205, 255);
const Color32 Color32::LightBlue = Color32::FromUInt8(173, 216, 230, 255);
const Color32 Color32::LightCoral = Color32::FromUInt8(240, 128, 128, 255);
const Color32 Color32::LightCyan = Color32::FromUInt8(224, 255, 255, 255);
const Color32 Color32::LightGoldenrodYellow = Color32::FromUInt8(250, 250, 210, 255);
const Color32 Color32::LightGray = Color32::FromUInt8(211, 211, 211, 255);
const Color32 Color32::LightGreen = Color32::FromUInt8(144, 238, 144, 255);
const Color32 Color32::LightPink = Color32::FromUInt8(255, 182, 193, 255);
const Color32 Color32::LightSalmon = Color32::FromUInt8(255, 160, 122, 255);
const Color32 Color32::LightSeaGreen = Color32::FromUInt8(32, 178, 170, 255);
const Color32 Color32::LightSkyBlue = Color32::FromUInt8(135, 206, 250, 255);
const Color32 Color32::LightSlateGray = Color32::FromUInt8(119, 136, 153, 255);
const Color32 Color32::LightSteelBlue = Color32::FromUInt8(176, 196, 222, 255);
const Color32 Color32::LightYellow = Color32::FromUInt8(255, 255, 224, 255);
const Color32 Color32::Lime = Color32::FromUInt8(0, 255, 0, 255);
const Color32 Color32::LimeGreen = Color32::FromUInt8(50, 205, 50, 255);
const Color32 Color32::Linen = Color32::FromUInt8(250, 240, 230, 255);
const Color32 Color32::Magenta = Color32::FromUInt8(255, 0, 255, 255);
const Color32 Color32::Maroon = Color32::FromUInt8(128, 0, 0, 255);
const Color32 Color32::MediumAquamarine = Color32::FromUInt8(102, 205, 170, 255);
const Color32 Color32::MediumBlue = Color32::FromUInt8(0, 0, 205, 255);
const Color32 Color32::MediumOrchid = Color32::FromUInt8(186, 85, 211, 255);
const Color32 Color32::MediumPurple = Color32::FromUInt8(147, 112, 219, 255);
const Color32 Color32::MediumSeaGreen = Color32::FromUInt8(60, 179, 113, 255);
const Color32 Color32::MediumSlateBlue = Color32::FromUInt8(123, 104, 238, 255);
const Color32 Color32::MediumSpringGreen = Color32::FromUInt8(0, 250, 154, 255);
const Color32 Color32::MediumTurquoise = Color32::FromUInt8(72, 209, 204, 255);
const Color32 Color32::MediumVioletRed = Color32::FromUInt8(199, 21, 133, 255);
const Color32 Color32::MidnightBlue = Color32::FromUInt8(25, 25, 112, 255);
const Color32 Color32::MintCream = Color32::FromUInt8(245, 255, 250, 255);
const Color32 Color32::MistyRose = Color32::FromUInt8(255, 228, 225, 255);
const Color32 Color32::Moccasin = Color32::FromUInt8(255, 228, 181, 255);
const Color32 Color32::NavajoWhite = Color32::FromUInt8(255, 222, 173, 255);
const Color32 Color32::Navy = Color32::FromUInt8(0, 0, 128, 255);
const Color32 Color32::OldLace = Color32::FromUInt8(253, 245, 230, 255);
const Color32 Color32::Olive = Color32::FromUInt8(128, 128, 0, 255);
const Color32 Color32::OliveDrab = Color32::FromUInt8(107, 142, 35, 255);
const Color32 Color32::Orange = Color32::FromUInt8(255, 165, 0, 255);
const Color32 Color32::OrangeRed = Color32::FromUInt8(255, 69, 0, 255);
const Color32 Color32::Orchid = Color32::FromUInt8(218, 112, 214, 255);
const Color32 Color32::PaleGoldenrod = Color32::FromUInt8(238, 232, 170, 255);
const Color32 Color32::PaleGreen = Color32::FromUInt8(152, 251, 152, 255);
const Color32 Color32::PaleTurquoise = Color32::FromUInt8(175, 238, 238, 255);
const Color32 Color32::PaleVioletRed = Color32::FromUInt8(219, 112, 147, 255);
const Color32 Color32::PapayaWhip = Color32::FromUInt8(255, 239, 213, 255);
const Color32 Color32::PeachPuff = Color32::FromUInt8(255, 218, 185, 255);
const Color32 Color32::Peru = Color32::FromUInt8(205, 133, 63, 255);
const Color32 Color32::Pink = Color32::FromUInt8(255, 192, 203, 255);
const Color32 Color32::Plum = Color32::FromUInt8(221, 160, 221, 255);
const Color32 Color32::PowderBlue = Color32::FromUInt8(176, 224, 230, 255);
const Color32 Color32::Purple = Color32::FromUInt8(128, 0, 128, 255);
const Color32 Color32::Red = Color32::FromUInt8(255, 0, 0, 255);
const Color32 Color32::RosyBrown = Color32::FromUInt8(188, 143, 143, 255);
const Color32 Color32::RoyalBlue = Color32::FromUInt8(65, 105, 225, 255);
const Color32 Color32::SaddleBrown = Color32::FromUInt8(139, 69, 19, 255);
const Color32 Color32::Salmon = Color32::FromUInt8(250, 128, 114, 255);
const Color32 Color32::SandyBrown = Color32::FromUInt8(244, 164, 96, 255);
const Color32 Color32::SeaGreen = Color32::FromUInt8(46, 139, 87, 255);
const Color32 Color32::SeaShell = Color32::FromUInt8(255, 245, 238, 255);
const Color32 Color32::Sienna = Color32::FromUInt8(160, 82, 45, 255);
const Color32 Color32::Silver = Color32::FromUInt8(192, 192, 192, 255);
const Color32 Color32::SkyBlue = Color32::FromUInt8(135, 206, 235, 255);
const Color32 Color32::SlateBlue = Color32::FromUInt8(106, 90, 205, 255);
const Color32 Color32::SlateGray = Color32::FromUInt8(112, 128, 144, 255);
const Color32 Color32::Snow = Color32::FromUInt8(255, 250, 250, 255);
const Color32 Color32::SpringGreen = Color32::FromUInt8(0, 255, 127, 255);
const Color32 Color32::SteelBlue = Color32::FromUInt8(70, 130, 180, 255);
const Color32 Color32::Tan = Color32::FromUInt8(210, 180, 140, 255);
const Color32 Color32::Teal = Color32::FromUInt8(0, 128, 128, 255);
const Color32 Color32::Thistle = Color32::FromUInt8(216, 191, 216, 255);
const Color32 Color32::Tomato = Color32::FromUInt8(255, 99, 71, 255);
const Color32 Color32::Turquoise = Color32::FromUInt8(64, 224, 208, 255);
const Color32 Color32::Violet = Color32::FromUInt8(238, 130, 238, 255);
const Color32 Color32::Wheat = Color32::FromUInt8(245, 222, 179, 255);
const Color32 Color32::White = Color32::FromUInt8(255, 255, 255, 255);
const Color32 Color32::WhiteSmoke = Color32::FromUInt8(245, 245, 245, 255);
const Color32 Color32::Yellow = Color32::FromUInt8(255, 255, 0, 255);
const Color32 Color32::YellowGreen = Color32::FromUInt8(154, 205, 50, 255);

Color32 Color32::Clamp(const Color32& color, real32 min, real32 max)
{
	return Color32(
		Math::Clamp(color.R, min, max),
		Math::Clamp(color.G, min, max),
		Math::Clamp(color.B, min, max),
		Math::Clamp(color.A, min, max));
}

Color32 Color32::Lerp(const Color32& value1, const Color32& value2, real32 amount)
{
	return Color32(
		Math::Lerp(value1.R, value2.R, amount),
		Math::Lerp(value1.G, value2.G, amount),
		Math::Lerp(value1.B, value2.B, amount),
		Math::Lerp(value1.A, value2.A, amount));
}

Color32 Color32::AdjustSaturation(const Color32& color, real32 s)
{
	real32 k_r = 0.5f;
	real32 k_g = 0.5f;
	real32 k_b = 0.5f;

	if (color.R < 0.5f)
		k_r = 0.5f;
	if (color.G < 0.5f)
		k_g = 0.5f;
	if (color.B < 0.5f)
		k_b = 0.5f;

	return Color32(
		color.R - s * (color.R - k_r) / 1.0f,
		color.G - s * (color.G - k_g) / 1.0f,
		color.B - s * (color.B - k_b) / 1.0f);
}

Color32 Color32::AdjustContrast(const Color32& color, real32 c)
{
	if (c > 0.5f)
	{
		c = c - 0.5f;
		return Color32(
			color.R - c * (color.R - 0.0f) / 0.5f,
			color.G - c * (color.G - 0.0f) / 0.5f,
			color.B - c * (color.B - 0.0f) / 0.5f);
	}
	else
	{
		c = 0.5f - c;
		return Color32(
			color.R - c * (color.R - 1.0f) / 0.5f,
			color.G - c * (color.G - 1.0f) / 0.5f,
			color.B - c * (color.B - 1.0f) / 0.5f);
	}
}

}
