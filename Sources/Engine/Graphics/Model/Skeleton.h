/*=============================================================================
Skeleton.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SKELETON_H_
#define _SE_SKELETON_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Model/Bone.h"
#include "Graphics/Model/Socket.h"
#include "Graphics/Animation/AnimationSet.h"

namespace SonataEngine
{

/** Skeleton. */
class SE_GRAPHICS_EXPORT Skeleton : public NamedObject
{
public:
	typedef Array<Bone*> BoneList;
	typedef Array<Socket*> SocketList;

protected:
	Bone* _rootBone;
	BoneList _bones;
	SocketList _sockets;
	AnimationSet* _animationSet;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Skeleton();

	/** Destructor. */
	virtual ~Skeleton();
	//@}

	/** Clone this object. */
	Skeleton* Clone();

	/** @name Properties. */
	//@{
	/** Gets or sets the root bone. */
	Bone* GetRootBone() const { return _rootBone; }
	void SetRootBone(Bone* value);

	/** Bones. */
	//@{
	int GetBoneCount() const;
	BoneList::Iterator GetBoneIterator() const;
	Bone* AddBone();
	void RemoveAllBones();
	Bone* GetBoneByIndex(int index) const;
	Bone* GetBoneByName(const String& name) const;
	//@}

	/** Sockets. */
	//@{
	int GetSocketCount() const;
	SocketList::Iterator GetSocketIterator() const;
	void AddSocket(Socket* value);
	void RemoveSocket(Socket* value);
	void RemoveAllSockets();
	Socket* GetSocketByIndex(int index) const;
	Socket* GetSocketByName(const String& name) const;
	//@}

	/** Gets the animation set. */
	AnimationSet* GetAnimationSet() const { return _animationSet; }

	/** Sets the animation set. */
	void SetAnimationSet(AnimationSet* value) { _animationSet = value; }

	/**
		Updates the skeleton.
		This method needs to be called to let the skeleton updates the
		bone transforms and the socket attachments.
	*/
	void Update(const TimeValue& timeValue);
};

typedef SmartPtr<Skeleton> SkeletonPtr;

}

#endif
