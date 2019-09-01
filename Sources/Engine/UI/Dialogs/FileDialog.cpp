/*=============================================================================
FileDialog.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FileDialog.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(FileDialog);
		SE_IMPLEMENT_REFLECTION(FileDialog);

		int32 spacing = 2;
		int32 margin = 5;
		int32 labelWidth = 60;
		int32 buttonWidth = 60;

		FileDialog::FileDialog() :
			super()
		{
			_Rectangle.Width = 250;
			_Rectangle.Height = 250;

			_Modal = true;

			_fileDialogMode = FileDialogMode_Open;
			_ShowDirectories = true;
			_Multiselect = false;
			_FilterIndex = 0;

			_labelPath = new Label();
			_labelFileName = new Label();
			_labelFilter = new Label();

			_labelPath->SetText(_T("Look in:"));
			_labelFileName->SetText(_T("File name:"));

			Children().Add(_labelPath);
			Children().Add(_labelFileName);
			Children().Add(_labelFilter);

			_OKButton = new PushButton();
			_CancelButton = new PushButton();
			_PathLabel = new Label();
			_filesListBox = new ListBox();
			_fileNameTextBox = new TextBox();
			_FilterComboBox = new ComboBox();

			_OKButton->SetWidth(buttonWidth);
			_CancelButton->SetWidth(buttonWidth);
			_CancelButton->SetText(_T("Cancel"));

			_ParentButton = new PushButton();
			_ParentButton->SetWidth(UISystem::Instance()->DefaultSize);
			_ParentButton->SetText(_T("Up"));

			_OKButton->Click += new DefaultEventDelegateM<FileDialog>(this, &FileDialog::OnOKButtonClick);
			_CancelButton->Click += new DefaultEventDelegateM<FileDialog>(this, &FileDialog::OnCancelButtonClick);
			_ParentButton->Click += new DefaultEventDelegateM<FileDialog>(this, &FileDialog::OnParentButtonClick);

			_filesListBox->SelectedItemChanged += new DefaultEventDelegateM<FileDialog>(this,
				&FileDialog::OnFilesListBoxSelectedItemChanged);
			_filesListBox->MouseDoubleClick += new DefaultEventDelegateM<FileDialog>(this,
				&FileDialog::OnFilesListBoxMouseDoubleClick);
			_FilterComboBox->SelectedItemChanged += new DefaultEventDelegateM<FileDialog>(this,
				&FileDialog::OnFilterComboBoxSelectedItemChanged);

			Children().Add(_OKButton);
			Children().Add(_CancelButton);
			Children().Add(_ParentButton);
			Children().Add(_PathLabel);
			Children().Add(_filesListBox);
			Children().Add(_fileNameTextBox);
			Children().Add(_FilterComboBox);
		}

		FileDialog::~FileDialog()
		{
			/*SE_DELETE(_OKButton);
			SE_DELETE(_CancelButton);
			SE_DELETE(_PathLabel);
			SE_DELETE(_filesListBox);
			SE_DELETE(_fileNameTextBox);
			SE_DELETE(_FilterComboBox);*/
		}

		String FileDialog::GetFileName() const
		{
			return _fileName;
		}

		void FileDialog::SetFileName(String value)
		{
			_fileName = value;
			_fileNameTextBox->SetText(_fileName);
		}

		void FileDialog::SetInitialDirectory(String value)
		{
			_InitialDirectory = value;
			_PathLabel->SetText(_InitialDirectory);
		}

		void FileDialog::UpdateLayout()
		{
			super::UpdateLayout();

			_ParentButton->SetPosition(PointInt(0, 0));

			_labelPath->SetPosition(PointInt(UISystem::Instance()->DefaultSize, 0));
			_labelPath->SetSize(SizeInt(labelWidth, UISystem::Instance()->DefaultSize));

			_PathLabel->SetPosition(PointInt(UISystem::Instance()->DefaultSize + labelWidth, 0));
			_PathLabel->SetSize(SizeInt(GetWidth() - labelWidth - UISystem::Instance()->DefaultSize,
				UISystem::Instance()->DefaultSize));
			_PathLabel->SetTextAlignment(Alignment_MiddleLeft);
			_PathLabel->SetText(_InitialDirectory);

			int32 y = GetHeight() - 3*UISystem::Instance()->DefaultSize;

			_filesListBox->SetPosition(PointInt(0, UISystem::Instance()->DefaultSize));
			_filesListBox->SetSize(SizeInt(GetWidth(), y));

			y = GetHeight() - 2*UISystem::Instance()->DefaultSize;

			_labelFileName->SetPosition(PointInt(0, y));
			_labelFileName->SetSize(SizeInt(labelWidth, UISystem::Instance()->DefaultSize));

			_fileNameTextBox->SetPosition(PointInt(labelWidth, y));
			_fileNameTextBox->SetSize(SizeInt(GetWidth() - labelWidth - buttonWidth,
				UISystem::Instance()->DefaultSize));

			_OKButton->SetPosition(PointInt(GetWidth() - buttonWidth, y));

			y = GetHeight() - UISystem::Instance()->DefaultSize;

			_labelFilter->SetPosition(PointInt(0, y));
			_labelFilter->SetSize(SizeInt(labelWidth, UISystem::Instance()->DefaultSize));

			_FilterComboBox->SetPosition(PointInt(labelWidth, y));
			_FilterComboBox->SetSize(SizeInt(GetWidth() - labelWidth - buttonWidth,
				UISystem::Instance()->DefaultSize));

			_CancelButton->SetPosition(PointInt(GetWidth() - buttonWidth, y));
		}

		void FileDialog::UpdateFilters()
		{
			int i;
			int count;

			count = _Filters.Count();
			if (count == 0)
			{
				_Filters.Add(_T("All Files|*.*"));
			}

			_FilterComboBox->Items().Clear();
			for (i=0; i<count; i++)
			{
				const String& filter = _Filters[i];
				int index = filter.IndexOf(_T('|'));
				String ext = filter.Right(filter.Length() - index - 1);
				String text;
				if (index != -1)
					text = filter.Left(index) + _T(" (") + ext + _T(")");
				else
					text = filter;
				ListItem* filterItem = new ListItem(text);
				filterItem->SetUserData(ext);
				_FilterComboBox->Items().Add(filterItem);
			}

			if (_FilterIndex >=0 && _FilterIndex < _FilterComboBox->Items().Count())
			{
				_FilterComboBox->SetSelectedIndex(_FilterIndex);
			}
		}

		bool FileDialog::OnOKButtonClick(Object* sender, const EventArgs& e)
		{
			if (_filesListBox->GetSelectedIndex() != -1)
			{
				String fileName = _filesListBox->GetSelectedItem()->GetUserData().ToString();
				String path = Path::Combine(_InitialDirectory, fileName);
				if (Directory::Exists(path))
				{
					SetInitialDirectory(path);
					Refresh();
					return true;
				}
			}

			_DialogResult = DialogResult_OK;

			if (_Multiselect)
			{
				int i;
				int count;

				count = _filesListBox->Items().Count();
				for (i=0; i<count; i++)
				{
					String fileName =  _filesListBox->GetSelectedItems().GetItem(i)->GetUserData().ToString();
					String path = Path::Combine(_InitialDirectory, fileName);
					if (!Directory::Exists(path)) //only add file names
					{
						_fileNames.Add(path);
					}
				}

				if (_fileNames.Count() > 0)
					_fileName = _fileNames[_fileNames.Count() - 1];
			}
			else
			{
				_fileName = _fileNameTextBox->GetText();

				// The file name was entered manually
				if (_filesListBox->GetSelectedIndex() == -1)
				{
					// Check if it has an extension
					if (Path::GetExtension(_fileName).IsEmpty())
					{
						// If not, append one using the current filter
						if (_FilterComboBox->GetSelectedItem() != NULL)
						{
							String ext = _FilterComboBox->GetSelectedItem()->GetUserData().ToString();
							int index = ext.IndexOf(_T('.'));
							if (index == -1)
								_fileName = _fileName + ext;
							else
								_fileName = _fileName + "." + ext.Right(ext.Length() - index - 1);
						}
					}
				}

				_fileName = Path::Combine(_InitialDirectory, _fileName);

				GetParent()->Children().Remove(this);
				//UI::UISystem::Instance()->DestroyWidget(this);
				ModalDialogClosed.Invoke(this, EventArgs::Empty);
			}

			return true;
		}

		bool FileDialog::OnCancelButtonClick(Object* sender, const EventArgs& e)
		{
			_DialogResult = DialogResult_Cancel;

			GetParent()->Children().Remove(this);
			ModalDialogClosed.Invoke(this, EventArgs::Empty);

			return true;
		}

		bool FileDialog::OnParentButtonClick(Object* sender, const EventArgs& e)
		{
			String path;
			int pos = _InitialDirectory.LastIndexOf(Environment::GetPathSeparator());
			if (pos == -1)
				path = ".";
			else
				path = _InitialDirectory.Left(pos);

			SetInitialDirectory(path);
			Refresh();

			return true;
		}

		bool FileDialog::OnFilesListBoxSelectedItemChanged(Object* sender, const EventArgs& e)
		{
			if (!_Multiselect || _fileDialogMode == FileDialogMode_Save)
			{
				ListItem* item = _filesListBox->GetSelectedItem();
				if (item != NULL)
				{
					String fileName =  item->GetUserData().ToString();
					String path = Path::Combine(_InitialDirectory, fileName);
					if (!Directory::Exists(path))
					{
						_fileNameTextBox->SetText(item->GetUserData().ToString());
					}
				}
			}

			return true;
		}

		bool FileDialog::OnFilesListBoxMouseDoubleClick(Object* sender, const EventArgs& e)
		{
			OnOKButtonClick(sender, e);

			return true;
		}

		bool FileDialog::OnFilterComboBoxSelectedItemChanged(Object* sender, const EventArgs& e)
		{
			_FilterIndex = _FilterComboBox->GetSelectedIndex();
			Refresh();

			return true;
		}

		void FileDialog::Show(Widget* owner)
		{
			super::Show(owner);

			UpdateLayout();
			UpdateFilters();
			Refresh();

			UISystem::Instance()->SetFocusedWidget(_fileNameTextBox);
		}

		void FileDialog::Refresh()
		{
			super::Refresh();

			int i;
			int count;

			if (_Title.IsEmpty())
			{
				_Title = (_fileDialogMode == FileDialogMode_Open) ?
					_T("Open file") : _T("Save file");
			}

			_labelFilter->SetText((_fileDialogMode == FileDialogMode_Open) ?
				_T("Files of type:") : _T("Save as type:"));
			_OKButton->SetText((_fileDialogMode == FileDialogMode_Open) ?
				_T("Open") : _T("Save"));

			_filesListBox->Items().Clear();

			Directory directory(Path::Combine(_InitialDirectory, _T("*.*")));

			if (_ShowDirectories)
			{
				const Array<String>& dirs = directory.GetDirectories();
				count = dirs.Count();
				for (i=0; i<count; i++)
				{
					ListItem* dirItem = new ListItem(_T("[") + dirs[i] + _T("]"));
					dirItem->SetUserData(dirs[i]);
					_filesListBox->Items().Add(dirItem);
				}
			}

			String filter;
			if (_FilterIndex >= 0 && _FilterIndex < _FilterComboBox->Items().Count())
				filter = _FilterComboBox->Items().GetItem(_FilterIndex)->GetUserData().ToString();
			else
				filter = _T("*.*");
			directory.SetName(Path::Combine(_InitialDirectory, filter));

			const Array<String>& files = directory.GetFiles();
			count = files.Count();
			for (i=0; i<count; i++)
			{
				ListItem* fileItem = new ListItem(files[i]);
				fileItem->SetUserData(files[i]);
				_filesListBox->Items().Add(fileItem);
			}

			_filesListBox->SetMultiSelection(_Multiselect && _fileDialogMode == FileDialogMode_Open);
		}

		void FileDialog::OnSize(EventArgs& e)
		{
			super::OnSize(e);

			UpdateLayout();
		}
	}
}
