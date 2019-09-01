/*=============================================================================
UISystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "UISystem.h"

using namespace SonataEngine::Input;

namespace SonataEngine
{
	namespace UI
	{
		const int32 UISystem::DefaultSize = 15;

		Vector2 UISystem::GetPositionFromAlignment(const SizeInt& size, const RectangleInt& area, Alignment alignment)
		{
			Vector2 position;

			switch (alignment)
			{
			case Alignment_TopLeft:
				position = Vector2(area.X, area.Y);
				break;
			case Alignment_TopCenter:
				position = Vector2(area.X +
					(area.Width - size.Width) / 2,
					area.Y);
				break;
			case Alignment_TopRight:
				position = Vector2(area.X +
					area.Width - size.Width,
					area.Y);
				break;
			case Alignment_MiddleLeft:
				position = Vector2(area.X,
					area.Y +
					(area.Height - size.Height) / 2);
				break;
			case Alignment_MiddleCenter:
				position = Vector2(area.X +
					(area.Width - size.Width) / 2,
					area.Y +
					(area.Height - size.Height) / 2);
				break;
			case Alignment_MiddleRight:
				position = Vector2(area.X +
					area.Width - size.Width,
					area.Y +
					(area.Height - size.Height) / 2);
				break;
			case Alignment_BottomLeft:
				position = Vector2(area.X,
					area.Y +
					area.Height - size.Height);
				break;
			case Alignment_BottomCenter:
				position = Vector2(area.X +
					(area.Width - size.Width) / 2,
					area.Y +
					area.Height - size.Height);
				break;
			case Alignment_BottomRight:
				position = Vector2(area.X +
					area.Width - size.Width,
					area.Y +
					area.Height - size.Height);
				break;
			}

			return position;
		}

		TextFormat UISystem::GetTextFormat(Alignment alignment)
		{
			TextFormat format;
			switch (alignment)
			{
			case Alignment_TopLeft:
				format.VAlignment = VerticalAlignment_Top;
				format.HAlignment = HorizontalAlignment_Left;
				break;
			case Alignment_TopCenter:
				format.VAlignment = VerticalAlignment_Top;
				format.HAlignment = HorizontalAlignment_Center;
				break;
			case Alignment_TopRight:
				format.VAlignment = VerticalAlignment_Top;
				format.HAlignment = HorizontalAlignment_Right;
				break;
			case Alignment_MiddleLeft:
				format.VAlignment = VerticalAlignment_Center;
				format.HAlignment = HorizontalAlignment_Left;
				break;
			case Alignment_MiddleCenter:
				format.VAlignment = VerticalAlignment_Center;
				format.HAlignment = HorizontalAlignment_Center;
				break;
			case Alignment_MiddleRight:
				format.VAlignment = VerticalAlignment_Center;
				format.HAlignment = HorizontalAlignment_Right;
				break;
			case Alignment_BottomLeft:
				format.VAlignment = VerticalAlignment_Bottom;
				format.HAlignment = HorizontalAlignment_Left;
				break;
			case Alignment_BottomCenter:
				format.VAlignment = VerticalAlignment_Bottom;
				format.HAlignment = HorizontalAlignment_Center;
				break;
			case Alignment_BottomRight:
				format.VAlignment = VerticalAlignment_Bottom;
				format.HAlignment = HorizontalAlignment_Right;
				break;
			}

			return format;
		}

		UISystem::UISystem() :
			Manager()
		{
			_camera = new Camera();
			_currentWidget = NULL;
			_DefaultFont = NULL;
			_CapturedWidget = NULL;
			_FocusedWidget = NULL;
			_LastHoveredWidget = NULL;
		}

		UISystem::~UISystem()
		{
		}

		void UISystem::SetCapture(Widget* value)
		{
			_CapturedWidget = value;
			_FocusedWidget = value;
		}

		bool UISystem::Create()
		{
			return true;
		}

		void UISystem::Destroy()
		{
			SE_RELEASE(_DefaultFont);
		}

		void UISystem::Update(real64 elapsed)
		{
			if (_currentWidget == NULL)
				return;

			int i;

			_currentWidget->Update(elapsed);

			InputMouse* mouse = (InputMouse*)InputSystem::Current()->
				GetInputDeviceByType(InputDeviceType_Mouse);
			InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
				GetInputDeviceByType(InputDeviceType_Keyboard);

			int32 x, y;
			mouse->GetMousePosition(x, y);
			int32 delta = mouse->GetMouseWheel();

			Widget* widgetHover = _currentWidget->GetChildAtPoint(PointInt(x, y));
			Widget* current;

			if (_CapturedWidget != NULL)
			{
				current = _CapturedWidget;
			}
			else
			{
				current = widgetHover;
			}

			// Mouse events occur in the following order (as in .NET):
			// MouseEnter 
			// MouseMove 
			// MouseHover / MouseDown / MouseWheel
			// MouseUp 
			// MouseLeave 

			if (current != NULL)
			{
				if (widgetHover != NULL && _LastHoveredWidget != widgetHover)
				{
					widgetHover->OnMouseEnter(EventArgs());
				}

				int relX, relY;
				mouse->GetMouseRelativePosition(relX, relY);
				if (relX != 0 || relY != 0)
				{
					if (_CapturedWidget != NULL)
						_CapturedWidget->OnMouseMove(MouseEventArgs(MouseButtons_None, x, y, delta));
					else
						current->OnMouseMove(MouseEventArgs(MouseButtons_None, x, y, delta));
				}

				if (mouse->IsMouseButtonDown(MouseButton_Left) && mouse->IsMouseToggled(MouseButton_Left))
				{
					_FocusedWidget = current;
					_FocusedWidget->MoveToFront();
					current->OnMouseDown(MouseEventArgs(MouseButtons_Left, x, y, delta));
				}
				else if (mouse->IsMouseButtonDown(MouseButton_Right) && mouse->IsMouseToggled(MouseButton_Right))
					current->OnMouseDown(MouseEventArgs(MouseButtons_Right, x, y, delta));
				else if (mouse->IsMouseButtonDown(MouseButton_Middle) && mouse->IsMouseToggled(MouseButton_Middle))
					current->OnMouseDown(MouseEventArgs(MouseButtons_Middle, x, y, delta));

				if (delta != 0)
				{
					current->OnMouseWheel(MouseEventArgs(MouseButtons_None, x, y, delta));
				}

				if (mouse->IsMouseButtonUp(MouseButton_Left) && mouse->IsMouseToggled(MouseButton_Left))
					current->OnMouseUp(MouseEventArgs(MouseButtons_Left, x, y, delta));
				else if (mouse->IsMouseButtonUp(MouseButton_Right) && mouse->IsMouseToggled(MouseButton_Right))
					current->OnMouseUp(MouseEventArgs(MouseButtons_Right, x, y, delta));
				else if (mouse->IsMouseButtonUp(MouseButton_Middle) && mouse->IsMouseToggled(MouseButton_Middle))
					current->OnMouseUp(MouseEventArgs(MouseButtons_Middle, x, y, delta));
			}

			if (_LastHoveredWidget != widgetHover)
			{
				if (_LastHoveredWidget != NULL)
				{
					_LastHoveredWidget->OnMouseLeave(EventArgs());
				}
			}

			if (_CapturedWidget != NULL)
			{
				if (mouse->IsMouseButtonUp(MouseButton_Left) && mouse->IsMouseToggled(MouseButton_Left))
					_CapturedWidget = NULL;
				else if (mouse->IsMouseButtonUp(MouseButton_Right) && mouse->IsMouseToggled(MouseButton_Right))
					_CapturedWidget = NULL;
				else if (mouse->IsMouseButtonUp(MouseButton_Middle) && mouse->IsMouseToggled(MouseButton_Middle))
					_CapturedWidget = NULL;
			}

			if (_FocusedWidget != NULL)
			{
				ModifierKeys modifiers = ModifierKeys_None;
				if (keyboard->IsKeyDown(Key_LeftControl) || keyboard->IsKeyDown(Key_RightControl))
					modifiers = (ModifierKeys)(modifiers | ModifierKeys_Control);
				if (keyboard->IsKeyDown(Key_LeftShift) || keyboard->IsKeyDown(Key_RightShift))
					modifiers = (ModifierKeys)(modifiers | ModifierKeys_Shift);
				if (keyboard->IsKeyDown(Key_LeftAlt) || keyboard->IsKeyDown(Key_RightAlt))
					modifiers = (ModifierKeys)(modifiers | ModifierKeys_Alt);

				for (i = 0; i < KeyCount; i++)
				{
					KeyCode key = (KeyCode)i;
					if (keyboard->IsKeyDown(key) && keyboard->IsKeyToggled(key))
					{
						_FocusedWidget->OnKeyDown(KeyEventArgs(key, modifiers));

						Char c = keyboard->KeyCodeToASCII(key, '\0');
						if (c != '\0')
						{
							_FocusedWidget->OnKeyChar(KeyCharEventArgs(c));
						}
					}
					else if (keyboard->IsKeyUp(key) && keyboard->IsKeyToggled(key))
						_FocusedWidget->OnKeyUp(KeyEventArgs(key, modifiers));
				}
			}

			if (current != NULL)
			{
				static real64 lastTime = TimeValue::GetTime();
				static real64 thisTime = TimeValue::GetTime();

				if (mouse->IsMouseClicked(MouseButton_Left))
				{
					thisTime = TimeValue::GetTime();

					if (thisTime - lastTime < 200)
					{
						current->OnMouseDoubleClick(EventArgs());
					}
					else
					{
						current->OnMouseClick(EventArgs());
					}

					lastTime = thisTime;
				}
			}

			_LastHoveredWidget = widgetHover;

			// Delete the removed widgets
			Widget* widget;
			foreach (widget, _removedWidgets, Array<Widget*>)
			{
				if (_currentWidget == widget || widget->Contains(_currentWidget))
					_currentWidget = NULL;
				if (_FocusedWidget == widget || widget->Contains(_FocusedWidget))
					_FocusedWidget = NULL;
				if (_CapturedWidget == widget || widget->Contains(_CapturedWidget))
					_CapturedWidget = NULL;
				if (_LastHoveredWidget == widget || widget->Contains(_LastHoveredWidget))
					_LastHoveredWidget = NULL;

				delete widget;
			}
			_removedWidgets.Clear();
		}

		void UISystem::Render()
		{
			RenderSystem* renderSystem = RenderSystem::Current();

			Viewport viewport = renderSystem->GetViewport();

			_camera->SetOrthographic(0, 0,
				viewport.GetWidth(), viewport.GetHeight(),
				-1.0f, 1.0f);
			renderSystem->SetProjectionTransform(_camera->GetProjection());
			renderSystem->SetViewTransform(_camera->GetView());
			renderSystem->SetWorldTransform(Matrix4::Identity);

			renderSystem->SetCullMode(CullMode_None);

			LightState lightState;
			lightState.Lighting = false;
			renderSystem->SetLightState(lightState);

			DepthState depthState;
			depthState.Enable = false;
			renderSystem->SetDepthState(depthState);

			AlphaState alphaState;
			alphaState.BlendEnable[0] = true;
			alphaState.SourceBlend = BlendMode_SourceAlpha;
			alphaState.DestinationBlend = BlendMode_InvSourceAlpha;
			renderSystem->SetAlphaState(alphaState);

			_ClipRectangle = RectangleInt(0, 0, viewport.GetWidth(), viewport.GetHeight());
			if (_currentWidget != NULL)
			{
				_currentWidget->Render();
			}
		}

		void UISystem::DestroyWidget(Widget* widget)
		{
			if (widget == NULL)
				return;

			if (!_removedWidgets.Contains(widget))
				_removedWidgets.Add(widget);
		}

		Widget* UISystem::GetWidgetByName(const String& name)
		{
			if (_currentWidget == NULL)
				return NULL;

			return _currentWidget->GetChildByName(name);
		}

		RectangleInt UISystem::GetClipRectangle() const
		{
			return _ClipRectangle;
		}

		void UISystem::SetClipRectangle(const RectangleInt& rect)
		{
			_ClipRectangle = rect;
			ScissorState scissorState;
			scissorState.Enable = true;
			scissorState.Rectangle = _ClipRectangle;
			RenderSystem::Current()->SetScissorState(scissorState);
		}

		void UISystem::DisableClipRectangle()
		{
			ScissorState scissorState;
			scissorState.Enable = false;
			RenderSystem::Current()->SetScissorState(scissorState);
		}

		void UISystem::DrawLine(const Color32& color, int32 width, const PointInt& p0, const PointInt& p1)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = width;

			RenderSystem::Current()->DrawLine(&pen, p0.X, p0.Y, p1.X, p1.Y);
		}

		void UISystem::DrawRectangle(const Color32& color, int32 width, const RectangleInt& rect)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = width;

			RenderSystem::Current()->DrawRectangle(&pen, rect.X, rect.Y,
				rect.X + rect.Width, rect.Y + rect.Height);
		}

		void UISystem::FillRectangle(const Color32& color, const RectangleInt& rect)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = 0;

			RenderSystem::Current()->DrawRectangle(&pen, rect.X, rect.Y,
				rect.X + rect.Width, rect.Y + rect.Height);
		}

		void UISystem::DrawCircle(const Color32& color, int32 width, const PointInt& center, int32 radius)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = width;

			RenderSystem::Current()->DrawCircle(&pen, center.X, center.Y, radius);
		}

		void UISystem::FillCircle(const Color32& color, const PointInt& center, int32 radius)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = 0;

			RenderSystem::Current()->DrawCircle(&pen, center.X, center.Y, radius);
		}

		void UISystem::DrawTriangle(const Color32& color, int32 width, const PointInt& p0, const PointInt& p1, const PointInt& p2)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = width;

			RenderSystem::Current()->DrawTriangle(&pen, p0.X, p0.Y, p1.X, p1.Y, p2.X, p2.Y);
		}

		void UISystem::FillTriangle(const Color32& color, const PointInt& p0, const PointInt& p1, const PointInt& p2)
		{
			Pen pen;
			pen.Color = color;
			pen.Width = 0;

			RenderSystem::Current()->DrawTriangle(&pen, p0.X, p0.Y, p1.X, p1.Y, p2.X, p2.Y);
		}

		void UISystem::DrawText(const String& text, Font* font, const RectangleInt& area, const Color32& color, Alignment alignment)
		{
			if (text.IsEmpty() || font == NULL)
				return;

			TextFormat format = GetTextFormat(alignment);
			RectangleReal areaReal = RectangleReal(area.X, area.Y, area.Width, area.Height);
			Text::DrawText(text, font, areaReal, color, format);
		}

		void UISystem::DrawImage(Texture* image, const RectangleInt& area, Alignment alignment, ImageLayout layout, AlphaState* alpha)
		{
			static Sprite* sprite = new Sprite();
			static SpriteFrame* frame = NULL;

			if (frame == NULL)
			{
				frame = new SpriteFrame();
				sprite->AddSpriteFrame(frame);
			}

			frame->SetTexture(image);
			SizeInt imageSize = SizeInt(image->GetWidth(), image->GetHeight());
			frame->SetSourceRect(RectangleInt(PointInt(0, 0), imageSize));

			Vector2 position;
			if (layout == ImageLayout_None)
				position = GetPositionFromAlignment(imageSize, area, alignment);
			else
				position = Vector2(area.X, area.Y);
			sprite->SetPosition(position);

			Vector2 size;
			Vector2 scale = Vector2::One;
			switch (layout)
			{
			case ImageLayout_None:
				size = Vector2(imageSize.Width, imageSize.Height);
				break;
			case ImageLayout_Tile:
				size = Vector2(area.Width, area.Height);
				scale = Vector2((real32)area.Width / imageSize.Width,
					(real32)area.Height / imageSize.Height);
				break;
			case ImageLayout_Stretch:
				size = Vector2(area.Width, area.Height);
				break;
			case ImageLayout_Scale:
				//TODO: AspectRatioMin, AspectRatioMax
				size = Vector2(area.Width, area.Height);
				if ((area.Width/imageSize.Width) < (area.Height/imageSize.Height))
				{
					size.X = area.Width;
					size.Y = imageSize.Height*(area.Height/imageSize.Height);
				}
				else
				{
					size.X = imageSize.Width*(area.Width/imageSize.Width);
					size.Y = area.Height;
				}
				break;
			}
			sprite->SetSize(size);
			sprite->SetImageScale(scale);

			if (alpha != NULL)
				sprite->SetAlphaState(*alpha);

			sprite->Render();
		}
	}
}
