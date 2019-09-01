/*=============================================================================
D3D9RenderContext.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9RenderContext.h"

namespace SE_D3D9
{

bool CheckDepthStencilFormat(D3DFORMAT depthFormat, LPDIRECT3D9 pD3D, UINT adapter, D3DDEVTYPE deviceType, D3DFORMAT adapterFormat)
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

D3D9RenderContext::D3D9RenderContext() :
	RenderContext(),
	_isValid(false),
	_D3DDevice(NULL),
	_D3DSwapChain(NULL)
{
}

D3D9RenderContext::~D3D9RenderContext()
{
	Destroy();
}

bool D3D9RenderContext::IsValid() const
{
	return _isValid;
}

bool D3D9RenderContext::Create(Window* window, const RenderContextDescription& desc)
{
	if (window == NULL)
	{
		return false;
	}

	if (_isValid)
	{
		if (!Destroy())
			return false;
	}

	HWND hWnd = (HWND)window->GetHandle();
	if (hWnd == NULL)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("D3D9RenderContext.Create"),
			_T("The specified window is not valid."));
		return false;
	}

	LPDIRECT3D9 pD3D = ((D3D9RenderSystem*)RenderSystem::Current())->GetD3D();
	HRESULT hr;

	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth = desc.Mode.Width;
	d3dpp.BackBufferHeight = desc.Mode.Height;

	if (desc.Mode.Depth > 16)
	{
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}

	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)desc.MultiSamplingType;
	d3dpp.MultiSampleQuality = desc.MultiSampleQuality;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = (HWND)window->GetHandle();
	d3dpp.Windowed = !desc.FullScreen;
	d3dpp.EnableAutoDepthStencil = desc.EnableDepthStencil;
	if (desc.EnableDepthStencil)
	{
		if (desc.Mode.Depth > 16)
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

			if (!CheckDepthStencilFormat(d3dpp.AutoDepthStencilFormat,
				pD3D, desc.Adapter.GetIndex(), D3DDEVTYPE_HAL,
				d3dpp.BackBufferFormat))
			{
				d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
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

	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	// The refresh rate must be zero for windowed mode
	if (desc.FullScreen)
	{
		d3dpp.FullScreen_RefreshRateInHz = desc.Mode.RefreshRate;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	if (desc.VSync)
    {
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }
    else
    {
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }

	DWORD behaviorFlags;
	D3DCAPS9 d3dcaps;
	hr = pD3D->GetDeviceCaps(desc.Adapter.GetIndex(), D3DDEVTYPE_HAL, &d3dcaps);
	if (FAILED(hr))
	{
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		Logger::Current()->Log(LogLevel::Warning, _T("D3D9RenderContext.Create"),
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
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderContext.Create"),
			_T("Failed to create the Direct3D9 device."));
		return false;
	}

	_window = window;
	_description = desc;
	_isValid = true;

	return true;
}

bool D3D9RenderContext::Destroy()
{
	if (!_isValid)
	{
		return false;
	}

	_window = NULL;
	_isValid = false;

	return true;
}

void D3D9RenderContext::SwapBuffers(WindowHandle handle)
{
	// Present the backbuffer contents to the display
	_D3DDevice->Present(NULL, NULL, (HWND)handle, NULL);
}

RectangleInt D3D9RenderContext::GetSafeArea() const
{
	if (!_isValid)
	{
		return RectangleInt::Empty;
	}

	return RectangleInt(0, 0,
		_description.Mode.Width,
		_description.Mode.Height);
}

}
