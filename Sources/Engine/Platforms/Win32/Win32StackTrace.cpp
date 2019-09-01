/*=============================================================================
Win32StackTrace.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/Debug/StackTrace.h"

#include <dbghelp.h>

namespace SonataEngine
{

struct SymbolsCallbackContext
{
	uint64 Offset;
	BaseArray<FrameMethodParameter>* Parameters;
};

BOOL CALLBACK EnumSymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	if ((pSymInfo->Flags & SYMFLAG_PARAMETER) != 0)
	{
		SymbolsCallbackContext *callbackContext = (SymbolsCallbackContext*)UserContext;

		FrameMethodParameter parameter;
		parameter.Name = pSymInfo->Name;
		parameter.Offset = pSymInfo->Address;
		parameter.Value = *((uint32*)(callbackContext->Offset + pSymInfo->Address - 8));

		callbackContext->Parameters->Add(parameter);
	}

	return TRUE;
}

void StackTrace::CaptureCallStack()
{
	// Retrieves the context of the currend thread
	HANDLE hThread = ::GetCurrentThread();
	CONTEXT context;
	::ZeroMemory(&context, sizeof(context));
	context.ContextFlags = CONTEXT_FULL;
	::GetThreadContext(hThread, &context);

	// Initializes the symbol handler for the current process
	HANDLE hProcess = ::GetCurrentProcess();
	::SymInitialize(hProcess, NULL, TRUE);

	// Sets the options mask
	::SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

	// Retrieves the stack trace
	STACKFRAME64 stackFrame;
	::ZeroMemory(&stackFrame, sizeof(stackFrame));
	stackFrame.AddrPC.Offset = context.Eip;
	stackFrame.AddrPC.Mode = AddrModeFlat; //Flat addressing
	stackFrame.AddrFrame.Offset = context.Ebp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;

	for (uint32 index = 0; ; index++)
	{
		if (!StackWalk64(
			IMAGE_FILE_MACHINE_I386, //Intel (32-bit)
			hProcess,
			hThread,
			&stackFrame,
			&context,
			NULL, //memory read services
			SymFunctionTableAccess64, //access to the run-time function table for the process
			SymGetModuleBase64, //module base for any given virtual address
			NULL //address translation for 16-bit addresses
			) || stackFrame.AddrPC.Offset == 0)
		{
			break;
		}

		StackFrame* frame = new StackFrame();
		frame->_Method.Offset = stackFrame.AddrPC.Offset;

		//Retrieves symbol information for the specified address
		static const uint32 symbolBufferSize = (sizeof(SYMBOL_INFO) + MAX_SYM_NAME + sizeof(ULONG64) - 1) / sizeof(ULONG64);
		static ULONG64 symbolBuffer[symbolBufferSize];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO) symbolBuffer;
		DWORD64 dwOffset;

		::ZeroMemory(pSymbol, symbolBufferSize);
		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;
		if (::SymFromAddr(hProcess, stackFrame.AddrPC.Offset, &dwOffset, pSymbol))
		{
			frame->_Method.Name = pSymbol->Name;
		}

		// Locates the source line for the specified address
		IMAGEHLP_LINE64 line;
		DWORD dwDisplacement;

		::ZeroMemory(&line, sizeof(IMAGEHLP_LINE64));
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		if (::SymGetLineFromAddr64(hProcess, stackFrame.AddrPC.Offset, &dwDisplacement, &line))
		{
			frame->_fileName = line.FileName;
			frame->_LineNumber = line.LineNumber;
		}

		// Enumerates all symbols in the process
		IMAGEHLP_STACK_FRAME sf;
		::ZeroMemory(&sf, sizeof(IMAGEHLP_STACK_FRAME));
		sf.InstructionOffset = stackFrame.AddrPC.Offset;
		sf.ReturnOffset = stackFrame.AddrReturn.Offset;
		sf.FrameOffset = stackFrame.AddrFrame.Offset;
		sf.StackOffset = stackFrame.AddrStack.Offset;
		sf.BackingStoreOffset = stackFrame.AddrBStore.Offset;
		sf.FuncTableEntry = (ULONG64) stackFrame.FuncTableEntry;
		sf.Params[0] = stackFrame.Params[0];
		sf.Params[1] = stackFrame.Params[1];
		sf.Params[2] = stackFrame.Params[2];
		sf.Params[3] = stackFrame.Params[3];
		sf.Virtual = stackFrame.Virtual;

		if (::SymSetContext(hProcess, &sf, NULL))
		{
			SymbolsCallbackContext callbackContext;
			callbackContext.Offset = stackFrame.AddrPC.Offset;
			callbackContext.Parameters = &frame->_Method.Parameters;

			if (!::SymEnumSymbols(
				hProcess,
				NULL, // base address of the module
				"*", // mask
				&EnumSymbolsCallback, // callback
				&callbackContext // user context
				))
			{
			}
		}

		_stackFrames.Add(frame);
	}

	// Deallocates all resources associated with the process handle
	::SymCleanup(hProcess);
}

}
