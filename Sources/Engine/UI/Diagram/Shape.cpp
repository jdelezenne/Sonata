/*=============================================================================
Shape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Shape.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Shape);
		SE_IMPLEMENT_REFLECTION(Shape);

		Shape::Shape() :
			super()
		{
		}

		Shape::~Shape()
		{
		}

		void Shape::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Shape::DoRender()
		{
			super::DoRender();
		}
	}
}
