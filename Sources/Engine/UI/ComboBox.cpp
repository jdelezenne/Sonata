/*=============================================================================
ComboBox.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ComboBox.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(ComboBox);
		SE_IMPLEMENT_REFLECTION(ComboBox);

		ComboBox::ComboBox() :
			super()
		{
			SetHasFrame(true);
			SetFrameSize(1);
			SetBackgroundColor(Color32::White);

			_Sorted = false;
			_DropDownWidth = 0;
			_DropDownHeight = 100;
			_Editable = true;
			_MaxLength = 0;

			_TextBox = new TextBox();
			_Button = new PushButton();
			_ListBox = new ListBox();

			Children().Add(_TextBox);
			Children().Add(_Button);
			Children().Add(_ListBox);

			_ListBox->SetPopup(true);
			_Button->Click += new DefaultEventDelegateM<ComboBox>(this, &ComboBox::OnButtonClick);
			_ListBox->MouseUp += new MouseEventDelegateM<ComboBox>(this, &ComboBox::OnListBoxMouseUp);

			_SelectedBackgroundColor = Color32::Blue;
			_SelectedTextColor = Color32::White;
		}

		ComboBox::~ComboBox()
		{
			SE_DELETE(_TextBox);
			SE_DELETE(_Button);
			SE_DELETE(_ListBox);
		}

		ListItemList& ComboBox::Items()
		{
			return _ListBox->Items();
		}

		int32 ComboBox::GetSelectedIndex() const
		{
			return _ListBox->GetSelectedIndex();
		}

		void ComboBox::SetSelectedIndex(int32 value)
		{
			return _ListBox->SetSelectedIndex(value);
		}

		ListItem* ComboBox::GetSelectedItem() const
		{
			return _ListBox->GetSelectedItem();
		}

		void ComboBox::SetSelectedItem(ListItem* value)
		{
			return _ListBox->SetSelectedItem(value);
		}

		String ComboBox::GetSelectedText() const
		{
			return _TextBox->GetText();
		}

		void ComboBox::SetSelectedText(String value)
		{
			return _TextBox->SetText(value);
		}

		int32 ComboBox::GetSelectionStart() const
		{
			return _TextBox->GetSelectionStart();
		}

		void ComboBox::SetSelectionStart(int32 value)
		{
			return _TextBox->SetSelectionStart(value);
		}

		int32 ComboBox::GetSelectionLength() const
		{
			return _TextBox->GetSelectionLength();
		}

		void ComboBox::SetSelectionLength(int32 value)
		{
			return _TextBox->SetSelectionLength(value);
		}

		bool ComboBox::GetSelected(int32 index) const
		{
			return _ListBox->GetSelected(index);
		}

		void ComboBox::SetSelected(int32 index, bool value)
		{
			return _ListBox->SetSelected(index, value);
		}

		int32 ComboBox::GetItemHeight(int32 index) const
		{
			return _ListBox->GetItemHeight(index);
		}

		RectangleInt ComboBox::GetItemRectangle(int32 index) const
		{
			return _ListBox->GetItemRectangle(index);
		}

		int32 ComboBox::GetTotalHeight() const
		{
			return _ListBox->GetTotalHeight();
		}

		void ComboBox::UpdateLayout()
		{
			int32 width = GetWidth();

			_TextBox->SetPosition(PointInt(0, 0));
			_TextBox->SetWidth(width - _Button->GetWidth());

			_Button->SetPosition(PointInt(_TextBox->GetWidth(), 0));

			_ListBox->SetPosition(PointInt(0, _TextBox->GetHeight()));
			int32 dropDownWidth = ((_DropDownWidth == 0) ? width : _DropDownWidth);
			_ListBox->SetSize(SizeInt(dropDownWidth, _DropDownHeight));
			_ListBox->Hide();
		}

		bool ComboBox::OnButtonClick(Object* sender, const EventArgs& e)
		{
			if (!_ListBox->GetVisible())
			{
				int32 index = _ListBox->FindText(_TextBox->GetText());
				if (index != -1)
					_ListBox->SetSelectedIndex(index);

				_ListBox->Show();

				DropDown.Invoke(this, EventArgs::Empty);
			}
			else
			{
				_ListBox->Hide();
			}

			return true;
		}

		bool ComboBox::OnListBoxMouseUp(Object* sender, const MouseEventArgs& e)
		{
			_ListBox->Hide();
			_TextBox->SetText(_ListBox->GetSelectedText());

			SelectedItemChanged.Invoke(this, EventArgs::Empty);

			return true;
		}

		void ComboBox::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);

			// There is always a selected item
			if (Items().Count() > 0 && GetSelectedIndex() == -1)
			{
				SetSelectedIndex(0);
			}

			_TextBox->SetText(_ListBox->GetSelectedText());
		}

		void ComboBox::DoRender()
		{
			super::DoRender();
		}

		void ComboBox::OnSize(EventArgs& e)
		{
			super::OnSize(e);

			int32 height = GetHeight();
			_TextBox->SetHeight(height);
			_Button->SetSize(SizeInt(height, height));

			UpdateLayout();
		}

		void ComboBox::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void ComboBox::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void ComboBox::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void ComboBox::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}
	}
}
