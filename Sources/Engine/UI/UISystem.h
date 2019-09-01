/*=============================================================================
UISystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_UISYSTEM_H_
#define _SE_UI_UISYSTEM_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for UI system implementations.

			@todo
				Send the DoubleClick event.
				Drag & Drop events.
				Mouse cursors.
				Use a stack of menus.
				Widgets: ToolTip, ListBox, ScrollBar, SpinBox, ComboBox, List, Tree, Model, Window
				Menu: MenuBar, ToolBar, StatusBar
				Layout: FlowLayout, TableLayout
				Dialogs: FileDialog, DirectoryDialog, ColorDialog, FontDialog

		*/
		class SE_UI_EXPORT UISystem : public Manager, public Singleton<UISystem>
		{
		public:
			static const int32 DefaultSize;

			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			UISystem();

			/** Destructor. */
			virtual ~UISystem();
			//@}

			/** @name Properties. */
			//@{
			Widget* GetCurrentWidget() const { return _currentWidget; }
			void SetCurrentWidget(Widget* value) { _currentWidget = value; }

			Font* GetDefaultFont() const { return _DefaultFont; }
			void SetDefaultFont(Font* value) { _DefaultFont = value; }

			Widget* GetFocusedWidget() const { return _FocusedWidget; }
			void SetFocusedWidget(Widget* value) { _FocusedWidget = value; }

			Widget* GetCapture() const { return _CapturedWidget; }
			void SetCapture(Widget* value);
			void ReleaseCapture() { _CapturedWidget = NULL; }
			//@}

			/** @name Creation / Destruction. */
			//@{
			/** Creates the UI system. */
			virtual bool Create();

			/** Destroys the UI system. */
			virtual void Destroy();
			//@}

			/** @name Operations. */
			//@{
			/** Updates the UI system. */
			virtual void Update(real64 elapsed);

			void Render();

			void DestroyWidget(Widget* widget);
			Widget* GetWidgetByName(const String& name);

			RectangleInt GetClipRectangle() const;
			void SetClipRectangle(const RectangleInt& rect);
			void DisableClipRectangle();

			void DrawLine(const Color32& color, int32 width, const PointInt& p0, const PointInt& p1);

			void DrawRectangle(const Color32& color, int32 width, const RectangleInt& rect);

			void FillRectangle(const Color32& color, const RectangleInt& rect);

			void DrawCircle(const Color32& color, int32 width, const PointInt& center, int32 radius);

			void FillCircle(const Color32& color, const PointInt& center, int32 radius);

			void DrawTriangle(const Color32& color, int32 width, const PointInt& p0, const PointInt& p1, const PointInt& p2);

			void FillTriangle(const Color32& color, const PointInt& p0, const PointInt& p1, const PointInt& p2);

			void DrawText(const String& text, Font* font, const RectangleInt& area, const Color32& color, Alignment alignment);

			void DrawImage(Texture* image, const RectangleInt& area, Alignment alignment, ImageLayout layout, AlphaState* alpha = NULL);
			//@}

			static Vector2 GetPositionFromAlignment(const SizeInt& size, const RectangleInt& area, Alignment alignment);
			static TextFormat GetTextFormat(Alignment alignment);

		protected:
			Camera* _camera;
			Widget* _currentWidget;
			FontPtr _DefaultFont;
			RectangleInt _ClipRectangle;
			Widget* _FocusedWidget;
			Widget* _CapturedWidget;
			Widget* _LastHoveredWidget;
			Array<Widget*> _removedWidgets;
		};
	}
}

#endif 
