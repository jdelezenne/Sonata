/*=============================================================================
NXPhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NXPhysicsSystem.h"
#include "NXPhysicsScene.h"

namespace SE_NX
{

MyAllocator::MyAllocator()
{
}

MyAllocator::~MyAllocator()
{
}

void* MyAllocator::malloc(NxU32 size)
{
	return Memory::Alloc(size);
}

void* MyAllocator::mallocDEBUG(NxU32 size, const char* fileName, int line)
{
	return Memory::Alloc(size);
}

void* MyAllocator::mallocDEBUG(NxU32 size, const char* fileName, int line, const char* className, NxMemoryType type)
{
	return Memory::Alloc(size);
}

void* MyAllocator::realloc(void* memory, NxU32 size)
{
	if (memory)
		return Memory::ReAlloc(memory, size);
	else
		return NULL;
}

void MyAllocator::free(void* memory)
{
	if (memory)
		Memory::Free(memory);
}


class MyErrorStream : public NxUserOutputStream
{
public:
	void reportError(NxErrorCode e, const char* message, const char* file, int line)
	{
		String error;

		switch (e)
		{
		case NXE_INVALID_PARAMETER:
			error = "invalid parameter";
			break;
		case NXE_INVALID_OPERATION:
			error = "invalid operation";
			break;
		case NXE_OUT_OF_MEMORY:
			error = "out of memory";
			break;
		case NXE_DB_INFO:
			error = "info";
			break;
		case NXE_DB_WARNING:
			error = "warning";
			break;
		default:
			error = "unknown error";
		}

		Logger::Current()->Log(LogLevel::Error, _T("PhysX"),
			error + _T(": ") + message +
			_T(" (") + String(file) + _T(" line ") + String(line) + _T(")"));

		if (e < 100)
		{
			SEthrow(Exception("PhysX SDK error."));
		}
	}

	NxAssertResponse reportAssertViolation(const char * message, const char *file, int line)
	{
		Logger::Current()->Log(LogLevel::Fatal, _T("PhysX"),
			_T("Access violation: ") + String(message) +
			_T(" (") + String(file) + _T(" line ") + String(line) + _T(")"));

		SE_ASSERT(false);
		return NX_AR_CONTINUE;
	}

	void print(const char* message)
	{
		Logger::Current()->Log(LogLevel::Debug, _T("PhysX"), message);
	}
};

MyErrorStream g_myErrorStream;

NXPhysicsSystem::NXPhysicsSystem() :
	PhysicsSystem()
{
	_NxPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, &_myAllocator, &g_myErrorStream);
	if (_NxPhysicsSDK == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("NXPhysicsSystem"),
			_T("Failed to initialize the PhysX SDK."));
	}

	_NxPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.02);

	NxInitCooking();
}

NXPhysicsSystem::~NXPhysicsSystem()
{
	Destroy();
}

PhysicsSystemDescription NXPhysicsSystem::GetDescription() const
{
	return _Description;
}

bool NXPhysicsSystem::Create(const PhysicsSystemDescription& desc)
{
	_Description = desc;

	return true;
}

void NXPhysicsSystem::Destroy()
{
	if (_NxPhysicsSDK != NULL)
	{
		_NxPhysicsSDK->release();
		_NxPhysicsSDK = NULL;
	}
}

void NXPhysicsSystem::Update(real64 elapsed)
{
	Array<ScenePtr>::Iterator it = _Scenes.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(elapsed);
	}
}

IScene* NXPhysicsSystem::CreateScene(const SceneDescription& desc)
{
	IScene* scene = new NXPhysicsScene(this, desc);
	_Scenes.Add(scene);
	return scene;
}

void NXPhysicsSystem::DestroyScene(IScene* scene)
{
	if (_Scenes.Contains(scene))
	{
		_Scenes.Remove(scene);
	}
}

}
