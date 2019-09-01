/*=============================================================================
AnimationSequence.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ANIMATIONSEQUENCE_H_
#define _SE_ANIMATIONSEQUENCE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationTrack.h"

namespace SonataEngine
{

/// <summary>
/// Continuity Mode.
/// </summary>
enum ContinuityMode
{
	/// Off.
    Off,

	/// Clamp.
    Clamp,

    /// Cycle.
    Cycle,
    
	/// Loop.
    Loop,
    
	/// Reverse.
    Reverse,
    
	/// Linear.
    Linear,
    
	/// Relative Cycle.
    RelativeCycle
};

/** Play modes. */
enum PlayMode
{
	/// Play forwards.
	PlayMode_Forwards,

	/// Play backwards.
	PlayMode_Backwards,

	/// Play forwards and then backwards.
	PlayMode_Oscillate
};

/**
	@brief Animation Sequence.

	An animation sequence contains a list of tracks.
*/
class SE_GRAPHICS_EXPORT AnimationSequence : public NamedObject
{
public:
	typedef Array<AnimationTrack*> AnimationTrackList;

protected:
	bool _isEnabled;
	InterpolationMode _interpolation;
	ContinuityMode _continuityIn;
	ContinuityMode _continuityOut;
	AnimationTrackList _tracks;

	PlayMode _playMode;
	real32 _playSpeed;
	bool _isLooping;
	bool _isPlaying;
	TimeValue _startTime;
	TimeValue _endTime;
	TimeValue _length;
	TimeValue _currentTime;
	TimeValue _lastTime;

public:
	/** Constructor / Destructor. */
	//@{
	AnimationSequence();
	virtual ~AnimationSequence();
	//@}

	/** Properties. */
	//@{
	/** Gets or sets a value indicating whether the animation is enabled. */
	bool IsEnabled() const { return _isEnabled; }
	void SetEnabled(bool value) { _isEnabled = value; }

	/** Gets the interpolation mode of the sequence. */
	InterpolationMode GetInterpolation() const { return _interpolation; }

	/** Sets the interpolation mode of the sequence. */
	void SetInterpolation(InterpolationMode value) { _interpolation = value; }

	/** Gets the pre-continuity mode of the sequence. */
	ContinuityMode GetContinuityIn() const { return _continuityIn; }

	/** Sets the pre-continuity mode of the sequence. */
	void SetContinuityIn(ContinuityMode value) { _continuityIn = value; }

	/** Gets the post-continuity mode of the sequence. */
	ContinuityMode GetContinuityOut() const { return _continuityOut; }

	/** Sets the post-continuity mode of the sequence. */
	void SetContinuityOut(ContinuityMode value) { _continuityOut = value; }

	/** Gets or sets the play mode of the animation. */
	PlayMode GetPlayMode() const { return _playMode; }
	void SetPlayMode(PlayMode value) { _playMode = value; }

	/** Gets or sets a the speed of the animation. */
	real32 GetPlaySpeed() const { return _playSpeed; }
	void SetPlaySpeed(real32 value) { _playSpeed = value; }

	/** Gets or sets a value indicating whether the animation is looping. */
	bool IsLooping() const { return _isLooping; }
	void SetLooping(bool value) { _isLooping = value; }

	/** Gets a value indicating whether the animation is playing. */
	bool IsPlaying() const { return _isPlaying; }

	/** Gets or sets the start time of the animation. */
	const TimeValue& GetStartTime() const { return _startTime; }
	void SetStartTime(const TimeValue& value);

	/** Gets or sets the end time of the animation. */
	const TimeValue& GetEndTime() const { return _endTime; }
	void SetEndTime(const TimeValue& value);

	/** Gets the length of the animation. */
	const TimeValue& GetLength() const { return _length; }

	/** Gets or sets the current time of the animation. */
	const TimeValue& GetCurrentTime() const { return _currentTime; }
	void SetCurrentTime(const TimeValue& value) { _currentTime = value; }
	//@}

	/** Animation tracks. */
	//@{
	/** Retrieves the total number of tracks. */
	int GetAnimationTrackCount() const;

	/** Adds a new track.
		@param value The track to add.
		@return true of the track is added; otherwise, false.
	*/
	bool AddAnimationTrack(AnimationTrack* value);

	/** Removes a track.
		@param value The track to remove.
		@return true of the track is removed; otherwise, false.
	*/
	bool RemoveAnimationTrack(AnimationTrack* value);

	/** Removes all the tracks. */
	void RemoveAllAnimationTracks();

	/** Gets the index of a track.
		@param value The track.
		@return The index if found, -1 otherwise.
	*/
	int GetAnimationTrackIndex(AnimationTrack* value) const;

	/** Gets a track by its index.
		@param index The index of the track to get.
		@return The track if found, NULL otherwise.
	*/
	AnimationTrack* GetAnimationTrackByIndex(int index) const;

	/** Gets a track by its name.
		@param name The name of the track to get.
		@return The track if found, NULL otherwise.
	*/
	AnimationTrack* GetAnimationTrackByName(const String& name) const;
	//@}

	/** Updates the animation sequence with the specified time value. */
	void Update(const TimeValue& timeValue);

	/** Starts the playback if the animation is stopped. */
	void Start();

	/** Stops the playback if the animation is running. */
	void Stop();
};

}

#endif 
