/*=============================================================================
EulerAngles.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "EulerAngles.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Quaternion.h"

namespace SonataEngine
{

const EulerAngles EulerAngles::Zero;

void EulerAngles::FromAxisAngle(const Vector3& axis, real angle)
{
	//TODO
}

void EulerAngles::ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const
{
	real sr, sp, sy, cr, cp, cy;
	
	Math::SinCos(Math::ToRadians(Yaw), sy, cy);
	Math::SinCos(Math::ToRadians(Pitch), sp, cp);
	Math::SinCos(Math::ToRadians(Roll), sr, cr);

	xAxis = Vector3(cp * cy, cp * sy, -sp);
	yAxis = Vector3(-sr * sp * cy + cr * sy, -sr * sp * sy + -cr * cy, -sr * cp);
	zAxis = Vector3(cr * sp * cy + -sr * -sy, cr * sp * sy + -sr * cy, cr * cp);
}

void EulerAngles::ToMatrix3(Matrix3& value) const
{
	Matrix3 mat;
	real sr, sp, sy, cr, cp, cy;

	Math::SinCos(Math::ToRadians(Yaw), sy, cy);
	Math::SinCos(Math::ToRadians(Pitch), sp, cp);
	Math::SinCos(Math::ToRadians(Roll), sr, cr);

	value.SetRow(0, Vector3(cp * cy, cp * sy, -sp));
	value.SetRow(1, Vector3(sr * sp * cy + cr * -sy, sr * sp * sy + cr * cy, sr * cp));
	value.SetRow(2, Vector3(cr * sp * cy + -sr * -sy, cr * sp * sy + -sr * cy, cr * cp));
}

void EulerAngles::ToQuaternion(Quaternion& value) const
{
	real sx, cx, sy, cy, sz, cz;
	real sxcy, cxcy, sxsy, cxsy;

	Math::SinCos(Math::ToRadians(Yaw) * 0.5f, sz, cz);
	Math::SinCos(Math::ToRadians(Pitch) * 0.5f, sy, cy);
	Math::SinCos(Math::ToRadians(Roll) * 0.5f, sx, cx);

	sxcy = sx * cy;
	cxcy = cx * cy;
	sxsy = sx * sy;
	cxsy = cx * sy;

	value = Quaternion(cxsy*sz - sxcy*cz, -cxsy*cz - sxcy*sz, sxsy*cz - cxcy*sz, cxcy*cz + sxsy*sz);
}

}
