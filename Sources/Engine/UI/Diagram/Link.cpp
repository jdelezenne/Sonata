/*=============================================================================
Link.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Link.h"
#include "UI/UISystem.h"
#include "UI/Diagram/Slot.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Link);
		SE_IMPLEMENT_REFLECTION(Link);

		Link::Link() :
			super()
		{
			_HasBackground = false;

			_LinkShape = LinkShape_Line;
			_LineColor = Color32::Black;
			_LineSize = 2;

			Slot* startSlot = new Slot();
			_Slots.Add(startSlot);
			startSlot->SetParent(this);

			Slot* endSlot = new Slot();
			_Slots.Add(endSlot);
			endSlot->SetParent(this);
		}

		Link::~Link()
		{
		}

		Slot* Link::GetStartSlot() const
		{
			return _Slots[0];
		}

		void Link::SetStartSlot(Slot* value)
		{
			_Slots[0] = value;
		}

		Slot* Link::GetEndSlot() const
		{
			return _Slots[1];
		}

		void Link::SetEndSlot(Slot* value)
		{
			_Slots[1] = value;
		}

		void Link::DrawLink(const PointInt& source, const PointInt& destination)
		{
			UISystem::Instance()->DisableClipRectangle();

			if (_LinkShape == LinkShape_Line)
			{
				UISystem::Instance()->DrawLine(GetLineColor(), GetLineSize(),
					source, destination);
			}
			else if (_LinkShape == LinkShape_OrthogonalLine)
			{
				int32 halfX = (source.X + destination.X) / 2;
				UISystem::Instance()->DrawLine(GetLineColor(), GetLineSize(),
					source, PointInt(halfX, source.Y));
				UISystem::Instance()->DrawLine(GetLineColor(), GetLineSize(),
					PointInt(halfX, source.Y), PointInt(halfX, destination.Y));
				UISystem::Instance()->DrawLine(GetLineColor(), GetLineSize(),
					PointInt(halfX, destination.Y), destination);
			}
		}

		void Link::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Link::DoRender()
		{
			super::DoRender();

			DrawLink(GetStartSlot()->GetPosition(), GetEndSlot()->GetPosition());
		}
	}
}
