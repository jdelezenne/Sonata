/*=============================================================================
TextBox.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TextBox.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		bool TextBox::_HideCaret = false;
		int32 TextBox::_CaretBlinkTime = 500;
		int32 TextBox::_CaretWidth = 1;

		SE_IMPLEMENT_CLASS(TextBox);
		SE_IMPLEMENT_REFLECTION(TextBox);

		TextBox::TextBox() :
			Widget()
		{
			SetHeight(UISystem::DefaultSize);
			SetHasFrame(true);
			SetFrameSize(1);
			SetBackgroundColor(Color32::White);

			_ReadOnly = false;
			_Multiline = false;
			_WordWrap = true;
			_PasswordChar = Char::Null;
			_ShortcutsEnabled = true;
			_MaxLength = 0;
			_HideSelection = true;
			_SelectionStart = 0;

			_TextObj = new Text();
			_InsertMode = true;
			_Dragging = false;
			_CaretVisible = true;
			_CaretPosition = 0;
			_LastCaretBlink = 0;

			_BackgroundColor = Color32::White;

			_NormalTextColor = Color32::Black;
			_SelectedBackColor = Color32::Gray;
			_SelectedTextColor = Color32::Black;
			_ReadOnlyBackColor = Color32(112, 128, 144);
			_ReadOnlyNormalTextColor = Color32::Black;
			_ReadOnlySelectedBackColor = Color32::Gray;
			_ReadOnlySelectedTextColor = Color32::Black;
			_CaretColor = Color32::Black;
		}

		TextBox::~TextBox()
		{
		}

		void TextBox::SetCaretPosition(int32 value)
		{
			//TODO: scroll
			_CaretPosition = Math::Clamp(value, 0, GetTextLength());
		}

		void TextBox::ResetCaretBlink()
		{
			_CaretVisible = true;
			_LastCaretBlink = TimeValue::GetTime();
		}

		void TextBox::InsertCharacterAt(int32 index, Char c)
		{
			// Get the text before the start of the selection
			String before = GetText().Substring(0, index);
			String after = String::Empty;

			// Get the text after the start of the selection if any
			if (index < GetText().Length())
			{
				after = GetText().Substring(index);
			}

			// Concatenate the text before the selection with the new selected text
			String text = String::Concat(before, c);

			// Concatenate the result with the text after the selection
			SetText(String::Concat(text, after));
		}

		void TextBox::RemoveCharacterAt(int32 index)
		{
			int32 length = GetTextLength();
			if (index == length - 1)
			{
				SetText(GetText().Left(index));
			}
			else if (index < length)
			{
				SetText(GetText().Substring(0, index) +
					GetText().Substring(index + 1));
			}
		}

		void TextBox::RemoveSelectedText()
		{
			int32 start = GetSelectionStart();
			int32 length = GetSelectionLength();
			SetSelectedText(String::Empty);

			if (_CaretPosition > start)
			{
				SetCaretPosition(_CaretPosition - length);
				_SelectionStart = _CaretPosition;
			}
		}

		String TextBox::GetPasswordText(const String& text)
		{
			if (GetPasswordChar() == Char::Null)
				return text;

			String result = text;
			int32 length = text.Length();

			for (int i = 0; i < length; i++)
			{
				if (!Char::IsWhiteSpace(text[i]))
					result[i] = GetPasswordChar();
			}

			return result;
		}

		void TextBox::SetMaxLength(int32 value)
		{
			if (value < 0)
				return;

			_MaxLength = value;

			// Refresh the text
			SetText(GetText());
		}

		void TextBox::SetText(const String& value)
		{
			if (value.Length() > GetMaxLength())
			{
				_Text = value;
			}
			else
			{
				_Text = value.Left(GetMaxLength());
			}

			//SetCaretPosition(_Text.Length());
			SelectText(_CaretPosition, 0);

			TextChanged.Invoke(this, EventArgs::Empty);
		}
		
		Array<String> TextBox::GetLines() const
		{
			//TODO: Need Split with String
			//return GetText().Split(Environment::NewLine());
			return Array<String>();
		}
		
		void TextBox::SetLines(const Array<String>& value)
		{
			String text;
			int32 count = value.Count();
			for (int i = 0; i < count; i++)
			{
				text += value[i];
				if (i < count - 1)
					text += Environment::NewLine();
			}

			SetText(text);
		}

		String TextBox::GetSelectedText() const
		{
			if (GetSelectionLength() == 0)
				return String::Empty;

			if (GetSelectionStart() < _CaretPosition)
				return GetText().Substring(GetSelectionStart(), GetSelectionLength());
			else
				return GetText().Substring(GetSelectionStart() - GetSelectionLength(), GetSelectionLength());
		}

		void TextBox::SetSelectedText(const String& value)
		{
			int32 start = (_CaretPosition < GetSelectionStart()) ? _CaretPosition : GetSelectionStart();
			int32 length = GetSelectionLength();

			// Get the text before the start of the selection
			String before = GetText().Substring(0, start);
			String after = String::Empty;

			// Get the text after the start of the selection if any
			if ((start + length) < GetText().Length())
			{
				after = GetText().Substring(start + length);
			}

			// Concatenate the text before the selection with the new selected text
			String text = String::Concat(before, value);

			// Concatenate the result with the text after the selection
			SetText(String::Concat(text, after));

			ClearUndo();
		}

		int32 TextBox::GetSelectionLength() const
		{
			return Math::Abs(_CaretPosition - _SelectionStart);
		}

		void TextBox::SetSelectionLength(int32 value)
		{
			SelectText(GetSelectionStart(), value);
		}

		void TextBox::SetSelectionStart(int32 value)
		{
			SelectText(value, GetSelectionLength());
		}

		void TextBox::Clear()
		{
			SetText(String::Empty);
		}

		void TextBox::AppendText(const String& text)
		{
			SetText(String::Concat(_Text, text));
		}

		void TextBox::SelectText(int32 start, int32 length)
		{
			start = Math::Clamp(start, 0, GetTextLength());
			length = Math::Clamp(length, 0, GetTextLength() - start);

			_SelectionStart = start;
			_CaretPosition = start + length;
		}

		void TextBox::SelectAll()
		{
			SelectText(0, GetText().Length());
		}

		void TextBox::DeselectAll()
		{
			SelectText(0, 0);
		}

		void TextBox::Copy()
		{
			Environment::SetClipboardData(NULL, GetSelectedText());
		}

		void TextBox::Cut()
		{
			Environment::SetClipboardData(NULL, GetSelectedText());
			RemoveSelectedText();
		}

		void TextBox::Paste()
		{
			int32 start = GetSelectionStart();
			int32 length = GetSelectionLength();
			String text = Environment::GetClipboardData(NULL);
			//TODO: Remove the invalid characters
			SetSelectedText(text);

			if (_CaretPosition > start)
				SetCaretPosition(GetSelectionStart() - length + text.Length());

			SetCaretPosition(Math::Min(_CaretPosition, start) + text.Length());
			_SelectionStart = _CaretPosition;
		}

		void TextBox::Undo()
		{
			//TODO
		}

		void TextBox::ClearUndo()
		{
			//TODO
		}

		Char TextBox::GetCharFromPosition(const PointInt& pt)
		{
			int32 index = GetCharIndexFromPosition(pt);

			String text = GetText();
			if ((index >= 0) && (index < text.Length()))
			{
				return text[index];
			}
			else
			{
				return Char::Null;
			}
		}

		int32 TextBox::GetCharIndexFromPosition(const PointInt& pt)
		{
			String text = GetPasswordText(GetText());
			int32 m_nFirstVisible = 0;

			TextFormat format;
			format.VAlignment = VerticalAlignment_Top;
			format.HAlignment = HorizontalAlignment_Left;

			SizeReal size = Text::MeasureText(text, GetFont(), format);

			// Return the first visible character in the text
			if (pt.X < 0)
			{
				return m_nFirstVisible;
			}

			// Return the last visible character in the text
			if (pt.X >= size.Width)
			{
				//TODO
				return text.Length();
			}

			int32 cch = 0;
			int32 width = pt.X;
			int32 cchHi = (text.Length() - 1) - m_nFirstVisible;

			// Binary search for closest char
			int32 cchLo = 0;
			cchHi++;
			int32 lastLowWidth = 0;
			int32 lastHighWidth = size.Width;

			while (cchLo < cchHi - 1)
			{
				cch = (cchHi + cchLo) / 2;
				size = Text::MeasureText(text.Left(cch), GetFont(), format);

				if (size.Width <= width)
				{
					cchLo = cch;
					lastLowWidth = size.Width;
				}
				else
				{
					cchHi = cch;
					lastHighWidth = size.Height;
				}
			}

			if (cchLo == cch)
			{
				if ((lastHighWidth - width) < (width - size.Width))
				{
					cch++;
				}
			}
			else
			{
				if ((width - lastLowWidth) < (size.Width - width))
				{
					cch--;
				}
			}

			return cch - 1;
		}

		PointInt TextBox::GetPositionFromCharIndex(int32 index)
		{
			if (index < 0 || index > GetTextLength())
				return PointInt(0, 0);

			if (GetFont() == NULL)
				return PointInt(0, 0);

			int32 m_nFirstVisible = 0;
			PointInt textPosition;

			TextFormat format;
			format.VAlignment = VerticalAlignment_Top;
			format.HAlignment = HorizontalAlignment_Left;

			String text = GetText().Substring(m_nFirstVisible, index);
			if (GetPasswordChar() != Char::Null)
				text = GetPasswordText(text);

			SizeReal size = Text::MeasureText(text, GetFont(), format);

			textPosition = PointInt(size.Width, 0);
			return textPosition;
		}

		void TextBox::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);

			real64 time = TimeValue::GetTime();
			if (time - _LastCaretBlink >= TextBox::GetCaretBlinkTime())
			{
				_CaretVisible = !_CaretVisible;
				_LastCaretBlink = time;
			}
		}

		void TextBox::DoRender()
		{
			Color32 color;
			bool hideSelection = (GetHideSelection() && UISystem::Instance()->GetFocusedWidget() != this);

			//FIX: Temporary change the background color if in read only mode
			if (GetReadOnly())
			{
				color = GetBackgroundColor();
				SetBackgroundColor(GetReadOnlyBackColor());
			}

			Widget::DoRender();

			if (GetReadOnly())
				SetBackgroundColor(color);

			RectangleInt textRect = LocalToGlobal(GetClientRectangle());

			// Compute the X coordinates of the first visible character
			int32 m_nFirstVisible = 0;
			int32 nXFirst = GetPositionFromCharIndex(m_nFirstVisible).X;

			// Compute the X coordinates of the selection rectangle
			int32 nCaretX = GetPositionFromCharIndex(_CaretPosition).X;
			int32 nSelStartX;
			if (_CaretPosition != GetSelectionStart())
				nSelStartX = GetPositionFromCharIndex(GetSelectionStart()).X;
			else
				nSelStartX = nCaretX;

			// Selection rectangle
			RectangleInt selectionRect;
			if (!hideSelection && _CaretPosition != GetSelectionStart())
			{
				int32 nSelLeftX = nCaretX;
				int32 nSelRightX = nSelStartX;

				// Swap if left is bigger than right
				if (nSelLeftX > nSelRightX)
					SE_Swap(nSelLeftX, nSelRightX);

				selectionRect = RectangleInt(nSelLeftX, textRect.GetTop(),
					nSelRightX - nSelLeftX, GetFont()->GetHeight());
				selectionRect.Offset(textRect.GetLeft() - nXFirst, 0);
				selectionRect.Intersect(selectionRect, textRect);

				color = GetReadOnly() ? GetReadOnlySelectedBackColor() : GetSelectedBackColor();
				UISystem::Instance()->FillRectangle(color, selectionRect);
			}

			if (!GetText().IsEmpty() && GetFont() != NULL)
			{
				_TextObj->SetFont(GetFont());
				_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_TopLeft));

				// Unselected text
				String text = (GetPasswordChar() == Char::Null ?
					GetText() : GetPasswordText(GetText()));

				color = GetReadOnly() ? GetReadOnlyNormalTextColor() : GetNormalTextColor();

				_TextObj->SetText(text);
				_TextObj->SetColor(color);
				_TextObj->SetBounds(RectangleReal(textRect.X, textRect.Y, textRect.Width, textRect.Height));
				_TextObj->Render();

				// Selected text
				if (!hideSelection && _CaretPosition != GetSelectionStart())
				{
					String selectedText = (GetPasswordChar() == Char::Null ?
						GetSelectedText() : GetPasswordText(GetSelectedText()));

					color = GetReadOnly() ? GetReadOnlySelectedTextColor() : GetSelectedTextColor();

					_TextObj->SetText(selectedText);
					_TextObj->SetColor(color);
					_TextObj->SetBounds(RectangleReal(
						selectionRect.X, selectionRect.Y, selectionRect.Width, selectionRect.Height));
					_TextObj->Render();
				}
			}

			// Caret
			if (UISystem::Instance()->GetFocusedWidget() == this)
			{
				if (!hideSelection && !TextBox::GetHideCaret() && _CaretVisible)
				{
					RectangleInt caretRect = RectangleInt(
						textRect.GetLeft() - nXFirst + nCaretX, textRect.GetTop(),
						0, GetFont()->GetHeight());
					caretRect.X -= TextBox::GetCaretWidth();
					caretRect.Width += TextBox::GetCaretWidth();

					UISystem::Instance()->FillRectangle(
						TextBox::GetCaretColor(), caretRect);
				}
			}
		}

		void TextBox::OnMouseMove(MouseEventArgs& e)
		{
			Widget::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;

			RectangleInt textRect = LocalToGlobal(GetClientRectangle());
			PointInt mouse = GlobalToLocal(e.Location());

			if (_Dragging)
			{
				// Compute the X coordinates of the first visible character
				int32 m_nFirstVisible = 0;
				PointInt firstCharPos = GetPositionFromCharIndex(m_nFirstVisible);

				int32 index = GetCharIndexFromPosition(PointInt(
					mouse.X + firstCharPos.X, mouse.Y + firstCharPos.Y));
				SetCaretPosition(index);
			}
		}

		void TextBox::OnMouseDown(MouseEventArgs& e)
		{
			Widget::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			RectangleInt textRect = LocalToGlobal(GetClientRectangle());
			PointInt mouse = GlobalToLocal(e.Location());

			if (e.Button() == MouseButtons_Left)
			{
				_Dragging = true;
				UISystem::Instance()->SetCapture(this);

				// Compute the X coordinates of the first visible character
				int32 m_nFirstVisible = 0;
				PointInt firstCharPos = GetPositionFromCharIndex(m_nFirstVisible);

				int32 index = GetCharIndexFromPosition(PointInt(
					mouse.X + firstCharPos.X, mouse.Y + firstCharPos.Y));
				SetCaretPosition(index);

				if ((e.Modifiers() & ModifierKeys_Shift) == 0)
				{
					_SelectionStart = _CaretPosition;
				}
			}

			ResetCaretBlink();
		}

		void TextBox::OnMouseUp(MouseEventArgs& e)
		{
			Widget::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->ReleaseCapture();
				_Dragging = false;
			}
		}

		void TextBox::OnMouseDoubleClick(EventArgs& e)
		{
			Widget::OnMouseDoubleClick(e);

			if (!GetEnabled() || !GetVisible())
				return;

			// Get the index of the left word
			bool alpha = false;
			int32 index = 0;
			for (int i = _CaretPosition - 1; i > 0; i--)
			{
				if (!Char::IsLetterOrDigit(_Text[i]))
				{
					if (i > 1 && Char::IsWhiteSpace(_Text[i-1]))
						continue;

					if (alpha)
					{
						index = i;
						break;
					}
				}
				alpha = true;
			}

			int32 wordStart = index;

			// Get the index of the right word
			int32 length = _Text.Length();
			index = _Text.Length();
			for (int i = _CaretPosition + 1; i < length; i++)
			{
				if (!Char::IsLetterOrDigit(_Text[i]))
				{
					if (i < length - 1 && Char::IsWhiteSpace(_Text[i+1]))
						continue;

					index = i;
					break;
				}
			}

			// Move the caret to the left word
			SetCaretPosition((wordStart == 0) ? wordStart : wordStart + 1);
			_SelectionStart = _CaretPosition;

			// Select the word
			SetSelectionLength(index - _CaretPosition);
		}

		void TextBox::OnKeyDown(KeyEventArgs& e)
		{
			Widget::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			switch (e.Key())
			{
			case Input::Key_Left:
				if ((e.Modifiers() & ModifierKeys_Control) != 0)
				{
					// Move the caret to the left word
					bool alpha = false;
					int32 index = 0;
					for (int i = _CaretPosition - 1; i > 0; i--)
					{
						if (!Char::IsLetterOrDigit(_Text[i]))
						{
							if (i > 1 && Char::IsWhiteSpace(_Text[i-1]))
								continue;

							if (alpha)
							{
								index = i;
								break;
							}
						}
						alpha = true;
					}
					SetCaretPosition((index == 0) ? index : index + 1);
				}
				else
				{
					// Move the caret to the left character
					SetCaretPosition(_CaretPosition - 1);
				}
				
				if ((e.Modifiers() & ModifierKeys_Shift) == 0)
				{
					_SelectionStart = _CaretPosition;
				}
				break;

			case Input::Key_Right:
				if ((e.Modifiers() & ModifierKeys_Control) != 0)
				{
					// Move the caret to the right word
					int32 length = _Text.Length();
					int32 index = _Text.Length();
					for (int i = _CaretPosition + 1; i < length; i++)
					{
						if (!Char::IsLetterOrDigit(_Text[i]))
						{
							if (i < length - 1 && Char::IsWhiteSpace(_Text[i+1]))
								continue;

							index = i;
							break;
						}
					}
					SetCaretPosition(index + 1);
				}
				else
				{
					// Move the caret to the right character
					SetCaretPosition(_CaretPosition + 1);
				}
				
				if ((e.Modifiers() & ModifierKeys_Shift) == 0)
				{
					_SelectionStart = _CaretPosition;
				}
				break;

			case Input::Key_Up:
				_SelectionStart = _CaretPosition;
				break;

			case Input::Key_Down:
				_SelectionStart = _CaretPosition;
				break;

			case Input::Key_Home:
				SetCaretPosition(0);

				if ((e.Modifiers() & ModifierKeys_Shift) == 0)
				{
					_SelectionStart = _CaretPosition;
				}
				break;

			case Input::Key_End:
				SetCaretPosition(GetTextLength());

				if ((e.Modifiers() & ModifierKeys_Shift) == 0)
				{
					_SelectionStart = _CaretPosition;
				}
				break;

			case Input::Key_Insert:
				if (GetReadOnly())
					break;

				if (GetShortcutsEnabled() && (e.Modifiers() & ModifierKeys_Shift) != 0)
				{
					// Paste from clipboard
					Paste();
				}
				else if (GetShortcutsEnabled() && (e.Modifiers() & ModifierKeys_Control) != 0)
				{
					// Copy to clipboard
					Copy();
				}
				else
				{
					// Toggle insert mode
					_InsertMode = !_InsertMode;
				}
				break;

			case Input::Key_Delete:
				if (GetReadOnly())
					break;

				//TODO: Modifiers keys for multiline text box
				if (GetShortcutsEnabled() && (e.Modifiers() & ModifierKeys_Control) != 0)
				{
					//TODO: Remove next word
				}
				else
				{
					if (GetSelectionLength() > 0)
					{
						RemoveSelectedText();
					}
					else
					{
						// Remove the character
						RemoveCharacterAt(_CaretPosition);
					}
				}
				break;

			case Input::Key_BackSpace:
				if (GetReadOnly())
					break;

				if (GetShortcutsEnabled() && (e.Modifiers() & ModifierKeys_Control) != 0)
				{
					//TODO: Remove previous word
					if (_CaretPosition > 0)
					{
					}
				}
				else
				{
					if (GetSelectionLength() > 0)
					{
						// Remove the selection
						RemoveSelectedText();
					}
					else
					{
						if (_CaretPosition > 0)
						{
							// Move the caret
							SetCaretPosition(_CaretPosition - 1);

							// Remove the character
							RemoveCharacterAt(_CaretPosition);
						}
					}
				}
				break;

			case Input::Key_Tab:
				//TODO: AcceptsTab
				//SetSelectedText(String(Char::Tab));
				break;

			case Input::Key_Return:
				//TODO: AcceptsReturn for multiline text box
				break;
			}

			ResetCaretBlink();
		}

		void TextBox::OnKeyChar(KeyCharEventArgs& e)
		{
			Widget::OnKeyChar(e);

			if (!GetEnabled() || !GetVisible())
				return;

			switch (e.KeyChar())
			{
			case 1: //Ctrl+A
				SelectAll();
				break;

			case 3: //Ctrl+C
				Copy();
				break;

			case 22: //Ctrl+V
				if (!GetReadOnly())
					Paste();
				break;

			case 24: //Ctrl+X
				if (!GetReadOnly())
					Cut();
				break;

			case 26: //Ctrl+Z
				if (!GetReadOnly())
					Undo();
				break;

			case '\b':
			case '\t':
			case '\n':
			case '\r':
				break;

			default:
				if (GetReadOnly())
					break;

				if (GetSelectionLength() > 0)
				{
					// Remove the selected text
					RemoveSelectedText();
				}

				if (!_InsertMode && _CaretPosition < GetTextLength())
				{
					// Replace the character at the caret position
					String text = GetText();
					text[_CaretPosition] = e.KeyChar();
					SetText(text);
				}
				else
				{
					// Insert the character at the caret position
					InsertCharacterAt(_CaretPosition, e.KeyChar());
				}

				// Move the caret
				SetCaretPosition(_CaretPosition + 1);
				_SelectionStart = _CaretPosition;
			}

			ResetCaretBlink();
		}
	}
}
