/*=============================================================================
EntryPoint.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _ENTRYPOINT_H_
#define _ENTRYPOINT_H_

//#include <Framework/System/Application.h>

extern void EntryPoint();

#if defined (WIN32) && !defined(_CONSOLE)
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = 0;
	LPWSTR* argv;

	args = CommandLineToArgvW(GetCommandLineW(), &argc);
	//SonataEngine::Application::Instance()->SetCommandLine(SonataEngine::CommandLine(argc, argv));

	EntryPoint();
	return 0;
}

#elif defined (_XBOX)
extern VOID __cdecl main()
{
	EntryPoint();
}

#else
extern int main(int argc, char** argv)
{
	//Framework::Application::Instance()->SetCommandLine(Framework::CommandLine(argc, argv));

	EntryPoint();
	return 0;
}
#endif

#endif 
