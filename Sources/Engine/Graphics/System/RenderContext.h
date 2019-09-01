/*=============================================================================
RenderContext.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RENDERCONTEXT_H_
#define _SE_RENDERCONTEXT_H_

#include "Graphics/System/RenderTarget.h"
#include "Graphics/System/SystemDisplay.h"

namespace SonataEngine
{

/** Render Context Description. */
struct RenderContextDescription
{
	DisplayAdapter Adapter;
	DisplayMode Mode;
	bool FullScreen;
	bool VSync;
	int32 MultiSamplingType;
	int32 MultiSampleQuality;
	bool EnableDepthStencil;

	RenderContextDescription() :
		FullScreen(false),
		VSync(false),
		MultiSamplingType(0),
		MultiSampleQuality(0),
		EnableDepthStencil(true)
	{
		SystemDisplay::GetPrimaryDisplayAdapter(Adapter);
		SystemDisplay::GetCurrentDisplayMode(Mode);
	}
};

/**
	@brief Rendering context.

	Base class for rendering context implementations.
	A rendering context creates the rendering device using the specified
	system window and settings.
*/
class SE_GRAPHICS_EXPORT RenderContext : public RenderTarget
{
protected:
	Window* _window;
	RenderContextDescription _description;

public:
	/** Destructor. */
	virtual ~RenderContext();

	virtual bool IsTexture() const { return false; }
	virtual PixelFormat GetPixelFormat() const;
	virtual int32 GetWidth() const;
	virtual int32 GetHeight() const;
	virtual int32 GetBitsPerPixel() const;

	/** @name Properties. */
	//@{
	/** Gets the target window. */
	Window* GetWindow() const { return _window; }

	/**
		Gets the settings using when the context was created.
		@return true if successful; otherwise, false.
	*/
	bool GetDescription(RenderContextDescription& desc) const;

	/** Returns whether the context is valid. */
	virtual bool IsValid() const = 0;
	//@}

	/** @name Creation / Destruction. */
	//@{
	/**
		Creates a new rendering context.
		@param window The target window where the rendering will occur.
		@param desc The creation settings.
		@return true if successful; otherwise, false.
	*/
	virtual bool Create(Window* window, const RenderContextDescription& desc) = 0;

	/**
		Destroys the rendering context.
		@return true if successful; otherwise, false.
	*/
	virtual bool Destroy() = 0;
	//@}

	/** Swap the buffers. */
	virtual void SwapBuffers(WindowHandle handle = NULL) = 0;

	/** Returns the safe area for the current display mode. */
	virtual RectangleInt GetSafeArea() const = 0;

protected:
	/** Constructor. */
	RenderContext();
};

}

#endif 
