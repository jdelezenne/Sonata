/*=============================================================================
SettingsDialog.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_SETTINGSDIALOG_H_
#define _PROCEDURAL_SETTINGSDIALOG_H_

#include "Common.h"

class SettingsDialog : public UI::Dialog
{
public:
	SettingsDialog();
	virtual ~SettingsDialog();

	virtual void Show(UI::Widget* owner);

protected:
	virtual bool OnButtonOKClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonCancelClick(Object* sender, const EventArgs& e);
};

#endif 
