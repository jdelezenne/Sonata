/*=============================================================================
Thread.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_THREAD_H_
#define _SE_THREAD_H_

#include "Core/Common.h"
#include "Core/Threading/Threading.h"

namespace SonataEngine
{

/** Specifies the execution states of a Thread. */
enum ThreadState
{
	/// The thread has been started.
	ThreadState_Running,

	/// The thread has stopped.
	ThreadState_Stopped,

	/// The thread has been suspended.
	ThreadState_Suspended,

	/// The Thread.Start method has not been invoked on the thread.
	ThreadState_Unstarted
};

/** Specifies the scheduling priority of a Thread. */
enum ThreadPriority
{
	/// The Thread can be scheduled after threads with any other priority.
	ThreadPriority_Lowest,

	/// The Thread can be scheduled after threads with Normal priority and before those with Lowest priority.
	ThreadPriority_BelowNormal,

	/// The Thread can be scheduled after threads with AboveNormal priority and before those with BelowNormal priority. Threads have Normal priority by default.
	ThreadPriority_Normal,

	/// The Thread can be scheduled after threads with Highest priority and before those with Normal priority.
	ThreadPriority_AboveNormal,

	/// The Thread can be scheduled before threads with any other priority.
	ThreadPriority_Highest
};

/** Represents the method that executes on a Thread. */
typedef void* (*ThreadStart)(void);

#ifdef WIN32
	typedef uint32 ThreadId;
#elif LINUX
#	include <pthread.h>
	typedef pthread_t ThreadId;
#elif XBOX
	typedef uint32 ThreadId;
#elif XENON
	typedef uint32 ThreadId;
#elif __CELLOS_LV2__
#	include <sys/ppu_thread.h>
	typedef sys_ppu_thread_t ThreadId;
#else
	typedef uint32 ThreadId;
#endif

class ThreadInternal;

/**
	@class Thread
	@group Threading
	@brief Creates and controls a thread, sets its priority, and gets its status.
*/
class SE_CORE_EXPORT Thread
{
public:
	/** A constant used to specify an infinite waiting period. */
	static const int32 Infinite;

public:
	/** Initializes a new instance of the Thread class. */
	Thread();

	/**
		Initializes a new instance of the Thread class.
		@param start
			A ThreadStart function to be invoked when this thread begins executing.
	*/
	Thread(ThreadStart start);

	/** Destructor. */
	virtual ~Thread();

	/** Gets a unique identifier for the currently running thread. */
	static ThreadId GetCurrentThreadId();

	/**
		Blocks the current thread for the specified time.
		@param millisecondsTimeout
			The number of milliseconds for which the thread is blocked.
			Specify Infinite to block the thread indefinitely.
	*/
	static void Sleep(int32 millisecondsTimeout);

	/** Ends the currently running thread. */
	static void Exit();

	/** Gets a unique identifier for the thread. */
	ThreadId GetThreadId() const;

	/** Gets or sets a value indicating the scheduling priority of the thread. */
	ThreadPriority GetPriority() const;
	void SetPriority(ThreadPriority value);

	/** Gets a value containing the states of the thread. */
	ThreadState GetThreadState() const;

	/** Gets whether the thread is still alive. */
	bool IsAlive() const;

	/** Starts the execution of the thread. */
	void Start();

	/** Ends a thread. */
	void Abort();

	/** Suspends the thread. */
	void Suspend();

	/** Resumes a thread that has been suspended. */
	void Resume();

	/** Blocks the thread until another thread terminates or the specified time elapses. */
	void Join(int32 millisecondsTimeout);

	/** Executes the thread function. */
	virtual void Run();

private:
	ThreadInternal* _internal;
};

}

#endif 
