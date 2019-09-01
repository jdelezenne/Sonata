/*=============================================================================
AnimationTrack.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ANIMATIONTRACK_H_
#define _SE_ANIMATIONTRACK_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/KeyFrame.h"

namespace SonataEngine
{

/**
	@brief Animation track.

	A track represents a subset of an animation sequence.
	Each type of track is defined by a set of specific key frames.
*/
class SE_GRAPHICS_EXPORT AnimationTrack : public NamedObject
{
	friend KeyFrame;

public:
	typedef List<KeyFrame*> KeyFrameList;

protected:
	bool _isEnabled;
	KeyFrameList _keyFrames;
	TimeValue _startTime;
	TimeValue _endTime;
	TimeValue _length;
	TimeValue _currentTime;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	AnimationTrack();

	/** Destructor. */
	virtual ~AnimationTrack();
	//@}

	/** Properties. */
	//@{
	/** Gets or sets a value indicating whether the animation track is enabled. */
	bool IsEnabled() const { return _isEnabled; }
	void SetEnabled(bool value) { _isEnabled = value; }

	/** Gets or sets the start time of the animation track. */
	const TimeValue& GetStartTime() const { return _startTime; }
	void SetStartTime(const TimeValue& value);

	/** Gets or sets the end time of the animation track. */
	const TimeValue& GetEndTime() const { return _endTime; }
	void SetEndTime(const TimeValue& value);

	/** Gets the length of the animation track. */
	const TimeValue& GetLength() const { return _length; }

	/** Gets or sets the current time of the animation track. */
	const TimeValue& GetCurrentTime() const { return _currentTime; }
	void SetCurrentTime(const TimeValue& value) { _currentTime = value; }
	//@}

	/** @name Key Frames. */
	//@{
	/** Retrieves the total number of key frames.
		@remarks
			To retrieve the number of key frame of a given type, see GetKeyFrameCountByType.
	*/
	int GetKeyFrameCount() const;

	/** Creates a new key frame at a given time value.
		@remarks
			The key frame is automatically added to the track.
			The key frames are sorted by time value.
			If a key frame already exists at the same time value,
			this key frame is returned instead.
		@param time The time value.
	*/
	virtual KeyFrame* AddKeyFrame(const TimeValue& timeValue);

	/** Removes a key frame.
		@param key frame The key frame to remove.
	*/
	void RemoveKeyFrame(KeyFrame* value);

	/** Removes all the key frames. */
	void RemoveAllKeyFrames();

	/** Gets the index of a key frame.
		@param key frame The key frame.
		@return The index if found, -1 otherwise.
	*/
	int GetKeyFrameIndex(KeyFrame* value) const;

	/** Gets a key frame by its index.
		@param index The index of the key frame to get.
		@return The key frame if found, NULL otherwise.
	*/
	KeyFrame* GetKeyFrameByIndex(int index) const;
	//@}

	/** @name Operations. */
	//@{
	/** Sets the start time and end time of the animation track. */
	void SetStartEndTimes(const TimeValue& startTime, const TimeValue& endTime);

	/** Sets the length of the animation track by setting the start time to zero and the end time to the specified length. */
	void SetLength(const TimeValue& length);

	/** Retrieves the number of key frame of a given type.
		To retrieve the total number of key frame, see GetKeyFrameCount.
	*/
	KeyFrame* GetKeyFrameAtTime(const TimeValue& timeValue) const;

	/** Retrieves the key frames around a given time value.
		@remarks
			The two returned key frames can be the same.
			To retrieve the total number of key frame, see GetKeyFrameCount.
		@param time The time value.
		@param keyA The key frame before the time value.
		@param keyB The key frame after the time value.
		@return The time difference between the two key frames.
	*/
	TimeValue GetKeyFramesAtTime(const TimeValue& timeValue, KeyFrame** keyA, KeyFrame** keyB) const;

	/** Sets the time value of the specified key frame.
		This method will sort the key frames.
	*/
	void SetKeyFrameTime(KeyFrame* keyFrame, const TimeValue& timeValue);

	/** Updates the animation track with the specified time value. */
	virtual void Update(const TimeValue& timeValue);
	//@}

protected:
	/** Creates a specialized key frame. */
	virtual KeyFrame* _CreateKeyFrame(const TimeValue& timeValue) = 0;
};

}

#endif 
