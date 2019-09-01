/*=============================================================================
SampleUI.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleUI.h"

/*#ifdef WIN32
#	include <Platforms/Win32/Win32ConsoleLogHandler.h>
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif*/

#define USE_XML 0
#define RESET_UI 0

Application* theApp = Application::Instance();

RenderSystem* gl_RenderSystem = NULL;
RenderSystem* d3d9_RenderSystem = NULL;

UI::PropertyGridBinder* binder = NULL;
UI::PropertyGrid* propgrid = NULL;

bool OnButtonClick(Object* sender, const EventArgs& e)
{
	//MessageBox(NULL, "click!", "", MB_OK);

	UI::FileDialog* dialog = new UI::FileDialog();
	dialog->SetRectangle(RectangleInt(20, 20, 350, 250));
	dialog->SetFileDialogMode(UI::FileDialogMode_Open);
	dialog->SetInitialDirectory(_T("C:\\"));
	dialog->Show(NULL);

	return true;
}

bool OnWindowGridUpdate(Object* sender, const EventArgs& ee)
{
	const UI::UpdateEventArgs e = (const UI::UpdateEventArgs&)ee;
	UI::Panel* windowGrid = (UI::Panel*)sender;

	UI::CheckBoxButton* buttonGrid = (UI::CheckBoxButton*)
		UI::UISystem::Instance()->GetWidgetByName("buttonGrid");

	if (buttonGrid != NULL)
	{
		if (buttonGrid->GetChecked())
		{
			if (windowGrid->GetHeight() < windowGrid->GetMaximumSize().Height)
			{
				propgrid->SetVisible(true);
				windowGrid->SetHeight(windowGrid->GetHeight() + e.ElapsedTime() * 200.0);
			}
		}
		else
		{
			if (windowGrid->GetHeight() > windowGrid->GetMinimumSize().Height)
				windowGrid->SetHeight(windowGrid->GetHeight() - e.ElapsedTime() * 200.0);
			else
				propgrid->SetVisible(false);
		}
	}

	return true;
}

Image* CreateImage(const String& fileName)
{
	Resource* resource;

	resource = ResourceHelper::LoadFromFile(fileName, SE_ID_DATA_IMAGE);
	if (resource == NULL)
		return NULL;

	return (Image*)resource->GetData();
}

Texture* CreateTexture(const String& fileName)
{
	Texture* texture;
	Image* image;

	image = CreateImage(fileName);
	if (!RenderSystem::Current()->CreateTexture(image, &texture))
		return NULL;

	return texture;
}

class GameCore : public Singleton<GameCore>
{
public:
	GameCore() :
		_MainWindow(NULL),
		_camera(NULL),
		_Screen(NULL)
	{
	}

	bool Create()
	{
/*#ifdef WIN32
		LogHandler* consoleHandler = new Win32ConsoleLogHandler();
		((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_None));

		LogHandler* debugHandler = new Win32DebugLogHandler();
		((DefaultLogFormatter*)debugHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_Source | LogOptions_Time));
#endif*/

		Logger* logger = new Logger();
		//logger->GetHandlers().Add(consoleHandler);
		//logger->GetHandlers().Add(debugHandler);
		Logger::SetCurrent(logger);

		FileSystem::Instance()->AddRootPath(_T("Data"));
		FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
		FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
		PluginManager::Instance()->ParsePlugins(Environment::GetCurrentDirectory());
		PluginManager::Instance()->CreateAllPlugins();

		if (!CreateRenderSystem())
			return false;

		if (!CreateInputSystem())
			return false;

		return true;
	}

	void Destroy()
	{
	}

	bool CreateRenderSystem()
	{
		_MainWindow = new Window();
		_MainWindow->Create(NULL, "UI Sample", 800, 600, false);

		theApp->AddWindow(_MainWindow);
		theApp->SetCurrentWindow(_MainWindow);

		_MainWindow->Sized.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnSize));

		ManagerPlugin* gl = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_GL);
		if (gl == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the OpenGL plugin."));
			//return false;
		}

		ManagerPlugin* d3d9 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_RENDERSYSTEM_D3D9);
		if (d3d9 == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the Direct3D9 plugin."));
			//return false;
		}

		gl_RenderSystem = (RenderSystem*)gl->CreateManager();
		d3d9_RenderSystem = (RenderSystem*)d3d9->CreateManager();
		RenderSystem::SetCurrent(d3d9_RenderSystem);

		OnCreate();

		return true;
	}

	bool CreateInputSystem()
	{
		ManagerPlugin* di8 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_INPUTSYSTEM_DI8);
		if (di8 == NULL)
		{
			Logger::Current()->Log(_T("Failed to get the DirectInput8 plugin."));
			//return false;
		}
		InputSystem* inputSystem = (InputSystem*)di8->CreateManager();
		InputSystem::SetCurrent(inputSystem);

		inputSystem->Create();

		InputMouse* mouse = (InputMouse*)inputSystem->GetInputDeviceByType(InputDeviceType_Mouse);
		InputKeyboard* keyboard = (InputKeyboard*)inputSystem->GetInputDeviceByType(InputDeviceType_Keyboard);

		if (mouse == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the mouse device."));
			return false;
		}

		if (keyboard == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the keyboard device."));
			return false;
		}

		mouse->Create(_MainWindow);
		keyboard->Create(_MainWindow);

		return true;
	}

	Font* CreateFont(const String& fileName)
	{
		Resource* resImage = ResourceHelper::LoadFromFile(fileName, SE_ID_DATA_IMAGE);
		if (resImage == NULL)
			return NULL;

		Image* fontImage = (Image*)resImage->GetData();
		if (fontImage == NULL)
			return NULL;

		Texture* texture;
		RenderSystem::Current()->CreateTexture(fontImage, &texture);

		String xmlName = Path::Combine(Path::GetDirectoryName(fileName),
			Path::ChangeExtension(fileName, _T("xml")));
		XMLReader reader(xmlName);
		SonataEngine::XMLDocument* document = reader.GetDocument();
		if (document == NULL)
			return NULL;

		Font* font = new Font();
		font->SetTexture(texture);

		const XMLNode::XMLAttributeList& fontAttributes = document->GetDocumentElement()->GetAttributes();
		for (int i=0; i<fontAttributes.Count(); i++)
		{
			if (fontAttributes[i]->GetName().CompareTo("Spacing", true) == 0)
				font->SetSpacing(fontAttributes[i]->GetValue().ToInt32());
			else if (fontAttributes[i]->GetName().CompareTo("SpaceWidth", true) == 0)
				font->SetSpaceWidth(fontAttributes[i]->GetValue().ToInt32());
			else if (fontAttributes[i]->GetName().CompareTo("Height", true) == 0)
				font->SetHeight(fontAttributes[i]->GetValue().ToInt32());
		}

		XMLNode* glyths = document->GetDocumentElement()->GetFirstChild();
		XMLNode* glyth = glyths->GetFirstChild();
		while (glyth != NULL)
		{
			FontGlyth fontGlyth;

			const XMLNode::XMLAttributeList& glythAttributes = glyth->GetAttributes();
			for (int i=0; i<glythAttributes.Count(); i++)
			{
				try
				{
					if (glythAttributes[i]->GetName().CompareTo("Character", true) == 0)
						fontGlyth.Character = glythAttributes[i]->GetValue().ToChar();
					else if (glythAttributes[i]->GetName().CompareTo("X", true) == 0)
						fontGlyth.Rectangle.X = glythAttributes[i]->GetValue().ToInt32();
					else if (glythAttributes[i]->GetName().CompareTo("Y", true) == 0)
						fontGlyth.Rectangle.Y = glythAttributes[i]->GetValue().ToInt32();
					else if (glythAttributes[i]->GetName().CompareTo("Width", true) == 0)
						fontGlyth.Rectangle.Width = glythAttributes[i]->GetValue().ToInt32();
				}
				catch (const FormatException&)
				{
				}
			}

			fontGlyth.Rectangle.Width -= fontGlyth.Rectangle.X;
			fontGlyth.Rectangle.Height = font->GetHeight();

			if (fontGlyth.Character != 0 && !fontGlyth.Rectangle.IsEmpty())
			{
				font->SetGlyth(fontGlyth.Character, fontGlyth);
			}

			glyth = glyth->GetNextSibling();
		}

		font->Build();

		delete fontImage;

		return font;
	}

	void CreateUI()
	{
		int i;

		_Screen = new UI::Panel();
		_Screen->SetHasFrame(false);
		_Screen->SetHasBackground(false);
		_Screen->SetSize(SizeInt(_MainWindow->GetClientWidth(), _MainWindow->GetClientHeight()));

			UI::Panel* panelInfo = new UI::Panel();
			panelInfo->SetPosition(PointInt(0, 0));
			panelInfo->SetSize(SizeInt(640, 80));
			panelInfo->SetHasFrame(false);
			panelInfo->SetHasBackground(false);

				UI::Label* labelInfo1 = new UI::Label();
				labelInfo1->SetPosition(PointInt(0, 0));
				labelInfo1->SetSize(SizeInt(640, 20));
				labelInfo1->SetText(_T("This user interface is either built from code or loaded from an XML file."));
				labelInfo1->SetForegroundColor(Color32::Red);
				labelInfo1->SetHasBackground(false);
				labelInfo1->SetTextAlignment(UI::Alignment_TopLeft);

				UI::Label* labelInfo2 = new UI::Label();
				labelInfo2->SetPosition(PointInt(0, 20));
				labelInfo2->SetSize(SizeInt(640, 20));
				labelInfo2->SetText(_T("Drag the windows around and try out the widgets."));
				labelInfo2->SetForegroundColor(Color32::Red);
				labelInfo2->SetHasBackground(false);
				labelInfo2->SetTextAlignment(UI::Alignment_TopLeft);

				UI::Label* labelInfo3 = new UI::Label();
				labelInfo3->SetPosition(PointInt(0, 40));
				labelInfo3->SetSize(SizeInt(640, 20));
				labelInfo3->SetText(_T("Toggle the property grid to change the properties using the reflection system."));
				labelInfo3->SetForegroundColor(Color32::Red);
				labelInfo3->SetHasBackground(false);
				labelInfo3->SetTextAlignment(UI::Alignment_TopLeft);

				UI::Label* labelInfo4 = new UI::Label();
				labelInfo4->SetPosition(PointInt(0, 60));
				labelInfo4->SetSize(SizeInt(640, 20));
				labelInfo4->SetText(_T("You can also toggle the Direct3D9 and OpenGL renderers at runtime!"));
				labelInfo4->SetForegroundColor(Color32::Red);
				labelInfo4->SetHasBackground(false);
				labelInfo4->SetTextAlignment(UI::Alignment_TopLeft);

			panelInfo->Children().Add(labelInfo1);
			panelInfo->Children().Add(labelInfo2);
			panelInfo->Children().Add(labelInfo3);
			panelInfo->Children().Add(labelInfo4);

			UI::Panel* windowCtrl1 = new UI::Panel();
			windowCtrl1->SetMovable(true);
			windowCtrl1->SetSizable(true);
			windowCtrl1->SetPosition(PointInt(50, 80));
			windowCtrl1->SetSize(SizeInt(300, 130));
			windowCtrl1->SetMinimumSize(SizeInt(260, 130));
			windowCtrl1->SetMaximumSize(SizeInt(350, 200));
			windowCtrl1->SetHasFrame(true);
			windowCtrl1->SetFrameSize(4);
			windowCtrl1->SetFrameColor(Color32::Red);
			windowCtrl1->SetHasBackground(true);
			Color32 windowCtrl1Color = Color32::Aqua;
			windowCtrl1Color.a = 0.6f;
			windowCtrl1->SetBackgroundColor(windowCtrl1Color);

				UI::Label* labelButton = new UI::Label();
				labelButton->SetPosition(PointInt(10, 10));
				labelButton->SetSize(SizeInt(80, 20));
				labelButton->SetText(_T("Push Button"));
				labelButton->SetForegroundColor(Color32::Blue);
				labelButton->SetHasBackground(false);
				labelButton->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::PushButton* button = new UI::PushButton();
				button->SetName("button");
				button->SetPosition(PointInt(10, 50));
				button->SetSize(SizeInt(80, 30));
				button->SetText(_T("Push Me!"));
				button->SetForegroundColor(Color32::Green);
				button->SetBackgroundColor(Color32::Gray);

				UI::Label* labelCheck = new UI::Label();
				labelCheck->SetPosition(PointInt(100, 10));
				labelCheck->SetSize(SizeInt(80, 20));
				labelCheck->SetText(_T("CheckBox"));
				labelCheck->SetForegroundColor(Color32::Blue);
				labelCheck->SetHasBackground(false);
				labelCheck->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::CheckBoxButton* checkbox = new UI::CheckBoxButton();
				checkbox->SetName("checkbox");
				checkbox->SetPosition(PointInt(100, 50));
				checkbox->SetSize(SizeInt(80, 30));
				checkbox->SetText(_T("Check Me!"));
				checkbox->SetForegroundColor(Color32::Green);
				checkbox->SetHasBackground(false);

				UI::Label* labelCheckInfo = new UI::Label();
				labelCheckInfo->SetName("labelCheckInfo");
				labelCheckInfo->SetPosition(PointInt(100, 70));
				labelCheckInfo->SetSize(SizeInt(80, 20));
				labelCheckInfo->SetForegroundColor(Color32::Red);
				labelCheckInfo->SetHasBackground(false);
				labelCheckInfo->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::Label* labelRadio = new UI::Label();
				labelRadio->SetPosition(PointInt(190, 10));
				labelRadio->SetSize(SizeInt(80, 20));
				labelRadio->SetText(_T("Radio"));
				labelRadio->SetForegroundColor(Color32::Green);
				labelRadio->SetHasBackground(false);
				labelRadio->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::RadioButton* radio = new UI::RadioButton();
				radio->SetPosition(PointInt(190, 50));
				radio->SetSize(SizeInt(80, 30));
				radio->SetText(_T("Select Me!"));
				radio->SetForegroundColor(Color32::Green);
				radio->SetHasBackground(false);

				UI::Label* labelResize = new UI::Label();
				labelResize->SetPosition(PointInt(60, 130));
				labelResize->SetSize(SizeInt(160, 20));
				labelResize->SetText(_T("Drag Me! Resize Me!"));
				labelResize->SetForegroundColor(Color32::Blue);
				labelResize->SetHasBackground(false);
				labelResize->SetTextAlignment(UI::Alignment_MiddleCenter);

			windowCtrl1->Children().Add(labelButton);
			windowCtrl1->Children().Add(labelCheck);
			windowCtrl1->Children().Add(labelRadio);
			windowCtrl1->Children().Add(button);
			windowCtrl1->Children().Add(checkbox);
			windowCtrl1->Children().Add(labelCheckInfo);
			windowCtrl1->Children().Add(radio);
			windowCtrl1->Children().Add(labelResize);

			UI::Panel* windowCtrl2 = new UI::Panel();
			windowCtrl2->SetMovable(true);
			windowCtrl2->SetSizable(true);
			windowCtrl2->SetPosition(PointInt(280, 300));
			windowCtrl2->SetSize(SizeInt(220, 260));
			windowCtrl2->SetHasFrame(true);
			windowCtrl2->SetFrameSize(1);
			windowCtrl2->SetFrameColor(Color32::Magenta);
			windowCtrl2->SetHasBackground(true);
			Color32 windowCtrl2Color = Color32::Green;
			windowCtrl2Color.a = 0.6f;
			windowCtrl2->SetBackgroundColor(windowCtrl2Color);

				UI::Label* labelProgressBar = new UI::Label();
				labelProgressBar->SetPosition(PointInt(10, 10));
				labelProgressBar->SetSize(SizeInt(80, 20));
				labelProgressBar->SetText(_T("Progress Bars"));
				labelProgressBar->SetForegroundColor(Color32::Blue);
				labelProgressBar->SetHasBackground(false);
				labelProgressBar->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::ProgressBar* progressBar1 = new UI::ProgressBar();
				progressBar1->SetName("progressBar1");
				progressBar1->SetOrientation(UI::Orientation_Horizontal);
				progressBar1->SetPosition(PointInt(20, 40));
				progressBar1->SetSize(SizeInt(50, 20));
				progressBar1->SetBackgroundColor(Color32::White);
				progressBar1->SetHasFrame(true);
				progressBar1->SetFrameSize(1);
				progressBar1->SetFrameColor(Color32::Black);
				progressBar1->SetBarColor(Color32::Blue);
				progressBar1->SetValue(24);

				UI::ProgressBar* progressBar2 = new UI::ProgressBar();
				progressBar2->SetName("progressBar2");
				progressBar2->SetOrientation(UI::Orientation_Vertical);
				progressBar2->SetPosition(PointInt(30, 80));
				progressBar2->SetSize(SizeInt(20, 50));
				progressBar2->SetBackgroundColor(Color32::White);
				progressBar2->SetHasFrame(true);
				progressBar2->SetFrameSize(1);
				progressBar2->SetFrameColor(Color32::Black);
				progressBar2->SetBarColor(Color32::Blue);
				progressBar2->SetValue(75);

				UI::Label* labelSlider = new UI::Label();
				labelSlider->SetPosition(PointInt(120, 10));
				labelSlider->SetSize(SizeInt(80, 20));
				labelSlider->SetText(_T("Sliders"));
				labelSlider->SetForegroundColor(Color32::Blue);
				labelSlider->SetHasBackground(false);
				labelSlider->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::Slider* slider1 = new UI::Slider();
				slider1->SetName("slider1");
				slider1->SetOrientation(UI::Orientation_Horizontal);
				slider1->SetPosition(PointInt(120, 40));
				slider1->SetSize(SizeInt(50, 20));
				slider1->SetBackgroundColor(Color32::White);
				slider1->SetHasFrame(true);
				slider1->SetFrameSize(1);
				slider1->SetFrameColor(Color32::Black);
				slider1->SetBarColor(Color32::Blue);
				slider1->SetValue(24);

				UI::Slider* slider2 = new UI::Slider();
				slider2->SetName("slider2");
				slider2->SetOrientation(UI::Orientation_Vertical);
				slider2->SetPosition(PointInt(130, 80));
				slider2->SetSize(SizeInt(20, 50));
				slider2->SetBackgroundColor(Color32::White);
				slider2->SetHasFrame(true);
				slider2->SetFrameSize(1);
				slider2->SetFrameColor(Color32::Black);
				slider2->SetBarColor(Color32::Blue);
				slider2->SetValue(75);

				UI::ScrollBar* scrollbar1 = new UI::ScrollBar();
				scrollbar1->SetName("scrollbar1");
				scrollbar1->SetOrientation(UI::Orientation_Horizontal);
				scrollbar1->SetPosition(PointInt(20, 160));
				scrollbar1->SetSize(SizeInt(90, 15));
				scrollbar1->SetValue(24);

				UI::ScrollBar* scrollbar2 = new UI::ScrollBar();
				scrollbar2->SetName("scrollbar2");
				scrollbar2->SetOrientation(UI::Orientation_Vertical);
				scrollbar2->SetPosition(PointInt(130, 140));
				scrollbar2->SetSize(SizeInt(15, 90));
				scrollbar2->SetValue(75);

			windowCtrl2->Children().Add(labelProgressBar);
			windowCtrl2->Children().Add(progressBar1);
			windowCtrl2->Children().Add(progressBar2);
			windowCtrl2->Children().Add(labelSlider);
			windowCtrl2->Children().Add(slider1);
			windowCtrl2->Children().Add(slider2);
			windowCtrl2->Children().Add(scrollbar1);
			windowCtrl2->Children().Add(scrollbar2);

			UI::Panel* windowText = new UI::Panel();
			windowText->SetMovable(true);
			windowText->SetSizable(true);
			windowText->SetPosition(PointInt(40, 250));
			windowText->SetSize(SizeInt(200, 150));
			windowText->SetHasFrame(true);
			windowText->SetFrameSize(1);
			windowText->SetFrameColor(Color32::Yellow);
			windowText->SetHasBackground(true);
			Color32 windowTextColor = Color32::Olive;
			windowTextColor.a = 0.6f;
			windowText->SetBackgroundColor(windowTextColor);

				UI::Label* labelText1 = new UI::Label();
				labelText1->SetPosition(PointInt(10, 10));
				labelText1->SetSize(SizeInt(80, 20));
				labelText1->SetText(_T("Editable TextBox"));
				labelText1->SetForegroundColor(Color32::Blue);
				labelText1->SetHasBackground(false);
				labelText1->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::TextBox* textbox1 = new UI::TextBox();
				textbox1->SetHasFrame(true);
				textbox1->SetFrameSize(1);
				textbox1->SetFrameColor(Color32::Blue);
				textbox1->SetPosition(PointInt(10, 50));
				textbox1->SetSize(SizeInt(80, 40));
				textbox1->SetBackgroundColor(Color32::White);
				textbox1->SetText("Change Me!.");

				UI::Label* labelText2 = new UI::Label();
				labelText2->SetPosition(PointInt(100, 10));
				labelText2->SetSize(SizeInt(80, 20));
				labelText2->SetText(_T("ReadOnly TextBox"));
				labelText2->SetForegroundColor(Color32::Blue);
				labelText2->SetHasBackground(false);
				labelText2->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::TextBox* textbox2 = new UI::TextBox();
				textbox2->SetHasFrame(true);
				textbox2->SetFrameSize(1);
				textbox2->SetFrameColor(Color32::Blue);
				textbox2->SetPosition(PointInt(100, 50));
				textbox2->SetSize(SizeInt(80, 40));
				textbox2->SetBackgroundColor(Color32::White);
				textbox2->SetText("I'm read only!");
				textbox2->SetReadOnly(true);

				UI::Label* labelTextInfo1 = new UI::Label();
				labelTextInfo1->SetPosition(PointInt(10, 100));
				labelTextInfo1->SetSize(SizeInt(160, 20));
				labelTextInfo1->SetText(_T("Select the text with the mouse!"));
				labelTextInfo1->SetForegroundColor(Color32::Red);
				labelTextInfo1->SetHasBackground(false);
				labelTextInfo1->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::Label* labelTextInfo2 = new UI::Label();
				labelTextInfo2->SetPosition(PointInt(10, 120));
				labelTextInfo2->SetSize(SizeInt(160, 20));
				labelTextInfo2->SetText(_T("and try shortcut keys!"));
				labelTextInfo2->SetForegroundColor(Color32::Red);
				labelTextInfo2->SetHasBackground(false);
				labelTextInfo2->SetTextAlignment(UI::Alignment_MiddleCenter);

			windowText->Children().Add(labelText1);
			windowText->Children().Add(textbox1);
			windowText->Children().Add(labelText2);
			windowText->Children().Add(textbox2);
			windowText->Children().Add(labelTextInfo1);
			windowText->Children().Add(labelTextInfo2);

			UI::Panel* windowList = new UI::Panel();
			windowList->SetMovable(true);
			windowList->SetSizable(true);
			windowList->SetPosition(PointInt(40, 440));
			windowList->SetSize(SizeInt(200, 150));
			windowList->SetHasFrame(true);
			windowList->SetFrameSize(1);
			windowList->SetFrameColor(Color32::Yellow);
			windowList->SetHasBackground(true);
			Color32 windowListColor = Color32::Olive;
			windowListColor.a = 0.6f;
			windowList->SetBackgroundColor(windowListColor);

				UI::Label* labelList1 = new UI::Label();
				labelList1->SetPosition(PointInt(10, 10));
				labelList1->SetSize(SizeInt(80, 20));
				labelList1->SetText(_T("ListBox"));
				labelList1->SetForegroundColor(Color32::Blue);
				labelList1->SetHasBackground(false);
				labelList1->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::ListBox* listbox1 = new UI::ListBox();
				listbox1->SetHasFrame(true);
				listbox1->SetFrameSize(1);
				listbox1->SetFrameColor(Color32::Blue);
				listbox1->SetPosition(PointInt(10, 50));
				listbox1->SetSize(SizeInt(80, 80));
				listbox1->SetBackgroundColor(Color32::White);
				for (i=0; i<20; i++)
					listbox1->Items().Add(new UI::ListItem(_T("Item ") + String::ToString(i)));
				//listbox1->SetMultiSelection(true);

				UI::Label* labelList2 = new UI::Label();
				labelList2->SetPosition(PointInt(100, 10));
				labelList2->SetSize(SizeInt(80, 20));
				labelList2->SetText(_T("ComboBox"));
				labelList2->SetForegroundColor(Color32::Blue);
				labelList2->SetHasBackground(false);
				labelList2->SetTextAlignment(UI::Alignment_MiddleCenter);

				UI::ComboBox* combobox1 = new UI::ComboBox();
				combobox1->SetHasFrame(true);
				combobox1->SetFrameSize(1);
				combobox1->SetFrameColor(Color32::Blue);
				combobox1->SetPosition(PointInt(100, 50));
				combobox1->SetSize(SizeInt(80, 15));
				combobox1->SetBackgroundColor(Color32::White);
				for (i=0; i<20; i++)
					combobox1->Items().Add(new UI::ListItem(_T("Item ") + String::ToString(i)));

			windowList->Children().Add(labelList1);
			windowList->Children().Add(listbox1);
			windowList->Children().Add(labelList2);
			windowList->Children().Add(combobox1);

			UI::Panel* windowGrid = new UI::Panel();
			windowGrid->SetMovable(true);
			windowGrid->SetSizable(true);
			windowGrid->SetName("windowGrid");
			windowGrid->SetPosition(PointInt(520, 10));
			windowGrid->SetSize(SizeInt(260, 50));
			windowGrid->SetMinimumSize(SizeInt(260, 50));
			windowGrid->SetMaximumSize(SizeInt(260, 530));
			windowGrid->SetHasFrame(true);
			windowGrid->SetFrameSize(1);
			windowGrid->SetFrameColor(Color32::Green);
			windowGrid->SetHasBackground(true);
			Color32 windowGridColor = Color32::Blue;
			windowGridColor.a = 0.6f;
			windowGrid->SetBackgroundColor(windowGridColor);

				UI::CheckBoxButton* buttonGrid = new UI::CheckBoxButton();
				buttonGrid->SetName("buttonGrid");
				buttonGrid->SetPosition(PointInt(50, 2));
				buttonGrid->SetSize(SizeInt(120, 30));
				buttonGrid->SetText(_T("Toggle Property Grid!"));
				buttonGrid->SetFrameColor(Color32::Green);
				buttonGrid->SetHasBackground(false);

				UI::Label* labelGrid = new UI::Label();
				labelGrid->SetPosition(PointInt(180, 10));
				labelGrid->SetSize(SizeInt(100, 30));
				labelGrid->SetText(_T("(slow :p)"));
				labelGrid->SetForegroundColor(Color32::Red);
				labelGrid->SetHasBackground(false);

				propgrid = new UI::PropertyGrid();
				propgrid->SetName("propertyGrid1");
				propgrid->SetVisible(false);
				propgrid->SetHasFrame(true);
				propgrid->SetFrameSize(1);
				propgrid->SetFrameColor(Color32::Black);
				propgrid->SetPosition(PointInt(5, 40));
				propgrid->SetSize(SizeInt(250, 480));
				propgrid->SetBackgroundColor(Color32::Gray);

			windowGrid->Children().Add(buttonGrid);
			windowGrid->Children().Add(labelGrid);
			windowGrid->Children().Add(propgrid);

			UI::Panel* windowRender = new UI::Panel();
			windowRender->SetPosition(PointInt(650, 550));
			windowRender->SetSize(SizeInt(130, 35));
			windowRender->SetHasFrame(true);
			windowRender->SetFrameSize(1);
			windowRender->SetFrameColor(Color32::Black);
			windowRender->SetHasBackground(true);
			windowRender->SetBackgroundColor(Color32::Red);

				UI::PushButton* button_gl = new UI::PushButton();
				button_gl->SetName("button_gl");
				button_gl->SetPosition(PointInt(5, 2));
				button_gl->SetSize(SizeInt(50, 30));
				button_gl->SetText(_T("OpenGL"));

				UI::PushButton* button_d3d9 = new UI::PushButton();
				button_d3d9->SetName("button_d3d9");
				button_d3d9->SetPosition(PointInt(70, 2));
				button_d3d9->SetSize(SizeInt(50, 30));
				button_d3d9->SetText(_T("Direct3D9"));

			windowRender->Children().Add(button_gl);
			windowRender->Children().Add(button_d3d9);

		_Screen->Children().Add(panelInfo);
		_Screen->Children().Add(windowCtrl1);
		_Screen->Children().Add(windowCtrl2);
		_Screen->Children().Add(windowText);
		_Screen->Children().Add(windowList);
		_Screen->Children().Add(windowGrid);
		_Screen->Children().Add(windowRender);
	}

	bool OnInit(Object* sender, const EventArgs& e)
	{
		OnReset();

		File* fileXml = new File("sampleui.Xml");
		XMLSerializer* serializerXml  = new XMLSerializer();

#if !USE_XML
		CreateUI();
#else
#	if RESET_UI
		CreateUI();

		FileStreamPtr streamXml = fileXml->Open(FileMode_Create);
		if (streamXml != NULL)
		{
			serializerXml->Serialize(streamXml.Get(), _Screen);
			streamXml->Close();
		}
#	else
		FileStreamPtr streamXml = fileXml->Open(FileMode_Open, FileAccess_Read);
		if (streamXml != NULL)
		{
			_Screen = (UI::Panel*)serializerXml->Deserialize(streamXml.Get());
			streamXml->Close();
		}
#	endif
#endif

		if (_Screen != NULL)
		{
			UI::UISystem::Instance()->SetCurrentWidget(_Screen);

			UI::PushButton* button = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("button");
			UI::PushButton* buttonGrid = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("buttonGrid");
			UI::PushButton* button_gl = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("button_gl");
			UI::PushButton* button_d3d9 = (UI::PushButton*)
				UI::UISystem::Instance()->GetWidgetByName("button_d3d9");
			UI::Panel* windowGrid = (UI::Panel*)
				UI::UISystem::Instance()->GetWidgetByName("windowGrid");
			propgrid = (UI::PropertyGrid*)
				UI::UISystem::Instance()->GetWidgetByName("propertyGrid1");

			if (button)
				button->Click += new EventFunctorSlot(&OnButtonClick);
			if (button_gl)
				button_gl->Click += new EventMethodSlot<GameCore>(this, &GameCore::OnButtonGLClick);
			if (button_d3d9)
				button_d3d9->Click += new EventMethodSlot<GameCore>(this, &GameCore::OnButtonD3D9Click);
			if (windowGrid)
				windowGrid->PreUpdate += new EventFunctorSlot(&OnWindowGridUpdate);

			binder = new UI::PropertyGridBinder;
			binder->SetPropertyGrid(propgrid);
		}

		return true;
	}

	static bool OnExit(Object* sender, const EventArgs& e)
	{
		UI::UISystem::Instance()->Destroy();

		InputSystem::Current()->Destroy();
		RenderSystem::Current()->Destroy();

		return true;
	}

	bool OnSize(Object* sender, const EventArgs& e)
	{
		return true;
	}

	bool OnUpdate(Object* sender, const UpdateEventArgs* e)
	{
		real64 elapsed = e->ElapsedFrameTime;

		UI::UISystem::Instance()->Update(elapsed);
		UI::Widget* focused = UI::UISystem::Instance()->GetFocusedWidget();

		if (binder != NULL && propgrid != NULL)
		{
			if (binder->GetSelectedObject() != focused && propgrid != focused && !propgrid->Contains(focused))
				binder->SetSelectedObject(focused);
		}

		UI::CheckBoxButton* checkbox = (UI::CheckBoxButton*)
			UI::UISystem::Instance()->GetWidgetByName("checkbox");
		UI::Label* labelCheckInfo = (UI::Label*)
			UI::UISystem::Instance()->GetWidgetByName("labelCheckInfo");
		if (checkbox != NULL && labelCheckInfo != NULL)
		{
			if (checkbox->GetChecked())
				labelCheckInfo->SetText(_T("Checked!"));
			else
				labelCheckInfo->SetText(String::Empty);
		}

		InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
			GetInputDeviceByType(InputDeviceType_Keyboard);
		if (keyboard->IsKeyDown(Key_Escape))
			Application::Instance()->Exit();

		return true;
	}

	bool OnBeginFrame(Object* sender, const EventArgs& e)
	{
		UI::UISystem::Instance()->Render();

		return true;
	}

	bool OnCreate()
	{
		RenderContextDescription desc;
		desc.Mode.SetWidth(800);
		desc.Mode.SetHeight(600);
		if (!RenderSystem::Current()->CreateRenderContext(_MainWindow, desc))
			return false;

		return true;
	}

	void ResetFont(UI::Widget* widget)
	{
		if (widget == NULL)
			return;

		widget->SetFont(UI::UISystem::Instance()->GetDefaultFont());

		UI::Widget::WidgetList::Iterator it = widget->Children().GetIterator();
		while (it.Next())
		{
			ResetFont(it.Current());
		}
	}

	bool OnReset()
	{
		SE_DELETE(_camera);
		_camera = new Camera();
		_camera->SetPerspective(45.0f,
			(real32)_MainWindow->GetClientWidth()/_MainWindow->GetClientHeight(),
			1.0f, 5000.0f);

		RenderSystem::Current()->SetClearColor(Color32::Gray);
		RenderSystem::Current()->SetCamera(_camera);
		RenderSystem::Current()->SetDefaultLight(false);

		UI::UISystem::Instance()->Create();
		String fontName = FileSystem::Instance()->GetFullPath("defaultfont.dds");
		UI::UISystem::Instance()->SetDefaultFont(CreateFont(fontName));

		ResetFont(UI::UISystem::Instance()->GetCurrentWidget());

		return true;
	}

	bool OnButtonGLClick(Object* sender, const EventArgs& e)
	{
		RenderSystem::Current()->Destroy();
		RenderSystem::SetCurrent(gl_RenderSystem);

		OnCreate();
		OnReset();

		return true;
	}

	bool OnButtonD3D9Click(Object* sender, const EventArgs& e)
	{
		RenderSystem::Current()->Destroy();
		RenderSystem::SetCurrent(d3d9_RenderSystem);

		OnCreate();
		OnReset();

		return true;
	}

protected:
	Window* _MainWindow;
	Camera* _camera;
	UI::Panel* _Screen;
};

void EntryPoint()
{
	Engine::Instance();

#ifdef SE_STATIC
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Debug"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Release"));
#	endif
#else
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\DebugDLL"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\ReleaseDLL"));
#	endif
#endif

	Console::WriteLine("UI Sample");
	Console::WriteLine("=========");

	// Register the events
	theApp->OnInit.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnInit));
	theApp->OnExit += new EventFunctorSlot(&GameCore::OnExit);
	theApp->OnUpdate.Add(new EventMethodSlot<GameCore>(GameCore::Instance(),
		(EventMethodSlot<GameCore>::MethodDelegate)&GameCore::OnUpdate));
	theApp->OnBeginFrame.Add(new EventMethodSlot<GameCore>(GameCore::Instance(), &GameCore::OnBeginFrame));

	try
	{
		if (GameCore::Instance()->Create())
		{
			// Run the application
			if (theApp->Create())
				theApp->Run();

			GameCore::Instance()->Destroy();
		}
		else
			Console::Read();
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}
}
