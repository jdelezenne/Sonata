/*=============================================================================
TextBox.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_TEXTBOX_H_
#define _SE_UI_TEXTBOX_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a text box widget.

			@todo
				Scroll the text box to keep the caret visible.
				Add scroll bars.
				Multiline.
				Replace by TextBoxBase, RichTextBox.
				Mask.
		*/
		class SE_UI_EXPORT TextBox : public Widget
		{
			SE_DECLARE_CLASS(TextBox, Widget);

			SE_BEGIN_REFLECTION(TextBox);
				SE_Field(_ReadOnly, bool, Public);
				SE_Field(_Multiline, bool, Public);
				SE_Field(_WordWrap, bool, Public);
				SE_Field(_PasswordChar, Char, Public);
				SE_Field(_ShortcutsEnabled, bool, Public);
				SE_Field(_MaxLength, int32, Public);
				SE_Field(_Text, String, Public);
				SE_Field(_HideSelection, bool, Public);
				SE_Field(_SelectionStart, int32, Public);
				SE_Field(_NormalTextColor, Color32, Public);
				SE_Field(_SelectedBackColor, Color32, Public);
				SE_Field(_SelectedTextColor, Color32, Public);
				SE_Field(_ReadOnlyBackColor, Color32, Public);
				SE_Field(_ReadOnlyNormalTextColor, Color32, Public);
				SE_Field(_ReadOnlySelectedTextColor, Color32, Public);
				SE_Field(_ReadOnlySelectedBackColor, Color32, Public);
				SE_Field(_CaretColor, Color32, Public);
			SE_END_REFLECTION(TextBox);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			TextBox();

			/** Destructor. */
			virtual ~TextBox();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets a value indicating whether text in the text box is read-only. */
			bool GetReadOnly() const { return _ReadOnly; }
			void SetReadOnly(bool value) { _ReadOnly = value; }

			/** Gets or sets a value indicating whether this is a multiline TextBox. */
			bool GetMultiline() const { return _Multiline; }
			void SetMultiline(bool value) { _Multiline = value; }

			/**
				Indicates whether a multiline text box widget automatically
				wraps words to the beginning of the next line when necessary.
			*/
			bool GetWordWrap() const { return _WordWrap; }
			void SetWordWrap(bool value) { _WordWrap = value; }

			/**
				Gets or sets the character used to mask characters of a
				password in a single-line TextBox.
			*/
			Char GetPasswordChar() const { return _PasswordChar; }
			void SetPasswordChar(Char value) { _PasswordChar = value; }

			/** Gets or sets a value indicating whether the defined shortcuts are enabled. */
			bool GetShortcutsEnabled() const { return _ShortcutsEnabled; }
			void SetShortcutsEnabled(bool value) { _ShortcutsEnabled = value; }

			/**
				Gets or sets the maximum number of characters the user can
				type or paste into the text box widget.
			*/
			int32 GetMaxLength() const { return _MaxLength; }
			void SetMaxLength(int32 value);

			/** Gets or sets the current text in the widget. */
			String GetText() const { return _Text; }
			void SetText(const String& value);

			/** Gets the length of text in the widget. */
			int32 GetTextLength() const { return _Text.Length(); }

			/** Gets or sets the lines of text in a text box widget. */
			Array<String> GetLines() const;
			void SetLines(const Array<String>& value);

			/**
				Gets or sets a value indicating whether the selected text
				in the text box widget remains highlighted when the widget loses focus.
			*/
			bool GetHideSelection() const { return _HideSelection; }
			void SetHideSelection(bool value) { _HideSelection = value; }

			/** Gets or sets a value indicating the currently selected text in the widget. */
			String GetSelectedText() const;
			void SetSelectedText(const String& value);

			/** Gets or sets the number of characters selected in the text box. */
			int32 GetSelectionLength() const;
			void SetSelectionLength(int32 value);

			/** Gets or sets the starting point of text selected in the text box. */
			int32 GetSelectionStart() const { return _SelectionStart; }
			void SetSelectionStart(int32 value);

			Color32 GetNormalTextColor() const { return _NormalTextColor; }
			void SetNormalTextColor(const Color32& value) { _NormalTextColor = value; }

			Color32 GetSelectedBackColor() const { return _SelectedBackColor; }
			void SetSelectedBackColor(const Color32& value) { _SelectedBackColor = value; }

			Color32 GetSelectedTextColor() const { return _SelectedTextColor; }
			void SetSelectedTextColor(const Color32& value) { _SelectedTextColor = value; }

			Color32 GetReadOnlyBackColor() const { return _ReadOnlyBackColor; }
			void SetReadOnlyBackColor(Color32 value) { _ReadOnlyBackColor = value; }

			Color32 GetReadOnlyNormalTextColor() const { return _ReadOnlyNormalTextColor; }
			void SetReadOnlyNormalTextColor(Color32 value) { _ReadOnlyNormalTextColor = value; }

			Color32 GetReadOnlySelectedTextColor() const { return _ReadOnlySelectedTextColor; }
			void SetReadOnlySelectedTextColor(Color32 value) { _ReadOnlySelectedTextColor = value; }

			Color32 GetReadOnlySelectedBackColor() const { return _ReadOnlySelectedBackColor; }
			void SetReadOnlySelectedBackColor(Color32 value) { _ReadOnlySelectedBackColor = value; }

			Color32 GetCaretColor() const { return _CaretColor; }
			void SetCaretColor(const Color32& value) { _CaretColor = value; }

			/** Gets the caret blink time. */
			static bool GetHideCaret() { return _HideCaret; }
			static void SetHideCaret(bool value) { _HideCaret = value; }

			/** Gets the caret blink time. */
			static int32 GetCaretBlinkTime() { return _CaretBlinkTime; }
			static void SetCaretBlinkTime(int32 value) { _CaretBlinkTime = value; }

			/** Gets the width of the caret. */
			static int32 GetCaretWidth() { return _CaretWidth; }
			static void SetCaretWidth(int32 value) { _CaretWidth = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Clears all text from the text box control. */
			void Clear();

			/** Appends text to the current text of a text box. */
			void AppendText(const String& text);

			/** Selects a range of text in the text box. */
			void SelectText(int32 start, int32 length);

			/** Selects all text in the text box. */
			void SelectAll();

			/** Deselects all text in the text box. */
			void DeselectAll();

			/** Copies the current selection in the text box to the Clipboard. */
			void Copy();

			/** Moves the current selection in the text box to the Clipboard. */
			void Cut();

			/** Replaces the current selection in the text box with the contents of the Clipboard. */
			void Paste();

			/** Undoes the last edit operation in the text box. */
			void Undo();

			/** Clears information about the most recent operation from the undo buffer of the text box. */
			void ClearUndo();

			/** Retrieves the character that is closest to the specified location within the control. */
			Char GetCharFromPosition(const PointInt& pt);

			/** Retrieves the index of the character nearest to the specified location. */
			int32 GetCharIndexFromPosition(const PointInt& pt);

			/** Retrieves the location within the control at the specified character index. */
			PointInt GetPositionFromCharIndex(int32 index);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnMouseDoubleClick(EventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			virtual void OnKeyChar(KeyCharEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the text changes. */
			DefaultEvent TextChanged;
			//@}

		protected:
			void SetCaretPosition(int32 value);
			void ResetCaretBlink();
			void InsertCharacterAt(int32 index, Char c);
			void RemoveCharacterAt(int32 index);
			void RemoveSelectedText();
			String GetPasswordText(const String& text);

		protected:
			bool _ReadOnly;
			bool _Multiline;
			bool _WordWrap;
			Char _PasswordChar;
			bool _ShortcutsEnabled;
			int32 _MaxLength;
			String _Text;
			bool _HideSelection;
			int32 _SelectionStart;

			TextPtr _TextObj;
			bool _InsertMode;
			bool _Dragging;
			bool _CaretVisible;
			int32 _CaretPosition;
			real64 _LastCaretBlink;

			Color32 _NormalTextColor;
			Color32 _SelectedBackColor;
			Color32 _SelectedTextColor;
			Color32 _ReadOnlyBackColor;
			Color32 _ReadOnlyNormalTextColor;
			Color32 _ReadOnlySelectedTextColor;
			Color32 _ReadOnlySelectedBackColor;
			Color32 _CaretColor;

			static bool _HideCaret;
			static int32 _CaretBlinkTime;
			static int32 _CaretWidth;
		};
	}
}

#endif 
