/*=============================================================================
Application.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_APPLICATION_H_
#define _SFW_APPLICATION_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/System/CommandLine.h"
#include "SampleFramework/System/EventTimer.h"

namespace SampleFramework
{

/** Base class for client applications. */
class SE_SAMPLEFRAMEWORK_EXPORT Application : public Context<Application>
{
protected:
	bool _isReady;
	CommandLine _commandLine;
	String _mediaPath;
	Window* _mainWindow;
	bool _isFullscreen;
	TimeValue _startTime;
	TimeValue _currentTime;
	TimeValue _lastTime;
	TimeValue _runTime;
	TimeValue _elapsedTime;
	TimeValue _lastFrame;
	real32 _minFramePeriod;

public:
	/** @name Constructors / Destructor. */
	//@{
	Application();
	virtual ~Application();
	//@}

	/** Gets a value indicating whether the application is ready. */
	bool IsReady() const;

	/** Gets or sets the command line. */
	const CommandLine& GetCommandLine() const { return _commandLine; }
	void SetCommandLine(const CommandLine& value) { _commandLine = value; }

	/** Gets or sets the media path. */
    const String& GetMediaPath() { return _mediaPath; }
    void SetMediaPath(const String& value) { _mediaPath = value; }

	/** Gets the main window. */
	Window* GetMainWindow() const { return _mainWindow; }

	/** Gets or sets a value indicating whether the application is in full screenmode. */
	bool IsFullscreen() const { return _isFullscreen; }
	void SetFullscreen(bool value) { _isFullscreen = value; }

	/** Gets the start time. */
	TimeValue GetStartTime() const { return _startTime; }

	/** Gets the current time. */
	TimeValue GetCurrentTime() const { return _currentTime; }

	/** Gets the elapsed time. */
	TimeValue GetElapsedTime() const { return _elapsedTime; }

	/** Gets or sets the minimum frame period. */
	real32 GetMinFramePeriod() const { return _minFramePeriod; }
	void SetMinFramePeriod(real32 value) { _minFramePeriod = value; }

	/** Creates the application. */
	virtual bool Create();

	/** Destroys the application. */
	virtual bool Destroy();

	/** Run the application. */
	virtual void Run();

	/** Starts the application and blocks until the application terminates. */
	bool Start();

	/** Terminates the execution of the application. */
	void Exit(int32 exitCode = 0);

	/** Creates the main window. */
	virtual bool CreateMainWindow();

	/** Called when the application is initialized. */
	virtual bool OnInitialize();

	/** Called when the application is being terminated. */
	virtual void OnTerminate();

	/** Called when the application is idle. */
	virtual void OnIdle();

	/** Called after a frame drawing. */
	virtual void OnDraw();

protected:
	static bool OnWindowClosed(Object* sender, const EventArgs& e);

	static bool OnWindowSized(Object* sender, const EventArgs& e);

	/**
		Updates the time of the applications.
		@return true if the elapsed time is under the frame rate limier; otherwise, false.
	*/
	bool UpdateTime();
};

}

#endif
