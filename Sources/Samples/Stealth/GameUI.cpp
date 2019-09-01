/*=============================================================================
GameUI.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "GameUI.h"
#include "SampleStealth.h"
#include "LevelDialog.h"
#include "SettingsDialog.h"

GameUI::GameUI()
{
	_Screen = NULL;
	_ListBoxEntities = NULL;
	_PropertyGrid = NULL;
	_PropertyGridBinder = new UI::PropertyGridBinder();
}

GameUI::~GameUI()
{
	SE_DELETE(_PropertyGridBinder);
}

void GameUI::Create()
{
	CreateUI();
	RefreshLevels();
}

void GameUI::Destroy()
{
}

UI::Widget* GameUI::GetChildByName(const String& name) const
{
	if (_Screen == NULL)
		return NULL;

	return _Screen->GetChildByName(name);
}

void GameUI::Initialize(Map* map)
{
	_ListBoxEntities->Items().Clear();
	_PropertyGridBinder->SetSelectedObject(NULL);

	int count = map->_Actors.Count();
	for (int i=0; i<count; i++)
	{
		AddEntity(map->_Actors[i]);
	}
}

void GameUI::SetEditor(bool value)
{
	UI::Panel* tabPanelEditor = (UI::Panel*)GetChildByName("tabPanelEditor");
	tabPanelEditor->SetVisible(value);

	UI::Panel* toolBarLevel = (UI::Panel*)GetChildByName("toolBarLevel");
	toolBarLevel->SetVisible(value);

	UI::PushButton* buttonGameMode = (UI::PushButton*)GetChildByName("buttonGameMode");
	if (buttonGameMode != NULL)
	{
		buttonGameMode->SetText(value ? "Game" : "Editor");
	}
}

void GameUI::RefreshLevels()
{
	_ComboBoxLevels->Items().Clear();

	String fileName = FileSystem::Instance()->GetFullPath("AI\\MGS\\levels\\Level1.xml");
	Directory dir(Path::Combine(Path::GetDirectoryName(fileName), "*.xml"));
	Array<String> files = dir.GetFiles();
	int count = files.Count();
	for (int i=0; i<count; i++)
	{
		_ComboBoxLevels->Items().Add(new UI::ListItem(files[i]));
	}
}

void GameUI::AddEntity(GameEntity* entity)
{
	UI::ListItem* item = new UI::ListItem(entity->_name);
	item->SetUserData(entity);
	_ListBoxEntities->Items().Add(item);
}

void GameUI::RemoveEntity(GameEntity* entity)
{
	UI::ListItemList::Iterator it = _ListBoxEntities->Items().GetIterator();
	while (it.Next())
	{
		UI::ListItem* item = it.Current();
		if (entity == (GameEntity*)item->GetUserData().ToObject())
		{
			_ListBoxEntities->Items().Remove(item);
			break;
		}
	}
}

void GameUI::SetSelectedObject(Object* obj)
{
	_PropertyGridBinder->SetSelectedObject(obj);
	_PropertyDialog->Children().Clear();

	String typeName = obj->GetType()->GetName() + "Dialog";
	if (TypeFactory::Instance()->GetType(typeName) != NULL)
	{
		UI::Panel* panel = (UI::Panel*)TypeFactory::Instance()->CreateInstance(typeName);
		_PropertyDialog->Children().Add(panel);
	}
}

Tile* GameUI::GetSelectedTile() const
{
	UI::ListItem* item = _ListBoxTiles->GetSelectedItem();
	if (item == NULL)
		return NULL;
	else
		return (Tile*)item->GetUserData().ToObject();
}

ActorDef* GameUI::GetSelectedActorDef() const
{
	UI::ListItem* item = _ListBoxActors->GetSelectedItem();
	if (item == NULL)
		return NULL;
	else
		return (ActorDef*)item->GetUserData().ToObject();
}

void GameUI::CreateUI()
{
	Tileset* tileset = GameCore::Instance()->GetWorld()->GetTileset();
	int tileCount = tileset->_Tiles.Count();
	int actorCount = tileset->_ActorDefs.Count();

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
		panelInfo->SetSize(SizeInt(40, 20));
		panelInfo->SetBackgroundColor(InfoBackColor);
		panelInfo->SetHasFrame(false);

			UI::Label* labelFPS = new UI::Label();
			labelFPS->SetName("labelFPS");
			labelFPS->SetPosition(PointInt(0, 0));
			labelFPS->SetSize(SizeInt(40, 20));
			labelFPS->SetForegroundColor(Color32::Red);
			labelFPS->SetHasBackground(false);
			labelFPS->SetTextAlignment(UI::Alignment_TopLeft);
			panelInfo->Children().Add(labelFPS);

		UI::Panel* toolBar = new UI::Panel();
		toolBar->SetMovable(true);
		toolBar->SetPosition(PointInt(370, 10));
		toolBar->SetSize(SizeInt(410, 40));
		toolBar->SetHasFrame(false);
		toolBar->SetHasBackground(true);
		toolBar->SetBackgroundColor(BackColor);

			UI::PushButton* buttonGameMode = new UI::PushButton();
			buttonGameMode->SetName("buttonGameMode");
			buttonGameMode->SetPosition(PointInt(10, 10));
			buttonGameMode->SetSize(SizeInt(70, 20));
			buttonGameMode->SetText(_T("GameMode"));
			buttonGameMode->SetForegroundColor(Color32::Blue);
			buttonGameMode->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonGameMode);

			UI::PushButton* buttonPause = new UI::PushButton();
			buttonPause->SetName("buttonPause");
			buttonPause->SetPosition(PointInt(90, 10));
			buttonPause->SetSize(SizeInt(70, 20));
			buttonPause->SetText(_T("Pause"));
			buttonPause->SetForegroundColor(Color32::Blue);
			buttonPause->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonPause);

			UI::PushButton* buttonSettings = new UI::PushButton();
			buttonSettings->SetName("buttonSettings");
			buttonSettings->SetPosition(PointInt(170, 10));
			buttonSettings->SetSize(SizeInt(70, 20));
			buttonSettings->SetText(_T("Settings..."));
			buttonSettings->SetForegroundColor(Color32::Blue);
			buttonSettings->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonSettings);

			UI::PushButton* buttonEntities = new UI::PushButton();
			buttonEntities->SetName("buttonEntities");
			buttonEntities->SetPosition(PointInt(250, 10));
			buttonEntities->SetSize(SizeInt(70, 20));
			buttonEntities->SetText(_T("Entities"));
			buttonEntities->SetForegroundColor(Color32::Blue);
			buttonEntities->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonEntities);

			UI::PushButton* buttonProperties = new UI::PushButton();
			buttonProperties->SetName("buttonProperties");
			buttonProperties->SetPosition(PointInt(330, 10));
			buttonProperties->SetSize(SizeInt(70, 20));
			buttonProperties->SetText(_T("Properties"));
			buttonProperties->SetForegroundColor(Color32::Blue);
			buttonProperties->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonProperties);

			UI::PushButton* buttonExit = new UI::PushButton();
			buttonExit->SetName("buttonExit");
			buttonExit->SetPosition(PointInt(410, 10));
			buttonExit->SetSize(SizeInt(70, 20));
			buttonExit->SetText(_T("Exit"));
			buttonExit->SetForegroundColor(Color32::Blue);
			buttonExit->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonExit);

		UI::Panel* windowEntities = new UI::Panel();
		windowEntities->SetName(_T("windowEntities"));
		windowEntities->SetMovable(true);
		windowEntities->SetPosition(PointInt(640, 60));
		windowEntities->SetSize(SizeInt(140, 180));
		windowEntities->SetHasBackground(true);
		windowEntities->SetBackgroundColor(BackColor);

			UI::Label* labelEntities = new UI::Label();
			labelEntities->SetPosition(PointInt(10, 5));
			labelEntities->SetSize(SizeInt(80, 20));
			labelEntities->SetText(_T("Entities:"));
			labelEntities->SetForegroundColor(Color32::Blue);
			labelEntities->SetHasBackground(false);
			labelEntities->SetTextAlignment(UI::Alignment_MiddleCenter);

			UI::ListBox* listBoxEntities = new UI::ListBox();
			listBoxEntities->SetName(_T("listBoxEntities"));
			listBoxEntities->SetHasFrame(true);
			listBoxEntities->SetFrameSize(1);
			listBoxEntities->SetFrameColor(Color32::Blue);
			listBoxEntities->SetPosition(PointInt(10, 20));
			listBoxEntities->SetSize(SizeInt(120, 150));
			listBoxEntities->SetBackgroundColor(Color32::White);

		windowEntities->Children().Add(labelEntities);
		windowEntities->Children().Add(listBoxEntities);

		UI::Panel* tabPanelEditor = new UI::Panel();
		tabPanelEditor->SetName("tabPanelEditor");
		tabPanelEditor->SetMovable(true);
		tabPanelEditor->SetPosition(PointInt(10, 120));
		tabPanelEditor->SetSize(SizeInt(160, 210));
		tabPanelEditor->SetHasBackground(true);
		tabPanelEditor->SetBackgroundColor(BackColor);

			UI::RadioButton* buttonTiles = new UI::RadioButton();
			buttonTiles->SetName("buttonTiles");
			buttonTiles->SetPosition(PointInt(5, 5));
			buttonTiles->SetSize(SizeInt(45, 20));
			buttonTiles->SetHasBackground(false);
			buttonTiles->SetText("Tiles");
			buttonTiles->SetChecked(true);
			tabPanelEditor->Children().Add(buttonTiles);

			UI::RadioButton* buttonActors = new UI::RadioButton();
			buttonActors->SetName("buttonActors");
			buttonActors->SetPosition(PointInt(55, 5));
			buttonActors->SetHasBackground(false);
			buttonActors->SetText("Actors");
			buttonActors->SetSize(SizeInt(45, 20));
			tabPanelEditor->Children().Add(buttonActors);

			UI::Panel* windowTiles = new UI::Panel();
			windowTiles->SetName("windowTiles");
			windowTiles->SetPosition(PointInt(5, 20));
			windowTiles->SetSize(SizeInt(160, 210));
			windowTiles->SetHasBackground(false);

				UI::Label* labelTiles = new UI::Label();
				labelTiles->SetPosition(PointInt(10, 10));
				labelTiles->SetSize(SizeInt(100, 30));
				labelTiles->SetText(_T("Tiles:"));
				labelTiles->SetForegroundColor(Color32::Red);
				labelTiles->SetHasBackground(false);
				windowTiles->Children().Add(labelTiles);

				UI::ListBox* listBoxTiles = new UI::ListBox();
				listBoxTiles->SetName("listBoxTiles");
				listBoxTiles->SetPosition(PointInt(10, 20));
				listBoxTiles->SetSize(SizeInt(100, 120));
				for (int i=0; i<tileCount; i++)
				{
					Tile* tile = tileset->_Tiles[i];
					String tileName = EnumObject::GetName(typeof(TileType), tile->_TileType);
					UI::ListItem* item = new UI::ListItem(tileName);
					item->SetUserData(tile);
					listBoxTiles->Items().Add(item);
				}
				windowTiles->Children().Add(listBoxTiles);

			UI::Panel* windowActorDefs = new UI::Panel();
			windowActorDefs->SetName("windowActorDefs");
			windowActorDefs->SetPosition(PointInt(5, 20));
			windowActorDefs->SetSize(SizeInt(160, 210));
			windowActorDefs->SetHasBackground(false);
			windowActorDefs->SetVisible(false);

				UI::Label* labelActorDef = new UI::Label();
				labelActorDef->SetPosition(PointInt(10, 10));
				labelActorDef->SetSize(SizeInt(100, 30));
				labelActorDef->SetText(_T("Actors:"));
				labelActorDef->SetForegroundColor(Color32::Red);
				labelActorDef->SetHasBackground(false);
				windowActorDefs->Children().Add(labelActorDef);

				UI::ListBox* listBoxActors = new UI::ListBox();
				listBoxActors->SetName("listBoxActors");
				listBoxActors->SetPosition(PointInt(10, 20));
				listBoxActors->SetSize(SizeInt(100, 120));
				for (int i=0; i<actorCount; i++)
				{
					ActorDef* actor = tileset->_ActorDefs[i];
					String actorName = EnumObject::GetName(typeof(ActorType), actor->_ActorType);
					UI::ListItem* item = new UI::ListItem(actor->_name + " (" + actorName + ")");
					item->SetUserData(actor);
					listBoxActors->Items().Add(item);
				}
				windowActorDefs->Children().Add(listBoxActors);

		tabPanelEditor->Children().Add(windowTiles);
		tabPanelEditor->Children().Add(windowActorDefs);

		UI::Panel* windowProperties = new UI::Panel();
		windowProperties->SetName("windowProperties");
		windowProperties->SetMovable(true);
		windowProperties->SetPosition(PointInt(520, 250));
		windowProperties->SetSize(SizeInt(260, 340));
		windowProperties->SetHasBackground(true);
		windowProperties->SetBackgroundColor(BackColor);
		windowProperties->SetVisible(false);

			UI::Label* labelGrid = new UI::Label();
			labelGrid->SetPosition(PointInt(10, 10));
			labelGrid->SetSize(SizeInt(100, 30));
			labelGrid->SetText(_T("Properties:"));
			labelGrid->SetForegroundColor(Color32::Red);
			labelGrid->SetHasBackground(false);
			windowProperties->Children().Add(labelGrid);

			UI::RadioButton* buttonGrid = new UI::RadioButton();
			buttonGrid->SetName("buttonGrid");
			buttonGrid->SetPosition(PointInt(5, 25));
			buttonGrid->SetSize(SizeInt(45, 20));
			buttonGrid->SetHasBackground(false);
			buttonGrid->SetText("Grid");
			buttonGrid->SetChecked(true);
			windowProperties->Children().Add(buttonGrid);

			UI::RadioButton* buttonDialog = new UI::RadioButton();
			buttonDialog->SetName("buttonDialog");
			buttonDialog->SetPosition(PointInt(55, 25));
			buttonDialog->SetHasBackground(false);
			buttonDialog->SetText("Dialog");
			buttonDialog->SetSize(SizeInt(45, 20));
			windowProperties->Children().Add(buttonDialog);

			UI::PropertyGrid* propertyGrid = new UI::PropertyGrid();
			propertyGrid->SetName("propertyGrid");
			propertyGrid->SetPosition(PointInt(5, 45));
			propertyGrid->SetSize(SizeInt(250, 290));
			propertyGrid->SetBackgroundColor(BackColor);
			windowProperties->Children().Add(propertyGrid);

			UI::Panel* propertyDialog = new UI::Panel();
			propertyDialog->SetName("propertyDialog");
			propertyDialog->SetPosition(PointInt(5, 45));
			propertyDialog->SetSize(SizeInt(250, 290));
			propertyDialog->SetHasBackground(false);
			propertyDialog->SetVisible(false);
			windowProperties->Children().Add(propertyDialog);

		UI::Panel* toolBarLevel = new UI::Panel();
		toolBarLevel->SetName("toolBarLevel");
		toolBarLevel->SetMovable(true);
		toolBarLevel->SetPosition(PointInt(10, 40));
		toolBarLevel->SetSize(SizeInt(120, 70));
		toolBarLevel->SetHasFrame(false);
		toolBarLevel->SetHasBackground(true);
		toolBarLevel->SetBackgroundColor(BackColor);

			UI::Label* labelLevel = new UI::Label();
			labelLevel->SetPosition(PointInt(10, 5));
			labelLevel->SetSize(SizeInt(40, 20));
			labelLevel->SetText(_T("Levels:"));
			labelLevel->SetForegroundColor(Color32::Red);
			labelLevel->SetHasBackground(false);
			toolBarLevel->Children().Add(labelLevel);

			UI::ComboBox* comboBoxLevels = new UI::ComboBox();
			comboBoxLevels->SetName("comboBoxLevels");
			comboBoxLevels->SetPosition(PointInt(10, 15));
			comboBoxLevels->SetSize(SizeInt(100, 20));
			toolBarLevel->Children().Add(comboBoxLevels);

			UI::PushButton* buttonNew = new UI::PushButton();
			buttonNew->SetName("buttonNew");
			buttonNew->SetPosition(PointInt(10, 40));
			buttonNew->SetSize(SizeInt(30, 20));
			buttonNew->SetText(_T("New"));
			buttonNew->SetForegroundColor(Color32::Blue);
			buttonNew->SetBackgroundColor(Color32::Gray);
			toolBarLevel->Children().Add(buttonNew);

			UI::PushButton* buttonLoad = new UI::PushButton();
			buttonLoad->SetName("buttonLoad");
			buttonLoad->SetPosition(PointInt(45, 40));
			buttonLoad->SetSize(SizeInt(30, 20));
			buttonLoad->SetText(_T("Load"));
			buttonLoad->SetForegroundColor(Color32::Blue);
			buttonLoad->SetBackgroundColor(Color32::Gray);
			toolBarLevel->Children().Add(buttonLoad);

			UI::PushButton* buttonSave = new UI::PushButton();
			buttonSave->SetName("buttonSave");
			buttonSave->SetPosition(PointInt(80, 40));
			buttonSave->SetSize(SizeInt(30, 20));
			buttonSave->SetText(_T("Save"));
			buttonSave->SetForegroundColor(Color32::Blue);
			buttonSave->SetBackgroundColor(Color32::Gray);
			toolBarLevel->Children().Add(buttonSave);

	_Screen->Children().Add(panelInfo);
	_Screen->Children().Add(toolBar);
	_Screen->Children().Add(windowEntities);
	_Screen->Children().Add(windowProperties);
	_Screen->Children().Add(tabPanelEditor);
	_Screen->Children().Add(toolBarLevel);

#else
	File* file = new File("GameUI.xml");
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
	_ComboBoxLevels = (UI::ComboBox*)GetChildByName("comboBoxLevels");
	buttonNew = (UI::PushButton*)GetChildByName("buttonNew");
	buttonLoad = (UI::PushButton*)GetChildByName("buttonLoad");
	buttonSave = (UI::PushButton*)GetChildByName("buttonSave");

	buttonGameMode = (UI::PushButton*)GetChildByName("buttonGameMode");
	buttonPause = (UI::PushButton*)GetChildByName("buttonPause");
	buttonSettings = (UI::PushButton*)GetChildByName("buttonSettings");
	buttonEntities = (UI::PushButton*)GetChildByName("buttonEntities");
	buttonProperties = (UI::PushButton*)GetChildByName("buttonProperties");
	buttonExit = (UI::PushButton*)GetChildByName("buttonExit");

	_ListBoxEntities = (UI::ListBox*)GetChildByName("listBoxEntities");

	buttonTiles = (UI::RadioButton*)GetChildByName("buttonTiles");
	buttonActors = (UI::RadioButton*)GetChildByName("buttonActors");

	_ListBoxTiles = (UI::ListBox*)GetChildByName("listBoxTiles");
	_ListBoxActors = (UI::ListBox*)GetChildByName("listBoxActors");

	buttonGrid = (UI::RadioButton*)GetChildByName("buttonGrid");
	buttonDialog = (UI::RadioButton*)GetChildByName("buttonDialog");

	_PropertyGrid = (UI::PropertyGrid*)GetChildByName("propertyGrid");
	_PropertyDialog = (UI::Panel*)GetChildByName("propertyDialog");
	_PropertyGridBinder->SetPropertyGrid(_PropertyGrid);

	// Events
	buttonNew->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);
	buttonLoad->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);
	buttonSave->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);

	buttonGameMode->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	buttonPause->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	buttonSettings->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	buttonEntities->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	buttonProperties->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	buttonExit->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);

	_ListBoxEntities->MouseClick += new EventMethodSlot<GameUI>(this,
		&GameUI::OnListBoxEntitiesMouseClick);
	_ListBoxEntities->MouseDoubleClick += new EventMethodSlot<GameUI>(this,
		&GameUI::OnListBoxEntitiesMouseDoubleClick);

	_ListBoxTiles = (UI::ListBox*)GetChildByName("listBoxTiles");
	_ListBoxTiles->MouseDoubleClick += new EventMethodSlot<GameUI>(this,
		&GameUI::OnListBoxTilesMouseDoubleClick);

	buttonTiles->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnTabPanelEditorPageChanged);
	buttonActors->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnTabPanelEditorPageChanged);

	_ListBoxActors = (UI::ListBox*)GetChildByName("listBoxActors");
	_ListBoxActors->MouseDoubleClick += new EventMethodSlot<GameUI>(this,
		&GameUI::OnListBoxActorsMouseDoubleClick);

	buttonGrid->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnTabPanelPropertiesPageChanged);
	buttonDialog->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnTabPanelPropertiesPageChanged);
}

bool GameUI::OnToolBarClick(Object* sender, const EventArgs& e)
{
	UI::Widget* widget = (UI::Widget*)sender;
	if (widget->GetName() == "buttonGameMode")
	{
		GameWorld* world = GameCore::Instance()->GetWorld();
		if (world->GetEditor())
		{
			world->NewGame();
			world->SetEditor(false);
		}
		else
		{
			world->EndGame();
			world->SetEditor(true);
		}
	}
	else if (widget->GetName() == "buttonPause")
	{
		GameWorld* world = GameCore::Instance()->GetWorld();
		world->SetPaused(!world->GetPaused());
	}
	else if (widget->GetName() == "buttonSettings")
	{
		static SettingsDialog* dialog = new SettingsDialog();
		dialog->SetPosition(PointInt(260, 220));
		dialog->Show(NULL);
	}
	else if (widget->GetName() == "buttonEntities")
	{
		UI::Panel* windowEntities = (UI::Panel*)GetChildByName("windowEntities");
		windowEntities->SetVisible(!windowEntities->GetVisible());
	}
	else if (widget->GetName() == "buttonProperties")
	{
		UI::Panel* windowProperties = (UI::Panel*)GetChildByName("windowProperties");
		windowProperties->SetVisible(!windowProperties->GetVisible());
	}
	else if (widget->GetName() == "buttonExit")
	{
		GameCore::Instance()->Exit();
	}

	return true;
}

bool GameUI::OnTabPanelEditorPageChanged(Object* sender, const EventArgs& e)
{
	UI::Widget* widget = (UI::Widget*)sender;

	UI::Panel* windowTiles = (UI::Panel*)GetChildByName("windowTiles");
	UI::Panel* windowActorDefs = (UI::Panel*)GetChildByName("windowActorDefs");

	windowTiles->SetVisible(false);
	windowActorDefs->SetVisible(false);

	if (widget->GetName() == "buttonTiles")
	{
		((UI::RadioButton*)GetChildByName("buttonActors"))->SetChecked(false);
		windowTiles->SetVisible(true);
		GameCore::Instance()->GetWorld()->SetEditorState(EditorState_Tiles);
	}
	else if (widget->GetName() == "buttonActors")
	{
		((UI::RadioButton*)GetChildByName("buttonTiles"))->SetChecked(false);
		windowActorDefs->SetVisible(true);
		GameCore::Instance()->GetWorld()->SetEditorState(EditorState_Actors);
	}

	return true;
}

bool GameUI::OnTabPanelPropertiesPageChanged(Object* sender, const EventArgs& e)
{
	UI::Widget* widget = (UI::Widget*)sender;

	UI::PropertyGrid* propertyGrid = (UI::PropertyGrid*)GetChildByName("propertyGrid");
	UI::Panel* propertyDialog = (UI::Panel*)GetChildByName("propertyDialog");

	propertyGrid->SetVisible(false);
	propertyDialog->SetVisible(false);

	if (widget->GetName() == "buttonGrid")
	{
		((UI::RadioButton*)GetChildByName("buttonDialog"))->SetChecked(false);
		propertyGrid->SetVisible(true);
	}
	else if (widget->GetName() == "buttonDialog")
	{
		((UI::RadioButton*)GetChildByName("buttonGrid"))->SetChecked(false);
		propertyDialog->SetVisible(true);
	}

	return true;
}

bool GameUI::OnToolBarEditorClick(Object* sender, const EventArgs& e)
{
	UI::Widget* widget = (UI::Widget*)sender;
	String fileName = FileSystem::Instance()->GetFullPath(
		Path::Combine("AI\\MGS\\levels", _ComboBoxLevels->GetSelectedText()));

	if (widget->GetName() == "buttonNew")
	{
		static LevelDialog* dialog = new LevelDialog();
		dialog->SetPosition(PointInt(260, 220));
		dialog->Show(NULL);
	}
	else if (widget->GetName() == "buttonLoad")
	{
		GameCore::Instance()->GetWorld()->LoadMap(fileName);
	}
	else if (widget->GetName() == "buttonSave")
	{
		GameCore::Instance()->GetWorld()->SaveMap(fileName);
	}

	return true;
}

bool GameUI::OnListBoxEntitiesMouseClick(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxEntities->GetSelectedItem();

	if (item != NULL)
	{
		Actor* actor = (Actor*)item->GetUserData().ToObject();
		GameCore::Instance()->GetWorld()->SetSelectedActor(actor);
		_PropertyGridBinder->SetSelectedObject(actor);
	}
	else
	{
		_PropertyGridBinder->SetSelectedObject(NULL);
	}

	return true;
}

bool GameUI::OnListBoxEntitiesMouseDoubleClick(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxEntities->GetSelectedItem();

	if (item != NULL)
	{
		GameCore::Instance()->GetWorld()->SetCursorOnSelectedActor();
	}

	return true;
}

bool GameUI::OnListBoxTilesMouseDoubleClick(Object* sender, const EventArgs& e)
{
	GameCore::Instance()->GetWorld()->OnToolBarTile();

	return true;
}

bool GameUI::OnListBoxActorsMouseDoubleClick(Object* sender, const EventArgs& e)
{
	GameCore::Instance()->GetWorld()->OnToolBarActor();

	return true;
}
