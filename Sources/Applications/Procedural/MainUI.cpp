/*=============================================================================
MainUI.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "MainUI.h"
#include "Procedural.h"
#include "SettingsDialog.h"
#include "Operators/Operators.h"

int leftPanelWidth = 80;
int rightPanelWidth = 160;
int toolBarHeight = 30;

int toolBarButtonLeft = 10;
#define ADDTOOLBARBUTTON(name, text) \
	{ \
		UI::PushButton* button = new UI::PushButton(); \
		button->SetName(#name); \
		button->SetPosition(PointInt(toolBarButtonLeft, (toolBarHeight - (toolBarHeight/2)) / 2)); \
		button->SetSize(SizeInt(70, toolBarHeight/2)); \
		button->SetText(text); \
		button->SetForegroundColor(Color32::Blue); \
		button->SetBackgroundColor(Color32::Gray); \
		toolBar->Children().Add(button); \
		button->Click += new EventMethodSlot<MainUI>(this, &MainUI::OnToolBarClick); \
		toolBarButtonLeft += 90; \
	}

String toolBoxSelection;
bool toolBoxDragging = false;

OperatorProperty* propertyGridProperty;

MainUI::MainUI()
{
	_Screen = NULL;
	_ListBoxCategory = NULL;
	_ListBoxToolBox = NULL;
	_PropertyGrid = NULL;

	_PropertyGridBinder = new UI::PropertyGridBinder();
	_DiagramController = NULL;
}

MainUI::~MainUI()
{
	Destroy();
}

void MainUI::Create()
{
	CreateUI();

	_ListBoxCategory->Items().Clear();
	_ListBoxToolBox->Items().Clear();

	OperatorLibrary* library = AppCore::Instance()->GetOperatorLibrary();
	const OperatorLibrary::CategoryList& categories = library->Categories();
	OperatorLibrary::CategoryList::Iterator it = categories.GetIterator();
	while (it.Next())
	{
		const OperatorCategory& category = it.Current();

		UI::ListItem* item = new UI::ListItem();
		item->SetText(category.Name + " (" + String::ToString(category.Operators.Count()) + ")");
		item->SetUserData(category.Name);
		_ListBoxCategory->Items().Add(item);
	}

	_PropertyGridBinder->SetSelectedObject(NULL);
}

void MainUI::Destroy()
{
	SE_DELETE(_DiagramController);
	SE_DELETE(_PropertyGridBinder);
}

void MainUI::Initialize()
{
	int clientWidth = AppCore::Instance()->GetMainWindow()->GetClientWidth();
	int clientHeight = AppCore::Instance()->GetMainWindow()->GetClientHeight() - toolBarHeight;

	UI::Diagram* diagram = AppCore::Instance()->GetWorkflow()->GetDiagram();
	diagram->SetPosition(PointInt(leftPanelWidth, toolBarHeight));
	diagram->SetSize(SizeInt(clientWidth - leftPanelWidth - rightPanelWidth,
		clientHeight));
	_Screen->Children().Add(diagram);

	SE_DELETE(_DiagramController);
	_DiagramController = new DiagramController(diagram);

	diagram->MouseDown += new EventMethodSlot<MainUI>(this,
		(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnDiagramMouseDown);
	diagram->MouseMove += new EventMethodSlot<DiagramController>(_DiagramController,
		(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnDiagramMouseMove);

	_PropertyGridBinder->SetSelectedObject(NULL);
}

void MainUI::RegisterTableEvents(UI::TableNode* table)
{
	table->MouseDown += new EventMethodSlot<MainUI>(this,
		(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnTableMouseDown);
	table->MouseDoubleClick += new EventMethodSlot<MainUI>(this,
		(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnTableMouseDoubleClick);

	table->MouseDown += new EventMethodSlot<DiagramController>(_DiagramController,
		(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnNodeMouseDown);
	table->KeyDown += new EventMethodSlot<DiagramController>(_DiagramController,
		(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnNodeKeyDown);

	UI::TableNode::TablePropertyList::Iterator itIn = table->InputProperties().GetIterator();
	while (itIn.Next())
	{
		UI::TableProperty* property = itIn.Current();
		property->PostRender += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTablePostRender);
		property->MouseMove += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseMove);
		property->MouseDown += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseDown);
		property->MouseUp += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseUp);

		property->MouseDown += new EventMethodSlot<MainUI>(this, (EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnNodeMouseDown);
	}

	UI::TableNode::TablePropertyList::Iterator itOut = table->OutputProperties().GetIterator();
	while (itOut.Next())
	{
		UI::TableProperty* property = itOut.Current();
		property->PostRender += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTablePostRender);
		property->MouseMove += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseMove);
		property->MouseDown += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseDown);
		property->MouseUp += new EventMethodSlot<DiagramController>(_DiagramController,
			(EventMethodSlot<DiagramController>::MethodDelegate)&DiagramController::OnTableMouseUp);

		property->MouseDown += new EventMethodSlot<MainUI>(this, (EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnNodeMouseDown);
	}
}

UI::Widget* MainUI::GetChildByName(const String& name) const
{
	if (_Screen == NULL)
		return NULL;
	else
		return _Screen->GetChildByName(name);
}

void MainUI::SetSelectedOperator(Operator* op)
{
	_PropertyGridBinder->SetSelectedObject(op);
}

void MainUI::CreateUI()
{
#if 1
	Color32 BackColor = Color32(224.0f/255.0f, 223.0f/255.0f, 227.0f/255.0f, 0.6f);
	Color32 InfoBackColor = Color32(0.6f, 0.6f, 0.6f, 0.6f);

	_Screen = new UI::Panel();
	_Screen->SetHasFrame(false);
	_Screen->SetHasBackground(false);
	_Screen->SetSize(SizeInt(AppCore::Instance()->GetMainWindow()->GetClientWidth(),
		AppCore::Instance()->GetMainWindow()->GetClientHeight()));

	UI::Panel* toolBar = new UI::Panel();
	toolBar->SetName("toolBar");
	toolBar->SetMovable(false);
	toolBar->SetHasFrame(false);
	toolBar->SetHasBackground(true);
	toolBar->SetBackgroundColor(BackColor);

		ADDTOOLBARBUTTON(buttonNew, "New");
		ADDTOOLBARBUTTON(buttonOpen, "Open...");
		ADDTOOLBARBUTTON(buttonSave, "Save");
		ADDTOOLBARBUTTON(buttonSaveAs, "Save As...");
		ADDTOOLBARBUTTON(buttonSettings, "Settings...");
		ADDTOOLBARBUTTON(buttonExport, "Export...");
		ADDTOOLBARBUTTON(buttonExit, "Exit");

	_Screen->Children().Add(toolBar);

	UI::ListBox* listBoxCategory = new UI::ListBox();
	listBoxCategory->SetName("listBoxCategory");
	listBoxCategory->SetHasFrame(true);
	listBoxCategory->SetFrameSize(1);
	listBoxCategory->SetFrameColor(Color32::Black);
	listBoxCategory->SetBackgroundColor(Color32::White);
	_Screen->Children().Add(listBoxCategory);

	UI::ListBox* listBoxToolBox = new UI::ListBox();
	listBoxToolBox->SetName("listBoxToolBox");
	listBoxToolBox->SetHasFrame(true);
	listBoxToolBox->SetFrameSize(1);
	listBoxToolBox->SetFrameColor(Color32::Black);
	listBoxToolBox->SetBackgroundColor(Color32::White);
	_Screen->Children().Add(listBoxToolBox);

	UI::PropertyGrid* propertyGrid = new UI::PropertyGrid();
	propertyGrid->SetName("propertyGrid");
	propertyGrid->SetHasFrame(true);
	propertyGrid->SetFrameSize(1);
	propertyGrid->SetFrameColor(Color32::Black);
	propertyGrid->SetBackgroundColor(Color32::Gray);
	_Screen->Children().Add(propertyGrid);

#else
	File* file = new File("MainUI.xml");
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_Screen = (UI::Panel*)serializer->Deserialize(stream.Get());
		stream->Close();
	}
#endif

	if (_Screen != NULL)
	{
		UI::UISystem::Instance()->SetCurrentWidget(_Screen);
	}

	// Objects
	toolBar = (UI::Panel*)GetChildByName("toolBar");
	_ListBoxCategory = (UI::ListBox*)GetChildByName("listBoxCategory");
	_ListBoxToolBox = (UI::ListBox*)GetChildByName("listBoxToolBox");
	_PropertyGrid = (UI::PropertyGrid*)GetChildByName("propertyGrid");

	_PropertyGridBinder->SetPropertyGrid(_PropertyGrid);

	int clientWidth = AppCore::Instance()->GetMainWindow()->GetClientWidth();
	int clientHeight = AppCore::Instance()->GetMainWindow()->GetClientHeight() - toolBarHeight;

	toolBar->SetPosition(PointInt(0, 0));
	toolBar->SetSize(SizeInt(clientWidth, toolBarHeight));
	_ListBoxCategory->SetPosition(PointInt(0, toolBarHeight));
	_ListBoxCategory->SetSize(SizeInt(leftPanelWidth, clientHeight / 2));
	_ListBoxToolBox->SetPosition(PointInt(0, toolBarHeight + clientHeight / 2));
	_ListBoxToolBox->SetSize(SizeInt(leftPanelWidth, clientHeight / 2));
	_PropertyGrid->SetPosition(PointInt(clientWidth - rightPanelWidth, toolBarHeight));
	_PropertyGrid->SetSize(SizeInt(rightPanelWidth, clientHeight));

	// Events
	_ListBoxCategory->MouseClick += new EventMethodSlot<MainUI>(this,
		&MainUI::OnListBoxCategoryMouseClick);
	_ListBoxToolBox->MouseClick += new EventMethodSlot<MainUI>(this,
		&MainUI::OnListBoxToolBoxMouseClick);
	_ListBoxToolBox->MouseDoubleClick += new EventMethodSlot<MainUI>(this,
		&MainUI::OnListBoxToolBoxMouseDoubleClick);

	_PropertyGrid->PropertyValueChanged += new EventMethodSlot<MainUI>(this,
		(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnPropertyGridPropertyValueChanged);
}

bool MainUI::OnToolBarClick(Object* sender, const EventArgs& e)
{
	UI::Widget* widget = (UI::Widget*)sender;

	Workflow* workflow = AppCore::Instance()->GetWorkflow();

	if (widget->GetName() == "buttonNew")
	{
		AppCore::Instance()->CreateDocument();
	}
	else if (widget->GetName() == "buttonOpen")
	{
		UI::FileDialog* dialog = new UI::FileDialog();
		dialog->SetPosition(PointInt(
			(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
			(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
		dialog->SetFileDialogMode(UI::FileDialogMode_Open);
		dialog->Filters().Add(_T("Workflow|*.xml"));
		dialog->SetInitialDirectory(Environment::GetCurrentDirectory());
		dialog->SetFileName(workflow->GetFileName());
		dialog->Show(NULL);

		dialog->ModalDialogClosed += new EventMethodSlot<MainUI>(this,
			(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnFileOpenDialog);
	}
	else if (widget->GetName() == "buttonSave")
	{
		if (workflow->GetFileName().IsEmpty())
		{
			UI::FileDialog* dialog = new UI::FileDialog();
			dialog->SetPosition(PointInt(
				(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
				(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
			dialog->SetFileDialogMode(UI::FileDialogMode_Save);
			dialog->Filters().Add(_T("Workflow|*.xml"));
			dialog->SetInitialDirectory(Environment::GetCurrentDirectory());
			dialog->Show(NULL);

			dialog->ModalDialogClosed += new EventMethodSlot<MainUI>(this,
				(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnFileSaveDialog);
		}
		else
		{
			AppCore::Instance()->SaveDocument(workflow->GetFileName());
		}
	}
	else if (widget->GetName() == "buttonSaveAs")
	{
		UI::FileDialog* dialog = new UI::FileDialog();
		dialog->SetPosition(PointInt(
			(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
			(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
		dialog->SetFileDialogMode(UI::FileDialogMode_Save);
		dialog->Filters().Add(_T("Workflow|*.xml"));
		dialog->SetInitialDirectory(Environment::GetCurrentDirectory());
		dialog->SetFileName(workflow->GetFileName());
		dialog->Show(NULL);

		dialog->ModalDialogClosed += new EventMethodSlot<MainUI>(this,
			(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnFileSaveDialog);
	}
	else if (widget->GetName() == "buttonSettings")
	{
		SettingsDialog* dialog = new SettingsDialog();
		dialog->SetPosition(PointInt(
			(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
			(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
		dialog->Show(NULL);
	}
	else if (widget->GetName() == "buttonExport")
	{
		Operator* op = workflow->GetSelectedOperator();

		if (op != NULL && (op->GetType()->IsSubclassOf(typeof(ColorOperatorBase)) ||
			op->GetType()->IsSubclassOf(typeof(FunctionOperatorBase))))
		{
			UI::FileDialog* dialog = new UI::FileDialog();
			dialog->SetPosition(PointInt(
				(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
				(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
			dialog->SetFileDialogMode(UI::FileDialogMode_Save);
			dialog->Filters().Add(_T("BMP Image|*.bmp"));
			dialog->Filters().Add(_T("DDS Image|*.dds"));
			dialog->Filters().Add(_T("JPG Image|*.jpg"));
			dialog->Filters().Add(_T("PCX Image|*.pcx"));
			dialog->Filters().Add(_T("PNG Image|*.png"));
			dialog->Filters().Add(_T("TGA Image|*.tga"));
			dialog->SetInitialDirectory(Environment::GetCurrentDirectory());
			dialog->Show(NULL);

			dialog->ModalDialogClosed += new EventMethodSlot<MainUI>(this,
				(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnFileExportDialog);
		}
	}
	else if (widget->GetName() == "buttonExit")
	{
		AppCore::Instance()->ExitApplication();
	}

	return true;
}

bool MainUI::OnListBoxCategoryMouseClick(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxCategory->GetSelectedItem();

	_ListBoxToolBox->Items().Clear();

	if (item != NULL)
	{
		OperatorLibrary* library = AppCore::Instance()->GetOperatorLibrary();
		const OperatorCategory* category = library->GetCategoryByName(item->GetUserData().ToString());
		if (category != NULL)
		{
			OperatorCategory::OperatorList::Iterator it = category->Operators.GetIterator();
			while (it.Next())
			{
				const OperatorDescription& description = it.Current();

				UI::ListItem* item = new UI::ListItem();
				item->SetText(description.Name);
				item->SetUserData(description.TypeName);
				_ListBoxToolBox->Items().Add(item);
			}
		}
	}

	return true;
}

bool MainUI::OnListBoxToolBoxMouseClick(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxToolBox->GetSelectedItem();

	if (item != NULL)
	{
		toolBoxDragging = true;
		toolBoxSelection = item->GetUserData().ToString();
	}

	return true;
}

bool MainUI::OnListBoxToolBoxMouseDoubleClick(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxToolBox->GetSelectedItem();

	if (item != NULL)
	{
	}

	return true;
}

bool MainUI::OnPropertyGridPropertyValueChanged(Object* sender, const UI::PropertyChangedEventArgs& e)
{
	Operator* op = AppCore::Instance()->GetWorkflow()->GetSelectedOperator();

	if (e.ChangedItem()->GetLabel() == "_name")
	{
		op->GetNode()->SetTitle(op->GetName());
	}

	op->Invalidate();

	return true;
}

bool MainUI::OnDiagramMouseDown(Object* sender, const UI::MouseEventArgs& e)
{
	Workflow* workflow = AppCore::Instance()->GetWorkflow();

	_PropertyGridBinder->SetSelectedObject(NULL);
	workflow->SetSelectedOperator(NULL);

	if (!toolBoxDragging)
	{
		return true;
	}

	Operator* op = NULL;

	if (!toolBoxSelection.IsEmpty())
	{
		op = AppCore::Instance()->GetOperatorLibrary()->CreateOperator(toolBoxSelection);
	}

	if (op != NULL)
	{
		op->SetName(toolBoxSelection);
		op->SetWorkflow(workflow);
		op->Create();

		UI::TableNode* table = op->CreateNode();
		table->SetPosition(workflow->GetDiagram()->GlobalToLocal(e.Location()));
		RegisterTableEvents(table);

		workflow->AddOperator(op);
		op->Invalidate();
		workflow->SetSelectedOperator(op);
		SetSelectedOperator(op);
	}

	toolBoxSelection = String::Empty;
	toolBoxDragging = false;

	_ListBoxToolBox->SetSelectedItem(NULL);

	return true;
}

bool MainUI::OnNodeMouseDown(Object* sender, const UI::MouseEventArgs& e)
{
	UI::TableProperty* property = (UI::TableProperty*)sender;
	UI::TableNode* table = (UI::TableNode*)property->GetOwner();

	Operator* op = (Operator*)table->GetUserData().ToObject();
	_PropertyGridBinder->SetSelectedObject(op);

	return true;
}

bool MainUI::OnTableMouseDown(Object* sender, const UI::MouseEventArgs& e)
{
	UI::TableNode* table = (UI::TableNode*)sender;

	Operator* op = (Operator*)table->GetUserData().ToObject();
	_PropertyGridBinder->SetSelectedObject(op);

	return true;
}

bool MainUI::OnTableMouseDoubleClick(Object* sender, const EventArgs& e)
{
	UI::TableNode* table = (UI::TableNode*)sender;

	Operator* op = (Operator*)table->GetUserData().ToObject();

	if (op->GetType() == (typeof(ImageOperator)))
	{
		String fileName = ((Operators::ImageOperator*)op)->FileName;

		UI::FileDialog* dialog = new UI::FileDialog();
		dialog->SetUserData(op);
		dialog->SetPosition(PointInt(
			(AppCore::Instance()->GetMainWindow()->GetClientWidth() - dialog->GetWidth()) / 2, 
			(AppCore::Instance()->GetMainWindow()->GetClientHeight() - dialog->GetHeight()) / 2));
		dialog->SetFileDialogMode(UI::FileDialogMode_Open);
		dialog->Filters().Add(_T("BMP Image|*.bmp"));
		dialog->Filters().Add(_T("DDS Image|*.dds"));
		dialog->Filters().Add(_T("JPG Image|*.jpg"));
		dialog->Filters().Add(_T("PCX Image|*.pcx"));
		dialog->Filters().Add(_T("PNG Image|*.png"));
		dialog->Filters().Add(_T("TGA Image|*.tga"));
		dialog->Filters().Add(_T("All Images|*.*"));
		dialog->SetFilterIndex(dialog->Filters().Count() - 1);

		String path;
		if (!fileName.IsEmpty())
			path = Path::GetDirectoryName(fileName);
		else
			path = Environment::GetCurrentDirectory();
		dialog->SetInitialDirectory(path);

		dialog->Show(NULL);

		dialog->ModalDialogClosed += new EventMethodSlot<MainUI>(this,
			(EventMethodSlot<MainUI>::MethodDelegate)&MainUI::OnFileImportDialog);
	}
	else
	{
		op->Invalidate();
	}

	return true;
}

bool MainUI::OnFileOpenDialog(Object* sender, const EventArgs& e)
{
	UI::FileDialog* dialog = (UI::FileDialog*)sender;

	if (dialog->GetDialogResult() == UI::DialogResult_OK)
	{
		AppCore::Instance()->OpenDocument(dialog->GetFileName());
	}

	return true;
}

bool MainUI::OnFileSaveDialog(Object* sender, const EventArgs& e)
{
	UI::FileDialog* dialog = (UI::FileDialog*)sender;

	if (dialog->GetDialogResult() == UI::DialogResult_OK)
	{
		AppCore::Instance()->SaveDocument(dialog->GetFileName());
	}

	return true;
}

bool MainUI::OnFileExportDialog(Object* sender, const EventArgs& e)
{
	UI::FileDialog* dialog = (UI::FileDialog*)sender;

	if (dialog->GetDialogResult() == UI::DialogResult_OK)
	{
		AppCore::Instance()->ExportOperator(dialog->GetFileName());
	}

	return true;
}

bool MainUI::OnFileImportDialog(Object* sender, const EventArgs& e)
{
	UI::FileDialog* dialog = (UI::FileDialog*)sender;

	if (dialog->GetDialogResult() == UI::DialogResult_OK)
	{
		Operators::ImageOperator* op = (Operators::ImageOperator*)dialog->GetUserData().ToObject();

		op->FileName = dialog->GetFileName();

		_PropertyGridBinder->SetSelectedObject(NULL);
		_PropertyGridBinder->SetSelectedObject(op);
		op->Invalidate();
	}

	return true;
}
