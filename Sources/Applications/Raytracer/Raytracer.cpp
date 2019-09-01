/*=============================================================================
Raytracer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DevILImageReader.h"
#include "DevILImageWriter.h"
#include "OBJModelReader.h"

#include "Raytracer.h"

#include "Camera.h"
#include "Light.h"
#include "PlaneShape.h"
#include "SceneObject.h"
#include "SphereShape.h"
#include "TriangleShape.h"
#include "Shader.h"

#include "LambertShader.h"
#include "PhongShader.h"

namespace Raytracer
{
	class RTShader;

	AppCore::AppCore()
	{
		_Settings = new RaytracerSettings();
		_Image = new RTImage();
	}

	AppCore::~AppCore()
	{
		Destroy();
	}

	bool AppCore::Create()
	{
		/*if (Directory::Exists(_T("Data")))
			FileSystem::Instance()->AddRootPath(_T("Data"));
		else if (Directory::Exists(_T("..\\..\\Data")))
			FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
		else if (Directory::Exists(_T("..\\..\\..\\Data")))
			FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
		else
		{
			Console::Error()->WriteLine(_T("Failed to find the root path!"));
			return false;
		}*/

		TextStreamLogHandler* consoleHandler = new TextStreamLogHandler(ConsoleStream::StandardOutput);
		((DefaultLogFormatter*)consoleHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_None));

	#ifdef WIN32
		Win32DebugLogHandler* debugHandler = new Win32DebugLogHandler();
		((DefaultLogFormatter*)debugHandler->GetFormatter())->SetOptions(
			(LogOptions)(LogOptions_Source | LogOptions_Time));
	#endif

		Logger* logger = new Logger();
		logger->GetHandlers().Add(consoleHandler);
		logger->GetHandlers().Add(debugHandler);
		Logger::SetCurrent(logger);

		//Environment::SetCurrentDirectory(FileSystem::Instance()->GetRootPath(0));

		return true;
	}

	void AppCore::Destroy()
	{
		SE_SAFE_DELETE(_Image);
		SE_SAFE_DELETE(_Settings);
	}

	void AppCore::LoadSettings(const String& fileName)
	{
		_Settings->_ResolutionX = 640;
		_Settings->_ResolutionY = 480;
		_Settings->_ScreenLeft = -4;
		_Settings->_ScreenRight = 4;
		_Settings->_ScreenTop = 3;
		_Settings->_ScreenBottom = -3;

		File* file = new File(fileName);

		FileStreamPtr stream = file->Open(FileMode_Open);
		if (stream != NULL)
		{
			TextStream text(stream);
			_Settings->_ResolutionX = text.ReadLine().ToInt32();
			_Settings->_ResolutionY = text.ReadLine().ToInt32();
			stream->Close();
		}

		delete file;

		_Image->Create(PixelFormat_R8G8B8, _Settings->_ResolutionX, _Settings->_ResolutionY);
	}

	void AppCore::LoadRTScene(const String& fileName)
	{
		Console::WriteLine(_T("Loading scene: ") + fileName);

		File* file = new File(fileName);
		XMLSerializer* serializer  = new XMLSerializer();

		FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
		if (stream != NULL)
		{
			_Scene = (RTScene*)serializer->Deserialize(stream.Get());
			stream->Close();
		}

		delete serializer;
		delete file;

		Console::WriteLine(_T("Scene loaded"));
	}

	void AppCore::SaveRTScene(const String& fileName)
	{
		if (_Scene == NULL)
			return;

		Console::WriteLine(_T("Saving scene: ") + fileName);

		File* file = new File(fileName);
		XMLSerializer* serializer  = new XMLSerializer();

		FileStreamPtr stream = file->Open(FileMode_Create);
		if (stream != NULL)
		{
			serializer->Serialize(stream.Get(), _Scene);
			stream->Close();
		}

		delete serializer;
		delete file;

		Console::WriteLine(_T("Scene saved"));
	}


	void AppCore::ExportColourBuffer(const String& fileName)
	{
		if (_Image == NULL)
			return;

		Console::WriteLine(_T("Exporting scene to: ") + fileName);

		DevILImageWriter writer;
		writer.SaveImage(fileName, _Image);

		Console::WriteLine(_T("Scene exported"));
	}

	void AppCore::RenderScanline()
	{
		if (_Scene == NULL)
			return;

		Console::WriteLine(_T("Rendering scene..."));

		int32 x, y;
		int32 width, height;
		int32 halfWidth, halfHeight;
		Colour32 colour;

		width = _Settings->_ResolutionX;
		height = _Settings->_ResolutionY;

		halfWidth = width / 2;
		halfHeight = height / 2;

		RenderState state;
		state._Options = &_Settings->_Options;

		if (_Scene->GetCamera() == NULL)
			return;

		state._Ray.Origin = _Scene->GetCamera()->GetPosition();
/*
		Vector3 m_P1 = Vector3(_Settings->_ScreenLeft, _Settings->_ScreenTop, 0 );
		Vector3 m_P2 = Vector3(_Settings->_ScreenRight, _Settings->_ScreenTop, 0 );
		Vector3 m_P3 = Vector3(_Settings->_ScreenRight, _Settings->_ScreenBottom, 0 );
		Vector3 m_P4 = Vector3(_Settings->_ScreenLeft, _Settings->_ScreenBottom, 0 );

		Vector3 zaxis = _Scene->GetRTCamera()->GetLookAt() - _Scene->GetRTCamera()->GetPosition();
		zaxis.Normalize();
		Vector3 up( 0, 1, 0 );
		Vector3 xaxis = up.Cross( zaxis );
		Vector3 yaxis = xaxis.Cross( -zaxis );
		Matrix4 m;
		m.v[0] = xaxis.x, m.v[1] = xaxis.y, m.v[2] = xaxis.z;
		m.v[4] = yaxis.x, m.v[5] = yaxis.y, m.v[6] = yaxis.z;
		m.v[8] = zaxis.x, m.v[9] = zaxis.y, m.v[10] = zaxis.z;
		m.Inverse();
		m.v[3] = _Scene->GetRTCamera()->GetPosition().x, m[7] = _Scene->GetRTCamera()->GetPosition().y, m[11] = _Scene->GetRTCamera()->GetPosition().z;

		ray.Origin = m.Transform(ray.Origin);
		m_P1 = m.Transform( m_P1 );
		m_P2 = m.Transform( m_P2 );
		m_P3 = m.Transform( m_P3 );
		m_P4 = m.Transform( m_P4 );

		real32 dx = (m_P2 - m_P1) / _Settings->_ResolutionX;
		real32 dy = (m_P4 - m_P1) / _Settings->_ResolutionY;
*/

		real32 dx = (real32)(_Settings->_ScreenRight - _Settings->_ScreenLeft) / _Settings->_ResolutionX;
		real32 dy = (real32)(_Settings->_ScreenBottom - _Settings->_ScreenTop) / _Settings->_ResolutionY;

		state._Scene = _Scene;
		_Scene->Update();

		real32 sx, sy;

		sy = _Settings->_ScreenTop;

		// Iterate over each pixel
		for (y=0; y<height; ++y)
		{
			sx = _Settings->_ScreenLeft;

			for (x=0; x<width; ++x)
			{
				state._RayType = RayType_Eye;

				state._Ray.Direction = Vector3(sx, sy, 0.0) - state._Ray.Origin;
				state._Ray.Direction.Normalize();

				// Raytrace the scene
				colour = Colour32::Black;
				_Scene->Raytrace(state, colour);

				// Clamp the colour
				colour = Colour32::Clamp(colour, 0.0f, 1.0f);

				// Set the colour value in the buffer
				_Image->SetRGB(x, y, Colour8(colour.r * 255.0f,
					colour.g * 255.0f, colour.b * 255.0f));

				sx += dx;
			}

			sy += dy;
		}

		Console::WriteLine(_T("Scene rendered"));
	}
}

using namespace Raytracer;

int main(int argc, char** argv)
{
	Engine::Instance();

	Console::WriteLine("Raytracer");
	Console::WriteLine("=========");

	if (argc != 2)
	{
		Console::WriteLine("Raytracer filename");
		return -1;
	}

	String fileName = argv[1];
	String baseName = Path::GetFileNameWithoutExtension(fileName);

	try
	{
		if (!AppCore::Instance()->Create())
		{
			Console::Error()->WriteLine("Failed to create the application!");
			return -1;
		}

		AppCore::Instance()->LoadSettings("settings.cfg");
		AppCore::Instance()->LoadRTScene(fileName);
		//AppCore::Instance()->SaveRTScene("Raytracer\\scene_out.xml");
		AppCore::Instance()->RenderScanline();
		AppCore::Instance()->ExportColourBuffer(baseName + ".bmp");
	}
	catch (const Exception& e)
	{
		Console::Error()->WriteLine(e.GetMessage());
	}

	Engine::DestroyInstance();
}
