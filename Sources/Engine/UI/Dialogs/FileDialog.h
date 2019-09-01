/*=============================================================================
FileDialog.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_FILEDIALOG_H_
#define _SE_UI_FILEDIALOG_H_

#include "UI/Common.h"
#include "UI/Dialog.h"
#include "UI/PushButton.h"
#include "UI/Label.h"
#include "UI/ListBox.h"
#include "UI/ComboBox.h"

namespace SonataEngine
{
	namespace UI
	{
		/** Mode of the file dialog. */
		enum FileDialogMode
		{
			/** Open file dialog. */
			FileDialogMode_Open,

			/** Save file dialog. */
			FileDialogMode_Save
		};

		/**
			@brief Displays a dialog from which the user can select a file.
		*/
		class SE_UI_EXPORT FileDialog : public Dialog
		{
			SE_DECLARE_CLASS(FileDialog, Dialog);

			SE_BEGIN_REFLECTION(FileDialog);
			SE_END_REFLECTION(FileDialog);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			FileDialog();

			/** Destructor. */
			virtual ~FileDialog();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the mode  of the file dialog. */
			FileDialogMode GetFileDialogMode() const { return _fileDialogMode; }
			void SetFileDialogMode(FileDialogMode value) { _fileDialogMode = value; }

			/** Gets or sets a value indicating whether the dialog box shows directories. */
			bool GetShowDirectories() const { return _ShowDirectories; }
			void SetShowDirectories(bool value) { _ShowDirectories = value; }

			/** Gets or sets a value indicating whether the open dialog box allows multiple files to be selected. */
			bool GetMultiselect() const { return _Multiselect; }
			void SetMultiselect(bool value) { _Multiselect = value; }

			/** Gets or sets a string containing the file name selected in the file dialog. */
			String GetFileName() const;
			void SetFileName(String value);

			/** Gets the file names of all selected files in the open file dialog. */
			Array<String> GetFileNames() const { return _fileNames; }

			/** Gets or sets the initial directory displayed by the file dialog. */
			String GetInitialDirectory() const { return _InitialDirectory; }
			void SetInitialDirectory(String value);

			/** Gets or sets the default file name extension. */
			String GetDefaultExtension() const { return _DefaultExtension; }
			void SetDefaultExtension(String value) { _DefaultExtension = value; }

			/** Gets or sets the file name filters. */
			Array<String>& Filters() { return _Filters; }

			/** Gets or sets the index of the filter currently selected in the file dialog. */
			int32 GetFilterIndex() const { return _FilterIndex; }
			void SetFilterIndex(int32 value) { _FilterIndex = value; }
			//@}

			/** @name Operations. */
			//@{
			virtual void Show(Widget* owner);

			virtual void Refresh();

			virtual void OnSize(EventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when a file is selected. */
			DefaultEvent FileSelected;
			//@}

		protected:
			virtual void UpdateLayout();
			virtual void UpdateFilters();

			virtual bool OnOKButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnCancelButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnParentButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnFilesListBoxSelectedItemChanged(Object* sender, const EventArgs& e);
			virtual bool OnFilesListBoxMouseDoubleClick(Object* sender, const EventArgs& e);
			virtual bool OnFilterComboBoxSelectedItemChanged(Object* sender, const EventArgs& e);

		protected:
			FileDialogMode _fileDialogMode;
			bool _ShowDirectories;
			bool _Multiselect;
			String _fileName;
			Array<String> _fileNames;
			String _InitialDirectory;
			String _DefaultExtension;
			Array<String> _Filters;
			int32 _FilterIndex;

			Label* _labelPath;
			Label* _labelFileName;
			Label* _labelFilter;

			PushButton* _OKButton;
			PushButton* _CancelButton;
			PushButton* _ParentButton;
			Label* _PathLabel;
			ListBox* _filesListBox;
			TextBox* _fileNameTextBox;
			ComboBox* _FilterComboBox;
		};
	}
}

#endif 
