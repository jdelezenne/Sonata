/*=============================================================================
ComboBox.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_COMBOBOX_H_
#define _SE_UI_COMBOBOX_H_

#include "UI/Common.h"
#include "UI/Widget.h"
#include "UI/TextBox.h"
#include "UI/PushButton.h"
#include "UI/ListBox.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a combo box widget.
		*/
		class SE_UI_EXPORT ComboBox : public Widget
		{
			SE_DECLARE_CLASS(ComboBox, Widget);

			SE_BEGIN_REFLECTION(ComboBox);
				SE_Field(_Sorted, bool, Public);
				SE_Field(_DropDownWidth, int32, Public);
				SE_Field(_DropDownHeight, int32, Public);
				SE_Field(_Editable, bool, Public);
				SE_Field(_MaxLength, int32, Public);
			SE_END_REFLECTION(ComboBox);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ComboBox();

			/** Destructor. */
			virtual ~ComboBox();
			//@}

			/** @name Properties. */
			//@{
			/** Gets the items of the ComboBox. */
			ListItemList& Items();

			/** Gets or sets a value indicating whether the items in the ComboBox are sorted alphabetically. */
			bool GetSorted() const { return _Sorted; }
			void SetSorted(bool value) { _Sorted = value; }

			/** Gets or sets the zero-based index of the currently selected item in a ComboBox. */
			int32 GetSelectedIndex() const;
			void SetSelectedIndex(int32 value);

			/** Gets or sets the currently selected item in a ComboBox. */
			ListItem* GetSelectedItem() const;
			void SetSelectedItem(ListItem* value);

			/** Gets or sets the width of the of the drop-down portion of a combo box. */
			int32 GetDropDownWidth() const { return _DropDownWidth; }
			void SetDropDownWidth(int32 value) { _DropDownWidth = value; }

			/** Gets or sets the height in pixels of the drop-down portion of the ComboBox. */
			int32 GetDropDownHeight() const { return _DropDownHeight; }
			void SetDropDownHeight(int32 value) { _DropDownHeight = value; }

			/** Gets or sets a value indicating whether the tetx portion is editable. */
			bool GetEditable() const { return _Editable; }
			void SetEditable(bool value) { _Editable = value; }

			/** Gets or sets the maximum number of characters possible in the editable portion of a combo box. */
			int32 GetMaxLength() const { return _MaxLength; }
			void SetMaxLength(int32 value) { _MaxLength = value; }

			/** Gets or sets the text that is selected in the editable portion of a ComboBox. */
			String GetSelectedText() const;
			void SetSelectedText(String value);

			/** Gets or sets the starting index of text selected in the combo box. */
			int32 GetSelectionStart() const;
			void SetSelectionStart(int32 value);

			/** Gets or sets the number of characters selected in the editable portion of the combo box. */
			int32 GetSelectionLength() const;
			void SetSelectionLength(int32 value);

			const Color32& GetSelectedBackgroundColor() const { return _SelectedBackgroundColor; }
			void SetSelectedBackgroundColor(const Color32& value) { _SelectedBackgroundColor = value; }

			const Color32& GetSelectedTextColor() const { return _SelectedTextColor; }
			void SetSelectedTextColor(const Color32& value) { _SelectedTextColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Returns a value indicating whether the specified item is selected. */
			bool GetSelected(int32 index) const;

			/** Selects or clears the selection for the specified item in a ComboBox. */
			void SetSelected(int32 index, bool value);

			/** Returns the height for an item in the ComboBox. */
			int32 GetItemHeight(int32 index) const;

			/** Returns the bounding rectangle for an item in the ComboBox. */
			RectangleInt GetItemRectangle(int32 index) const;

			/** Gets the combined height of all items in the ComboBox. */
			int32 GetTotalHeight() const;

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnSize(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the selected item changes. */
			DefaultEvent SelectedItemChanged;

			/** Occurs when an item is being drawn. */
			DefaultEvent DrawItem;

			/** Occurs when the drop-down portion of a ComboBox is shown. */
			DefaultEvent DropDown;
			//@}

		protected:
			void UpdateLayout();

			virtual bool OnButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnListBoxMouseUp(Object* sender, const MouseEventArgs& e);

		protected:
			bool _Sorted;
			int32 _DropDownWidth;
			int32 _DropDownHeight;
			bool _Editable;
			int32 _MaxLength;

			TextBox* _TextBox;
			PushButton* _Button;
			ListBox* _ListBox;

			Color32 _SelectedBackgroundColor;
			Color32 _SelectedTextColor;
		};
	}
}

#endif 
