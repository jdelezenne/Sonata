/*=============================================================================
Skeleton.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Skeleton.h"

namespace SonataEngine
{

Skeleton::Skeleton() :
	NamedObject(),
	_rootBone(NULL),
	_animationSet(NULL)
{
}

Skeleton::~Skeleton()
{
	RemoveAllBones();
}

Skeleton* Skeleton::Clone()
{
	Skeleton* skeleton = new Skeleton();

	skeleton->_name = _name;
	skeleton->_rootBone = _rootBone;
	skeleton->_bones = _bones;
	skeleton->_sockets = _sockets;

	return skeleton;
}

void Skeleton::SetRootBone(Bone* value)
{
	if (_bones.Contains(value))
	{
		_rootBone = value;
	}
}

int Skeleton::GetBoneCount() const
{
	return _bones.Count();
}

Skeleton::BoneList::Iterator Skeleton::GetBoneIterator() const
{
	return _bones.GetIterator();
}

Bone* Skeleton::AddBone()
{
	Bone* bone = new Bone(this, _bones.Count());
	_bones.Add(bone);
	return bone;
}

void Skeleton::RemoveAllBones()
{
	BoneList::Iterator it = _bones.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}

	_bones.Clear();
}

Bone* Skeleton::GetBoneByIndex(int index) const
{
	return _bones[index];
}

Bone* Skeleton::GetBoneByName(const String& name) const
{
	BoneList::Iterator it = _bones.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
			return it.Current();
	}

	return NULL;
}

int Skeleton::GetSocketCount() const
{
	return _sockets.Count();
}

Skeleton::SocketList::Iterator Skeleton::GetSocketIterator() const
{
	return _sockets.GetIterator();
}

void Skeleton::AddSocket(Socket* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}
	else if (value->GetParentSkeleton() != NULL)
	{
		return;
	}

	_sockets.Add(value);
	value->SetParentSkeleton(this);
}

void Skeleton::RemoveSocket(Socket* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}

	_sockets.Remove(value);
}

void Skeleton::RemoveAllSockets()
{
	_sockets.Clear();
}

Socket* Skeleton::GetSocketByIndex(int index) const
{
	return _sockets[index];
}

Socket* Skeleton::GetSocketByName(const String& name) const
{
	SocketList::Iterator it = _sockets.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
		{
			return it.Current();
		}
	}

	return NULL;
}

void Skeleton::Update(const TimeValue& timeValue)
{
	if (_animationSet != NULL)
	{
		_animationSet->Update(timeValue);
	}

	if (_rootBone != NULL)
	{
		_rootBone->Update();
	}

	SocketList::Iterator it = _sockets.GetIterator();
	while (it.Next())
	{
		it.Current()->Update();
	}
}

}
