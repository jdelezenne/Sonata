/*=============================================================================
Application.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Application.h"

#include <Graphics/GraphicsResourceHandler.h>
#include <Audio/AudioResourceHandler.h>

using namespace SonataEngine::Input;
using namespace SonataEngine::Audio;

namespace SampleFramework
{

static GraphicsResourceHandler* s_GraphicsResourceHandler;
static AudioResourceHandler* s_AudioResourceHandler;

Application::Application() :
	_isReady(false),
	_mainWindow(NULL),
	_isFullscreen(false)
{
	s_GraphicsResourceHandler = new GraphicsResourceHandler();
	s_AudioResourceHandler = new AudioResourceHandler();

	_minFramePeriod = 1.0f / 100.0f;
}

Application::~Application()
{
	SE_DELETE(s_GraphicsResourceHandler);
	SE_DELETE(s_AudioResourceHandler);
}

bool Application::CreateMainWindow()
{
	_mainWindow = new Window();
	_mainWindow->Create("Sample Application", -1, -1);
	_mainWindow->Closed += new DefaultEventDelegateF(&OnWindowClosed);
	_mainWindow->Sized += new DefaultEventDelegateF(&OnWindowSized);
	return true;
}

bool Application::OnWindowClosed(Object* sender, const EventArgs& e)
{
	Application::Current()->Exit();
	return false;
}

bool Application::OnWindowSized(Object* sender, const EventArgs& e)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer != NULL)
	{
		Window* window = Application::Current()->GetMainWindow();
		renderer->SetViewport(Viewport(0, 0, window->GetClientWidth(), window->GetClientHeight()));
	}

	return true;
}

bool Application::Create()
{
	if (_isReady)
	{
		return false;
	}

	_startTime = TimeValue::GetTime();

	if (!OnInitialize())
	{
		Exit();
		return false;
	}

	_isReady = true;

	return true;
}

bool Application::Destroy()
{
	if (!_isReady)
	{
		return false;
	}

	OnTerminate();

	if (_mainWindow != NULL)
	{
		_mainWindow->Destroy();
		SE_DELETE(_mainWindow);
	}

	_isReady = false;

	return true;
}

bool Application::UpdateTime()
{
	TimeValue currentTime = TimeValue::GetTime();

	if (!_mainWindow->IsActive())
	{
		_lastTime = currentTime;
		return false;
	}

	if ((real64)_lastTime == 0.0)
	{
		_lastTime = currentTime;
	}

	_elapsedTime = (currentTime - _lastTime);
	_lastTime = _currentTime;
	_currentTime = currentTime;

	_runTime += _elapsedTime;

	if (_runTime < (_lastFrame + TimeValue(_minFramePeriod)))
	{
		return false;
	}

	_lastFrame = _runTime;

	return true;
}

void Application::Run()
{
	if (!_isReady)
	{
		return;
	}

	if (_mainWindow != NULL)
	{
		_mainWindow->Update();
	}

	if (!UpdateTime())
	{
		return;
	}

	// Update the input system
	InputSystem* inputSystem = InputSystem::Current();
	if (inputSystem != NULL)
	{
		inputSystem->ReadAllDevices();
	}

	// Idle
	OnIdle();

	if (!_isReady)
	{
		return;
	}

	// Rendering
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer != NULL)
	{
		renderer->Clear();
		renderer->BeginScene();

		OnDraw();

		renderer->EndScene();
		renderer->SwapBuffers();
	}
}

bool Application::Start()
{
	while (_isReady)
	{
		Run();
	}

	return true;
}

void Application::Exit(int32 exitCode)
{
	Destroy();
}

bool Application::OnInitialize()
{
	return true;
}

void Application::OnTerminate()
{
}

void Application::OnIdle()
{
}

void Application::OnDraw()
{
}

}
