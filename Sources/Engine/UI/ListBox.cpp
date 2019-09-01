/*=============================================================================
ListBox.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ListBox.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_ABSTRACT(ListItem);
		SE_IMPLEMENT_REFLECTION(ListItem);

		ListItem::ListItem() :
			Object()
		{
			_Owner = NULL;
			_Enabled = true;
			_Selected = false;

			_TextObj = new Text();
		}

		ListItem::ListItem(const String& text) :
			Object()
		{
			_Owner = NULL;
			_Enabled = true;
			_Selected = false;

			_Text = text;

			_TextObj = new Text();
		}

		ListItem::~ListItem()
		{
		}

		int32 ListItem::GetHeight() const
		{
			if (_Owner == NULL)
				return 0;
			else
				return (int32)Text::MeasureText(_Text, _Owner->GetFont()).Height;
		}

		void ListItem::Update(real64 elapsed)
		{
		}

		void ListItem::Render(const PointInt& pt)
		{
			if (GetOwner() == NULL)
				return;

			RectangleInt itemRect = GetOwner()->LocalToGlobal(RectangleInt(pt.X, pt.Y,
				GetOwner()->GetClientSize().Width, GetHeight()));

			if (GetSelected())
			{
				UISystem::Instance()->FillRectangle(
					((ListBox*)GetOwner())->GetSelectedBackgroundColor(), itemRect);
			}

			if (!GetText().IsEmpty() && GetOwner()->GetFont() != NULL)
			{
				Color32 color = GetSelected() ?
					((ListBox*)GetOwner())->GetSelectedTextColor() : GetOwner()->GetForegroundColor();

				_TextObj->SetText(GetText());
				_TextObj->SetFont(GetOwner()->GetFont());
				_TextObj->SetColor(color);
				_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_TopLeft));
				_TextObj->SetBounds(RectangleReal(itemRect.X, itemRect.Y, itemRect.Width, itemRect.Height));
				_TextObj->Render();
			}
		}


		SE_IMPLEMENT_CLASS(ListBox);
		SE_IMPLEMENT_REFLECTION(ListBox);

		ListBox::ListBox() :
			super()
		{
			SetHasFrame(true);
			SetFrameSize(1);
			SetBackgroundColor(Color32::White);

			_Items.SetOwner(this);
			_AllowSelection = true;
			_MultiSelection = false;
			_Sorted = false;
			_ScrollAlwaysVisible = false;

			_FocusedIndex = -1;
			_VScrollBar = new ScrollBar();

			Children().Add(_VScrollBar);

			_VScrollBar->SetOrientation(Orientation_Vertical);
			_VScrollBar->ValueChanged += new DefaultEventDelegateM<ListBox>(this, &ListBox::OnScrollBarValueChanged);

			_BackgroundColor = Color32::White;

			_SelectedBackgroundColor = Color32::Blue;
			_SelectedTextColor = Color32::White;

			UpdateScrollBars();
		}

		ListBox::~ListBox()
		{
			//SE_DELETE(_VScrollBar);

			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				delete it.Current();
			}
		}

		int32 ListBox::GetSelectedIndex() const
		{
			if (!_AllowSelection)
				return -1;

			int32 count = _Items.Count();
			for (int i=0; i<count; i++)
			{
				ListItem* item = _Items[i];
				if (item->GetSelected())
					return i;
			}

			return -1;
		}

		void ListBox::SetSelectedIndex(int32 value)
		{
			if ((value < 0 && value != -1) || (value >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return;
			}

			if (!_AllowSelection)
				return;

			if (!_MultiSelection)
			{
				ClearSelected();
			}

			if (value != -1)
			{
				_Items[value]->SetSelected(true);
			}
		}

		ListItem* ListBox::GetSelectedItem() const
		{
			if (!_AllowSelection)
				return NULL;

			int32 count = _Items.Count();
			for (int i=0; i<count; i++)
			{
				ListItem* item = _Items[i];
				if (item->GetSelected())
					return item;
			}

			return NULL;
		}

		void ListBox::SetSelectedItem(ListItem* value)
		{
			if (value != NULL && value->GetOwner() != this)
			{
				return;
			}

			if (!_AllowSelection)
				return;

			if (!_MultiSelection)
			{
				ClearSelected();
			}

			if (value != NULL)
			{
				value->SetSelected(true);
			}
		}

		String ListBox::GetSelectedText() const
		{
			ListItem* item = GetSelectedItem();
			if (item != NULL)
				return item->GetText();
			else
				return String::Empty;
		}

		void ListBox::SetSelectedText(const String& value)
		{
			ListItem* item = GetSelectedItem();
			if (item != NULL)
				return item->SetText(value);
		}

		const Array<int32> ListBox::GetSelectedIndices() const
		{
			if (!_AllowSelection)
				return Array<int32>();

			Array<int32> indices;

			int32 count = _Items.Count();
			for (int i=0; i<count; i++)
			{
				ListItem* item = _Items[i];
				if (item->GetSelected())
					indices.Add(i);
			}

			return indices;
		}

		const Array<ListItem*> ListBox::GetSelectedItems() const
		{
			if (!_AllowSelection)
				return Array<ListItem*>();

			Array<ListItem*> items;

			int32 count = _Items.Count();
			for (int i=0; i<count; i++)
			{
				ListItem* item = _Items[i];
				if (item->GetSelected())
					items.Add(item);
			}

			return items;
		}

		void ListBox::ClearSelected()
		{
			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				it.Current()->SetSelected(false);
			}
		}

		bool ListBox::GetSelected(int32 index) const
		{
			if ((index < 0) || (index >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return false;
			}

			return _Items[index]->GetSelected();
		}

		void ListBox::SetSelected(int32 index, bool value)
		{
			if ((index < 0) || (index >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return;
			}

			if (!_AllowSelection)
				return;

			bool selected = _Items[index]->GetSelected();

			if (!_MultiSelection)
			{
				ClearSelected();
			}

			_Items[index]->SetSelected(value);

			if (selected != value)
			{
				SelectedItemChanged.Invoke(this, EventArgs::Empty);
			}
		}

		int32 ListBox::GetItemHeight(int32 index) const
		{
			if ((index < 0) || (index >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return 0;
			}

			return _Items[index]->GetHeight();
		}

		RectangleInt ListBox::GetItemRectangle(int32 index) const
		{
			if ((index < 0) || (index >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return RectangleInt::Empty;
			}

			RectangleInt rect;
			rect.X = 0;

			rect.Y = _VScrollBar->GetValue();
			int32 i = 0;
			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next() && i != index)
			{
				rect.Y += it.Current()->GetHeight();
				i++;
			}

			rect.Width = GetClientSize().Width;
			if (_VScrollBar->GetVisible())
				rect.Width -= _VScrollBar->GetWidth();
			rect.Height = _Items[index]->GetHeight();

			return rect;
		}

		int32 ListBox::GetTotalHeight() const
		{
			int32 height = 0;

			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				height += it.Current()->GetHeight();
			}

			return height;
		}

		int32 ListBox::GetIndexFromPoint(const PointInt& pt) const
		{
			if (!GetClientRectangle().Contains(pt))
				return -1;

			int32 y = -_VScrollBar->GetValue();

			int32 count = _Items.Count();
			for (int i=0; i<count; i++)
			{
				ListItem* item = _Items[i];
				y += item->GetHeight();
				if (pt.Y < y)
					return i;
			}

			return -1;
		}

		ListItem* ListBox::GetItemFromPoint(const PointInt& pt) const
		{
			int32 index = GetIndexFromPoint(pt);
			if (index == -1)
				return NULL;
			else
				return _Items[index];
		}

		int32 ListBox::GetFirstIndex() const
		{
			return GetIndexFromPoint(PointInt(0, _VScrollBar->GetValue()));
		}

		int32 ListBox::FindText(const String& text, int32 startIndex, bool matchExact, bool caseSensitive) const
		{
			if (_Items.Count() == 0 && startIndex == 0)
				return -1;

			if ((startIndex < 0) || (startIndex >= _Items.Count()))
			{
				SEthrow(ArgumentOutOfRangeException("index"));
				return -1;
			}

			int32 count = _Items.Count();
			for (int i=startIndex; i<count; i++)
			{
				ListItem* item = _Items[i];
				if (matchExact)
				{
					if (item->GetText().CompareTo(text, !caseSensitive) == 0)
						return i;
				}
				else
				{
					if (item->GetText().Left(text.Length()).CompareTo(text, !caseSensitive) == 0)
						return i;
				}
			}

			return -1;
		}

		void ListBox::UpdateLayout()
		{
			bool needVScroll = (_ScrollAlwaysVisible || (GetTotalHeight() > GetHeight()));
			_VScrollBar->SetVisible(needVScroll);

			_VScrollBar->SetPosition(PointInt(GetWidth() - _VScrollBar->GetWidth(), 0));
			_VScrollBar->SetHeight(GetHeight());
		}

		void ListBox::UpdateScrollBars()
		{
			if (_Items.Count() > 0)
				_VScrollBar->SetSingleStep(_Items[0]->GetHeight());
			else
				_VScrollBar->SetSingleStep(1);

			int32 visibleHeight = GetHeight();
			_VScrollBar->SetPageStep(visibleHeight);
			_VScrollBar->SetMaximum(GetTotalHeight());
		}

		bool ListBox::OnScrollBarValueChanged(Object* sender, const EventArgs& e)
		{
			return true;
		}

		void ListBox::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);

			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				ListItem* item = it.Current();
				item->Update(elapsed);
			}
		}

		void ListBox::DoRender()
		{
			super::DoRender();

			RectangleInt itemRect = GetClientRectangle();
			int32 y = 0;

			int index = 0;
			ListItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				ListItem* item = it.Current();

				if (itemRect.Y > itemRect.Height)
				{
					//Don't draw items after the bottom
					break;
				}
				else if (y >= _VScrollBar->GetValue())
				{
					if (DrawItem.Invoke(this, DrawItemEventArgs(index, item, GetItemRectangle(index))))
					{
						item->Render(PointInt(itemRect.X, itemRect.Y));
					}

					y += item->GetHeight();
					itemRect.Y += item->GetHeight();
				}
				else
				{
					y += item->GetHeight();
				}

				index++;
			}
		}

		void ListBox::Refresh()
		{
			super::Refresh();

			UpdateLayout();
			UpdateScrollBars();
		}

		void ListBox::OnSize(EventArgs& e)
		{
			super::OnSize(e);

			UpdateLayout();
		}

		void ListBox::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (UISystem::Instance()->GetCapture() == this)
			{
			}
		}

		void ListBox::OnMouseWheel(MouseEventArgs& e)
		{
			super::OnMouseWheel(e);

			if (!GetEnabled() || !GetVisible())
				return;

			_VScrollBar->OnMouseWheel(e);
		}

		void ListBox::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->SetCapture(this);

				PointInt mouse = GlobalToLocal(e.Location());

				if (!GetAllowSelection())
					return;

				int32 index = GetIndexFromPoint(mouse);
				if ((index < 0) || (index >= _Items.Count()))
					return;

				if ((e.Modifiers() & ModifierKeys_Control) != 0)
				{
					SetSelected(index, !GetSelected(index));
				}
				else
				{
					if (GetMultiSelection())
						ClearSelected();

					SetSelected(index, true);
				}

				if (GetMultiSelection())
				{
					if ((e.Modifiers() & ModifierKeys_Shift) != 0)
					{
						if (_FocusedIndex != -1 && _FocusedIndex != index)
						{
							if (_FocusedIndex < index)
							{
								for (int i=_FocusedIndex+1; i<=index; i++)
									SetSelected(i, true);
							}
							if (_FocusedIndex > index)
							{
								for (int i=_FocusedIndex-1; i>=index; i--)
									SetSelected(i, true);
							}
						}
					}
				}

				_FocusedIndex = index;
			}
		}

		void ListBox::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->ReleaseCapture();
			}
		}

		void ListBox::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (_Items.Count() > 0)
			{
				if (e.Key() == Input::Key_Left || e.Key() == Input::Key_Up)
				{
					if (_FocusedIndex - 1 >= 0)
					{
						SetSelectedIndex(_FocusedIndex - 1);
						int32 index = GetSelectedIndex();
						_FocusedIndex--;
						RectangleInt rect = GetItemRectangle(index);
						if (rect.Y < 0)
						{
							_VScrollBar->SetValue(rect.Y);
						}
					}
				}
				else if (e.Key() == Input::Key_Right || e.Key() == Input::Key_Down)
				{
					if (_FocusedIndex + 1 < _Items.Count())
					{
						SetSelectedIndex(_FocusedIndex + 1);
						int32 index = GetSelectedIndex();
						_FocusedIndex++;
						RectangleInt rect = GetItemRectangle(index);
						if (rect.GetBottom() > GetHeight())
						{
							_VScrollBar->SetValue(_VScrollBar->GetValue() + rect.GetHeight());
						}
					}
				}
			}
		}
	}
}
