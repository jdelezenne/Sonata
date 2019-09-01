/*=============================================================================
D3D8RenderContext.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8RenderContext.h"

namespace SE_D3D8
{

bool CheckDepthStencilFormat(D3DFORMAT depthFormat, LPDIRECT3D8 pD3D, UINT adapter, D3DDEVTYPE deviceType, D3DFORMAT adapterFormat)
{
	// Check the depth-stencil format.
	HRESULT hr = pD3D->CheckDeviceFormat(adapter, deviceType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthFormat);

	if (SUCCEEDED(hr))
	{
		// Check whether the depth format is compatible.
		hr = pD3D->CheckDepthStencilMatch(adapter, deviceType, adapterFormat,
			adapterFormat, depthFormat);

		if (SUCCEEDED(hr))
			return true;
	}

	return false;
}

D3D8RenderContext::D3D8RenderContext() :
	RenderContext(),
	_Valid(false),
	_D3DDevice(NULL),
	_D3DSwapChain(NULL)
{
}

D3D8RenderContext::~D3D8RenderContext()
{
	Destroy();
}

bool D3D8RenderContext::IsValid() const
{
	return _Valid;
}

bool D3D8RenderContext::Create(Window* window, const RenderContextDescription& desc)
{
	if (window == NULL)
	{
		return false;
	}

	if (_Valid)
	{
		if (!Destroy())
			return false;
	}

	HWND hWnd = (HWND)window->GetHandle();
	if (hWnd == NULL)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create"),
			_T("The specified window is not valid."));
		return false;
	}

	LPDIRECT3D8 pD3D = ((D3D8RenderSystem*)RenderSystem::Current())->GetD3D();
	HRESULT hr;

	D3DPRESENT_PARAMETERS d3dpp;
	::ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = desc.Mode.GetWidth();
	d3dpp.BackBufferHeight = desc.Mode.GetHeight();

	if (desc.Mode.GetDepth() > 16)
	{
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}

	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)desc.MultiSamplingType;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = (HWND)window->GetHandle();
	d3dpp.Windowed = !desc.FullScreen;
	d3dpp.EnableAutoDepthStencil = desc.EnableDepthStencil;
	if (desc.EnableDepthStencil)
	{
		if (desc.Mode.GetDepth() > 16)
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D32;

			if (!CheckDepthStencilFormat(d3dpp.AutoDepthStencilFormat,
				pD3D, desc.Adapter.GetIndex(), D3DDEVTYPE_HAL,
				d3dpp.BackBufferFormat))
			{
				d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			}
		}
		else
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}
	}
	else
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	}

	// The refresh rate must be zero for windowed mode
	if (desc.FullScreen)
	{
		d3dpp.FullScreen_RefreshRateInHz = desc.Mode.GetRefreshRate();
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	if (desc.VSync)
    {
        d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }
    else
    {
        d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }

	DWORD behaviorFlags;
	D3DCAPS8 d3dcaps;
	hr = pD3D->GetDeviceCaps(desc.Adapter.GetIndex(), D3DDEVTYPE_HAL, &d3dcaps);
	if (FAILED(hr))
	{
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		Logger::Current()->Log(LogLevel::Warning, _T("D3D8RenderContext.Create"),
			_T("Failed to retrieve the device capabilities."));
	}
	else
	{
		if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else if (d3dcaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
		{
			behaviorFlags = D3DCREATE_PUREDEVICE;
		}
		else
		{
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
	}

	hr = pD3D->CreateDevice(desc.Adapter.GetIndex(), D3DDEVTYPE_HAL,
		hWnd, behaviorFlags, &d3dpp, &_D3DDevice);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderContext.Create"),
			_T("Failed to create the Direct3D8 device."));
		return false;
	}

	_Window = window;
	_Description = desc;
	_Valid = true;

	return true;
}

bool D3D8RenderContext::Destroy()
{
	if (!_Valid)
		return false;

	_Window = NULL;
	_Valid = false;

	return true;
}

void D3D8RenderContext::SwapBuffers()
{
	//if (_Window != NULL)
	//	::SwapBuffers(::GetDC((HWND)_Window->GetHandle()));
}

}
