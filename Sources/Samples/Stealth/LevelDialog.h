/*=============================================================================
LevelDialog.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_LEVELDIALOG_H_
#define _SE_LEVELDIALOG_H_

#include "Common.h"

class LevelDialog : public UI::Dialog
{
public:
	LevelDialog();
	virtual ~LevelDialog();

	virtual void Show(UI::Widget* owner);

protected:
	virtual bool OnButtonOKClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonCancelClick(Object* sender, const EventArgs& e);

protected:
	UI::PushButton* _ButtonOK;
	UI::PushButton* _ButtonCancel;
	UI::TextBox* _TextBoxX;
	UI::TextBox* _TextBoxY;
	UI::TextBox* _TextBoxDepth;
};

#endif 
