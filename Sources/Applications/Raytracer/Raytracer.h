/*=============================================================================
Raytracer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_RAYTRACER_H_
#define _RAYTRACER_RAYTRACER_H_

#include "Common.h"
#include "Scene.h"
#include "Image.h"

namespace Raytracer
{
	struct RaytracerSettings
	{
		RaytracerSettings() :
			_ResolutionX(640),
			_ResolutionY(480)
		{
		}

		int32 _ResolutionX;
		int32 _ResolutionY;
		int32 _ScreenLeft;
		int32 _ScreenTop;
		int32 _ScreenRight;
		int32 _ScreenBottom;
		int32 _AntiAliasing;
		RenderOptions _Options;
	};

	class AppCore : public Singleton<AppCore>
	{
	public:
		AppCore();
		virtual ~AppCore();

		bool Create();
		void Destroy();

		void LoadSettings(const String& fileName);
		void LoadRTScene(const String& fileName);
		void SaveRTScene(const String& fileName);
		void ExportColourBuffer(const String& fileName);
		void RenderScanline();

		RaytracerSettings* GetSettings() const { return _Settings; }

	protected:
		RaytracerSettings* _Settings;
		RTImage* _Image;
		RTScene* _Scene;
	};
}

#endif
