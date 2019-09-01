/*=============================================================================
PropertyGrid.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PropertyGrid.h"
#include "UI/UISystem.h"
#include "Core/Serialization/ISerializer.h"

namespace SonataEngine
{
	namespace UI
	{
		PropertyGridItem::PropertyGridItem(const String& label, const Variant& value)
		{
			_Editor = new TypeEditor();
			_Label = label;
			_Value = value;

			_Editor->SetText(_Value.ToString());
			_Editor->SetBackgroundColor(Color32::White);
		}

		PropertyGridItem::~PropertyGridItem()
		{
			if (_userData.GetData())
				delete _userData.GetData();
		}

		Variant PropertyGridItem::GetValue() const
		{
			return _Value;
		}

		void PropertyGridItem::SetValue(const Variant& value)
		{
			_PreviousValue = _Value;
			_Value = value;
		}

		void PropertyGridItem::SetText(const String& value)
		{
			if (_Editor != NULL)
			{
				_Editor->SetText(value);
			}
		}


		SE_IMPLEMENT_CLASS(PropertyGrid);
		SE_IMPLEMENT_REFLECTION(PropertyGrid);

		PropertyGrid::PropertyGrid() :
			Widget()
		{
			PropertyGridHandler = new DefaultEventDelegateM<PropertyGrid>(
				this, &PropertyGrid::OnTextChanged);

			_SelectedItem = NULL;
		}

		PropertyGrid::~PropertyGrid()
		{
			delete PropertyGridHandler;
		}

		PropertyGridItem* PropertyGrid::GetItemFromPosition(const PointInt& pt)
		{
			int32 RowHeight = GetFont()->GetHeight() + 2;

			RectangleInt rect = GetClientRectangle();
			RectangleInt editorRect;

			PropertyGridItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				PropertyGridItem* item = it.Current();

				TypeEditor* editor = item->GetEditor();
				editorRect = rect;
				editorRect.X = rect.Width / 2 + 1;
				editorRect.Width = (rect.Width - 2) / 2;
				editorRect.Height = RowHeight;

				if (editorRect.Contains(pt))
					return item;

				rect.Y += RowHeight;
			}

			return NULL;
		}

		bool PropertyGrid::OnTextChanged(Object* sender, const EventArgs& e)
		{
			TypeEditor* editor = (TypeEditor*)sender;

			PropertyGridItem* changedItem = NULL;
			PropertyGridItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				PropertyGridItem* item = it.Current();

				if (item->GetEditor() == editor)
				{
					changedItem = item;
					break;
				}
			}

			if (changedItem != NULL)
			{
				changedItem->SetValue(editor->GetText());
				PropertyValueChanged.Invoke(this, PropertyChangedEventArgs(
					changedItem, changedItem->GetPreviousValue()));
			}

			return true;
		}

		void PropertyGrid::AddItem(PropertyGridItem* item)
		{
			_Items.Add(item);
			item->GetEditor()->SetParent(this);
			item->GetEditor()->TextChanged += PropertyGridHandler;
		}

		void PropertyGrid::RemoveAllItems()
		{

			PropertyGridItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				PropertyGridItem* item = it.Current();

				item->GetEditor()->SetParent(NULL);
				//item->GetEditor()->TextChanged.Remove(PropertyGridHandler);
			}

			_Items.Clear();
		}

		void PropertyGrid::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);

			if (_SelectedItem != NULL)
			{
				_SelectedItem->GetEditor()->DoUpdate(elapsed);
			}
		}

		void PropertyGrid::DoRender()
		{
			Widget::DoRender();

			if (GetFont() == NULL)
				return;

			int32 RowHeight = GetFont()->GetHeight() + 2;

			RectangleInt rect = GetClientRectangle();
			RectangleInt textRect;
			RectangleInt editorRect;

			PropertyGridItemList::Iterator it = _Items.GetIterator();
			while (it.Next())
			{
				PropertyGridItem* item = it.Current();

				textRect = LocalToGlobal(rect);
				textRect.Width = (rect.Width - 2) / 2;
				textRect.Height = RowHeight;
				UISystem::Instance()->DrawText(item->GetLabel(), GetFont(),
					textRect, GetForegroundColor(), Alignment_TopLeft);

				TypeEditor* editor = item->GetEditor();
				editorRect = rect;
				editorRect.X = rect.Width / 2 + 1;
				editorRect.Width = (rect.Width - 2) / 2;
				editorRect.Height = RowHeight;
				editor->SetRectangle(editorRect);
				editor->DoRender();

				rect.Y += RowHeight;
			}
		}

		void PropertyGrid::OnMouseEnter(EventArgs& e)
		{
			Widget::OnMouseEnter(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void PropertyGrid::OnMouseLeave(EventArgs& e)
		{
			Widget::OnMouseLeave(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void PropertyGrid::OnMouseMove(MouseEventArgs& e)
		{
			Widget::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;

			PointInt mouse = GlobalToLocal(e.Location());
			PropertyGridItem* item = GetItemFromPosition(mouse);
			if (item != NULL)
			{
				item->GetEditor()->OnMouseMove(e);
			}
		}

		void PropertyGrid::OnMouseDown(MouseEventArgs& e)
		{
			Widget::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			PointInt mouse = GlobalToLocal(e.Location());
			PropertyGridItem* item = GetItemFromPosition(mouse);
			if (item != NULL)
			{
				_SelectedItem = item;
				item->GetEditor()->OnMouseDown(e);
			}
		}

		void PropertyGrid::OnMouseUp(MouseEventArgs& e)
		{
			Widget::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			PointInt mouse = GlobalToLocal(e.Location());
			PropertyGridItem* item = GetItemFromPosition(mouse);
			if (item != NULL)
			{
				item->GetEditor()->OnMouseUp(e);
			}
		}

		void PropertyGrid::OnKeyDown(KeyEventArgs& e)
		{
			Widget::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void PropertyGrid::OnKeyUp(KeyEventArgs& e)
		{
			Widget::OnKeyUp(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		PropertyGridBinder::PropertyGridBinder() :
			Object()
		{
			PropertyGridBinderHandler = new PropertyChangedEventDelegateM<PropertyGridBinder>(
				this, &PropertyGridBinder::OnPropertyValueChanged);

			_PropertyGrid = NULL;
			_SelectedObject = NULL;
		}

		PropertyGridBinder::~PropertyGridBinder()
		{
			delete PropertyGridBinderHandler;
		}

		String PropertyGridBinder_FieldToString(Object* object)
		{
			String str;

			Array<MemberInfo*> members;
			members = ISerializer::GetSerializableMembers(object->GetType());

			BaseArray<Variant> values;
			values = ISerializer::GetObjectValues(object, members);

			int32 index = 0;
			BaseArray<Variant>::Iterator it = values.GetIterator();
			while (it.Next())
			{
				const Variant& value = it.Current();
				if (index > 0)
					str = String::Concat(str, ", ");
				str = String::Concat(str, value.ToString());
				index++;
			}

			return str;
		}

		void PropertyGridBinder::PopulateObject(Object* object, GridItemData* parent, int32 offset)
		{
			// Populate the property grid
			Array<MemberInfo*> members;
			members = ISerializer::GetSerializableMembers(object->GetType());

			BaseArray<Variant> values;
			values = ISerializer::GetObjectValues(object, members);

			String offStr;
			for (int i=0; i<offset; i++)
				offStr += "      ";

			int32 index = 0;
			BaseArray<Variant>::Iterator it = values.GetIterator();
			while (it.Next())
			{
				const Variant& value = it.Current();
				const MemberInfo* member = members[index];

				if (member->GetMemberType() == MemberTypes_Field)
				{
					FieldInfo* field = (FieldInfo*)member;
					TypeInfo* type = field->GetFieldType();
					
					if (type->IsStruct())
					{
						Object* obj = TypeFactory::Instance()->CreateInstance(type->GetName());

						StructObject* structObj = (StructObject*)obj;
						void* ptr = value.GetData();
						structObj->Set(ptr);

						PropertyGridItem* item = new PropertyGridItem(
							offStr + member->GetName(), value);
						GridItemData* itemData = new GridItemData;
						itemData->parent = parent;
						itemData->object = object;
						itemData->member = member;
						item->SetUserData(Variant((Object*)itemData));
						item->SetText(PropertyGridBinder_FieldToString(structObj));
						_PropertyGrid->AddItem(item);

						PopulateObject(structObj, itemData, offset+1);
						delete obj;
					}
					else
					{
						PropertyGridItem* item = new PropertyGridItem(
							offStr + member->GetName(), value);
						GridItemData* itemData = new GridItemData;
						itemData->parent = parent;
						itemData->object = object;
						itemData->member = member;
						item->SetUserData(Variant((Object*)itemData));
						if (type->IsEnum())
						{
							item->SetText(EnumObject::GetName(type, Variant((int32)value.ToEnum())));
						}
						_PropertyGrid->AddItem(item);
					}
				}

				index++;
			}
		}

		void PropertyGridBinder::PopulateType(const String& name, TypeInfo* type, const Variant& value, int32 offset)
		{
			String offStr;
			for (int i=0; i<offset; i++)
				offStr += "      ";

			if (type->IsClass())
			{
				PopulateObject(value.ToObject(), NULL, 0);
			}
			else if (type->IsStruct())
			{
				Object* obj = TypeFactory::Instance()->CreateInstance(type->GetName());

				StructObject* structObj = value.ToStruct();
				//StructObject* structObj = (StructObject*)obj;
				//void* ptr = value.GetData();
				//structObj->Set(ptr);

				PropertyGridItem* item = new PropertyGridItem(
					offStr + name, value);
				GridItemData* itemData = new GridItemData;
				itemData->parent = NULL;
				itemData->object = structObj;
				itemData->member = NULL;
				item->SetUserData(Variant((Object*)itemData));
				item->SetText(PropertyGridBinder_FieldToString(structObj));
				_PropertyGrid->AddItem(item);

				PopulateObject(structObj, itemData, offset+1);
				delete obj;
			}
			else
			{
				PropertyGridItem* item = new PropertyGridItem(
					offStr + name, value);
				GridItemData* itemData = new GridItemData;
				itemData->parent = NULL;
				itemData->object = NULL;
				itemData->member = NULL;
				item->SetUserData(Variant((Object*)itemData));
				if (type->IsEnum())
				{
					item->SetText(EnumObject::GetName(type, Variant((int32)value.ToEnum())));
				}
				_PropertyGrid->AddItem(item);
			}
		}

		void PropertyGridBinder::PopulatePropertyGrid()
		{
			_PropertyGrid->RemoveAllItems();

			if (_SelectedObject == NULL || _SelectedObject->GetType() == NULL)
				return;

			PopulateObject(_SelectedObject, NULL, 0);
		}

		bool PropertyGridBinder::OnPropertyValueChanged(Object* sender, const PropertyChangedEventArgs& e)
		{
			if (_SelectedObject == NULL)
				return true;

			GridItemData* itemData = (GridItemData*)e.ChangedItem()->GetUserData().GetData();
			GridItemData* parentData = itemData->parent;
			if (itemData->member != NULL)
			{
				if (itemData->member->GetMemberType() == MemberTypes_Field)
				{
					FieldInfo* field = (FieldInfo*)itemData->member;
					if (parentData != NULL)
					{
						if (parentData->member->GetMemberType() == MemberTypes_Field)
						{
							FieldInfo* fieldParent = (FieldInfo*)parentData->member;

							Object* obj = TypeFactory::Instance()->CreateInstance(
								fieldParent->GetFieldType()->GetName());

							StructObject* structObj = (StructObject*)obj;
							void* ptr = fieldParent->GetValue(parentData->object).GetData();
							structObj->Set(ptr);

							field->SetValue(structObj, e.ChangedItem()->GetValue());
							fieldParent->SetValue(parentData->object, Variant((StructObject*)structObj));
							delete obj;
						}
					}
					else
					{
						if (!field->GetFieldType()->IsStruct())
							field->SetValue(itemData->object, e.ChangedItem()->GetValue());
					}
				}
			}

			return true;
		}

		void PropertyGridBinder::SetPropertyGrid(PropertyGrid* value)
		{
			if (value == NULL)
				return;

			//TODO: Clear previous property grid items & event
			//_PropertyGrid->PropertyValueChanged.Remove(PropertyGridBinderHandler);

			_PropertyGrid = value;
			_PropertyGrid->PropertyValueChanged += PropertyGridBinderHandler;

			if (_SelectedObject != NULL)
			{
				PopulatePropertyGrid();
			}
		}

		void PropertyGridBinder::SetSelectedObject(Object* value)
		{
			bool selectedObjectChanged = (_SelectedObject != value);
			_SelectedObject = value;

			if (selectedObjectChanged)
			{
				if (_PropertyGrid != NULL)
				{
					PopulatePropertyGrid();
				}

				SelectedObjectChanged.Invoke(this, EventArgs::Empty);
			}
		}
	}
}
