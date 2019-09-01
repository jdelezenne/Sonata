/*=============================================================================
AnimationSet.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AnimationSet.h"

namespace SonataEngine
{

AnimationSet::AnimationSet() :
	NamedObject()
{
}

AnimationSet::~AnimationSet()
{
	AnimationSequenceList::Iterator it = _sequences.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

int AnimationSet::GetAnimationSequenceCount() const
{
	return _sequences.Count();
}

bool AnimationSet::AddAnimationSequence(AnimationSequence* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		_sequences.Add(value);
		return true;
	}
}

bool AnimationSet::RemoveAnimationSequence(AnimationSequence* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		_sequences.Remove(value);
		return true;
	}
}

void AnimationSet::RemoveAllAnimationSequences()
{
	_sequences.Clear();
}

int AnimationSet::GetAnimationSequenceIndex(AnimationSequence* value) const
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return -1;
	}
	else
	{
		return _sequences.IndexOf(value);
	}
}

AnimationSequence* AnimationSet::GetAnimationSequenceByIndex(int index) const
{
	return _sequences[index];
}

AnimationSequence* AnimationSet::GetAnimationSequenceByName(const String& name) const
{
	AnimationSequenceList::Iterator it = _sequences.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
		{
			return it.Current();
		}
	}

	return NULL;
}

void AnimationSet::Update(const TimeValue& timeValue)
{
	TimeValue newTime = timeValue;

	// Updates the sequences.
	AnimationSequenceList::Iterator it = _sequences.GetIterator();
	while (it.Next())
	{
		AnimationSequence* sequence = it.Current();
		if (sequence->IsEnabled())
		{
			sequence->Update(newTime);
		}
	}
}

}
