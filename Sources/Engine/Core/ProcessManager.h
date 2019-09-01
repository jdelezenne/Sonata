/*=============================================================================
ProcessManager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PROCESSMANAGER_H_
#define _SE_PROCESSMANAGER_H_

#include "Core/Common.h"
#include "Core/Singleton.h"
#include "Core/IProcess.h"
#include "Core/Containers/Array.h"

namespace SonataEngine
{

/** Base class for the managers. */
class SE_CORE_EXPORT ProcessManager : public Singleton<ProcessManager>
{
public:
	typedef Array<IProcess*> ProcessList;

	/** @name Constructors / Destructor. */
	//@{
	ProcessManager();
	virtual ~ProcessManager();
	//@}

	/** Processes. */
	//@{
	//TODO
	//@}

	IProcess* GetActiveProcess() const { return _ActiveProcess; }
	void SetActiveProcess(IProcess* activeProcess) { _ActiveProcess = activeProcess; }

protected:
	ProcessList _Processes;
	IProcess* _ActiveProcess;
};

}

#endif 
