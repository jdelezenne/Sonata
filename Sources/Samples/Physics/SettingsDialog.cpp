/*=============================================================================
SettingsDialog.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SettingsDialog.h"
#include "SamplePhysics.h"
#include "Simulation.h"

SettingsDialog::SettingsDialog() :
	Dialog()
{
	SetSize(SizeInt(220, 220));

	_ShapeDialog = new ShapeDialog();

	// Labels
	int top = 5;

	UI::Label* labelTimeStep = new UI::Label();
	labelTimeStep->SetPosition(PointInt(5, top+=20));
	labelTimeStep->SetSize(SizeInt(80, 20));
	labelTimeStep->SetText(_T("Time Step:"));
	Children().Add(labelTimeStep);
	top+=20;

	UI::Label* labelBroadPhase = new UI::Label();
	labelBroadPhase->SetPosition(PointInt(5, top+=20));
	labelBroadPhase->SetSize(SizeInt(80, 20));
	labelBroadPhase->SetText(_T("BroadPhase:"));
	Children().Add(labelBroadPhase);

	UI::Label* labelCollisionDetection = new UI::Label();
	labelCollisionDetection->SetPosition(PointInt(5, top+=20));
	labelCollisionDetection->SetSize(SizeInt(80, 20));
	labelCollisionDetection->SetText(_T("CollisionDetection:"));
	Children().Add(labelCollisionDetection);

	UI::Label* labelGravity = new UI::Label();
	labelGravity->SetPosition(PointInt(5, top+=20));
	labelGravity->SetSize(SizeInt(80, 20));
	labelGravity->SetText(_T("Gravity:"));
	Children().Add(labelGravity);

	UI::Label* labelIntegration = new UI::Label();
	labelIntegration->SetPosition(PointInt(5, top+=20));
	labelIntegration->SetSize(SizeInt(80, 20));
	labelIntegration->SetText(_T("Integration:"));
	Children().Add(labelIntegration);

	UI::Label* labelShape = new UI::Label();
	labelShape->SetPosition(PointInt(5, top+=20));
	labelShape->SetSize(SizeInt(80, 20));
	labelShape->SetText(_T("Thrown Shape:"));
	Children().Add(labelShape);

	// Controls
	top = 5;

	UI::ComboBox* comboBoxTimeStep = new UI::ComboBox();
	comboBoxTimeStep->SetName("comboBoxTimeStep");
	comboBoxTimeStep->SetPosition(PointInt(80, top+=20));
	comboBoxTimeStep->SetSize(SizeInt(80, 20));
	comboBoxTimeStep->Items().Add(new UI::ListItem("Fixed"));
	comboBoxTimeStep->Items().Add(new UI::ListItem("Variable"));
	Children().Add(comboBoxTimeStep);

	UI::TextBox* textBoxTimeStep = new UI::TextBox();
	textBoxTimeStep->SetName("textBoxTimeStep");
	textBoxTimeStep->SetPosition(PointInt(80, top+=20));
	textBoxTimeStep->SetSize(SizeInt(80, 20));
	Children().Add(textBoxTimeStep);

	UI::CheckBoxButton* checkBoxBroadPhase = new UI::CheckBoxButton();
	checkBoxBroadPhase->SetName("checkBoxBroadPhase");
	checkBoxBroadPhase->SetPosition(PointInt(80, top+=20));
	checkBoxBroadPhase->SetSize(SizeInt(80, 20));
	checkBoxBroadPhase->SetEnabled(false);
	Children().Add(checkBoxBroadPhase);

	UI::CheckBoxButton* checkBoxCollisionDetection = new UI::CheckBoxButton();
	checkBoxCollisionDetection->SetName("checkBoxCollisionDetection");
	checkBoxCollisionDetection->SetPosition(PointInt(80, top+=20));
	checkBoxCollisionDetection->SetSize(SizeInt(80, 20));
	checkBoxCollisionDetection->SetEnabled(false);
	Children().Add(checkBoxCollisionDetection);

	UI::TextBox* textBoxGravityX = new UI::TextBox();
	textBoxGravityX->SetName("textBoxGravityX");
	textBoxGravityX->SetPosition(PointInt(80, top+=20));
	textBoxGravityX->SetSize(SizeInt(40, 20));
	Children().Add(textBoxGravityX);
	UI::TextBox* textBoxGravityY = new UI::TextBox();
	textBoxGravityY->SetName("textBoxGravityY");
	textBoxGravityY->SetPosition(PointInt(120, top));
	textBoxGravityY->SetSize(SizeInt(40, 20));
	Children().Add(textBoxGravityY);
	UI::TextBox* textBoxGravityZ = new UI::TextBox();
	textBoxGravityZ->SetName("textBoxGravityZ");
	textBoxGravityZ->SetPosition(PointInt(160, top));
	textBoxGravityZ->SetSize(SizeInt(40, 20));
	Children().Add(textBoxGravityZ);

	UI::ComboBox* comboBoxIntegration = new UI::ComboBox();
	comboBoxIntegration->SetName("comboBoxIntegration");
	comboBoxIntegration->SetPosition(PointInt(80, top+=20));
	comboBoxIntegration->SetSize(SizeInt(80, 20));
	comboBoxIntegration->Items().Add(new UI::ListItem("Euler"));
	comboBoxIntegration->Items().Add(new UI::ListItem("Midpoint"));
	comboBoxIntegration->Items().Add(new UI::ListItem("RK4"));
	Children().Add(comboBoxIntegration);

	UI::PushButton* buttonShape = new UI::PushButton();
	buttonShape->SetName("buttonShape");
	buttonShape->SetPosition(PointInt(80, top+=20));
	buttonShape->SetSize(SizeInt(40, 20));
	buttonShape->SetText(_T("Edit..."));
	buttonShape->SetForegroundColor(Color32::Blue);
	buttonShape->SetBackgroundColor(Color32::Gray);
	Children().Add(buttonShape);

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
	_ComboBoxTimeStep = (UI::ComboBox*)GetChildByName("comboBoxTimeStep");
	_TextBoxTimeStep = (UI::TextBox*)GetChildByName("textBoxTimeStep");
	_CheckBoxBroadPhase = (UI::CheckBoxButton*)GetChildByName("checkBoxBroadPhase");
	_CheckBoxCollisionDetection = (UI::CheckBoxButton*)GetChildByName("checkBoxCollisionDetection");
	_TextBoxGravityX = (UI::TextBox*)GetChildByName("textBoxGravityX");
	_TextBoxGravityY = (UI::TextBox*)GetChildByName("textBoxGravityY");
	_TextBoxGravityZ = (UI::TextBox*)GetChildByName("textBoxGravityZ");
	_ComboBoxIntegration = (UI::ComboBox*)GetChildByName("comboBoxIntegration");
	_ButtonShape = (UI::PushButton*)GetChildByName("buttonShape");

	_ButtonOK = (UI::PushButton*)GetChildByName("buttonOK");
	_ButtonCancel = (UI::PushButton*)GetChildByName("buttonCancel");

	// Events
	_ButtonOK->Click += new EventMethodSlot<SettingsDialog>(this,
		&SettingsDialog::OnButtonOKClick);
	_ButtonCancel->Click += new EventMethodSlot<SettingsDialog>(this,
		&SettingsDialog::OnButtonCancelClick);

	_ButtonShape->Click += new EventMethodSlot<SettingsDialog>(this,
		&SettingsDialog::OnButtonShapeClick);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::Show(Widget* owner)
{
	UI::Dialog::Show(owner);

	PhysicsWorld* world = GameCore::Instance()->GetWorld();
	Physics::IScene* scene = world->GetPhysicsScene();
	Physics::SceneDescription desc = scene->GetDescription();

	_ComboBoxTimeStep->SetSelectedIndex(scene->GetTimeStepType());
	_TextBoxTimeStep->SetText(String::ToString(scene->GetTimeStep()));
	_CheckBoxBroadPhase->SetChecked(desc._BroadPhase);
	_CheckBoxCollisionDetection->SetChecked(desc._CollisionDetection);
	_TextBoxGravityX->SetText(String::ToString(scene->GetGravity().X));
	_TextBoxGravityY->SetText(String::ToString(scene->GetGravity().Y));
	_TextBoxGravityZ->SetText(String::ToString(scene->GetGravity().Z));
	_ComboBoxIntegration->SetSelectedIndex(world->GetIntegrationType()-1);
}

bool SettingsDialog::OnButtonOKClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_OK;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	PhysicsWorld* world = GameCore::Instance()->GetWorld();
	Physics::IScene* scene = world->GetPhysicsScene();

	scene->SetTimeStepType((TimeStepType)_ComboBoxTimeStep->GetSelectedIndex());
	scene->SetTimeStep(_TextBoxTimeStep->GetText().ToReal32());

	Vector3 gravity;
	gravity.X = _TextBoxGravityX->GetText().ToReal32();
	gravity.Y = _TextBoxGravityY->GetText().ToReal32();
	gravity.Z = _TextBoxGravityZ->GetText().ToReal32();
	scene->SetGravity(gravity);

	world->SetIntegrationType((IntegrationType)_ComboBoxIntegration->GetSelectedIndex());

	Physics::ShapeType shape = _ShapeDialog->GetShape();
	real density = 10.0;
	Plane plane(Vector3::UnitY, 0.0f);
	BoundingSphere sphere(Vector3::Zero, 1.0);
	OBB box(Vector3::Zero, Vector3(1.0, 1.0, 1.0), Matrix3::Identity);
	if (shape == Physics::ShapeType_Plane)
	{
	}
	else if (shape == Physics::ShapeType_Sphere)
	{
		sphere = _ShapeDialog->GetSphere();
		density = _ShapeDialog->GetDensity();
		if (density == 0.0)
			density = Physics::ISphereShape::ComputeDensity(sphere, _ShapeDialog->GetMass());
	}
	else if (shape == Physics::ShapeType_Box)
	{
		box = _ShapeDialog->GetBox();
		density = _ShapeDialog->GetDensity();
		if (density == 0.0)
			density = Physics::IBoxShape::ComputeDensity(box, _ShapeDialog->GetMass());
	}
	world->SetThrownShape(shape, density, sphere, box);

	return true;
}

bool SettingsDialog::OnButtonCancelClick(Object* sender, const EventArgs& e)
{
	_DialogResult = UI::DialogResult_Cancel;
	GetParent()->Children().Remove(this);
	UI::UISystem::Instance()->SetFocusedWidget(NULL);

	return true;
}

bool SettingsDialog::OnButtonShapeClick(Object* sender, const EventArgs& e)
{
	Children().Add(_ShapeDialog);
	_ShapeDialog->SetPosition(PointInt(260, 220));
	_ShapeDialog->Show(NULL);

	return true;
}
