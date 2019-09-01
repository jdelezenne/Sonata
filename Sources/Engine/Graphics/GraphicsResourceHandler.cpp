/*=============================================================================
GraphicsResourceHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GraphicsResourceHandler.h"

#include "Graphics/Image.h"
#include "Graphics/IO/ImageDataPlugin.h"
#include "Graphics/IO/ImageReader.h"
#include "Graphics/IO/ImageWriter.h"

#include "Graphics/Model/Model.h"
#include "Graphics/IO/ModelDataPlugin.h"
#include "Graphics/IO/ModelReader.h"
#include "Graphics/IO/ModelWriter.h"

#include "Graphics/Scene/Scene.h"
#include "Graphics/IO/SceneDataPlugin.h"
#include "Graphics/IO/SceneReader.h"
#include "Graphics/IO/SceneWriter.h"

namespace SonataEngine
{

GraphicsResourceHandler::GraphicsResourceHandler() :
	ResourceHandler()
{
}

GraphicsResourceHandler::~GraphicsResourceHandler()
{
}

#define SE_ID_DATA_FONT SE_ID_DATA
#define SE_ID_DATA_SHADER SE_ID_DATA

bool GraphicsResourceHandler::CanHandle(const SE_ID& type)
{
	if (type == SE_ID_DATA_IMAGE) return true;
	if (type == SE_ID_DATA_FONT) return true;
	if (type == SE_ID_DATA_SCENE) return true;
	if (type == SE_ID_DATA_MODEL) return true;
	if (type == SE_ID_DATA_SHADER) return true;

	return false;
}

Resource* GraphicsResourceHandler::Load(const String& name, const SE_ID& type, const String& path, Stream& stream)
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

			if (dataPlugin->GetDataType() == SE_ID_DATA_IMAGE)
			{
				ImageDataPlugin* imagePlugin = (ImageDataPlugin*)dataPlugin;
				ImageReader* reader = imagePlugin->CreateReader();
				if (reader == NULL)
				{
					return NULL;
				}

				Image* image = reader->LoadImage(stream);
				imagePlugin->DestroyReader(reader);

				if (image == NULL)
				{
					return NULL;
				}

				Resource* resource = new Resource();
				resource->SetResourceType(SE_ID_DATA_IMAGE);
				resource->SetSize(image->GetDataSize());
				resource->SetData(image);

				return resource;
			}

			else if (dataPlugin->GetDataType() == SE_ID_DATA_SCENE)
			{
				SceneDataPlugin* scenePlugin = (SceneDataPlugin*)dataPlugin;
				SceneReader* reader = scenePlugin->CreateReader();
				if (reader == NULL)
				{
					return NULL;
				}

				Scene* scene = reader->LoadScene(stream);
				scenePlugin->DestroyReader(reader);

				if (scene == NULL)
				{
					return NULL;
				}

				Resource* resource = new Resource();
				resource->SetResourceType(SE_ID_DATA_SCENE);
				resource->SetSize(0);
				resource->SetData(scene);

				return resource;
			}

			else if (dataPlugin->GetDataType() == SE_ID_DATA_MODEL)
			{
				ModelDataPlugin* modelPlugin = (ModelDataPlugin*)dataPlugin;
				ModelReader* reader = modelPlugin->CreateReader();
				if (reader == NULL)
				{
					return NULL;
				}

				Model* model = reader->LoadModel(stream);
				modelPlugin->DestroyReader(reader);

				if (model == NULL)
				{
					return NULL;
				}

				Resource* resource = new Resource();
				resource->SetResourceType(SE_ID_DATA_MODEL);
				resource->SetSize(0);
				resource->SetData(model);

				return resource;
			}
		}
	}

	return NULL;
}

bool GraphicsResourceHandler::Save(Resource* resource, const String& path, Stream& stream)
{
	if (resource == NULL)
		return false;

	if (!CanHandle(resource->GetResourceType()))
		return false;

	String ext = Path::GetExtension(path);

	PluginManager::PluginList::Iterator it = PluginManager::Instance()->GetPluginIterator();
	while (it.Next())
	{
		Plugin* plugin = it.Current();
		if (plugin->GetPluginType() != SE_ID_DATA)
			continue;

		DataPlugin* dataPlugin = (DataPlugin*)plugin;
		if (dataPlugin->GetDataType() != resource->GetResourceType())
			continue;

		if (dataPlugin->CanWrite() && dataPlugin->CanHandle(stream))
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

				// Extension not supported
				if (!found)
				{
					continue;
				}
			}

			if (dataPlugin->GetDataType() == SE_ID_DATA_IMAGE)
			{
				ImageDataPlugin* imagePlugin = (ImageDataPlugin*)dataPlugin;
				ImageWriter* writer = imagePlugin->CreateWriter();
				if (writer == NULL)
				{
					return NULL;
				}

				Image* image = (Image*)resource->GetData();
				bool res = writer->SaveImage(stream, image);
				imagePlugin->DestroyWriter(writer);

				return res;
			}

			else if (dataPlugin->GetDataType() == SE_ID_DATA_SCENE)
			{
				SceneDataPlugin* scenePlugin = (SceneDataPlugin*)dataPlugin;
				SceneWriter* writer = scenePlugin->CreateWriter();
				if (writer == NULL)
				{
					return NULL;
				}

				Scene* scene = (Scene*)resource->GetData();
				bool res = writer->SaveScene(stream, scene);
				scenePlugin->DestroyWriter(writer);

				return res;
			}

			else if (dataPlugin->GetDataType() == SE_ID_DATA_MODEL)
			{
				ModelDataPlugin* modelPlugin = (ModelDataPlugin*)dataPlugin;
				ModelWriter* writer = modelPlugin->CreateWriter();
				if (writer == NULL)
				{
					return NULL;
				}

				Model* model = (Model*)resource->GetData();
				bool res = writer->SaveModel(stream, model);
				modelPlugin->DestroyWriter(writer);

				return res;
			}
		}
	}

	return false;
}

bool GraphicsResourceHandler::Unload(Resource* resource)
{
	if (resource == NULL)
		return false;

	if (!CanHandle(resource->GetResourceType()))
		return false;

	return false;
}

// Creates an instance of this handler
namespace { GraphicsResourceHandler gGraphicsResourceHandler(); }

}
