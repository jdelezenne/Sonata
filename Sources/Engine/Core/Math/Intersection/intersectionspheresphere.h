/*=============================================================================
IntersectionSphereSphere.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_INTERSECTIONSPHERESPHERE_H_
#define _SE_INTERSECTIONSPHERESPHERE_H_

#include "Core/Common.h"
#include "Core/Containers/SimpleArray.h"
#include "Core/Math/BoundingSphere.h"
#include "Core/Math/BoundingBox.h"

namespace SonataEngine
{

/** Sphere-Sphere intersection. */
class SE_CORE_EXPORT IntersectionSphereSphere
{
public:
	static bool Intersect(const Sphere& sphere0, const Sphere& sphere1);
	static bool Contact(const Sphere& sphere0, const Sphere& sphere1, SimpleArray<IntersectionContact>* contacts = NULL);
};

/** Plane-Sphere intersection. */
class SE_CORE_EXPORT IntersectionPlaneSphere
{
public:
	static bool Intersect(const Plane& plane, const Sphere& sphere);
	static bool Contact(const Plane& plane, const Sphere& sphere, SimpleArray<IntersectionContact>* contacts = NULL);
};

/** Box-Box intersection. */
class SE_CORE_EXPORT IntersectionBoxBox
{
public:
	static bool Intersect(const Box& box0, const Box& box1);
	static bool Contact(const Box& box0, const Box& box1, SimpleArray<IntersectionContact>* contacts = NULL);
};

/** Box-Plane intersection. */
class SE_CORE_EXPORT IntersectionBoxPlane
{
public:
	static bool Intersect(const Box& box, const Plane& plane);
	static bool Contact(const Box& box, const Plane& plane, SimpleArray<IntersectionContact>* contacts = NULL);
};

/** Box-Sphere intersection. */
class SE_CORE_EXPORT IntersectionBoxSphere
{
public:
	static bool Intersect(const Box& box, const Sphere& sphere);
	static bool Contact(const Box& box, const Sphere& sphere, SimpleArray<IntersectionContact>* contacts = NULL);
};

}

#endif 
