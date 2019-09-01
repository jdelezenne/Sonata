/*=============================================================================
SettingsDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SettingsDialog.h"
#include "SampleStealth.h"

SettingsDialog::SettingsDialog() :
	UI::Dialog()
{
	SetSize(SizeInt(220, 220));

	// Labels
	int top = 5;

	UI::Label* labelCellSize = new UI::Label();
	labelCellSize->SetPosition(PointInt(5, top+=20));
	labelCellSize->SetSize(SizeInt(80, 20));
	labelCellSize->SetText(_T("Cell Size:"));
	Children().Add(labelCellSize);

	UI::Label* labelDivisions = new UI::Label();
	labelDivisions->SetPosition(PointInt(5, top+=20));
	labelDivisions->SetSize(SizeInt(80, 20));
	labelDivisions->SetText(_T("Divisions:"));
	Children().Add(labelDivisions);

	UI::Label* labelBoundingBoxes = new UI::Label();
	labelBoundingBoxes->SetPosition(PointInt(5, top+=20));
	labelBoundingBoxes->SetSize(SizeInt(80, 20));
	labelBoundingBoxes->SetText(_T("Bounding Boxes:"));
	Children().Add(labelBoundingBoxes);

	// Controls
	top = 5;

	UI::TextBox* textBoxCellSize = new UI::TextBox();
	textBoxCellSize->SetName("textBoxCellSize");
	textBoxCellSize->SetPosition(PointInt(80, top+=20));
	textBoxCellSize->SetSize(SizeInt(80, 20));
	Children().Add(textBoxCellSize);

	UI::TextBox* textBoxDivisions = new UI::TextBox();
	textBoxDivisions->SetName("textBoxDivisions");
	textBoxDivisions->SetPosition(PointInt(80, top+=20));
	textBoxDivisions->SetSize(SizeInt(80, 20));
	Children().Add(textBoxDivisions);

	UI::CheckBoxButton* checkBoxBoundingBoxes = new UI::CheckBoxButton();
	checkBoxBoundingBoxes->SetName("checkBoxBoundingBoxes");
	checkBoxBoundingBoxes->SetPosition(PointInt(80, top+=20));
	checkBoxBoundingBoxes->SetSize(SizeInt(80, 20));
	Children().Add(checkBoxBoundingBoxes);

	// Buttons
	UI::PushButton* buttonOK = new UI::PushButton();
	buttonOK->SetName("buttonOK");
	buttonOK->SetPosition(PointInt(60, 180));
	buttonOK->SetSize(SizeInt(70, 20));
	buttonOK->SetText(_T("OK"));
	buttonOK->SetForegroundColor(Color32::Blue);
	buttonOK->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonOK);

	UI::PushButton* buttonCancel = new UI::PushButton();
	buttonCancel->SetName("buttonCancel");
	buttonCancel->SetPosition(PointInt(140, 180));
	buttonCancel->SetSize(SizeInt(70, 20));
	buttonCancel->SetText(_T("Cancel"));
	buttonCancel->SetForegroundColor(Color32::Blue);
	buttonCancel->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonCancel);

	// Objects
	textBoxCellSize = (UI::TextBox*)GetChildByName("textBoxCellSize");
	textBoxDivisions = (UI::TextBox*)GetChildByName("textBoxDivisions");
	checkBoxBoundingBoxes = (UI::CheckBoxButton*)GetChildByName("checkBoxBoundingBoxes");

	buttonOK = (UI::PushButton*)GetChildByName("buttonOK");
	buttonCancel = (UI::PushButton*)GetChildByName("buttonCancel");

	// Events
	buttonOK->Click += new EventMethodSlot<SettingsDialog>(this,
		&SettingsDialog::OnButtonOKClick);
	buttonCancel->Click += new EventMethodSlot<SettingsDialog>(this,
		&SettingsDialog::OnButtonCancelClick);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::Show(UI::Widget* owner)
{
	UI::Dialog::Show(owner);

	GameWorld* world = GameCore::Instance()->GetWorld();
	Map* map = world->GetMap();

	UI::TextBox* textBoxCellSize = (UI::TextBox*)GetChildByName("textBoxCellSize");
	UI::TextBox* textBoxDivisions = (UI::TextBox*)GetChildByName("textBoxDivisions");
	UI::CheckBoxButton* checkBoxBoundingBoxes = (UI::CheckBoxButton*)GetChildByName("checkBoxBoundingBoxes");

	textBoxCellSize->SetText(String::ToString(map->_CellSize));
	textBoxDivisions->SetText(String::ToString(map->_Divisions));
	checkBoxBoundingBoxes->SetChecked(world->GetShowBoundingBoxes());
}

bool SettingsDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_OK;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	GameWorld* world = GameCore::Instance()->GetWorld();
	Map* map = world->GetMap();

	UI::TextBox* textBoxCellSize = (UI::TextBox*)GetChildByName("textBoxCellSize");
	UI::TextBox* textBoxDivisions = (UI::TextBox*)GetChildByName("textBoxDivisions");
	UI::CheckBoxButton* checkBoxBoundingBoxes = (UI::CheckBoxButton*)GetChildByName("checkBoxBoundingBoxes");

	real32 cellSize = textBoxCellSize->GetText().ToReal32();
	int32 divisions = textBoxDivisions->GetText().ToInt32();
	world->SetShowBoundingBoxes(checkBoxBoundingBoxes->GetChecked());

	divisions = Math::Clamp(divisions, 1, 8);
	map->_CellSize = Math::Clamp(cellSize, 4.0f, 256.0f);

	if (map->_Divisions != divisions)
	{
		map->_Divisions = divisions;
		map->Initialize();
	}

	return true;
}

bool SettingsDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}
