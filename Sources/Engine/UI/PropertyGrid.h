/*=============================================================================
PropertyGrid.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_PROPERTYGRID_H_
#define _SE_UI_PROPERTYGRID_H_

#include "UI/Common.h"
#include "UI/Widget.h"
#include "UI/TextBox.h"

namespace SonataEngine
{
	namespace UI
	{
		//FIX: To be replaced by type editors. By default will be a text box.
		typedef TextBox TypeEditor;

		/**
			@brief Represents an item in a property grid.
		*/
		class SE_UI_EXPORT PropertyGridItem
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			PropertyGridItem(const String& label, const Variant& value);

			/** Destructor. */
			virtual ~PropertyGridItem();
			//@}

			/** @name Properties. */
			//@{
			TypeEditor* GetEditor() const { return _Editor; }

			/** Gets the text of the grid item. */
			String GetLabel() const { return _Label; }

			/** Gets the current value of the grid item. */
			Variant GetValue() const;
			void SetValue(const Variant& value);

			/** Gets the previous value of the grid item. */
			Variant GetPreviousValue() const { return _PreviousValue; }

			/** Gets or sets user data associated with the widget. */
			Variant GetUserData() const { return _userData; }
			void SetUserData(const Variant& value) { _userData = value; }

			void SetText(const String& value);
			//@}

		protected:
			TypeEditor* _Editor;
			String _Label;
			Variant _Value;
			Variant _PreviousValue;
			Variant _userData;
		};

		/**
			@brief Provides data for the property grid events.
		*/
		class PropertyChangedEventArgs : public EventArgs
		{
		public:
			PropertyChangedEventArgs(PropertyGridItem* changedItem, const Variant& previousValue) :
				EventArgs(),
				_ChangedItem(changedItem),
				_PreviousValue(previousValue)
			{
			}

			/** Gets the GridItem that was changed. */
			PropertyGridItem* ChangedItem() const { return _ChangedItem; }

			/** Gets the value of the grid item before it was changed. */
			Variant PreviousValue() const { return _PreviousValue; }

		protected:
			PropertyGridItem* _ChangedItem;
			Variant _PreviousValue;
		};
		SE_DEFINE_EVENT(PropertyChanged, PropertyChangedEventArgs);

		/**
			@brief Provides a user interface for browsing the properties of an object.

			@todo
				Provide TypeEditor derived classes: NumericEditor, ColorEditor,
					FileNameEditor, DirectoryEditor, ResourceEditor.
				Need Struct and Enum reflection.
				Use editor attributes (reflection) to customize the grid items.
				Implement the grid as a ListBox and allow sub items.
				Add categories.
		*/
		class SE_UI_EXPORT PropertyGrid : public Widget
		{
			SE_DECLARE_CLASS(PropertyGrid, Widget);

			SE_BEGIN_REFLECTION(PropertyGrid);
			SE_END_REFLECTION(PropertyGrid);

		public:
			typedef Array<PropertyGridItem*> PropertyGridItemList;

			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			PropertyGrid();

			/** Destructor. */
			virtual ~PropertyGrid();
			//@}

			/** @name Properties. */
			//@{
			void AddItem(PropertyGridItem* item);
			void RemoveAllItems();

			PropertyGridItem* GetSelectedItem() const { return _SelectedItem; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseEnter(EventArgs& e);
			virtual void OnMouseLeave(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			virtual void OnKeyUp(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when a property value changes. */
			Event<PropertyChangedEventArgs> PropertyValueChanged;

			/** Occurs when the selected GridItem is changed. */
			DefaultEvent SelectedGridItemChanged;
			//@}

		protected:
			PropertyGridItem* GetItemFromPosition(const PointInt& pt);

			DefaultEventDelegateM<PropertyGrid>* PropertyGridHandler;
			bool OnTextChanged(Object* sender, const EventArgs& e);

		protected:
			PropertyGridItemList _Items;
			PropertyGridItem* _SelectedItem;
		};

		/**
			@brief Binds the property grid to an object instance.
		*/
		class SE_UI_EXPORT PropertyGridBinder : public Object
		{
		public:
			struct GridItemData
			{
				GridItemData* parent;
				Object* object;
				const MemberInfo* member;
			};

			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			PropertyGridBinder();

			/** Destructor. */
			virtual ~PropertyGridBinder();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the property grid that displays properties. */
			PropertyGrid* GetPropertyGrid() const { return _PropertyGrid; }
			void SetPropertyGrid(PropertyGrid* value);

			/** Gets or sets the object for which the grid displays properties. */
			Object* GetSelectedObject() const { return _SelectedObject; }
			void SetSelectedObject(Object* value);
			//@}

			/** @name Operations. */
			//@{
			void PopulateObject(Object* object, GridItemData* parent, int32 offset = 0);
			void PopulateType(const String& name, TypeInfo* type, const Variant& value, int32 offset = 0);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the selected object is changed. */
			DefaultEvent SelectedObjectChanged;
			//@}

		protected:
			void PopulatePropertyGrid();

			PropertyChangedEventDelegateM<PropertyGridBinder>* PropertyGridBinderHandler;
			bool OnPropertyValueChanged(Object* sender, const PropertyChangedEventArgs& e);

		protected:
			PropertyGrid* _PropertyGrid;
			Object* _SelectedObject;
		};
	}
}

#endif 
