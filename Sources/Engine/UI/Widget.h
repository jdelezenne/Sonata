/*=============================================================================
Widget.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_WIDGET_H_
#define _SE_UI_WIDGET_H_

#include "UI/Common.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for widgets (widgets), which are user interface objects.

			@todo
				The widget is moved when resized past its size.
				Fix the clipping rectangle to prevent child from being pickable when they
				are on their parent frame
				Fix the clipping rectangle used for rendering.
		*/
		class SE_UI_EXPORT Widget : public Object
		{
			SE_DECLARE_ABSTRACT(Widget, Object);

			SE_BEGIN_REFLECTION(Widget);
				SE_Field(_name, String, Public);
				SE_Field(_Enabled, bool, Public);
				SE_Field(_Visible, bool, Public);
				SE_Field(_Transparent, bool, Public);
				SE_Field(_Popup, bool, Public);
				SE_Field(_MinimumSize, SizeInt, Public);
				SE_Field(_MaximumSize, SizeInt, Public);
				SE_Field(_Rectangle, RectangleInt, Public);
				SE_Field(_HasFrame, bool, Public);
				SE_Field(_FrameSize, int32, Public);
				SE_Field(_FrameColor, Color32, Public);
				SE_Field(_HasBackground, bool, Public);
				SE_Field(_ForegroundColor, Color32, Public);
				SE_Field(_BackgroundColor, Color32, Public);
				//SE_Field(_BackgroundImage, Texture, Public);
				//SE_Field(_Font, Font, Public);
			SE_END_REFLECTION(Widget);

		public:
			class WidgetList : public Array<Widget*>
			{
				typedef Array<Widget*> super;

			public:
				WidgetList(Widget* owner) :
					super(),
					_Owner(owner)
				{
				}

				virtual void Add(Widget* value)
				{
					super::Add(value);
					value->InternalSetParent(_Owner);
				}

				virtual void Insert(int32 index, Widget* value)
				{
					super::Insert(index, value);
					value->InternalSetParent(_Owner);
				}

				virtual void Remove(Widget* value)
				{
					value->InternalSetParent(NULL);
					super::Remove(value);
					//TODO: UISystem notification
				}

				virtual void Clear()
				{
					WidgetList::Iterator it = GetIterator();
					while (it.Next())
					{
						it.Current()->InternalSetParent(NULL);
					}

					super::Clear();
				}

			protected:
				Widget* _Owner;
			};

			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Widget();

			/** Destructor. */
			virtual ~Widget();
			//@}

			virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream);
			virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
			virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream);
			virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

			/** @name Properties. */
			//@{
			/** Gets or sets the name of the widget. */
			String GetName() const { return _name; }
			void SetName(const String& value) { _name = value; }

			/** Gets or sets the parent container of the widget. */
			Widget* GetParent() const { return _Parent; }
			void SetParent(Widget* value);

			/** Gets or sets user data associated with the widget. */
			Variant GetUserData() const { return _userData; }
			void SetUserData(const Variant& value) { _userData = value; }

			/** Gets or sets a value indicating whether the widget is enabled. */
			bool GetEnabled() const;
			void SetEnabled(bool value);

			/** Gets or sets a value indicating whether the widget is visible. */
			bool GetVisible() const;
			void SetVisible(bool value);

			/** Gets or sets a value indicating whether the widget is transparent. */
			bool GetTransparent() const { return _Transparent; }
			void SetTransparent(bool value) { _Transparent = value; }

			/** Gets or sets a value indicating whether the widget is a popup. */
			bool GetPopup() const { return _Popup; }
			void SetPopup(bool value) { _Popup = value; }

			/** Gets or sets the size that is the lower limit that can specified. */
			SizeInt GetMinimumSize() const { return _MinimumSize; }
			void SetMinimumSize(const SizeInt& value) { _MinimumSize = value; }

			/** Gets or sets the size that is the upper limit that can specified. */
			SizeInt GetMaximumSize() const { return _MaximumSize; }
			void SetMaximumSize(const SizeInt& value) { _MaximumSize = value; }

			/**
				Gets or sets the size and location of the widget relative to
				the parent widget.
			*/
			RectangleInt GetRectangle() const { return _Rectangle; }
			void SetRectangle(const RectangleInt& value);
			void SetRectangle(int32 x, int32 y, int32 width, int32 height);

			/** Gets or sets padding within the widget. */
			RectangleInt GetPadding() const { return _Padding; }
			void SetPadding(const RectangleInt& value) { _Padding = value; }

			/**
				Gets or sets the coordinates of the upper-left corner of the
				widget relative to the upper-left corner of its parent.
			*/
			PointInt GetPosition() const { return _Rectangle.GetLocation(); }
			void SetPosition(const PointInt& value) { _Rectangle.SetLocation(value); }

			/** Gets or sets the height and width of the widget. */
			SizeInt GetSize() const { return _Rectangle.GetSize(); }
			void SetSize(const SizeInt& value) { SetRectangle(RectangleInt(GetPosition(), value)); }

			/**
				Gets or sets the coordinates of the left edge of the
				widget relative to the left edge of its parent.
			*/
			int32 GetLeft() const { return _Rectangle.GetLocation().X; }
			void SetLeft(int32 value);

			/**
				Gets or sets the coordinates of the top edge of the
				widget relative to the top edge of its parent.
			*/
			int32 GetTop() const { return _Rectangle.GetLocation().Y; }
			void SetTop(int32 value);

			/** Gets the coordinates of the right edge of the widget. */
			int32 GetRight() const { return (_Rectangle.GetLocation().X + _Rectangle.GetSize().Width); }

			/** Gets the coordinates of the bottom edge of the widget. */
			int32 GetBottom() const { return (_Rectangle.GetLocation().Y + _Rectangle.GetSize().Height); }

			/** Gets or sets the width of the widget. */
			int32 GetWidth() const { return _Rectangle.GetSize().Width; }
			void SetWidth(int32 value);

			/** Gets or sets the height of the widget. */
			int32 GetHeight() const { return _Rectangle.GetSize().Height; }
			void SetHeight(int32 value);

			/** Gets the list of widgets contained within the widget. */
			WidgetList& Children() { return *_Children; }

			/** Gets a value indicating whether the widget contains one or more child widgets. */
			bool HasChildren() const { return (_Children->Count() > 0); }

			Widget* GetChildByName(const String& name) const;

			/** Retrieves the child widget that is located at the specified coordinates. */
			Widget* GetChildAtPoint(const PointInt& value) const;

			/** Retrieves a value indicating whether the specified position is contained in the widget. */
			virtual bool Contains(const PointInt& value) const;

			/** Retrieves a value indicating whether the specified widget is a child of the widget. */
			bool Contains(Widget* value) const;

			bool GetHasFrame() const { return _HasFrame; }
			void SetHasFrame(bool value) { _HasFrame = value; }

			/** Gets or sets the frame size of the widget. */
			int32 GetFrameSize() const { return _FrameSize; }
			void SetFrameSize(int32 value) { _FrameSize = value; }

			/** Gets or sets the frame color of the widget. */
			Color32 GetFrameColor() const { return _FrameColor; }
			void SetFrameColor(Color32 value) { _FrameColor = value; }

			bool GetHasBackground() const { return _HasBackground; }
			void SetHasBackground(bool value) { _HasBackground = value; }

			/** Gets or sets the foreground color for the widget. */
			Color32 GetForegroundColor() const { return _ForegroundColor; }
			void SetForegroundColor(const Color32& value) { _ForegroundColor = value; }

			/** Gets or sets the background color for the widget. */
			Color32 GetBackgroundColor() const { return _BackgroundColor; }
			void SetBackgroundColor(const Color32& value) { _BackgroundColor = value; }

			/** Gets or sets the background image displayed in the widget. */
			Texture* GetBackgroundImage() const { return _BackgroundImage; }
			void SetBackgroundImage(Texture* value) { _BackgroundImage = value; }

			Font* GetFont() const { return _Font; }
			void SetFont(Font* value) { _Font = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Enables the widget. */
			void Enable();

			/** Disables the widget. */
			void Disable();

			/** Shows the widget to the user. */
			void Show();

			/** Hides the widget from the user. */
			void Hide();

			/** Moves the widget to the front of the z-order. */
			void MoveToFront();

			/** Moves the widget to the back of the z-order. */
			void MoveToBack();

			/** Converts the specified local coordinate to a global coordinate. */
			RectangleInt LocalToGlobal(const RectangleInt& value) const;
			PointInt LocalToGlobal(const PointInt& value) const;
			int32 LocalToGlobalX(int32 value) const;
			int32 LocalToGlobalY(int32 value) const;

			/** Converts the specified global coordinate to a local coordinate. */
			RectangleInt GlobalToLocal(const RectangleInt& value) const;
			PointInt GlobalToLocal(const PointInt& value) const;
			int32 GlobalToLocalX(int32 value) const;
			int32 GlobalToLocalY(int32 value) const;

			/** Gets the origin of the client area of the widget. */
			virtual PointInt GetClientOrigin() const;

			/** Gets the size of the client area of the widget. */
			virtual SizeInt GetClientSize() const;

			/** Gets the rectangle of the client area of the widget. */
			virtual RectangleInt GetClientRectangle() const;

			/** Gets the clip rectangle of the widget. */
			RectangleInt GetClipRectangle() const;

			/** Updates the widget. */
			void Update(real64 elapsed);

			/** Renders the widget. */
			void Render();

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void Refresh();

			virtual void OnMove(EventArgs& e);
			virtual void OnSize(EventArgs& e);
			virtual void OnRender(DrawEventArgs& e);
			virtual void OnMouseEnter(EventArgs& e);
			virtual void OnMouseLeave(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseHover(MouseEventArgs& e);
			virtual void OnMouseWheel(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnMouseClick(EventArgs& e);
			virtual void OnMouseDoubleClick(EventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			virtual void OnKeyUp(KeyEventArgs& e);
			virtual void OnKeyChar(KeyCharEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the widget is created. */
			DefaultEvent Create;

			/** Occurs when the widget is being destroyed. */
			DefaultEvent Destroy;

			/** Occurs when the Enabled property value has changed. */
			DefaultEvent EnabledChanged;

			/** Occurs when the Visible property value changes. */
			DefaultEvent VisibleChanged;

			/** Occurs when the widget receives or loses focus. */
			DefaultEvent ActiveChanged;

			/** Occurs when the widget is moved. */
			DefaultEvent Move;

			/** Occurs when the widget is resized. */
			DefaultEvent Resize;

			/** Occurs when the mouse pointer enters the widget. */
			DefaultEvent MouseEnter;

			/** Occurs when the mouse pointer leaves the widget. */
			DefaultEvent MouseLeave;
			
			/** Occurs when the mouse pointer is moved over the widget. */
			Event<MouseEventArgs> MouseMove;
			
			/** Occurs when the mouse pointer rests on the widget. */
			Event<MouseEventArgs> MouseHover;
			
			/** Occurs when the mouse wheel moves while the widget has focus. */
			Event<MouseEventArgs> MouseWheel;
			
			/** Occurs when the mouse pointer is over the widget and a mouse button is pressed. */
			Event<MouseEventArgs> MouseDown;
			
			/** Occurs when the mouse pointer is over the widget and a mouse button is released. */
			Event<MouseEventArgs> MouseUp;
			
			/** Occurs when the widget is clicked by the mouse. */
			DefaultEvent MouseClick;
			
			/** Occurs when the widget is double clicked by the mouse. */
			DefaultEvent MouseDoubleClick;

			/** Occurs when a key is pressed while the widget has focus. */
			Event<KeyEventArgs> KeyDown;

			/** Occurs when a key is released while the widget has focus. */
			Event<KeyEventArgs> KeyUp;

			/** Occurs when a character key is pressed while the widget has focus. */
			Event<KeyCharEventArgs> KeyChar;

			/** Occurs when an object is dragged into the widget's bounds. */
			DefaultEvent DragEnter;

			/** Occurs when an object is dragged out of the widget's bounds. */
			DefaultEvent DragLeave;

			/** Occurs when an object is dragged over the widget's bounds. */
			DefaultEvent DragOver;

			/** Occurs when a drag-and-drop operation is completed. */
			DefaultEvent DragDrop;

			/** Occurs before a widget is updated. */
			Event<UpdateEventArgs> PreUpdate;

			/** Occurs after a widget is updated. */
			Event<UpdateEventArgs> PostUpdate;

			/** Occurs before a widget is rendered. */
			Event<DrawEventArgs> PreRender;

			/** Occurs after a widget is rendered. */
			Event<DrawEventArgs> PostRender;
			//@}

		protected:
			friend class WidgetList;
			void InternalSetParent(Widget* value);

		protected:
			String _name;
			Widget* _Parent;
			Variant _userData;
			bool _Enabled;
			bool _Visible;
			bool _Transparent;
			bool _Popup;

			SizeInt _MinimumSize;
			SizeInt _MaximumSize;
			RectangleInt _Rectangle;
			RectangleInt _Padding;

			WidgetList* _Children;

			bool _HasFrame;
			int32 _FrameSize;
			Color32 _FrameColor;

			bool _HasBackground;
			Color32 _ForegroundColor;
			Color32 _BackgroundColor;
			Texture* _BackgroundImage;

			FontPtr _Font;
		};
	}
}

#endif 
