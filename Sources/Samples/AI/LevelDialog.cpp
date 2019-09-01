/*=============================================================================
LevelDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "LevelDialog.h"
#include "SampleAI.h"

LevelDialog::LevelDialog() :
	UI::Dialog()
{
	SetSize(SizeInt(220, 160));

	// Labels
	int top = 5;

	UI::Label* labelShape = new UI::Label();
	labelShape->SetPosition(PointInt(5, top+=20));
	labelShape->SetSize(SizeInt(80, 20));
	labelShape->SetText(_T("X:"));
	Children().Add(labelShape);

	UI::Label* labelX = new UI::Label();
	labelX->SetPosition(PointInt(5, top+=20));
	labelX->SetSize(SizeInt(80, 20));
	labelX->SetText(_T("Y:"));
	Children().Add(labelX);

	// Controls
	top = 5;

	UI::TextBox* textBoxX = new UI::TextBox();
	textBoxX->SetName("textBoxX");
	textBoxX->SetPosition(PointInt(80, top+=20));
	textBoxX->SetSize(SizeInt(80, 20));
	textBoxX->SetText("10");
	Children().Add(textBoxX);

	UI::TextBox* textBoxY = new UI::TextBox();
	textBoxY->SetName("textBoxY");
	textBoxY->SetPosition(PointInt(80, top+=20));
	textBoxY->SetSize(SizeInt(80, 20));
	textBoxY->SetText("10");
	Children().Add(textBoxY);

	// Buttons
	UI::PushButton* buttonOK = new UI::PushButton();
	buttonOK->SetName("buttonOK");
	buttonOK->SetPosition(PointInt(60, 130));
	buttonOK->SetSize(SizeInt(70, 20));
	buttonOK->SetText(_T("OK"));
	buttonOK->SetForegroundColor(Color32::Blue);
	buttonOK->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonOK);

	UI::PushButton* buttonCancel = new UI::PushButton();
	buttonCancel->SetName("buttonCancel");
	buttonCancel->SetPosition(PointInt(140, 130));
	buttonCancel->SetSize(SizeInt(70, 20));
	buttonCancel->SetText(_T("Cancel"));
	buttonCancel->SetForegroundColor(Color32::Blue);
	buttonCancel->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonCancel);

	// Objects
	_TextBoxX = (UI::TextBox*)GetChildByName("textBoxX");
	_TextBoxY = (UI::TextBox*)GetChildByName("textBoxY");

	_ButtonOK = (UI::PushButton*)GetChildByName("buttonOK");
	_ButtonCancel = (UI::PushButton*)GetChildByName("buttonCancel");

	// Events
	_ButtonOK->Click += new EventMethodSlot<LevelDialog>(this,
		&LevelDialog::OnButtonOKClick);
	_ButtonCancel->Click += new EventMethodSlot<LevelDialog>(this,
		&LevelDialog::OnButtonCancelClick);
}

LevelDialog::~LevelDialog()
{
}

void LevelDialog::Show(Widget* owner)
{
	UI::Dialog::Show(owner);
}

bool LevelDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_OK;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	int32 x = _TextBoxX->GetText().ToInt32();
	int32 y = _TextBoxY->GetText().ToInt32();

	GameCore::Instance()->GetWorld()->NewMap(x, y);

	return true;
}

bool LevelDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}
