/*=============================================================================
Scene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Scene.h"
#include "Graphics/System/RenderSystem.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(Scene);
SE_IMPLEMENT_REFLECTION(Scene);

Scene::Scene() :
	SceneObject(),
	_root(NULL),
	_ambientColor(Color32::Black),
	_sky(NULL),
	_enableShadowPlanes(true),
	_shadowPlanesIntensity(1.0f),
	_enableShadows(true),
	_animationSet(NULL)
{
}

Scene::~Scene()
{
	_root = NULL;
}

void Scene::AddObject(SceneObject* obj)
{
	if (!_sceneObjects.Contains(obj))
	{
		_sceneObjects.Add(obj);
	}
}

SceneObject* Scene::FindObject(const String& name)
{
	SceneObjectList::Iterator it = _sceneObjects.GetIterator();
	while (it.Next())
	{
		SceneObject* obj = it.Current();
		if (obj->GetName() == name)
		{
			return obj;
		}
	}
	return NULL;
}

SceneObject* Scene::FindObjectOfType(const String& name, const TypeInfo* type)
{
	SceneObjectList::Iterator it = _sceneObjects.GetIterator();
	while (it.Next())
	{
		SceneObject* obj = it.Current();
		if (obj->GetName() == name && obj->GetType() == type)
		{
			return obj;
		}
	}
	return NULL;
}

void Scene::RemoveObject(SceneObject* obj)
{
	if (_sceneObjects.Contains(obj))
	{
		_sceneObjects.Remove(obj);
	}
}

Scene::SceneObjectList::Iterator Scene::GetSceneObjectIterator() const
{
	return _sceneObjects.GetIterator();
}

void Scene::Initialize()
{
	_root = new SceneNode();
}
void Scene::Update(const TimeValue& timeValue)
{
	if (_animationSet != NULL)
	{
		_animationSet->Update(timeValue);
	}

	if (_sky != NULL)
	{
		_sky->Update(timeValue);
	}

	if (_root != NULL)
	{
		_root->Update(timeValue);
	}
}

void Scene::Render()
{
	if (_sky != NULL)
	{
		_sky->Render();
	}
}

SEPointer(Scene);

}
