/*=============================================================================
MainUI.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_MAINUI_H_
#define _PROCEDURAL_MAINUI_H_

#include "Common.h"
#include "Workflow.h"
#include "DiagramController.h"

class MainUI
{
public:
	MainUI();
	virtual ~MainUI();

	void Create();
	void Destroy();

	void Initialize();

	void RegisterTableEvents(UI::TableNode* table);

	UI::Widget* GetChildByName(const String& name) const;

	void SetSelectedOperator(Operator* op);

protected:
	void CreateUI();

	bool OnToolBarClick(Object* sender, const EventArgs& e);
	bool OnListBoxCategoryMouseClick(Object* sender, const EventArgs& e);
	bool OnListBoxToolBoxMouseClick(Object* sender, const EventArgs& e);
	bool OnListBoxToolBoxMouseDoubleClick(Object* sender, const EventArgs& e);
	bool OnPropertyGridPropertyValueChanged(Object* sender, const UI::PropertyChangedEventArgs& e);
	bool OnDiagramMouseDown(Object* sender, const UI::MouseEventArgs& e);
	bool OnTableMouseDown(Object* sender, const UI::MouseEventArgs& e);
	bool OnTableMouseDoubleClick(Object* sender, const EventArgs& e);
	bool OnNodeMouseDown(Object* sender, const UI::MouseEventArgs& e);

	bool OnFileOpenDialog(Object* sender, const EventArgs& e);
	bool OnFileSaveDialog(Object* sender, const EventArgs& e);
	bool OnFileExportDialog(Object* sender, const EventArgs& e);
	bool OnFileImportDialog(Object* sender, const EventArgs& e);

protected:
	UI::Widget* _Screen;

	UI::ListBox* _ListBoxCategory;
	UI::ListBox* _ListBoxToolBox;

	UI::PropertyGrid* _PropertyGrid;
	UI::PropertyGridBinder* _PropertyGridBinder;

	DiagramController* _DiagramController;
};

#endif 
