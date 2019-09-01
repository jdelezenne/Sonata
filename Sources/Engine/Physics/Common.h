/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_COMMON_H_
#define _SE_PHYSICS_COMMON_H_

#define SE_ID_PHYSICSSYSTEM SonataEngine::SE_ID(0xe315eff9,0x5fc64d18)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_PHYSICS_EXPORT
#	else
#		ifdef SE_ENGINEPHYSICS
#			define SE_PHYSICS_EXPORT __declspec(dllexport)
#		else
#			define SE_PHYSICS_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_PHYSICS_EXPORT
#endif

namespace SonataEngine
{
	namespace Physics
	{
		typedef uint16 ShapeGroup;
		typedef uint16 BodyGroup;
		typedef uint16 CollisionGroup;

		typedef uint32 MaterialIndex;

		/** Types of time steps. */
		enum TimeStepType
		{
			/** The simulation automatically subdivides the passed elapsed time into substeps. */
			TimeStepType_Fixed,

			/** The simulation uses the elapsed time. */
			TimeStepType_Variable
		};

		/** Types of shapes. */
		enum ShapeType
		{
			ShapeType_Plane,
			ShapeType_Sphere,
			ShapeType_Box,
			ShapeType_Capsule,
			ShapeType_Mesh,
			ShapeType_Compound
		};

		/** Types of bodies. */
		enum BodyType
		{
			BodyType_Static,
			BodyType_Dynamic
		};

		/** Contact pair types. */
		enum ContactPairType
		{
			ContactPairType_Ignore = (1<<0),
			ContactPairType_NotifyOnStartTouch = (1<<1),
			ContactPairType_NotifyOnEndTouch = (1<<2),
			ContactPairType_NotifyOnTouch = (1<<3),
			ContactPairType_NotifyOnImpact = (1<<4),
			ContactPairType_NotifyOnRoll = (1<<5),
			ContactPairType_NotifyOnSlide = (1<<6),
			ContactPairType_NotifyAll = (ContactPairType_NotifyOnStartTouch|ContactPairType_NotifyOnEndTouch|ContactPairType_NotifyOnTouch|ContactPairType_NotifyOnImpact|ContactPairType_NotifyOnRoll|ContactPairType_NotifyOnSlide)
		};
	}
}

#endif
