/*=============================================================================
SceneNode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENENODE_H_
#define _SE_SCENENODE_H_

#include "Graphics/Scene/SceneObject.h"

namespace SonataEngine
{

/** @brief Scene node.
	Represents a node in a scene graph.
*/
class SE_GRAPHICS_EXPORT SceneNode : public SceneObject
{
	SE_DECLARE_CLASS(SceneNode, SceneObject);
	SE_BEGIN_REFLECTION(SceneNode);
	SE_END_REFLECTION(SceneNode);

public:
	typedef Array<SceneObjectPtr> SceneNodeList;

protected:
	SceneNodeList _children;
	BoundingBox _childBoundingBox;
	BoundingSphere _childBoundingSphere;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	SceneNode();

	/** Destructor. */
	virtual ~SceneNode();
	//@}

	/** Returns whether this object is a node. */
	virtual bool IsNode() const { return true; }

	/** Gets a value indicating whether the specified object is a child of the current object. */
	bool IsChild(SceneObject* value);

	/**
		Gets the number of objects in the list.
		@return The number of objects in the list.
	*/
	int GetChildCount() const;

	/**
		Gets the child list iterator.
		@return The child list iterator.
	*/
	SceneNodeList::Iterator GetChildIterator() const;

	/**
		Adds an object to the end of the child list.
		@param value The object to add to the child list.
		@return true if the object is added; otherwise, false.
	*/
	bool AddChild(SceneObject* value);

	/**
		Inserts an object in the child list at the specified location.
		@param index Zero-based index of the object to insert.
		@param value The object to insert to the child list.
		@return true if the object is inserted; otherwise, false.
	*/
	bool InsertChild(int index, SceneObject* value);

	/**
		Removes an object from the child list.
		@param value The object to remove from the child list.
		@return true if the object is removed; otherwise, false.
	*/
	bool RemoveChild(SceneObject* value);

	/**
		Removes all the object from the child list.
	*/
	void RemoveAllChildren();

	/**
		Gets the object at the specified index.
		@param index Zero-based index of the object to get.
		@return The object at the specified index.
	*/
	SceneObject* GetChild(int index) const;

	/**
		Sets the object at the specified index.
		@param index Zero-based index of the object to set.
		@param value The object to add to the child list.
		@return true if the object is set; otherwise, false.
	*/
	bool SetChild(int index, SceneObject* value);

	virtual SceneObject* GetObjectByName(const String& name) const;

	virtual void Update(const TimeValue& timeValue);

	virtual void Render();

protected:
	virtual void _NotifyLocalTransformUpdate();
	virtual void _NotifyWorldTransformUpdate();
	virtual void _NotifyWorldBoundsUpdate();

	virtual void _UpdateWorldBounds();
};

SEPointer(SceneNode);

}

#endif
