/*=============================================================================
AudioResourceHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AudioResourceHandler.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Plugins/PluginManager.h"
#include "Core/Plugins/DataPlugin.h"

#include "Audio/AudioSystem.h"
#include "Audio/SoundDataPlugin.h"
#include "Audio/SoundReader.h"
#include "Audio/SoundWriter.h"

namespace SonataEngine
{
	namespace Audio
	{
		AudioResourceHandler::AudioResourceHandler() :
			ResourceHandler()
		{
		}

		AudioResourceHandler::~AudioResourceHandler()
		{
		}

		bool AudioResourceHandler::CanHandle(const SE_ID& type)
		{
			if (type == SE_ID_DATA_SOUND) return true;

			return false;
		}

		Resource* AudioResourceHandler::Load(const String& name, const SE_ID& type, const String& path, Stream& stream)
		{
			if (!CanHandle(type))
				return NULL;

			String ext = Path::GetExtension(path);

			PluginManager::PluginList::Iterator it = PluginManager::Instance()->GetPluginIterator();
			while (it.Next())
			{
				Plugin* plugin = it.Current();
				if (plugin->GetPluginType() != SE_ID_DATA)
					continue;

				DataPlugin* dataPlugin = (DataPlugin*)plugin;
				if (dataPlugin->GetDataType() != type)
					continue;

				if (dataPlugin->CanRead() && dataPlugin->CanHandle(stream))
				{
					if (!ext.IsEmpty() && stream.GetStreamType() == StreamType_File)
					{
						Array<String> extensions = dataPlugin->GetExtensions();
						bool found = false;
						for (int i=0; i<extensions.Count(); i++)
						{
							if (extensions[i].CompareTo(ext, true) == 0)
							{
								found = true;
								break;
							}
						}

						// extension not supported
						if (!found)
							continue;
					}

					if (dataPlugin->GetDataType() == SE_ID_DATA_SOUND)
					{
						SoundDataPlugin* soundPlugin = (SoundDataPlugin*)dataPlugin;
						SoundReader* reader = soundPlugin->CreateReader();
						if (reader == NULL)
						{
							return NULL;
						}

						if (!reader->OpenSound(stream))
							return NULL;

						Sound* sound = AudioSystem::Current()->OpenSoundSample(*reader);
						if (sound == NULL)
							return NULL;

						Resource* resource = new Resource();
						resource->SetResourceType(SE_ID_DATA_SOUND);
						resource->SetSize(sound->GetLength());
						resource->SetData(sound);

						return resource;
					}
				}
			}

			return NULL;
		}
		
		bool AudioResourceHandler::Save(Resource* resource, const String& path, Stream& stream)
		{
			return false;
		}

		bool AudioResourceHandler::Unload(Resource* resource)
		{
			if (resource == NULL)
				return false;

			if (!CanHandle(resource->GetResourceType()))
				return false;

			return false;
		}

		// Creates an instance of this handler
		namespace { AudioResourceHandler gAudioResourceHandler(); }
	}
}
