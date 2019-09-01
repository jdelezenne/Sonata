/*=============================================================================
RWPhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RWPhysicsSystem.h"
#include "RWPhysicsScene.h"

namespace SE_RW
{

static RwMemoryFunctions MemoryFunctions;

class RWMemory
{
public:
	static void* Alloc(size_t size, RwUInt32)
	{
		return Memory::Alloc(size);
	}

	static void* Calloc(size_t num, size_t size, RwUInt32)
	{
		return Memory::Calloc(num, size);
	}

	static void* ReAlloc(void* ptr, size_t size, RwUInt32)
	{
		return Memory::ReAlloc(ptr, size);
	}

	static void Free(void* ptr)
	{
		return Memory::Free(ptr);
	}
};

static RwBool AttachPlugins()
{
	/*
	 * Attach world plug-in...
	 */
	/*if( !RpWorldPluginAttach())
	{
		return FALSE;
	}

	if (!RpCollisionPluginAttach())
	{
		return FALSE;
	}*/

	return TRUE;
}

RWPhysicsSystem::RWPhysicsSystem() :
	PhysicsSystem()
{
}

RWPhysicsSystem::~RWPhysicsSystem()
{
	Destroy();
}

PhysicsSystemDescription RWPhysicsSystem::GetDescription() const
{
	return _Description;
}

bool RWPhysicsSystem::Create(const PhysicsSystemDescription& desc)
{
	_Description = desc;

	MemoryFunctions.rwmalloc  = RWMemory::Alloc;
	MemoryFunctions.rwfree    = RWMemory::Free;
	MemoryFunctions.rwrealloc = RWMemory::ReAlloc;
	MemoryFunctions.rwcalloc  = RWMemory::Calloc;

    if (!RwEngineInit(&MemoryFunctions, 0, rsRESOURCESDEFAULTARENASIZE))
    {
		Logger::Current()->Log(LogLevel::Error, _T("RWPhysicsSystem"),
			_T("Failed to initialize the RenderWare Physics SDK."));
		return false;
    }

	if (!AttachPlugins())
	{
		Logger::Current()->Log(LogLevel::Error, _T("RWPhysicsSystem"),
			_T("Failed to attach the RenderWare plugins."));
		return false;
	}

	return true;
}

void RWPhysicsSystem::Destroy()
{
	RwEngineTerm();
}

void RWPhysicsSystem::Update(real64 elapsed)
{
	Array<ScenePtr>::Iterator it = _Scenes.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(elapsed);
	}
}

IScene* RWPhysicsSystem::CreateScene(const SceneDescription& desc)
{
	IScene* scene = new RWPhysicsScene(this, desc);
	_Scenes.Add(scene);
	return scene;
}

void RWPhysicsSystem::DestroyScene(IScene* scene)
{
	if (_Scenes.Contains(scene))
	{
		_Scenes.Remove(scene);
	}
}

}
