/*=============================================================================
PhysicsUI.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "PhysicsUI.h"
#include "SamplePhysics.h"
#include "SettingsDialog.h"
#include "BodyDialog.h"

PhysicsUI::PhysicsUI()
{
	_Screen = NULL;
	_ListBoxBodies = NULL;
	_PropertyGrid = NULL;
	_PropertyGridBinder = new UI::PropertyGridBinder();
}

PhysicsUI::~PhysicsUI()
{
	SE_DELETE(_PropertyGridBinder);
}

void PhysicsUI::Create()
{
	CreateUI();
	RefreshScenes();
}

void PhysicsUI::Destroy()
{
}

void PhysicsUI::RefreshScenes()
{
	_ComboBoxScenes->Items().Clear();

	String fileName = FileSystem::Instance()->GetFullPath("Physics\\Scene1.xml");
	Directory dir(Path::Combine(Path::GetDirectoryName(fileName), "*.xml"));
	Array<String> files = dir.GetFiles();
	int count = files.Count();
	for (int i=0; i<count; i++)
	{
		_ComboBoxScenes->Items().Add(new UI::ListItem(files[i]));
	}
}

void PhysicsUI::Initialize()
{
	_ListBoxBodies->Items().Clear();
	_PropertyGridBinder->SetSelectedObject(NULL);
}

void PhysicsUI::CreateUI()
{
#if 1
	Color32 BackColor = Color32(224.0f/255.0f, 223.0f/255.0f, 227.0f/255.0f, 0.6f);
	Color32 InfoBackColor = Color32(0.6f, 0.6f, 0.6f, 0.6f);

	_Screen = new UI::Panel();
	_Screen->SetHasFrame(false);
	_Screen->SetHasBackground(false);
	_Screen->SetSize(SizeInt(GameCore::Instance()->GetMainWindow()->GetClientWidth(),
		GameCore::Instance()->GetMainWindow()->GetClientHeight()));

		UI::Panel* panelInfo = new UI::Panel();
		panelInfo->SetPosition(PointInt(0, 0));
		panelInfo->SetSize(SizeInt(640, 80));
		panelInfo->SetHasFrame(false);
		panelInfo->SetHasBackground(false);

			UI::Label* labelInfo1 = new UI::Label();
			labelInfo1->SetPosition(PointInt(0, 0));
			labelInfo1->SetSize(SizeInt(120, 20));
			labelInfo1->SetText(_T("WASD - Camera movement"));
			labelInfo1->SetBackgroundColor(InfoBackColor);
			labelInfo1->SetForegroundColor(Color32::Red);
			labelInfo1->SetTextAlignment(UI::Alignment_TopLeft);

			UI::Label* labelInfo2 = new UI::Label();
			labelInfo2->SetPosition(PointInt(0, 20));
			labelInfo2->SetSize(SizeInt(120, 20));
			labelInfo2->SetText(_T("IJKLUM - Force"));
			labelInfo2->SetBackgroundColor(InfoBackColor);
			labelInfo2->SetForegroundColor(Color32::Red);
			labelInfo2->SetTextAlignment(UI::Alignment_TopLeft);

			UI::Label* labelInfo3 = new UI::Label();
			labelInfo3->SetPosition(PointInt(0, 40));
			labelInfo3->SetSize(SizeInt(120, 20));
			labelInfo3->SetText(_T("Tab - Next Body"));
			labelInfo3->SetBackgroundColor(InfoBackColor);
			labelInfo3->SetForegroundColor(Color32::Red);
			labelInfo3->SetTextAlignment(UI::Alignment_TopLeft);

			UI::Label* labelInfo4 = new UI::Label();
			labelInfo4->SetPosition(PointInt(0, 60));
			labelInfo4->SetSize(SizeInt(120, 20));
			labelInfo4->SetText(_T("Space - Throw Body"));
			labelInfo4->SetBackgroundColor(InfoBackColor);
			labelInfo4->SetForegroundColor(Color32::Red);
			labelInfo4->SetTextAlignment(UI::Alignment_TopLeft);

		panelInfo->Children().Add(labelInfo1);
		panelInfo->Children().Add(labelInfo2);
		panelInfo->Children().Add(labelInfo3);
		panelInfo->Children().Add(labelInfo4);

		UI::Panel* toolBar = new UI::Panel();
		toolBar->SetMovable(true);
		toolBar->SetPosition(PointInt(370, 10));
		toolBar->SetSize(SizeInt(410, 40));
		toolBar->SetHasFrame(false);
		toolBar->SetHasBackground(true);
		toolBar->SetBackgroundColor(BackColor);

			UI::PushButton* buttonPause = new UI::PushButton();
			buttonPause->SetName("buttonPause");
			buttonPause->SetPosition(PointInt(10, 10));
			buttonPause->SetSize(SizeInt(70, 20));
			buttonPause->SetText(_T("Pause"));
			buttonPause->SetForegroundColor(Color32::Blue);
			buttonPause->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonPause);

			UI::PushButton* buttonSettings = new UI::PushButton();
			buttonSettings->SetName("buttonSettings");
			buttonSettings->SetPosition(PointInt(90, 10));
			buttonSettings->SetSize(SizeInt(70, 20));
			buttonSettings->SetText(_T("Settings..."));
			buttonSettings->SetForegroundColor(Color32::Blue);
			buttonSettings->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonSettings);

			UI::PushButton* buttonBodies = new UI::PushButton();
			buttonBodies->SetName("buttonBodies");
			buttonBodies->SetPosition(PointInt(170, 10));
			buttonBodies->SetSize(SizeInt(70, 20));
			buttonBodies->SetText(_T("Bodies"));
			buttonBodies->SetForegroundColor(Color32::Blue);
			buttonBodies->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonBodies);

			UI::PushButton* buttonProperties = new UI::PushButton();
			buttonProperties->SetName("buttonProperties");
			buttonProperties->SetPosition(PointInt(250, 10));
			buttonProperties->SetSize(SizeInt(70, 20));
			buttonProperties->SetText(_T("Properties"));
			buttonProperties->SetForegroundColor(Color32::Blue);
			buttonProperties->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonProperties);

			UI::PushButton* buttonExit = new UI::PushButton();
			buttonExit->SetName("buttonExit");
			buttonExit->SetPosition(PointInt(330, 10));
			buttonExit->SetSize(SizeInt(70, 20));
			buttonExit->SetText(_T("Exit"));
			buttonExit->SetForegroundColor(Color32::Blue);
			buttonExit->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonExit);

		UI::Panel* windowBodies = new UI::Panel();
		windowBodies->SetName(_T("windowBodies"));
		windowBodies->SetMovable(true);
		windowBodies->SetPosition(PointInt(520, 60));
		windowBodies->SetSize(SizeInt(260, 180));
		windowBodies->SetHasBackground(true);
		windowBodies->SetBackgroundColor(BackColor);

			UI::Label* labelBodies = new UI::Label();
			labelBodies->SetPosition(PointInt(10, 5));
			labelBodies->SetSize(SizeInt(80, 20));
			labelBodies->SetText(_T("Bodies:"));
			labelBodies->SetForegroundColor(Color32::Blue);
			labelBodies->SetHasBackground(false);
			labelBodies->SetTextAlignment(UI::Alignment_MiddleCenter);

			UI::ListBox* listBoxBodies = new UI::ListBox();
			listBoxBodies->SetName(_T("listBoxBodies"));
			listBoxBodies->SetHasFrame(true);
			listBoxBodies->SetFrameSize(1);
			listBoxBodies->SetFrameColor(Color32::Blue);
			listBoxBodies->SetPosition(PointInt(10, 20));
			listBoxBodies->SetSize(SizeInt(80, 150));
			listBoxBodies->SetBackgroundColor(Color32::White);

			UI::PushButton* buttonAddBody = new UI::PushButton();
			buttonAddBody->SetName("buttonAddBody");
			buttonAddBody->SetPosition(PointInt(100, 50));
			buttonAddBody->SetSize(SizeInt(70, 20));
			buttonAddBody->SetText(_T("Add..."));
			buttonAddBody->SetForegroundColor(Color32::Blue);
			buttonAddBody->SetBackgroundColor(Color32::Gray);

			UI::PushButton* buttonRemoveBody = new UI::PushButton();
			buttonRemoveBody->SetName("buttonRemoveBody");
			buttonRemoveBody->SetPosition(PointInt(100, 80));
			buttonRemoveBody->SetSize(SizeInt(70, 20));
			buttonRemoveBody->SetText(_T("Remove"));
			buttonRemoveBody->SetForegroundColor(Color32::Blue);
			buttonRemoveBody->SetBackgroundColor(Color32::Gray);
			buttonRemoveBody->SetVisible(false);

		windowBodies->Children().Add(labelBodies);
		windowBodies->Children().Add(listBoxBodies);
		windowBodies->Children().Add(buttonAddBody);
		windowBodies->Children().Add(buttonRemoveBody);

		UI::Panel* windowProperties = new UI::Panel();
		windowProperties->SetName("windowProperties");
		windowProperties->SetMovable(true);
		windowProperties->SetPosition(PointInt(520, 250));
		windowProperties->SetSize(SizeInt(260, 340));
		windowProperties->SetHasBackground(true);
		windowProperties->SetBackgroundColor(BackColor);

			UI::Label* labelGrid = new UI::Label();
			labelGrid->SetPosition(PointInt(10, 10));
			labelGrid->SetSize(SizeInt(100, 30));
			labelGrid->SetText(_T("Properties:"));
			labelGrid->SetForegroundColor(Color32::Red);
			labelGrid->SetHasBackground(false);
			windowProperties->Children().Add(labelGrid);

			UI::PropertyGrid* propertyGrid = new UI::PropertyGrid();
			propertyGrid->SetName("propertyGrid");
			propertyGrid->SetPosition(PointInt(5, 20));
			propertyGrid->SetSize(SizeInt(250, 310));
			propertyGrid->SetBackgroundColor(BackColor);
			windowProperties->Children().Add(propertyGrid);

		UI::Panel* toolBarScene = new UI::Panel();
		toolBarScene->SetMovable(true);
		toolBarScene->SetPosition(PointInt(140, 10));
		toolBarScene->SetSize(SizeInt(120, 70));
		toolBarScene->SetHasFrame(false);
		toolBarScene->SetHasBackground(true);
		toolBarScene->SetBackgroundColor(BackColor);

			UI::Label* labelScene = new UI::Label();
			labelScene->SetPosition(PointInt(10, 5));
			labelScene->SetSize(SizeInt(40, 20));
			labelScene->SetText(_T("Scenes:"));
			labelScene->SetForegroundColor(Color32::Red);
			labelScene->SetHasBackground(false);
			toolBarScene->Children().Add(labelScene);

			UI::ComboBox* comboBoxScenes = new UI::ComboBox();
			comboBoxScenes->SetName("comboBoxScenes");
			comboBoxScenes->SetPosition(PointInt(10, 15));
			comboBoxScenes->SetSize(SizeInt(100, 20));
			toolBarScene->Children().Add(comboBoxScenes);

			UI::PushButton* buttonNew = new UI::PushButton();
			buttonNew->SetName("buttonNew");
			buttonNew->SetPosition(PointInt(10, 40));
			buttonNew->SetSize(SizeInt(30, 20));
			buttonNew->SetText(_T("New"));
			buttonNew->SetForegroundColor(Color32::Blue);
			buttonNew->SetBackgroundColor(Color32::Gray);
			toolBarScene->Children().Add(buttonNew);

			UI::PushButton* buttonLoad = new UI::PushButton();
			buttonLoad->SetName("buttonLoad");
			buttonLoad->SetPosition(PointInt(45, 40));
			buttonLoad->SetSize(SizeInt(30, 20));
			buttonLoad->SetText(_T("Load"));
			buttonLoad->SetForegroundColor(Color32::Blue);
			buttonLoad->SetBackgroundColor(Color32::Gray);
			toolBarScene->Children().Add(buttonLoad);

			UI::PushButton* buttonSave = new UI::PushButton();
			buttonSave->SetName("buttonSave");
			buttonSave->SetPosition(PointInt(80, 40));
			buttonSave->SetSize(SizeInt(30, 20));
			buttonSave->SetText(_T("Save"));
			buttonSave->SetForegroundColor(Color32::Blue);
			buttonSave->SetBackgroundColor(Color32::Gray);
			toolBarScene->Children().Add(buttonSave);

	_Screen->Children().Add(panelInfo);
	_Screen->Children().Add(toolBar);
	_Screen->Children().Add(windowBodies);
	_Screen->Children().Add(windowProperties);
	_Screen->Children().Add(toolBarScene);
#else
	File* file = new File("PhysicsUI.xml");
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
	_ButtonPause = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonPause");
	_ButtonSettings = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonSettings");
	_ButtonBodies = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonBodies");
	_ButtonProperties = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonProperties");
	_ButtonExit = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonExit");

	_ListBoxBodies = (UI::ListBox*)UI::UISystem::Instance()->GetWidgetByName("listBoxBodies");
	_ButtonAddBody = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonAddBody");
	_ButtonRemoveBody = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonRemoveBody");

	_PropertyGrid = (UI::PropertyGrid*)UI::UISystem::Instance()->GetWidgetByName("propertyGrid");
	_PropertyGridBinder->SetPropertyGrid(_PropertyGrid);

	_ComboBoxScenes = (UI::ComboBox*)UI::UISystem::Instance()->GetWidgetByName("comboBoxScenes");
	_ButtonNew = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonNew");
	_ButtonLoad = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonLoad");
	_ButtonSave = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonSave");

	// Events
	_ButtonPause->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarClick);
	_ButtonSettings->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarClick);
	_ButtonBodies->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarClick);
	_ButtonProperties->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarClick);
	_ButtonExit->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarClick);

	_ListBoxBodies->SelectedItemChanged += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnListBoxBodiesSelectedItemChanged);

	_ButtonAddBody->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnButtonAddBodyClick);
	_ButtonRemoveBody->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnButtonRemoveBodyClick);

	_ButtonNew->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarSceneClick);
	_ButtonLoad->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarSceneClick);
	_ButtonSave->Click += new EventMethodSlot<PhysicsUI>(this,
		&PhysicsUI::OnToolBarSceneClick);
}

bool PhysicsUI::OnToolBarClick(Object* sender, const EventArgs& e)
{
	if (sender == _ButtonPause)
	{
		PhysicsWorld* world = GameCore::Instance()->GetWorld();
		if (world != NULL)
		{
			world->SetPaused(!world->GetPaused());
		}
	}
	else if (sender == _ButtonSettings)
	{
		PhysicsWorld* world = GameCore::Instance()->GetWorld();
		if (world != NULL)
		{
			static SettingsDialog* dialog = new SettingsDialog();
			dialog->SetPosition(PointInt(220, 200));
			dialog->Show(NULL);
		}
	}
	else if (sender == _ButtonBodies)
	{
		UI::Panel* windowBodies = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("windowBodies");
		windowBodies->SetVisible(!windowBodies->GetVisible());
	}
	else if (sender == _ButtonProperties)
	{
		UI::Panel* windowProperties = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("windowProperties");
		windowProperties->SetVisible(!windowProperties->GetVisible());
	}
	else if (sender == _ButtonExit)
	{
		GameCore::Instance()->Exit();
	}

	return true;
}

bool PhysicsUI::OnToolBarSceneClick(Object* sender, const EventArgs& e)
{
	String fileName = FileSystem::Instance()->GetFullPath(
		Path::Combine("Physics", _ComboBoxScenes->GetSelectedText()));

	if (sender == _ButtonNew)
	{
		GameCore::Instance()->NewWorld();
	}
	else if (sender == _ButtonLoad)
	{
		GameCore::Instance()->LoadWorld(fileName);
	}
	else if (sender == _ButtonSave)
	{
		GameCore::Instance()->SaveWorld(fileName);
	}

	return true;
}

bool PhysicsUI::OnListBoxBodiesSelectedItemChanged(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxBodies->GetSelectedItem();

	if (item != NULL)
	{
		PhysicsEntity* entity = (PhysicsEntity*)item->GetUserData().ToObject();

		_PropertyGridBinder->SetSelectedObject(entity->GetPhysicsBody());

		GameCore::Instance()->GetWorld()->SetControlledEntity(entity);
	}
	else
	{
		_PropertyGridBinder->SetSelectedObject(NULL);
	}

	return true;
}

bool PhysicsUI::OnButtonAddBodyClick(Object* sender, const EventArgs& e)
{
	PhysicsWorld* world = GameCore::Instance()->GetWorld();
	if (world != NULL)
	{
		static BodyDialog* dialog = new BodyDialog();
		dialog->SetPosition(PointInt(220, 200));
		dialog->Show(NULL);
	}

	return true;
}

bool PhysicsUI::OnButtonRemoveBodyClick(Object* sender, const EventArgs& e)
{
	PhysicsWorld* world = GameCore::Instance()->GetWorld();
	if (world != NULL)
	{
	}

	return true;
}

void PhysicsUI::AddEntity(PhysicsEntity* entity)
{
	UI::ListItem* item = new UI::ListItem(entity->GetPhysicsBody()->GetName());
	item->SetUserData(entity);
	_ListBoxBodies->Items().Add(item);
}

void PhysicsUI::RemoveEntity(PhysicsEntity* entity)
{
	UI::ListItemList::Iterator it = _ListBoxBodies->Items().GetIterator();
	while (it.Next())
	{
		UI::ListItem* item = it.Current();
		if (entity == (PhysicsEntity*)item->GetUserData().ToObject())
		{
			_ListBoxBodies->Items().Remove(item);
			break;
		}
	}
}
