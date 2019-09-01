/*=============================================================================
SettingsDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SettingsDialog.h"
#include "Procedural.h"

SettingsDialog::SettingsDialog() :
	UI::Dialog()
{
	SetSize(SizeInt(220, 220));

	// Labels
	int top = 5;

	UI::Label* labelImageSize = new UI::Label();
	labelImageSize->SetPosition(PointInt(5, top));
	labelImageSize->SetSize(SizeInt(80, 20));
	labelImageSize->SetText(_T("Image Size:"));
	Children().Add(labelImageSize);

	UI::Label* labelPreviewSize = new UI::Label();
	labelPreviewSize->SetPosition(PointInt(5, top+=20));
	labelPreviewSize->SetSize(SizeInt(80, 20));
	labelPreviewSize->SetText(_T("Preview Size:"));
	Children().Add(labelPreviewSize);

	// Controls
	top = 5;

	UI::TextBox* textBoxImageSizeW = new UI::TextBox();
	textBoxImageSizeW->SetName("textBoxImageSizeW");
	textBoxImageSizeW->SetPosition(PointInt(100, top));
	textBoxImageSizeW->SetSize(SizeInt(40, 20));
	Children().Add(textBoxImageSizeW);

	UI::TextBox* textBoxImageSizeH = new UI::TextBox();
	textBoxImageSizeH->SetName("textBoxImageSizeH");
	textBoxImageSizeH->SetPosition(PointInt(150, top));
	textBoxImageSizeH->SetSize(SizeInt(40, 20));
	Children().Add(textBoxImageSizeH);
	top += 20;

	UI::TextBox* textBoxPreviewSizeW = new UI::TextBox();
	textBoxPreviewSizeW->SetName("textBoxPreviewSizeW");
	textBoxPreviewSizeW->SetPosition(PointInt(100, top));
	textBoxPreviewSizeW->SetSize(SizeInt(40, 20));
	Children().Add(textBoxPreviewSizeW);

	UI::TextBox* textBoxPreviewSizeH = new UI::TextBox();
	textBoxPreviewSizeH->SetName("textBoxPreviewSizeH");
	textBoxPreviewSizeH->SetPosition(PointInt(150, top));
	textBoxPreviewSizeH->SetSize(SizeInt(40, 20));
	Children().Add(textBoxPreviewSizeH);
	top += 20;

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
	textBoxImageSizeW = (UI::TextBox*)GetChildByName("textBoxImageSizeW");
	textBoxImageSizeH = (UI::TextBox*)GetChildByName("textBoxImageSizeH");
	textBoxPreviewSizeW = (UI::TextBox*)GetChildByName("textBoxPreviewSizeW");
	textBoxPreviewSizeH = (UI::TextBox*)GetChildByName("textBoxPreviewSizeH");

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

	AppSettings* settings = AppCore::Instance()->Settings();

	UI::TextBox* textBoxImageSizeW = (UI::TextBox*)GetChildByName("textBoxImageSizeW");
	UI::TextBox* textBoxImageSizeH = (UI::TextBox*)GetChildByName("textBoxImageSizeH");
	UI::TextBox* textBoxPreviewSizeW = (UI::TextBox*)GetChildByName("textBoxPreviewSizeW");
	UI::TextBox* textBoxPreviewSizeH = (UI::TextBox*)GetChildByName("textBoxPreviewSizeH");

	textBoxImageSizeW->SetText(String::ToString(settings->_ImageSize.Width));
	textBoxImageSizeH->SetText(String::ToString(settings->_ImageSize.Height));
	textBoxPreviewSizeW->SetText(String::ToString(settings->_PreviewSize.Width));
	textBoxPreviewSizeH->SetText(String::ToString(settings->_PreviewSize.Height));
}

bool SettingsDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_OK;

	AppSettings* settings = AppCore::Instance()->Settings();

	UI::TextBox* textBoxImageSizeW = (UI::TextBox*)GetChildByName("textBoxImageSizeW");
	UI::TextBox* textBoxImageSizeH = (UI::TextBox*)GetChildByName("textBoxImageSizeH");
	UI::TextBox* textBoxPreviewSizeW = (UI::TextBox*)GetChildByName("textBoxPreviewSizeW");
	UI::TextBox* textBoxPreviewSizeH = (UI::TextBox*)GetChildByName("textBoxPreviewSizeH");

	settings->_ImageSize.Width = textBoxImageSizeW->GetText().ToInt32();
	settings->_ImageSize.Height = textBoxImageSizeH->GetText().ToInt32();
	settings->_PreviewSize.Width = textBoxPreviewSizeW->GetText().ToInt32();
	settings->_PreviewSize.Height = textBoxPreviewSizeH->GetText().ToInt32();

	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->DestroyWidget(this);

	return true;
}

bool SettingsDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;

	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->DestroyWidget(this);

	return true;
}
