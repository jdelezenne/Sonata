/*=============================================================================
ShapeDialog.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_SHAPEDIALOG_H_
#define _SE_SHAPEDIALOG_H_

#include "Common.h"

class ShapeDialog : public UI::Dialog
{
public:
	ShapeDialog();
	virtual ~ShapeDialog();

	virtual void Show(Widget* owner);

	Physics::ShapeType GetShape() const;
	real GetMass() const;
	real GetDensity() const;
	Plane GetPlane() const;
	BoundingSphere GetSphere() const;
	OBB GetBox() const;

protected:
	virtual bool OnButtonOKClick(Object* sender, const EventArgs& e);
	virtual bool OnButtonCancelClick(Object* sender, const EventArgs& e);

protected:
	UI::PushButton* _ButtonOK;
	UI::PushButton* _ButtonCancel;
	UI::ComboBox* _ComboBoxShape;
	UI::TextBox* _TextBoxMass;
	UI::TextBox* _TextBoxDensity;
	UI::TextBox* _TextBoxRadius;
	UI::TextBox* _TextBoxExtentsX;
	UI::TextBox* _TextBoxExtentsY;
	UI::TextBox* _TextBoxExtentsZ;
	UI::ComboBox* _ComboBoxMaterial;
};

#endif 
