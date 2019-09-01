/*=============================================================================
Blending.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

Vector4 _GetBlendFactor(BlendMode mode, const Colour32& s, const Colour32& d, const Colour32& bf)
{
	Vector4 factor;
	real32 f;

	switch (mode)
	{
	case BM_Zero:
		factor = Vector4(0.0, 0.0, 0.0, 0.0);
		break;
	case BM_One:
		factor = Vector4(1.0, 1.0, 1.0, 1.0);
		break;
	case BM_SourceColour:
		factor = Vector4(s.red, s.green, s.blue, s.alpha);
		break;
	case BM_InvSourceColour:
		factor = Vector4(1.0f - s.red, 1.0f - s.green, 1.0f - s.blue, 1.0f - s.alpha);
		break;
	case BM_SourceAlpha:
		factor = Vector4(s.alpha, s.alpha, s.alpha, s.alpha);
		break;
	case BM_InvSourceAlpha:
		factor = Vector4(1.0f - s.alpha, 1.0f - s.alpha, 1.0f - s.alpha, 1.0f - s.alpha);
		break;
	case BM_DestinationAlpha:
		factor = Vector4(d.alpha, d.alpha, d.alpha, d.alpha);
		break;
	case BM_InvDestinationAlpha:
		factor = Vector4(1.0f - d.alpha, 1.0f - d.alpha, 1.0f - d.alpha, 1.0f - d.alpha);
		break;
	case BM_DestinationColour:
		factor = Vector4(d.red, d.green, d.blue, d.alpha);
		break;
	case BM_InvDestinationColour:
		factor = Vector4(1.0f - d.red, 1.0f - d.green, 1.0f - d.blue, 1.0f - d.alpha);
		break;
	case BM_SourceAlphaSat:
		f = Math::Min(s.alpha, 1.0f - d.alpha);
		factor = Vector4(f, f, f, 1.0);
		break;
	case BM_BlendFactor:
		factor = Vector4(bf.red, bf.green, bf.blue, bf.alpha);
		break;
	case BM_InvBlendFactor:
		factor = Vector4(1.0f - bf.red, 1.0f - bf.green, 1.0f - bf.blue, 1.0f - bf.alpha);
		break;
	}

	return factor;
}

void SoftwareRenderer::Blending(const Colour32& current, Colour32& colour)
{
	if (!States.AlphaState.BlendEnable)
		return;

	Vector4 sf, df;
	Colour32 sc, dc, result;

	sf = _GetBlendFactor(States.AlphaState.SourceBlend, colour,
		current, States.AlphaState.BlendFactor);
	df = _GetBlendFactor(States.AlphaState.DestinationBlend, colour,
		current, States.AlphaState.BlendFactor);

	sc = Colour32(colour.red * sf.x, colour.green * sf.y, colour.blue * sf.z, colour.alpha * sf.w);
	dc = Colour32(current.red * df.x, current.green * df.y, current.blue * df.z, current.alpha * df.w);

	switch (States.AlphaState.BlendOperation)
	{
	case BO_Add:
		result = sc + dc;
		break;
	case BO_Subtract:
		result = sc - dc;
		break;
	case BO_RevSubtract:
		result = dc - sc;
		break;
	case BO_Min:
		result = Colour32(Math::Min(sc.red, dc.red), Math::Min(sc.green, dc.green),
			Math::Min(sc.blue, dc.blue), Math::Min(sc.alpha, dc.alpha));
		break;
	case BO_Max:
		result = Colour32(Math::Max(sc.red, dc.red), Math::Max(sc.green, dc.green),
			Math::Max(sc.blue, dc.blue), Math::Max(sc.alpha, dc.alpha));
		break;
	}

	colour = result;
}
