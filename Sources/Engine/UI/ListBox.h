/*=============================================================================
ListBox.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_LISTBOX_H_
#define _SE_UI_LISTBOX_H_

#include "UI/Common.h"
#include "UI/Widget.h"
#include "UI/ScrollBar.h"

namespace SonataEngine
{
	namespace UI
	{
		class SE_UI_EXPORT ListItem : public Object
		{
			SE_DECLARE_CLASS(ListItem, Object);

			SE_BEGIN_REFLECTION(ListItem);
				SE_Field(_Enabled, bool, Public);
				SE_Field(_Text, String, Public);
			SE_END_REFLECTION(ListItem);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ListItem();
			ListItem(const String& text);

			/** Destructor. */
			virtual ~ListItem();
			//@}

			/** @name Properties. */
			//@{
			/** Gets the owner widget of this item. */
			Widget* GetOwner() const { return _Owner; }

			/** Gets or sets user data associated with the item. */
			Variant GetUserData() const { return _userData; }
			void SetUserData(const Variant& value) { _userData = value; }

			/** Gets or sets a value indicating whether the item is enabled. */
			bool GetEnabled() const { return _Enabled; }
			void SetEnabled(bool value) { _Enabled = value; }

			/** Gets or sets a value indicating whether the item is selected. */
			bool GetSelected() const { return _Selected; }
			void SetSelected(bool value) { _Selected = value; }

			/** Gets the height of the item. */
			int32 GetHeight() const;

			/** Gets or sets the text of the item. */
			const String& GetText() const { return _Text; }
			void SetText(const String& value) { _Text = value; }
			//@}

			/** Updates of the item. */
			void Update(real64 elapsed);

			/** Renders the item. */
			void Render(const PointInt& pt);

		protected:
			friend class ListItemList;
			void SetOwner(Widget* value) { _Owner = value; }

		protected:
			Widget* _Owner;
			Variant _userData;
			bool _Enabled;
			bool _Selected;
			String _Text;

			TextPtr _TextObj;
		};

		/** Provides data for the draw item events. */
		class SE_UI_EXPORT DrawItemEventArgs : public EventArgs
		{
		public:
			DrawItemEventArgs(int32 index, ListItem* item, RectangleInt rectangle) :
				EventArgs(),
				_Index(index),
				_Item(item),
				_Rectangle(rectangle)
			{
			}

			/** Gets the index value of the item that is being drawn. */
			int32 Index() const { return _Index; }

			/** Gets the item that is being drawn. */
			ListItem* Item() const { return _Item; }

			/** Gets the rectangle that represents the bounds of the item that is being drawn. */
			RectangleInt Rectangle() const { return _Rectangle; }

		protected:
			int32 _Index;
			ListItem* _Item;
			RectangleInt _Rectangle;
		};

		class SE_UI_EXPORT ListItemList : public Array<ListItem*>
		{
			typedef Array<ListItem*> super;

		public:
			ListItemList() :
				super(),
				_Owner(NULL)
			{
			}

			ListItemList(Widget* owner) :
				super(),
				_Owner(owner)
			{
			}

			void SetOwner(Widget* value) { _Owner = value; }

			virtual void Add(ListItem* value)
			{
				super::Add(value);
				value->SetOwner(_Owner);
				_Owner->Refresh();
			}

			virtual void Insert(int32 index, ListItem* value)
			{
				super::Insert(index, value);
				value->SetOwner(_Owner);
			}

			virtual void Remove(ListItem* value)
			{
				value->SetOwner(NULL);
				super::Remove(value);
				_Owner->Refresh();
			}

			virtual void Clear()
			{
				ListItemList::Iterator it = GetIterator();
				while (it.Next())
				{
					it.Current()->SetOwner(NULL);
				}

				super::Clear();
				_Owner->Refresh();
			}

		protected:
			Widget* _Owner;
		};

		/**
			@brief Represents a widget to display a list of items.
		*/
		class SE_UI_EXPORT ListBox : public Widget
		{
			SE_DECLARE_CLASS(ListBox, Widget);

			SE_BEGIN_REFLECTION(ListBox);
				SE_Field(_AllowSelection, bool, Public);
				SE_Field(_MultiSelection, bool, Public);
				SE_Field(_Sorted, bool, Public);
			SE_END_REFLECTION(ListBox);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ListBox();

			/** Destructor. */
			virtual ~ListBox();
			//@}

			/** @name Properties. */
			//@{
			/** Gets the items of the ListBox. */
			ListItemList& Items() { return _Items; }

			/** Gets a value indicating whether the ListBox currently enables selection of list items. */
			bool GetAllowSelection() const { return _AllowSelection; }
			void SetAllowSelection(bool value) { _AllowSelection = value; }

			/** Gets or sets whether multiple items can be selected. */
			bool GetMultiSelection() const { return _MultiSelection; }
			void SetMultiSelection(bool value) { _MultiSelection = value; }

			/** Gets or sets a value indicating whether the items in the ListBox are sorted alphabetically. */
			bool GetSorted() const { return _Sorted; }
			void SetSorted(bool value) { _Sorted = value; }

			/** Gets or sets a value indicating whether the vertical scroll bar is shown at all times. */
			bool GetScrollAlwaysVisible() const { return _ScrollAlwaysVisible; }
			void SetScrollAlwaysVisible(bool value) { _ScrollAlwaysVisible = value; }

			/** Gets or sets the zero-based index of the currently selected item in a ListBox. */
			int32 GetSelectedIndex() const;
			void SetSelectedIndex(int32 value);

			/** Gets or sets the currently selected item in a ListBox. */
			ListItem* GetSelectedItem() const;
			void SetSelectedItem(ListItem* value);

			/** Gets or sets the text of the currently selected item in the ListBox. */
			String GetSelectedText() const;
			void SetSelectedText(const String& value);

			/**
				Gets a list that contains the zero-based indexes of all
				currently selected items in the ListBox.
			*/
			const Array<int32> GetSelectedIndices() const;

			/**
				Gets a list that contains the currently selected items
				in the ListBox.
			*/
			const Array<ListItem*> GetSelectedItems() const;

			const Color32& GetSelectedBackgroundColor() const { return _SelectedBackgroundColor; }
			void SetSelectedBackgroundColor(const Color32& value) { _SelectedBackgroundColor = value; }

			const Color32& GetSelectedTextColor() const { return _SelectedTextColor; }
			void SetSelectedTextColor(const Color32& value) { _SelectedTextColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Unselects all items in the ListBox. */
			void ClearSelected();

			/** Returns a value indicating whether the specified item is selected. */
			bool GetSelected(int32 index) const;

			/** Selects or clears the selection for the specified item in a ListBox. */
			void SetSelected(int32 index, bool value);

			/** Returns the height for an item in the ListBox. */
			int32 GetItemHeight(int32 index) const;

			/** Returns the bounding rectangle for an item in the ListBox. */
			RectangleInt GetItemRectangle(int32 index) const;

			/** Gets the combined height of all items in the ListBox. */
			int32 GetTotalHeight() const;

			/** Returns the zero-based index of the item at the specified coordinates. */
			int32 GetIndexFromPoint(const PointInt& pt) const;

			/** Returns the item at the specified coordinates. */
			ListItem* GetItemFromPoint(const PointInt& pt) const;

			/** Gets or sets the index of the first visible item in the ListBox. */
			int32 GetFirstIndex() const;

			/** Finds the first item after the given index which starts with the given string. */
			int32 FindText(const String& text, int32 startIndex = 0, bool matchExact = false, bool caseSensitive = false) const;

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void Refresh();

			virtual void OnSize(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseWheel(MouseEventArgs& e);
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
			//@}

		protected:
			void UpdateLayout();
			void UpdateScrollBars();

			virtual bool OnScrollBarValueChanged(Object* sender, const EventArgs& e);

		protected:
			ListItemList _Items;
			bool _AllowSelection;
			bool _MultiSelection;
			bool _Sorted;
			bool _ScrollAlwaysVisible;

			int32 _FocusedIndex;
			ScrollBar* _VScrollBar;

			Color32 _SelectedBackgroundColor;
			Color32 _SelectedTextColor;
		};
	}
}

#endif 
