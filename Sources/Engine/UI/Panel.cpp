/*=============================================================================
Panel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Panel.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Panel);
		SE_IMPLEMENT_REFLECTION(Panel);

		Panel::Panel() :
			super()
		{
			_Movable = false;
			_Sizable = false;

			_Moving = false;
			_SizingFrame = FrameLocation_None;
		}

		Panel::~Panel()
		{
		}

		void Panel::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Panel::DoRender()
		{
			super::DoRender();
		}

		void Panel::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);

			if (_Moving)
			{
				RectangleInt rect = GetRectangle();
				PointInt mouse = GlobalToLocal(e.Location());
				PointInt diff = PointInt(mouse.X - _MovingOrigin.X, mouse.Y - _MovingOrigin.Y);

				if (_SizingFrame == FrameLocation_None)
				{
					SetPosition(PointInt(rect.X + diff.X, rect.Y + diff.Y));
				}
				else
				{
					switch (_SizingFrame)
					{
					case FrameLocation_Left:
						SetRectangle(rect.X + diff.X, rect.Y,
							rect.Width - diff.X, rect.Height);
						break;
					case FrameLocation_Top:
						SetRectangle(rect.X, rect.Y + diff.Y,
							rect.Width, rect.Height - diff.Y);
						break;
					case FrameLocation_Right:
						SetRectangle(rect.X, rect.Y,
							_SizingOrigin.Width + diff.X, rect.Height);
						break;
					case FrameLocation_Bottom:
						SetRectangle(rect.X, rect.Y,
							rect.Width, _SizingOrigin.Height + diff.Y);
						break;
					case FrameLocation_TopLeft:
						SetRectangle(rect.X + diff.X, rect.Y + diff.Y,
							rect.Width - diff.X, rect.Height - diff.Y);
						break;
					case FrameLocation_TopRight:
						SetRectangle(rect.X, rect.Y + diff.Y,
							_SizingOrigin.Width + diff.X, rect.Height - diff.Y);
						break;
					case FrameLocation_BottomLeft:
						SetRectangle(rect.X + diff.X, rect.Y,
							rect.Width - diff.X, _SizingOrigin.Height + diff.Y);
						break;
					case FrameLocation_BottomRight:
						SetRectangle(rect.X, rect.Y,
							_SizingOrigin.Width + diff.X, _SizingOrigin.Height + diff.Y);
						break;
					}
				}
			}
		}

		void Panel::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left && GetMovable())
			{
				_Moving = true;
				PointInt mouse = GlobalToLocal(e.Location());
				_MovingOrigin = mouse;
				_SizingOrigin = GetSize();

				if (!GetHasFrame())
				{
					_SizingFrame = FrameLocation_None;
				}
				else
				{
					if (RectangleInt(0, GetFrameSize(),
						GetFrameSize(), GetHeight()-2*GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_Left;
					}
					else if (RectangleInt(GetFrameSize(), 0,
						GetWidth(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_Top;
					}
					else if (RectangleInt(GetWidth()-GetFrameSize(), GetFrameSize(),
						GetFrameSize(), GetHeight()-2*GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_Right;
					}
					else if (RectangleInt(GetFrameSize(), GetHeight()-GetFrameSize(),
						GetWidth()-2*GetFrameSize(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_Bottom;
					}
					else if (RectangleInt(0, 0,
						GetFrameSize(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_TopLeft;
					}
					else if (RectangleInt(GetWidth()-GetFrameSize(), 0,
						GetFrameSize(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_TopRight;
					}
					else if (RectangleInt(0, GetHeight()-GetFrameSize(),
						GetFrameSize(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_BottomLeft;
					}
					else if (RectangleInt(GetWidth()-GetFrameSize(), GetHeight()-GetFrameSize(),
						GetFrameSize(), GetFrameSize()).Contains(mouse))
					{
						_SizingFrame = FrameLocation_BottomRight;
					}
					else
					{
						_SizingFrame = FrameLocation_None;
					}
				}

				UISystem::Instance()->SetCapture(this);
			}
		}

		void Panel::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left && GetMovable())
			{
				if (_Moving)
				{
					_Moving = false;
					UISystem::Instance()->ReleaseCapture();
				}
			}
		}
	}
}
