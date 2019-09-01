/*=============================================================================
XModelPlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XModelPlugin.h"
#include "XModelReader.h"
#include "SDKMESHModelReader.h"
#include "XATGModelReader.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_X
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[3]=
{
	PluginDescription(
		SE_ID_DATAMODEL_X,
		0x01,
		"Microsoft DirectX X",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_DATAMODEL_SDKMESH,
		0x01,
		"Microsoft DirectX SDKMesh",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_DATAMODEL_XATG,
		0x01,
		"Microsoft Xbox XATG",
		"Julien Delezenne")
};

class XModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	XModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("x");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new XModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

class SDKMESHModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	SDKMESHModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("sdkmesh");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[1];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new SDKMESHModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

class XATGModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	XATGModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("xatg");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[2];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new XATGModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT int GetPluginCount()
{
	return 3;
}

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new XModelPlugin();
		case 1: return new SDKMESHModelPlugin();
		case 2: return new XATGModelPlugin();
	}
	return NULL;
}

SE_EXPORT void DestroyPlugin(Plugin* plugin)
{
	SE_DELETE(plugin);
}

#ifndef SE_STATIC
}
#endif

#ifdef SE_STATIC
PluginModule::PluginModule()
{
	PluginSymbols symbols;
	symbols.GetPluginCount = GetPluginCount;
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

}
