/*=============================================================================
Widget.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Widget.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_ABSTRACT(Widget);
		SE_IMPLEMENT_REFLECTION(Widget);

		Widget::Widget() :
			Object()
		{
			_Parent = NULL;
			_Enabled = true;
			_Visible = true;
			_Transparent = false;
			_Popup = false;

			_MinimumSize = SizeInt(0, 0);
			_MaximumSize = SizeInt(-1, -1);
			_Rectangle = RectangleInt(PointInt(0, 0), _MinimumSize);
			_Padding = RectangleInt(0, 0, 0, 0);

			_Children = new WidgetList(this);

			_HasFrame = false;
			_FrameSize = 0;
			_FrameColor = Color32::Black;

			_HasBackground = true;
			_ForegroundColor = Color32::Black;
			_BackgroundColor = Color32::Gray;
			_BackgroundImage = NULL;

			_Font = UISystem::Instance()->GetDefaultFont();
		}

		Widget::~Widget()
		{
			WidgetList::Iterator it = _Children->GetIterator();
			while (it.Next())
			{
				delete it.Current();
			}

			SE_DELETE(_Children);
		}

		void Widget::OnSerialized(BinarySerializer* context, BinaryStream& stream)
		{
			super::OnSerialized(context, stream);

			int count = _Children->Count();
			stream.WriteInt32(count);
			for (int i=0; i<count; i++)
			{
				context->Serialize(stream, _Children->GetItem(i), _Children->GetItem(i)->GetType());
			}
		}

		void Widget::OnDeserialized(BinarySerializer* context, BinaryStream& stream)
		{
			super::OnDeserialized(context, stream);

			int count = stream.ReadInt32();
			for (int i=0; i<count; i++)
			{
				Widget* child = (Widget*)context->Deserialize(stream);
				if (child != NULL)
					_Children->Add(child);
			}
		}

		void Widget::OnSerialized(XMLSerializer* context, XMLElement* element)
		{
			super::OnSerialized(context, element);

			XMLDocument* document = element->GetOwnerDocument();
			XMLElement* array = document->CreateElement("Children");
			int count = _Children->Count();
			array->SetAttribute("Count", String::ToString(count));
			for (int i=0; i<count; i++)
			{
				XMLElement* item = document->CreateElement("Child");
				context->Serialize(document, item, _Children->GetItem(i), _Children->GetItem(i)->GetType());
				array->AppendChild(item);
			}
			element->AppendChild(array);
		}

		void Widget::OnDeserialized(XMLSerializer* context, XMLElement* element)
		{
			super::OnDeserialized(context, element);

			XMLElement* array = (XMLElement*)element->GetItem("Children");
			int count = array->GetAttribute("Count").ToInt32();
			XMLElement* item = (XMLElement*)array->GetItem("Child");
			for (int i=0; i<count; i++)
			{
				XMLElement* type = (XMLElement*)item->GetFirstChild();
				Widget* child = (Widget*)context->Deserialize(item->GetOwnerDocument(), type);
				if (child != NULL)
					_Children->Add(child);
				item = (XMLElement*)item->GetNextSibling();
			}
		}

		void Widget::InternalSetParent(Widget* value)
		{
			_Parent = value;
		}

		void Widget::SetParent(Widget* value)
		{
			if (_Parent != NULL)
				_Parent->Children().Remove(this);

			if (value != NULL)
				value->Children().Add(this);
		}

		bool Widget::GetEnabled() const
		{
			if (!_Enabled)
				return false;

			if (_Parent == NULL)
				return true;

			return _Parent->GetEnabled();
		}

		void Widget::SetEnabled(bool value)
		{
			_Enabled = value;
		}

		bool Widget::GetVisible() const
		{
			if (!_Visible)
				return false;

			if (_Parent == NULL)
				return true;

			return _Parent->GetVisible();
		}

		void Widget::SetVisible(bool value)
		{
			_Visible = value;
		}

		void Widget::SetRectangle(const RectangleInt& value)
		{
			RectangleInt rect = value;

			if (rect.Width < GetMinimumSize().Width)
			{
				rect.Width = GetMinimumSize().Width;
			}
			else if (GetMaximumSize().Width > -1 && rect.Width > GetMaximumSize().Width)
			{
				rect.Width = GetMaximumSize().Width;
			}
			if (rect.Height < GetMinimumSize().Height)
			{
				rect.Height = GetMinimumSize().Height;
			}
			else if (GetMaximumSize().Height > -1 && rect.Height > GetMaximumSize().Height)
			{
				rect.Height = GetMaximumSize().Height;
			}

			bool moved = (_Rectangle.GetLocation() != rect.GetLocation());
			bool sized = (_Rectangle.GetSize() != rect.GetSize());

			_Rectangle = rect;

			if (moved)
				Move.Invoke(this, EventArgs::Empty);
			if (sized)
				Resize.Invoke(this, EventArgs::Empty);

			OnSize(EventArgs());
		}

		void Widget::SetRectangle(int32 x, int32 y, int32 width, int32 height)
		{
			SetRectangle(RectangleInt(x, y, width, height));
		}

		void Widget::SetLeft(int32 value)
		{
			SetRectangle(RectangleInt(PointInt(value, _Rectangle.GetLocation().Y), GetSize()));
		}

		void Widget::SetTop(int32 value)
		{
			SetRectangle(RectangleInt(PointInt(_Rectangle.GetLocation().X, value), GetSize()));
		}

		void Widget::SetWidth(int32 value)
		{
			SetRectangle(RectangleInt(GetPosition(), SizeInt(value, _Rectangle.GetSize().Height)));
		}

		void Widget::SetHeight(int32 value)
		{
			SetRectangle(RectangleInt(GetPosition(), SizeInt(_Rectangle.GetSize().Width, value)));
		}

		Widget* Widget::GetChildByName(const String& name) const
		{
			WidgetList::Iterator it = _Children->GetIterator();
			while (it.Next())
			{
				Widget* child = it.Current();
				if (child->GetName() == name)
					return child;

				Widget* widget = child->GetChildByName(name);
				if (widget != NULL)
					return widget;
			}

			return NULL;
		}

		Widget* Widget::GetChildAtPoint(const PointInt& value) const
		{
			//WidgetList::Iterator it = _Children->GetIterator();
			//while (it.Next())
			int count = _Children->Count();
			for (int i=count-1; i>=0; i--)
			{
				//Widget* child = it.Current();
				Widget* child = _Children->GetItem(i);
				if (child->GetVisible())
				{
					Widget* hit = child->GetChildAtPoint(value);
					if (hit != NULL)
						return hit;
					else if (child->Contains(value))
						return child;
				}
			}

			return NULL;
		}

		bool Widget::Contains(const PointInt& value) const
		{
			if (!GetVisible() || GetTransparent())
				return false;

			//RectangleInt rect = LocalToGlobal(RectangleInt(PointInt(0, 0), GetSize()));
			RectangleInt rect = GetClipRectangle();
			return rect.Contains(value.X, value.Y);
		}

		bool Widget::Contains(Widget* value) const
		{
			if (value == NULL)
				return false;

			//WidgetList::Iterator it = _Children->GetIterator();
			//while (it.Next())
			int count = _Children->Count();
			for (int i=count-1; i>=0; i--)
			{
				//Widget* child = it.Current();
				Widget* child = _Children->GetItem(i);
				if (child == value)
					return true;

				if (child->Contains(value))
					return true;
			}

			return false;
		}

		void Widget::Enable()
		{
			_Enabled = true;
		}

		void Widget::Disable()
		{
			_Enabled = false;
		}

		void Widget::Show()
		{
			_Visible = true;
		}

		void Widget::Hide()
		{
			_Visible = false;
		}

		void Widget::MoveToFront()
		{
			Widget* parent = GetParent();
			if (parent == NULL)
				return;

			if (parent->Children().Count() > 1)
			{
				parent->Children().Remove(this);
				parent->Children().Add(this);
			}

			parent->MoveToFront();
		}

		void Widget::MoveToBack()
		{
			Widget* parent = GetParent();
			if (parent == NULL)
				return;

			if (parent->Children().Count() > 1)
			{
				parent->Children().Remove(this);
				parent->Children().Insert(0, this);
			}
		}

		RectangleInt Widget::LocalToGlobal(const RectangleInt& value) const
		{
			PointInt result = LocalToGlobal(value.GetLocation());

			return RectangleInt(result, value.GetSize());
		}

		PointInt Widget::LocalToGlobal(const PointInt& value) const
		{
			PointInt result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->LocalToGlobal(value);
			}

			result.X += GetLeft();
			result.Y += GetTop();

			return result;
		}

		int32 Widget::LocalToGlobalX(int32 value) const
		{
			int32 result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->LocalToGlobalX(value);
			}

			result += GetLeft();

			return result;
		}

		int32 Widget::LocalToGlobalY(int32 value) const
		{
			int32 result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->LocalToGlobalY(value);
			}

			result += GetTop();

			return result;
		}

		RectangleInt Widget::GlobalToLocal(const RectangleInt& value) const
		{
			PointInt result = GlobalToLocal(value.GetLocation());

			return RectangleInt(result, value.GetSize());
		}

		PointInt Widget::GlobalToLocal(const PointInt& value) const
		{
			PointInt result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->GlobalToLocal(value);
			}

			result.X -= GetLeft();
			result.Y -= GetTop();

			return result;
		}

		int32 Widget::GlobalToLocalX(int32 value) const
		{
			int32 result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->GlobalToLocalX(value);
			}

			result -= GetLeft();

			return result;
		}

		int32 Widget::GlobalToLocalY(int32 value) const
		{
			int32 result = value;
			if (GetParent() != NULL)
			{
				result = GetParent()->GlobalToLocalY(value);
			}

			result -= GetTop();

			return result;
		}

		PointInt Widget::GetClientOrigin() const
		{
			PointInt result = PointInt(0, 0);
			if (GetHasFrame())
			{
				result.X += GetFrameSize();
				result.Y += GetFrameSize();
			}

			return result;
		}

		SizeInt Widget::GetClientSize() const
		{
			SizeInt result = GetRectangle().GetSize();
			if (GetHasFrame())
			{
				result.Width -= GetFrameSize() * 2;
				result.Height -= GetFrameSize() * 2;

				result.Width = Math::Max(0, result.Width);
				result.Height = Math::Max(0, result.Height);
			}

			return result;
		}

		RectangleInt Widget::GetClientRectangle() const
		{
			RectangleInt result = RectangleInt(PointInt(0, 0), GetSize());
			if (GetHasFrame())
			{
				result.X += GetFrameSize();
				result.Y += GetFrameSize();
				result.Width -= GetFrameSize() * 2;
				result.Height -= GetFrameSize() * 2;

				result.Width = Math::Max(0, result.Width);
				result.Height = Math::Max(0, result.Height);
			}

			return result;
		}

		RectangleInt Widget::GetClipRectangle() const
		{
			RectangleInt result;

			RectangleInt displayRect = LocalToGlobal(RectangleInt(PointInt(0, 0), GetSize()));
			//RectangleInt displayRect = LocalToGlobal(GetClientRectangle());
			if (!GetPopup() && GetParent() != NULL)
				result = RectangleInt::Intersect(GetParent()->GetClipRectangle(), displayRect);
			else
				result = displayRect;

			return result;
		}

		void Widget::Update(real64 elapsed)
		{
			PreUpdate.Invoke(this, UpdateEventArgs(elapsed));

			DoUpdate(elapsed);

			WidgetList::Iterator it = _Children->GetIterator();
			while (it.Next())
			{
				it.Current()->Update(elapsed);
			}

			PostUpdate.Invoke(this, UpdateEventArgs(elapsed));
		}

		void Widget::Render()
		{
			if (!GetVisible())
				return;

			RectangleInt clipRect = GetClipRectangle();
			if (GetParent() != NULL)
			{
				UISystem::Instance()->SetClipRectangle(clipRect);
			}

			if (PreRender.Invoke(this, DrawEventArgs(NULL, clipRect)))
			{
				DoRender();
			}

			RectangleInt clientRect = GetClientRectangle();
			clientRect.SetLocation(PointInt(clientRect.X + GetLeft(), clientRect.Y + GetTop()));
			UISystem::Instance()->SetClipRectangle(RectangleInt::Intersect(clipRect, clientRect));

			WidgetList::Iterator it = _Children->GetIterator();
			while (it.Next())
			{
				it.Current()->Render();
			}

			PostRender.Invoke(this, DrawEventArgs(NULL, clipRect));
		}

		void Widget::DoUpdate(real64 elapsed)
		{
		}

		void Widget::DoRender()
		{
			if (GetHasFrame())
			{
				RectangleInt frameRect = LocalToGlobal(RectangleInt(PointInt(0, 0), GetSize()));
				UISystem::Instance()->DrawRectangle(GetFrameColor(), GetFrameSize(), frameRect);
			}

			if (GetHasBackground())
			{
				if (GetBackgroundImage() == NULL)
				{
					RectangleInt bgRect = LocalToGlobal(GetClientRectangle());
					UISystem::Instance()->FillRectangle(GetBackgroundColor(), bgRect);
				}
				else
				{
					UISystem::Instance()->DrawImage(GetBackgroundImage(),
						LocalToGlobal(GetClientRectangle()), Alignment_TopLeft, ImageLayout_Tile);
				}
			}
		}

		void Widget::Refresh()
		{
		}

		void Widget::OnMove(EventArgs& e)
		{
			Move.Invoke(this, e);
		}

		void Widget::OnSize(EventArgs& e)
		{
			Resize.Invoke(this, e);
		}

		void Widget::OnRender(DrawEventArgs& e)
		{
		}

		void Widget::OnMouseEnter(EventArgs& e)
		{
			MouseEnter.Invoke(this, e);
		}

		void Widget::OnMouseLeave(EventArgs& e)
		{
			MouseLeave.Invoke(this, e);
		}

		void Widget::OnMouseMove(MouseEventArgs& e)
		{
			MouseMove.Invoke(this, e);
		}

		void Widget::OnMouseHover(MouseEventArgs& e)
		{
			MouseHover.Invoke(this, e);
		}

		void Widget::OnMouseWheel(MouseEventArgs& e)
		{
			MouseWheel.Invoke(this, e);
		}

		void Widget::OnMouseDown(MouseEventArgs& e)
		{
			MouseDown.Invoke(this, e);
		}

		void Widget::OnMouseUp(MouseEventArgs& e)
		{
			MouseUp.Invoke(this, e);
		}

		void Widget::OnMouseClick(EventArgs& e)
		{
			MouseClick.Invoke(this, e);
		}

		void Widget::OnMouseDoubleClick(EventArgs& e)
		{
			MouseDoubleClick.Invoke(this, e);
		}

		void Widget::OnKeyDown(KeyEventArgs& e)
		{
			KeyDown.Invoke(this, e);
		}

		void Widget::OnKeyUp(KeyEventArgs& e)
		{
			KeyUp.Invoke(this, e);
		}

		void Widget::OnKeyChar(KeyCharEventArgs& e)
		{
			KeyChar.Invoke(this, e);
		}
	}
}
