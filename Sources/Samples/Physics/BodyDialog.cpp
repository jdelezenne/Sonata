/*=============================================================================
BodyDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "BodyDialog.h"
#include "SamplePhysics.h"
#include "Simulation.h"

BodyDialog::BodyDialog() :
	Dialog()
{
	SetSize(SizeInt(220, 240));

	_ShapeDialog = new ShapeDialog();

	// Labels
	int top = 5;

	UI::Label* labelName = new UI::Label();
	labelName->SetPosition(PointInt(5, top+=20));
	labelName->SetSize(SizeInt(80, 20));
	labelName->SetText(_T("Name:"));
	Children().Add(labelName);

	UI::Label* labelShape = new UI::Label();
	labelShape->SetPosition(PointInt(5, top+=20));
	labelShape->SetSize(SizeInt(80, 20));
	labelShape->SetText(_T("Shape:"));
	Children().Add(labelShape);

	UI::Label* labelPosition = new UI::Label();
	labelPosition->SetPosition(PointInt(5, top+=20));
	labelPosition->SetSize(SizeInt(80, 20));
	labelPosition->SetText(_T("Position:"));
	Children().Add(labelPosition);

	UI::Label* labelOrientation = new UI::Label();
	labelOrientation->SetPosition(PointInt(5, top+=20));
	labelOrientation->SetSize(SizeInt(80, 20));
	labelOrientation->SetText(_T("Orientation:"));
	Children().Add(labelOrientation);

	UI::Label* labelLinearVelocity = new UI::Label();
	labelLinearVelocity->SetPosition(PointInt(5, top+=20));
	labelLinearVelocity->SetSize(SizeInt(80, 20));
	labelLinearVelocity->SetText(_T("Linear Velocity:"));
	Children().Add(labelLinearVelocity);

	UI::Label* labelAngularVelocity = new UI::Label();
	labelAngularVelocity->SetPosition(PointInt(5, top+=20));
	labelAngularVelocity->SetSize(SizeInt(80, 20));
	labelAngularVelocity->SetText(_T("Angular Velocity:"));
	Children().Add(labelAngularVelocity);

	UI::Label* labelColor = new UI::Label();
	labelColor->SetPosition(PointInt(5, top+=20));
	labelColor->SetSize(SizeInt(80, 20));
	labelColor->SetText(_T("Color:"));
	Children().Add(labelColor);

	// Controls
	top = 5;

	UI::TextBox* textBoxName = new UI::TextBox();
	textBoxName->SetName("textBoxName");
	textBoxName->SetPosition(PointInt(80, top+=20));
	textBoxName->SetSize(SizeInt(80, 20));
	Children().Add(textBoxName);

	UI::PushButton* buttonShape = new UI::PushButton();
	buttonShape->SetName("buttonShape");
	buttonShape->SetPosition(PointInt(80, top+=20));
	buttonShape->SetSize(SizeInt(40, 20));
	buttonShape->SetText(_T("Edit..."));
	buttonShape->SetForegroundColor(Color32::Blue);
	buttonShape->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonShape);

	UI::TextBox* textBoxPosX = new UI::TextBox();
	textBoxPosX->SetName("textBoxPosX");
	textBoxPosX->SetPosition(PointInt(80, top+=20));
	textBoxPosX->SetSize(SizeInt(30, 20));
	textBoxPosX->SetText(_T("0.0"));
	Children().Add(textBoxPosX);
	UI::TextBox* textBoxPosY = new UI::TextBox();
	textBoxPosY->SetName("textBoxPosY");
	textBoxPosY->SetPosition(PointInt(110, top));
	textBoxPosY->SetSize(SizeInt(30, 20));
	textBoxPosY->SetText(_T("10.0"));
	Children().Add(textBoxPosY);
	UI::TextBox* textBoxPosZ = new UI::TextBox();
	textBoxPosZ->SetName("textBoxPosZ");
	textBoxPosZ->SetPosition(PointInt(140, top));
	textBoxPosZ->SetSize(SizeInt(30, 20));
	textBoxPosZ->SetText(_T("0.0"));
	Children().Add(textBoxPosZ);

	UI::TextBox* textBoxRotX = new UI::TextBox();
	textBoxRotX->SetName("textBoxRotX");
	textBoxRotX->SetPosition(PointInt(80, top+=20));
	textBoxRotX->SetSize(SizeInt(30, 20));
	textBoxRotX->SetText(_T("0.0"));
	Children().Add(textBoxRotX);
	UI::TextBox* textBoxRotY = new UI::TextBox();
	textBoxRotY->SetName("textBoxRotY");
	textBoxRotY->SetPosition(PointInt(110, top));
	textBoxRotY->SetSize(SizeInt(30, 20));
	textBoxRotY->SetText(_T("0.0"));
	Children().Add(textBoxRotY);
	UI::TextBox* textBoxRotZ = new UI::TextBox();
	textBoxRotZ->SetName("textBoxRotZ");
	textBoxRotZ->SetPosition(PointInt(140, top));
	textBoxRotZ->SetSize(SizeInt(30, 20));
	textBoxRotZ->SetText(_T("0.0"));
	Children().Add(textBoxRotZ);

	UI::TextBox* textBoxLinVelX = new UI::TextBox();
	textBoxLinVelX->SetName("textBoxLinVelX");
	textBoxLinVelX->SetPosition(PointInt(80, top+=20));
	textBoxLinVelX->SetSize(SizeInt(30, 20));
	textBoxLinVelX->SetText(_T("0.0"));
	Children().Add(textBoxLinVelX);
	UI::TextBox* textBoxLinVelY = new UI::TextBox();
	textBoxLinVelY->SetName("textBoxLinVelY");
	textBoxLinVelY->SetPosition(PointInt(110, top));
	textBoxLinVelY->SetSize(SizeInt(30, 20));
	textBoxLinVelY->SetText(_T("0.0"));
	Children().Add(textBoxLinVelY);
	UI::TextBox* textBoxLinVelZ = new UI::TextBox();
	textBoxLinVelZ->SetName("textBoxLinVelZ");
	textBoxLinVelZ->SetPosition(PointInt(140, top));
	textBoxLinVelZ->SetSize(SizeInt(30, 20));
	textBoxLinVelZ->SetText(_T("0.0"));
	Children().Add(textBoxLinVelZ);

	UI::TextBox* textBoxAngVelX = new UI::TextBox();
	textBoxAngVelX->SetName("textBoxAngVelX");
	textBoxAngVelX->SetPosition(PointInt(80, top+=20));
	textBoxAngVelX->SetSize(SizeInt(30, 20));
	textBoxAngVelX->SetText(_T("0.0"));
	Children().Add(textBoxAngVelX);
	UI::TextBox* textBoxAngVelY = new UI::TextBox();
	textBoxAngVelY->SetName("textBoxAngVelY");
	textBoxAngVelY->SetPosition(PointInt(110, top));
	textBoxAngVelY->SetSize(SizeInt(30, 20));
	textBoxAngVelY->SetText(_T("0.0"));
	Children().Add(textBoxAngVelY);
	UI::TextBox* textBoxAngVelZ = new UI::TextBox();
	textBoxAngVelZ->SetName("textBoxAngVelZ");
	textBoxAngVelZ->SetPosition(PointInt(140, top));
	textBoxAngVelZ->SetSize(SizeInt(30, 20));
	textBoxAngVelZ->SetText(_T("0.0"));
	Children().Add(textBoxAngVelZ);

	Color32 Color = GetRandomColor();
	UI::TextBox* textBoxColorR = new UI::TextBox();
	textBoxColorR->SetName("textBoxColorR");
	textBoxColorR->SetPosition(PointInt(80, top+=20));
	textBoxColorR->SetSize(SizeInt(30, 20));
	textBoxColorR->SetText(String::ToString((uint8)(Color.r*255)));
	Children().Add(textBoxColorR);
	UI::TextBox* textBoxColorG = new UI::TextBox();
	textBoxColorG->SetName("textBoxColorG");
	textBoxColorG->SetPosition(PointInt(110, top));
	textBoxColorG->SetSize(SizeInt(30, 20));
	textBoxColorG->SetText(String::ToString((uint8)(Color.g*255)));
	Children().Add(textBoxColorG);
	UI::TextBox* textBoxColorB = new UI::TextBox();
	textBoxColorB->SetName("textBoxColorB");
	textBoxColorB->SetPosition(PointInt(140, top));
	textBoxColorB->SetSize(SizeInt(30, 20));
	textBoxColorB->SetText(String::ToString((uint8)(Color.b*255)));
	Children().Add(textBoxColorB);

	UI::Label* previewColor = new UI::Label();
	previewColor->SetName("previewColor");
	previewColor->SetPosition(PointInt(170, top));
	previewColor->SetSize(SizeInt(20, 20));
	previewColor->SetBackgroundColor(Color);
	Children().Add(previewColor);

	// Buttons
	UI::PushButton* buttonOK = new UI::PushButton();
	buttonOK->SetName("buttonOK");
	buttonOK->SetPosition(PointInt(60, 200));
	buttonOK->SetSize(SizeInt(70, 20));
	buttonOK->SetText(_T("OK"));
	buttonOK->SetForegroundColor(Color32::Blue);
	buttonOK->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonOK);

	UI::PushButton* buttonCancel = new UI::PushButton();
	buttonCancel->SetName("buttonCancel");
	buttonCancel->SetPosition(PointInt(140, 200));
	buttonCancel->SetSize(SizeInt(70, 20));
	buttonCancel->SetText(_T("Cancel"));
	buttonCancel->SetForegroundColor(Color32::Blue);
	buttonCancel->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonCancel);

	// Objects
	_TextBoxName = (UI::TextBox*)GetChildByName("textBoxName");
	_PreviewColor = (UI::Label*)GetChildByName("previewColor");
	_ButtonShape = (UI::PushButton*)GetChildByName("buttonShape");
	_TextBoxPosX = (UI::TextBox*)GetChildByName("textBoxPosX");
	_TextBoxPosY = (UI::TextBox*)GetChildByName("textBoxPosY");
	_TextBoxPosZ = (UI::TextBox*)GetChildByName("textBoxPosZ");
	_TextBoxRotX = (UI::TextBox*)GetChildByName("textBoxRotX");
	_TextBoxRotY = (UI::TextBox*)GetChildByName("textBoxRotY");
	_TextBoxRotZ = (UI::TextBox*)GetChildByName("textBoxRotZ");
	_TextBoxLinVelX = (UI::TextBox*)GetChildByName("textBoxLinVelX");
	_TextBoxLinVelY = (UI::TextBox*)GetChildByName("textBoxLinVelY");
	_TextBoxLinVelZ = (UI::TextBox*)GetChildByName("textBoxLinVelZ");
	_TextBoxAngVelX = (UI::TextBox*)GetChildByName("textBoxAngVelX");
	_TextBoxAngVelY = (UI::TextBox*)GetChildByName("textBoxAngVelY");
	_TextBoxAngVelZ = (UI::TextBox*)GetChildByName("textBoxAngVelZ");
	_TextBoxColorR = (UI::TextBox*)GetChildByName("textBoxColorR");
	_TextBoxColorG = (UI::TextBox*)GetChildByName("textBoxColorG");
	_TextBoxColorB = (UI::TextBox*)GetChildByName("textBoxColorB");

	_ButtonOK = (UI::PushButton*)GetChildByName("buttonOK");
	_ButtonCancel = (UI::PushButton*)GetChildByName("buttonCancel");

	// Events
	_ButtonOK->Click += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnButtonOKClick);
	_ButtonCancel->Click += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnButtonCancelClick);

	_ButtonShape->Click += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnButtonShapeClick);

	_TextBoxColorR->TextChanged += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnTextBoxColorTextChanged);
	_TextBoxColorG->TextChanged += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnTextBoxColorTextChanged);
	_TextBoxColorB->TextChanged += new EventMethodSlot<BodyDialog>(this,
		&BodyDialog::OnTextBoxColorTextChanged);
}

BodyDialog::~BodyDialog()
{
}

void BodyDialog::Show(Widget* owner)
{
	UI::Dialog::Show(owner);

	_TextBoxName->SetText("entity" +
		String::ToString(GameCore::Instance()->GetWorld()->GetEntityCount()));

	Color32 Color = GetRandomColor();
	_TextBoxColorR->SetText(String::ToString((uint8)(Color.r*255)));
	_TextBoxColorG->SetText(String::ToString((uint8)(Color.g*255)));
	_TextBoxColorB->SetText(String::ToString((uint8)(Color.b*255)));

	_PreviewColor->SetBackgroundColor(Color);
}

bool BodyDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	String name = _TextBoxName->GetText();

	Vector3 position;
	position.X = _TextBoxPosX->GetText().ToReal32();
	position.Y = _TextBoxPosY->GetText().ToReal32();
	position.Z = _TextBoxPosZ->GetText().ToReal32();

	Vector3 orientation;
	orientation.X = _TextBoxRotX->GetText().ToReal32();
	orientation.Y = _TextBoxRotY->GetText().ToReal32();
	orientation.Z = _TextBoxRotZ->GetText().ToReal32();

	Vector3 linvel;
	linvel.X = _TextBoxLinVelX->GetText().ToReal32();
	linvel.Y = _TextBoxLinVelY->GetText().ToReal32();
	linvel.Z = _TextBoxLinVelZ->GetText().ToReal32();

	Vector3 angvel;
	angvel.X = _TextBoxAngVelX->GetText().ToReal32();
	angvel.Y = _TextBoxAngVelY->GetText().ToReal32();
	angvel.Z = _TextBoxAngVelZ->GetText().ToReal32();

	Color32 color;
	color.r = _TextBoxColorR->GetText().ToUInt8() / 255.0f;
	color.g = _TextBoxColorG->GetText().ToUInt8() / 255.0f;
	color.b = _TextBoxColorB->GetText().ToUInt8() / 255.0f;
	color.a = 1.0f;

	Physics::ShapeType shape = _ShapeDialog->GetShape();
	PhysicsEntity* entity = NULL;
	real density = 10.0;
	if (shape == Physics::ShapeType_Plane)
	{
		Plane plane(Vector3::UnitY, 0.0f);
		entity = GameCore::Instance()->GetWorld()->CreateEntity();
		entity->CreatePlane(plane);
	}
	else if (shape == Physics::ShapeType_Sphere)
	{
		BoundingSphere sphere(Vector3::Zero, 1.0);
		sphere = _ShapeDialog->GetSphere();
		density = _ShapeDialog->GetDensity();
		if (density == 0.0)
			density = Physics::ISphereShape::ComputeDensity(sphere, _ShapeDialog->GetMass());
		entity = GameCore::Instance()->GetWorld()->CreateEntity();
		entity->CreateSphere(sphere, density);
	}
	else if (shape == Physics::ShapeType_Box)
	{
		OBB box(Vector3::Zero, Vector3(1.0, 1.0, 1.0), Matrix3::Identity);
		box = _ShapeDialog->GetBox();
		density = _ShapeDialog->GetDensity();
		if (density == 0.0)
			density = Physics::IBoxShape::ComputeDensity(box, _ShapeDialog->GetMass());
		entity = GameCore::Instance()->GetWorld()->CreateEntity();
		entity->CreateBox(box, density);
	}

	if (entity != NULL)
	{
		entity->GetPhysicsBody()->SetName(name);
		entity->GetPhysicsBody()->SetPosition(position);
		entity->GetPhysicsBody()->SetOrientationMatrix(Matrix3::RotationYawPitchRoll(
			orientation.X, orientation.Y, orientation.Z));
		entity->GetPhysicsBody()->SetLinearVelocity(linvel);
		entity->GetPhysicsBody()->SetAngularVelocity(angvel);
		entity->GetMesh()->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0)->MaterialState.DiffuseColor = color;

		GameCore::Instance()->AddEntity(entity);
	}

	_DialogResult = UI::DialogResult_OK;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}

bool BodyDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}

bool BodyDialog::OnButtonShapeClick(Object* sender, const EventArgs& e)
{
	Children().Add(_ShapeDialog);
	_ShapeDialog->SetPosition(PointInt(260, 220));
	_ShapeDialog->Show(NULL);

	return true;
}

bool BodyDialog::OnTextBoxColorTextChanged(Object* sender, const EventArgs& e)
{
	Color32 color;
	color.r = _TextBoxColorR->GetText().ToUInt8() / 255.0f;
	color.g = _TextBoxColorG->GetText().ToUInt8() / 255.0f;
	color.b = _TextBoxColorB->GetText().ToUInt8() / 255.0f;
	color.a = 1.0f;

	_PreviewColor->SetBackgroundColor(color);

	return true;
}
