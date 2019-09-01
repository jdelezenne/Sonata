/*=============================================================================
BodyDialog.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_BODYDIALOG_H_
#define _SE_BODYDIALOG_H_

#include "Common.h"
#include "ShapeDialog.h"

class BodyDialog : public UI::Dialog
{
public:
	BodyDialog();
	virtual ~BodyDialog();

	virtual void Show(Widget* owner);

protected:
	virtual bool OnButtonOKClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonCancelClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonShapeClick(Object* sender, const EventArgs& e);
	virtual bool OnTextBoxColorTextChanged(Object* sender, const EventArgs& e);

protected:
	ShapeDialog* _ShapeDialog;

	UI::PushButton* _ButtonOK;
	UI::PushButton* _ButtonCancel;
	UI::TextBox* _TextBoxName;
	UI::PushButton* _ButtonShape;
	UI::TextBox* _TextBoxPosX;
	UI::TextBox* _TextBoxPosY;
	UI::TextBox* _TextBoxPosZ;
	UI::TextBox* _TextBoxRotX;
	UI::TextBox* _TextBoxRotY;
	UI::TextBox* _TextBoxRotZ;
	UI::TextBox* _TextBoxLinVelX;
	UI::TextBox* _TextBoxLinVelY;
	UI::TextBox* _TextBoxLinVelZ;
	UI::TextBox* _TextBoxAngVelX;
	UI::TextBox* _TextBoxAngVelY;
	UI::TextBox* _TextBoxAngVelZ;
	UI::TextBox* _TextBoxColorR;
	UI::TextBox* _TextBoxColorG;
	UI::TextBox* _TextBoxColorB;
	UI::Label* _PreviewColor;
};

#endif 
