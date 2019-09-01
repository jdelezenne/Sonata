/*=============================================================================
GameUI.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "GameUI.h"
#include "SampleAI.h"
#include "LevelDialog.h"

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
}

void GameUI::Destroy()
{
}

void GameUI::CreateUI()
{
	Tileset* tileset = GameCore::Instance()->GetWorld()->GetTileset();
	int tileCount = tileset->_Tiles.Count();
	int unitCount = tileset->_UnitDefs.Count();

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
		panelInfo->SetVisible(false);

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

			UI::PushButton* buttonEntities = new UI::PushButton();
			buttonEntities->SetName("buttonEntities");
			buttonEntities->SetPosition(PointInt(170, 10));
			buttonEntities->SetSize(SizeInt(70, 20));
			buttonEntities->SetText(_T("Entities"));
			buttonEntities->SetForegroundColor(Color32::Blue);
			buttonEntities->SetBackgroundColor(Color32::Gray);
			toolBar->Children().Add(buttonEntities);

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

		UI::Panel* windowEntities = new UI::Panel();
		windowEntities->SetName(_T("windowEntities"));
		windowEntities->SetMovable(true);
		windowEntities->SetPosition(PointInt(640, 60));
		windowEntities->SetSize(SizeInt(140, 180));
		windowEntities->SetHasBackground(true);
		windowEntities->SetBackgroundColor(BackColor);
		windowEntities->SetVisible(false);

			UI::Label* labelEntities = new UI::Label();
			labelEntities->SetPosition(PointInt(10, 5));
			labelEntities->SetSize(SizeInt(80, 20));
			labelEntities->SetText(_T("Entities:"));
			labelEntities->SetForegroundColor(Color32::Blue);
			labelEntities->SetHasBackground(false);
			labelEntities->SetTextAlignment(UI::Alignment_MiddleCenter);
			windowEntities->Children().Add(labelEntities);

			UI::ListBox* listBoxEntities = new UI::ListBox();
			listBoxEntities->SetName(_T("listBoxEntities"));
			listBoxEntities->SetHasFrame(true);
			listBoxEntities->SetFrameSize(1);
			listBoxEntities->SetFrameColor(Color32::Blue);
			listBoxEntities->SetPosition(PointInt(10, 20));
			listBoxEntities->SetSize(SizeInt(120, 150));
			listBoxEntities->SetBackgroundColor(Color32::White);
			windowEntities->Children().Add(listBoxEntities);

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

			UI::PropertyGrid* propertyGrid = new UI::PropertyGrid();
			propertyGrid->SetName("propertyGrid");
			propertyGrid->SetPosition(PointInt(5, 20));
			propertyGrid->SetSize(SizeInt(250, 310));
			propertyGrid->SetBackgroundColor(BackColor);

		windowProperties->Children().Add(labelGrid);
		windowProperties->Children().Add(propertyGrid);

		UI::Panel* windowTiles = new UI::Panel();
		windowTiles->SetName("windowTiles");
		windowTiles->SetMovable(true);
		windowTiles->SetPosition(PointInt(5, 120));
		windowTiles->SetSize(SizeInt(180, 310));
		windowTiles->SetHasBackground(true);
		windowTiles->SetBackgroundColor(BackColor);

			UI::Label* labelTiles = new UI::Label();
			labelTiles->SetPosition(PointInt(10, 10));
			labelTiles->SetSize(SizeInt(100, 30));
			labelTiles->SetText(_T("Tiles:"));
			labelTiles->SetForegroundColor(Color32::Red);
			labelTiles->SetHasBackground(false);
			windowTiles->Children().Add(labelTiles);

			int tileSize = 40;
			int tileWidth = (windowTiles->GetWidth() - 10);
			int tileX = 10;
			int tileY = 20;
			for (int i=0; i<tileCount; i++)
			{
				Tile* tile = tileset->_Tiles[i];
				UI::StaticImage* image = new UI::StaticImage();
				image->SetName("tile" + String::ToString(i));
				image->SetPosition(PointInt(tileX, tileY));
				image->SetSize(SizeInt(tileSize, tileSize));
				image->SetImage(tile->_Sprite->GetSpriteFrame(0)->GetTexture());
				image->SetUserData(tile);
				windowTiles->Children().Add(image);

				if (tileX + tileSize < tileWidth)
				{
					tileX += tileSize;
				}
				else
				{
					tileX = 10;
					tileY += tileSize;
				}
			}

		UI::Panel* windowUnitDefs = new UI::Panel();
		windowUnitDefs->SetName("windowUnitDefs");
		windowUnitDefs->SetMovable(true);
		windowUnitDefs->SetPosition(PointInt(5, 440));
		windowUnitDefs->SetSize(SizeInt(180, 210));
		windowUnitDefs->SetHasBackground(true);
		windowUnitDefs->SetBackgroundColor(BackColor);

			UI::Label* labelUnitDef = new UI::Label();
			labelUnitDef->SetPosition(PointInt(10, 10));
			labelUnitDef->SetSize(SizeInt(100, 30));
			labelUnitDef->SetText(_T("Units:"));
			labelUnitDef->SetForegroundColor(Color32::Red);
			labelUnitDef->SetHasBackground(false);
			windowUnitDefs->Children().Add(labelUnitDef);

			int unitSize = 40;
			int unitWidth = (windowUnitDefs->GetWidth() - 10);
			int unitX = 10;
			int unitY = 20;
			for (int i=0; i<unitCount; i++)
			{
				UnitDef* unit = tileset->_UnitDefs[i];
				UI::StaticImage* image = new UI::StaticImage();
				image->SetName("unit" + String::ToString(i));
				image->SetPosition(PointInt(unitX, unitY));
				image->SetSize(SizeInt(unitSize, unitSize));
				image->SetSprite(unit->_Sprite);
				image->SetUseSprite(true);

				image->SetUserData(unit);
				windowUnitDefs->Children().Add(image);

				if (unitX + unitSize < unitWidth)
				{
					unitX += unitSize;
				}
				else
				{
					unitX = 10;
					unitY += unitSize;
				}
			}

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
	_Screen->Children().Add(windowTiles);
	_Screen->Children().Add(windowUnitDefs);
	_Screen->Children().Add(toolBarLevel);

#else
	File* file = new File("GameUI.Xml");
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
	_ButtonNew = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonNew");
	_ButtonLoad = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonLoad");
	_ButtonSave = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonSave");

	_ButtonPause = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonPause");
	_ButtonSettings = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonSettings");
	_ButtonEntities = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonEntities");
	_ButtonProperties = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonProperties");
	_ButtonExit = (UI::PushButton*)UI::UISystem::Instance()->GetWidgetByName("buttonExit");

	_ListBoxEntities = (UI::ListBox*)UI::UISystem::Instance()->GetWidgetByName("listBoxEntities");

	_PropertyGrid = (UI::PropertyGrid*)UI::UISystem::Instance()->GetWidgetByName("propertyGrid");
	_PropertyGridBinder->SetPropertyGrid(_PropertyGrid);

	// Events
	_ButtonNew->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);
	_ButtonLoad->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);
	_ButtonSave->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarEditorClick);

	_ButtonPause->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	_ButtonSettings->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	_ButtonEntities->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	_ButtonProperties->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);
	_ButtonExit->Click += new EventMethodSlot<GameUI>(this,
		&GameUI::OnToolBarClick);

	_ListBoxEntities->SelectedItemChanged += new EventMethodSlot<GameUI>(this,
		&GameUI::OnListBoxEntitiesSelectedItemChanged);

	for (int i=0; i<tileCount; i++)
	{
		UI::StaticImage* image = (UI::StaticImage*)UI::UISystem::Instance()->GetWidgetByName(
			"tile" + String::ToString(i));
		image->MouseClick += new EventMethodSlot<GameUI>(this,
			&GameUI::OnTileMouseClick);
	}

	for (int i=0; i<unitCount; i++)
	{
		UI::StaticImage* image = (UI::StaticImage*)UI::UISystem::Instance()->GetWidgetByName(
			"unit" + String::ToString(i));
		image->MouseClick += new EventMethodSlot<GameUI>(this,
			&GameUI::OnUnitMouseClick);
	}
}

void GameUI::SetEditor(bool value)
{
		UI::Panel* windowTiles = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("windowTiles");
		windowTiles->SetVisible(value);

		UI::Panel* toolBarLevel = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("toolBarLevel");
		toolBarLevel->SetVisible(value);

		UI::Panel* windowUnitDefs = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("windowUnitDefs");
		windowUnitDefs->SetVisible(value);
}

bool GameUI::OnToolBarClick(Object* sender, const EventArgs& e)
{
	if (sender == _ButtonPause)
	{
		GameWorld* world = GameCore::Instance()->GetWorld();
		world->SetPaused(!world->GetPaused());
	}
	else if (sender == _ButtonSettings)
	{
	}
	else if (sender == _ButtonEntities)
	{
		UI::Panel* windowEntities = (UI::Panel*)UI::UISystem::Instance()->GetWidgetByName("windowEntities");
		windowEntities->SetVisible(!windowEntities->GetVisible());
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

bool GameUI::OnToolBarEditorClick(Object* sender, const EventArgs& e)
{
	if (sender == _ButtonNew)
	{
		static LevelDialog* dialog = new LevelDialog();
		dialog->SetPosition(PointInt(260, 220));
		dialog->Show(NULL);
	}
	else if (sender == _ButtonLoad)
	{
		String map = FileSystem::Instance()->GetFullPath("AI\\AW\\AWMap.Xml");
		GameCore::Instance()->GetWorld()->LoadMap(map);
	}
	else if (sender == _ButtonSave)
	{
		String map = FileSystem::Instance()->GetFullPath("AI\\AW\\AWMap.Xml");
		GameCore::Instance()->GetWorld()->SaveMap(map);
	}

	return true;
}

bool GameUI::OnListBoxEntitiesSelectedItemChanged(Object* sender, const EventArgs& e)
{
	UI::ListItem* item = _ListBoxEntities->GetSelectedItem();

	if (item != NULL)
	{
		GameEntity* entity = (GameEntity*)item->GetUserData().ToObject();

		_PropertyGridBinder->SetSelectedObject(entity);

		entity->GetGameWorld()->SetControlledEntity(entity);
	}
	else
	{
		_PropertyGridBinder->SetSelectedObject(NULL);
	}

	return true;
}

bool GameUI::OnTileMouseClick(Object* sender, const EventArgs& e)
{
	GameWorld* world = GameCore::Instance()->GetWorld();
	Map* map = world->GetMap();
	PointInt cursor = world->GetCursor();

	Tile* tile = (Tile*)((UI::Widget*)sender)->GetUserData().ToObject();
	map->SetCellTileAt(tile, cursor.X, cursor.Y);

	return true;
}

bool GameUI::OnUnitMouseClick(Object* sender, const EventArgs& e)
{
	GameWorld* world = GameCore::Instance()->GetWorld();
	Map* map = world->GetMap();
	PointInt cursor = world->GetCursor();

	UnitDef* unitdef = (UnitDef*)((UI::Widget*)sender)->GetUserData().ToObject();

	Unit* unit = new Unit();
	unit->_UnitDef = unitdef;
	unit->_X = cursor.X;
	unit->_Y = cursor.Y;
	map->AddUnit(unit);

	return true;
}

void GameUI::AddEntity(GameEntity* entity)
{
	UI::ListItem* item = new UI::ListItem(entity->GetName());
	item->SetUserData(entity);
	_ListBoxEntities->Items().Add(item);
}

void GameUI::RemoveEntity(GameEntity* entity)
{
}
