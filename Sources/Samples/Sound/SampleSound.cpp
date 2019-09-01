/*=============================================================================
SampleSound.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SampleSound.h"

Application* theApp = Application::Instance();

class GameCore : public Singleton<GameCore>
{
public:
	GameCore()
	{
	}

	bool Create()
	{
		LogHandler* debugHandler = new Win32DebugLogHandler();
		((DefaultLogFormatter*)debugHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_Source | LogOptions_Time));

		Logger* logger = new Logger();
		logger->GetHandlers().Add(debugHandler);
		Logger::SetCurrent(logger);

		FileSystem::Instance()->AddRootPath(_T("Data"));
		FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
		FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
		PluginManager::Instance()->ParsePlugins(Environment::GetCurrentDirectory());
		PluginManager::Instance()->CreateAllPlugins();

		if (!CreateAudioSystem())
			return false;

		return true;
	}

	void Destroy()
	{
	}

	bool CreateAudioSystem()
	{
		ManagerPlugin* ds8 = (ManagerPlugin*)PluginManager::Instance()->GetPluginByID(SE_ID_AUDIOSYSTEM_DS8);
		if (ds8 == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("Failed to get the DirectSound8 plugin."));
			return false;
		}

		AudioSystem::SetCurrent((AudioSystem*)ds8->CreateManager());

		AudioSystemDescription desc;
		AudioSystem::Current()->Create(desc);

		return true;
	}

protected:
};

void EntryPoint()
{
	Console::WriteLine("Sound Sample");
	Console::WriteLine("============");

	try
	{
		if (!GameCore::Instance()->Create())
			return;

		Resource* resource = ResourceHelper::LoadFromFile(_T("Sounds\\Counting.wav"), SE_ID_DATA_SOUND);
		if (resource == NULL)
			return;

		Sound* sound = (Sound*)resource->GetData();
		//Sound* sound = AudioSystem::Current()->OpenSoundSample(_T("Sounds\\test.wav"));
		if (sound == NULL)
			return;

		Channel* channel;

		SEchar key;
		do
		{
			//key = Console::ReadKey()
			key = Console::Read();

			switch (key)
			{
			case '1':
				channel = AudioSystem::Current()->CreateChannel(sound);
				channel->Play();
				break;
			}

			AudioSystem::Current()->Update();

		} while (key != 27);
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}

	AudioSystem::Current()->Destroy();

	GameCore::Instance()->Destroy();
}
