/*=============================================================================
SettingsDialog.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_SETTINGSDIALOG_H_
#define _SE_SETTINGSDIALOG_H_

#include "Common.h"
#include "ShapeDialog.h"

class SettingsDialog : public UI::Dialog
{
public:
	SettingsDialog();
	virtual ~SettingsDialog();

	virtual void Show(Widget* owner);

protected:
	virtual bool OnButtonOKClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonCancelClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonShapeClick(Object* sender, const EventArgs& e);

protected:
	ShapeDialog* _ShapeDialog;

	UI::PushButton* _ButtonOK;
	UI::PushButton* _ButtonCancel;
	UI::ComboBox* _ComboBoxTimeStep;
	UI::TextBox* _TextBoxTimeStep;
	UI::CheckBoxButton* _CheckBoxBroadPhase;
	UI::CheckBoxButton* _CheckBoxCollisionDetection;
	UI::TextBox* _TextBoxGravityX;
	UI::TextBox* _TextBoxGravityY;
	UI::TextBox* _TextBoxGravityZ;
	UI::ComboBox* _ComboBoxIntegration;
	UI::PushButton* _ButtonShape;
};

#endif 
