/*=============================================================================
AnimationSet.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ANIMATIONSET_H_
#define _SE_ANIMATIONSET_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationSequence.h"

namespace SonataEngine
{

/**
	@brief Animation Set.

	An animation set contains a list of sequences.
*/
class SE_GRAPHICS_EXPORT AnimationSet : public NamedObject
{
public:
	typedef Array<AnimationSequence*> AnimationSequenceList;

protected:
	AnimationSequenceList _sequences;

public:
	/** Constructor / Destructor. */
	//@{
	AnimationSet();
	virtual ~AnimationSet();
	//@}

	/** Properties. */
	//@{
	//@}

	/** Animation sequences. */
	//@{
	/** Retrieves the total number of sequences. */
	int GetAnimationSequenceCount() const;

	/** Adds a new sequence.
		@param value The sequence to add.
		@return true of the sequence is added; otherwise, false.
	*/
	bool AddAnimationSequence(AnimationSequence* value);

	/** Removes a sequence.
		@param value The sequence to remove.
		@return true of the sequence is removed; otherwise, false.
	*/
	bool RemoveAnimationSequence(AnimationSequence* value);

	/** Removes all the sequences. */
	void RemoveAllAnimationSequences();

	/** Gets the index of a sequence.
		@param value The sequence.
		@return The index if found, -1 otherwise.
	*/
	int GetAnimationSequenceIndex(AnimationSequence* value) const;

	/** Gets a sequence by its index.
		@param index The index of the sequence to get.
		@return The sequence if found, NULL otherwise.
	*/
	AnimationSequence* GetAnimationSequenceByIndex(int index) const;

	/** Gets a sequence by its name.
		@param name The name of the sequence to get.
		@return The sequence if found, NULL otherwise.
	*/
	AnimationSequence* GetAnimationSequenceByName(const String& name) const;
	//@}

	/** Updates the animation with the specified time value. */
	void Update(const TimeValue& timeValue);
};

}

#endif 
