/*=============================================================================
ShapeDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "ShapeDialog.h"
#include "SamplePhysics.h"
#include "Simulation.h"

ShapeDialog::ShapeDialog() :
	Dialog()
{
	SetSize(SizeInt(220, 160));

	// Labels
	int top = 5;

	UI::Label* labelShape = new UI::Label();
	labelShape->SetPosition(PointInt(5, top+=20));
	labelShape->SetSize(SizeInt(80, 20));
	labelShape->SetText(_T("Shape:"));
	Children().Add(labelShape);

	UI::Label* labelMass = new UI::Label();
	labelMass->SetPosition(PointInt(5, top+=20));
	labelMass->SetSize(SizeInt(80, 20));
	labelMass->SetText(_T("Mass:"));
	Children().Add(labelMass);

	UI::Label* labelDensity = new UI::Label();
	labelDensity->SetPosition(PointInt(5, top+=20));
	labelDensity->SetSize(SizeInt(80, 20));
	labelDensity->SetText(_T("Density:"));
	Children().Add(labelDensity);

	UI::Label* labelRadius = new UI::Label();
	labelRadius->SetPosition(PointInt(5, top+=20));
	labelRadius->SetSize(SizeInt(80, 20));
	labelRadius->SetText(_T("Radius:"));
	Children().Add(labelRadius);

	UI::Label* labelExtents = new UI::Label();
	labelExtents->SetPosition(PointInt(5, top+=20));
	labelExtents->SetSize(SizeInt(80, 20));
	labelExtents->SetText(_T("Extents:"));
	Children().Add(labelExtents);

	/*UI::Label* labelMaterial = new UI::Label();
	labelMaterial->SetPosition(PointInt(5, top+=20));
	labelMaterial->SetSize(SizeInt(80, 20));
	labelMaterial->SetText(_T("Material:"));
	Children().Add(labelMaterial);*/

	// Controls
	top = 5;

	UI::ComboBox* comboBoxShape = new UI::ComboBox();
	comboBoxShape->SetName("comboBoxShape");
	comboBoxShape->SetPosition(PointInt(80, top+=20));
	comboBoxShape->SetSize(SizeInt(80, 20));
	//comboBoxShape->Items().Add(new UI::ListItem(_T("Plane")));
	comboBoxShape->Items().Add(new UI::ListItem(_T("Sphere")));
	comboBoxShape->Items().Add(new UI::ListItem(_T("Box")));
	comboBoxShape->SetSelectedIndex(0);
	Children().Add(comboBoxShape);

	UI::TextBox* textBoxMass = new UI::TextBox();
	textBoxMass->SetName("textBoxMass");
	textBoxMass->SetPosition(PointInt(80, top+=20));
	textBoxMass->SetSize(SizeInt(80, 20));
	textBoxMass->SetText("0.0");
	Children().Add(textBoxMass);

	UI::TextBox* textBoxDensity = new UI::TextBox();
	textBoxDensity->SetName("textBoxDensity");
	textBoxDensity->SetPosition(PointInt(80, top+=20));
	textBoxDensity->SetSize(SizeInt(80, 20));
	textBoxDensity->SetText("1.0");
	Children().Add(textBoxDensity);

	UI::TextBox* textBoxRadius = new UI::TextBox();
	textBoxRadius->SetName("textBoxRadius");
	textBoxRadius->SetPosition(PointInt(80, top+=20));
	textBoxRadius->SetSize(SizeInt(80, 20));
	textBoxRadius->SetText("1.0");
	Children().Add(textBoxRadius);

	UI::TextBox* textBoxExtentsX = new UI::TextBox();
	textBoxExtentsX->SetName("textBoxExtentsX");
	textBoxExtentsX->SetPosition(PointInt(80, top+=20));
	textBoxExtentsX->SetSize(SizeInt(30, 20));
	textBoxExtentsX->SetText(_T("1.0"));
	Children().Add(textBoxExtentsX);
	UI::TextBox* textBoxExtentsY = new UI::TextBox();
	textBoxExtentsY->SetName("textBoxExtentsY");
	textBoxExtentsY->SetPosition(PointInt(110, top));
	textBoxExtentsY->SetSize(SizeInt(30, 20));
	textBoxExtentsY->SetText(_T("1.0"));
	Children().Add(textBoxExtentsY);
	UI::TextBox* textBoxExtentsZ = new UI::TextBox();
	textBoxExtentsZ->SetName("textBoxExtentsZ");
	textBoxExtentsZ->SetPosition(PointInt(140, top));
	textBoxExtentsZ->SetSize(SizeInt(30, 20));
	textBoxExtentsZ->SetText(_T("1.0"));
	Children().Add(textBoxExtentsZ);

	/*UI::ComboBox* comboBoxMaterial = new UI::ComboBox();
	comboBoxMaterial->SetName("comboBoxMaterial");
	comboBoxMaterial->SetPosition(PointInt(80, top+=20));
	comboBoxMaterial->SetSize(SizeInt(80, 20));
	Children().Add(comboBoxMaterial);*/

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
	_ComboBoxShape = (UI::ComboBox*)GetChildByName("comboBoxShape");
	_TextBoxMass = (UI::TextBox*)GetChildByName("textBoxMass");
	_TextBoxDensity = (UI::TextBox*)GetChildByName("textBoxDensity");
	_TextBoxRadius = (UI::TextBox*)GetChildByName("textBoxRadius");
	_TextBoxExtentsX = (UI::TextBox*)GetChildByName("textBoxExtentsX");
	_TextBoxExtentsY = (UI::TextBox*)GetChildByName("textBoxExtentsY");
	_TextBoxExtentsZ = (UI::TextBox*)GetChildByName("textBoxExtentsZ");
	_ComboBoxMaterial = (UI::ComboBox*)GetChildByName("comboBoxMaterial");

	_ButtonOK = (UI::PushButton*)GetChildByName("buttonOK");
	_ButtonCancel = (UI::PushButton*)GetChildByName("buttonCancel");

	// Events
	_ButtonOK->Click += new EventMethodSlot<ShapeDialog>(this,
		&ShapeDialog::OnButtonOKClick);
	_ButtonCancel->Click += new EventMethodSlot<ShapeDialog>(this,
		&ShapeDialog::OnButtonCancelClick);
}

ShapeDialog::~ShapeDialog()
{
}

void ShapeDialog::Show(Widget* owner)
{
	UI::Dialog::Show(owner);

	/*_ComboBoxMaterial->Items().Clear();

	Physics::IScene* scene = GameCore::Instance()->GetWorld()->GetPhysicsScene();
	int count = scene->GetMaterialCount();
	for (int i=0; i<count; i++)
	{
		Physics::IMaterial* material = scene->GetMaterial(i);

		_ComboBoxMaterial->Items().Add(new UI::ListItem("Material " + String::ToString(i)));
	}*/
}

Physics::ShapeType ShapeDialog::GetShape() const
{
	return (Physics::ShapeType)(_ComboBoxShape->GetSelectedIndex()+1);
}

real ShapeDialog::GetMass() const
{
	return _TextBoxMass->GetText().ToReal32();
}

real ShapeDialog::GetDensity() const
{
	return _TextBoxDensity->GetText().ToReal32();
}

Plane ShapeDialog::GetPlane() const
{
	Plane plane;

	return plane;
}

BoundingSphere ShapeDialog::GetSphere() const
{
	BoundingSphere sphere;

	real radius = _TextBoxRadius->GetText().ToReal32();

	sphere.Radius = radius;

	return sphere;
}

OBB ShapeDialog::GetBox() const
{
	OBB box;

	Vector3 extents;
	extents.X = _TextBoxExtentsX->GetText().ToReal32();
	extents.Y = _TextBoxExtentsY->GetText().ToReal32();
	extents.Z = _TextBoxExtentsZ->GetText().ToReal32();

	box.Extents = extents;

	return box;
}

bool ShapeDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_OK;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}

bool ShapeDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}
