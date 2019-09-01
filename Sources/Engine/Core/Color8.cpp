/*=============================================================================
Color8.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Color8.h"
#include "Core/Object.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

const Color8 Color8::Transparent = Color8(255, 255, 255, 0);
const Color8 Color8::AliceBlue = Color8(240, 248, 255, 255);
const Color8 Color8::AntiqueWhite = Color8(250, 235, 215, 255);
const Color8 Color8::Aqua = Color8(0, 255, 255, 255);
const Color8 Color8::Aquamarine = Color8(127, 255, 212, 255);
const Color8 Color8::Azure = Color8(240, 255, 255, 255);
const Color8 Color8::Beige = Color8(245, 245, 220, 255);
const Color8 Color8::Bisque = Color8(255, 228, 196, 255);
const Color8 Color8::Black = Color8(0, 0, 0, 255);
const Color8 Color8::BlanchedAlmond = Color8(255, 235, 205, 255);
const Color8 Color8::Blue = Color8(0, 0, 255, 255);
const Color8 Color8::BlueViolet = Color8(138, 43, 226, 255);
const Color8 Color8::Brown = Color8(165, 42, 42, 255);
const Color8 Color8::BurlyWood = Color8(222, 184, 135, 255);
const Color8 Color8::CadetBlue = Color8(95, 158, 160, 255);
const Color8 Color8::Chartreuse = Color8(127, 255, 0, 255);
const Color8 Color8::Chocolate = Color8(210, 105, 30, 255);
const Color8 Color8::Coral = Color8(255, 127, 80, 255);
const Color8 Color8::CornflowerBlue = Color8(100, 149, 237, 255);
const Color8 Color8::Cornsilk = Color8(255, 248, 220, 255);
const Color8 Color8::Crimson = Color8(220, 20, 60, 255);
const Color8 Color8::Cyan = Color8(0, 255, 255, 255);
const Color8 Color8::DarkBlue = Color8(0, 0, 139, 255);
const Color8 Color8::DarkCyan = Color8(0, 139, 139, 255);
const Color8 Color8::DarkGoldenrod = Color8(184, 134, 11, 255);
const Color8 Color8::DarkGray = Color8(169, 169, 169, 255);
const Color8 Color8::DarkGreen = Color8(0, 100, 0, 255);
const Color8 Color8::DarkKhaki = Color8(189, 183, 107, 255);
const Color8 Color8::DarkMagenta = Color8(139, 0, 139, 255);
const Color8 Color8::DarkOliveGreen = Color8(85, 107, 47, 255);
const Color8 Color8::DarkOrange = Color8(255, 140, 0, 255);
const Color8 Color8::DarkOrchid = Color8(153, 50, 204, 255);
const Color8 Color8::DarkRed = Color8(139, 0, 0, 255);
const Color8 Color8::DarkSalmon = Color8(233, 150, 122, 255);
const Color8 Color8::DarkSeaGreen = Color8(143, 188, 139, 255);
const Color8 Color8::DarkSlateBlue = Color8(72, 61, 139, 255);
const Color8 Color8::DarkSlateGray = Color8(47, 79, 79, 255);
const Color8 Color8::DarkTurquoise = Color8(0, 206, 209, 255);
const Color8 Color8::DarkViolet = Color8(148, 0, 211, 255);
const Color8 Color8::DeepPink = Color8(255, 20, 147, 255);
const Color8 Color8::DeepSkyBlue = Color8(0, 191, 255, 255);
const Color8 Color8::DimGray = Color8(105, 105, 105, 255);
const Color8 Color8::DodgerBlue = Color8(30, 144, 255, 255);
const Color8 Color8::Firebrick = Color8(178, 34, 34, 255);
const Color8 Color8::FloralWhite = Color8(255, 250, 240, 255);
const Color8 Color8::ForestGreen = Color8(34, 139, 34, 255);
const Color8 Color8::Fuchsia = Color8(255, 0, 255, 255);
const Color8 Color8::Gainsboro = Color8(220, 220, 220, 255);
const Color8 Color8::GhostWhite = Color8(248, 248, 255, 255);
const Color8 Color8::Gold = Color8(255, 215, 0, 255);
const Color8 Color8::Goldenrod = Color8(218, 165, 32, 255);
const Color8 Color8::Gray = Color8(128, 128, 128, 255);
const Color8 Color8::Green = Color8(0, 128, 0, 255);
const Color8 Color8::GreenYellow = Color8(173, 255, 47, 255);
const Color8 Color8::Honeydew = Color8(240, 255, 240, 255);
const Color8 Color8::HotPink = Color8(255, 105, 180, 255);
const Color8 Color8::IndianRed = Color8(205, 92, 92, 255);
const Color8 Color8::Indigo = Color8(75, 0, 130, 255);
const Color8 Color8::Ivory = Color8(255, 255, 240, 255);
const Color8 Color8::Khaki = Color8(240, 230, 140, 255);
const Color8 Color8::Lavender = Color8(230, 230, 250, 255);
const Color8 Color8::LavenderBlush = Color8(255, 240, 245, 255);
const Color8 Color8::LawnGreen = Color8(124, 252, 0, 255);
const Color8 Color8::LemonChiffon = Color8(255, 250, 205, 255);
const Color8 Color8::LightBlue = Color8(173, 216, 230, 255);
const Color8 Color8::LightCoral = Color8(240, 128, 128, 255);
const Color8 Color8::LightCyan = Color8(224, 255, 255, 255);
const Color8 Color8::LightGoldenrodYellow = Color8(250, 250, 210, 255);
const Color8 Color8::LightGray = Color8(211, 211, 211, 255);
const Color8 Color8::LightGreen = Color8(144, 238, 144, 255);
const Color8 Color8::LightPink = Color8(255, 182, 193, 255);
const Color8 Color8::LightSalmon = Color8(255, 160, 122, 255);
const Color8 Color8::LightSeaGreen = Color8(32, 178, 170, 255);
const Color8 Color8::LightSkyBlue = Color8(135, 206, 250, 255);
const Color8 Color8::LightSlateGray = Color8(119, 136, 153, 255);
const Color8 Color8::LightSteelBlue = Color8(176, 196, 222, 255);
const Color8 Color8::LightYellow = Color8(255, 255, 224, 255);
const Color8 Color8::Lime = Color8(0, 255, 0, 255);
const Color8 Color8::LimeGreen = Color8(50, 205, 50, 255);
const Color8 Color8::Linen = Color8(250, 240, 230, 255);
const Color8 Color8::Magenta = Color8(255, 0, 255, 255);
const Color8 Color8::Maroon = Color8(128, 0, 0, 255);
const Color8 Color8::MediumAquamarine = Color8(102, 205, 170, 255);
const Color8 Color8::MediumBlue = Color8(0, 0, 205, 255);
const Color8 Color8::MediumOrchid = Color8(186, 85, 211, 255);
const Color8 Color8::MediumPurple = Color8(147, 112, 219, 255);
const Color8 Color8::MediumSeaGreen = Color8(60, 179, 113, 255);
const Color8 Color8::MediumSlateBlue = Color8(123, 104, 238, 255);
const Color8 Color8::MediumSpringGreen = Color8(0, 250, 154, 255);
const Color8 Color8::MediumTurquoise = Color8(72, 209, 204, 255);
const Color8 Color8::MediumVioletRed = Color8(199, 21, 133, 255);
const Color8 Color8::MidnightBlue = Color8(25, 25, 112, 255);
const Color8 Color8::MintCream = Color8(245, 255, 250, 255);
const Color8 Color8::MistyRose = Color8(255, 228, 225, 255);
const Color8 Color8::Moccasin = Color8(255, 228, 181, 255);
const Color8 Color8::NavajoWhite = Color8(255, 222, 173, 255);
const Color8 Color8::Navy = Color8(0, 0, 128, 255);
const Color8 Color8::OldLace = Color8(253, 245, 230, 255);
const Color8 Color8::Olive = Color8(128, 128, 0, 255);
const Color8 Color8::OliveDrab = Color8(107, 142, 35, 255);
const Color8 Color8::Orange = Color8(255, 165, 0, 255);
const Color8 Color8::OrangeRed = Color8(255, 69, 0, 255);
const Color8 Color8::Orchid = Color8(218, 112, 214, 255);
const Color8 Color8::PaleGoldenrod = Color8(238, 232, 170, 255);
const Color8 Color8::PaleGreen = Color8(152, 251, 152, 255);
const Color8 Color8::PaleTurquoise = Color8(175, 238, 238, 255);
const Color8 Color8::PaleVioletRed = Color8(219, 112, 147, 255);
const Color8 Color8::PapayaWhip = Color8(255, 239, 213, 255);
const Color8 Color8::PeachPuff = Color8(255, 218, 185, 255);
const Color8 Color8::Peru = Color8(205, 133, 63, 255);
const Color8 Color8::Pink = Color8(255, 192, 203, 255);
const Color8 Color8::Plum = Color8(221, 160, 221, 255);
const Color8 Color8::PowderBlue = Color8(176, 224, 230, 255);
const Color8 Color8::Purple = Color8(128, 0, 128, 255);
const Color8 Color8::Red = Color8(255, 0, 0, 255);
const Color8 Color8::RosyBrown = Color8(188, 143, 143, 255);
const Color8 Color8::RoyalBlue = Color8(65, 105, 225, 255);
const Color8 Color8::SaddleBrown = Color8(139, 69, 19, 255);
const Color8 Color8::Salmon = Color8(250, 128, 114, 255);
const Color8 Color8::SandyBrown = Color8(244, 164, 96, 255);
const Color8 Color8::SeaGreen = Color8(46, 139, 87, 255);
const Color8 Color8::SeaShell = Color8(255, 245, 238, 255);
const Color8 Color8::Sienna = Color8(160, 82, 45, 255);
const Color8 Color8::Silver = Color8(192, 192, 192, 255);
const Color8 Color8::SkyBlue = Color8(135, 206, 235, 255);
const Color8 Color8::SlateBlue = Color8(106, 90, 205, 255);
const Color8 Color8::SlateGray = Color8(112, 128, 144, 255);
const Color8 Color8::Snow = Color8(255, 250, 250, 255);
const Color8 Color8::SpringGreen = Color8(0, 255, 127, 255);
const Color8 Color8::SteelBlue = Color8(70, 130, 180, 255);
const Color8 Color8::Tan = Color8(210, 180, 140, 255);
const Color8 Color8::Teal = Color8(0, 128, 128, 255);
const Color8 Color8::Thistle = Color8(216, 191, 216, 255);
const Color8 Color8::Tomato = Color8(255, 99, 71, 255);
const Color8 Color8::Turquoise = Color8(64, 224, 208, 255);
const Color8 Color8::Violet = Color8(238, 130, 238, 255);
const Color8 Color8::Wheat = Color8(245, 222, 179, 255);
const Color8 Color8::White = Color8(255, 255, 255, 255);
const Color8 Color8::WhiteSmoke = Color8(245, 245, 245, 255);
const Color8 Color8::Yellow = Color8(255, 255, 0, 255);
const Color8 Color8::YellowGreen = Color8(154, 205, 50, 255);

Color8 Color8::Clamp(const Color8& color, uint8 min, uint8 max)
{
	return Color8(
		Math::Clamp(color.R, min, max),
		Math::Clamp(color.G, min, max),
		Math::Clamp(color.B, min, max),
		Math::Clamp(color.A, min, max));
}

Color8 Color8::Lerp(const Color8& value1, const Color8& value2, real32 amount)
{
	return Color8(
		Math::Lerp(value1.R, value2.R, amount),
		Math::Lerp(value1.G, value2.G, amount),
		Math::Lerp(value1.B, value2.B, amount),
		Math::Lerp(value1.A, value2.A, amount));
}

Color8 Color8::AdjustSaturation(const Color8& color, uint8 s)
{
	uint8 k_r = 127;
	uint8 k_g = 127;
	uint8 k_b = 127;

	if (color.R < 127)
		k_r = 127;
	if (color.G < 127)
		k_g = 127;
	if (color.B < 127)
		k_b = 127;

	return Color8(
		color.R - s * (color.R - k_r) / 255,
		color.G - s * (color.G - k_g) / 255,
		color.B - s * (color.B - k_b) / 255);
}

Color8 Color8::AdjustContrast(const Color8& color, uint8 c)
{
	if (c > 127)
	{
		c = c - 127;
		return Color8(
			color.R - c * (color.R - 0) / 127,
			color.G - c * (color.G - 0) / 127,
			color.B - c * (color.B - 0) / 127);
	}
	else
	{
		c = 127 - c;
		return Color8(
			color.R - c * (color.R - 255) / 127,
			color.G - c * (color.G - 255) / 127,
			color.B - c * (color.B - 255) / 127);
	}
}

}
