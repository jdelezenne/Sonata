/*=============================================================================
SceneApplication.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_SCENEAPPLICATION_H_
#define _SFW_SCENEAPPLICATION_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{

enum LogMode
{
	LogMode_None = 0,
	LogMode_Debug = (1<<0),
	LogMode_Console = (1<<1),
	LogMode_File = (1<<2),
};

/** Base class for scene-based applications. */
class SE_SAMPLEFRAMEWORK_EXPORT SceneApplication : public Application
{
protected:
	LogMode _logMode;
	ScenePtr _scene;
	CameraPtr _camera;
	CameraPtr _screenCamera;
	RenderSystemPtr _renderSystem;
	ShaderSystemPtr _shaderSystem;
	InputSystemPtr _inputSystem;
	AudioSystemPtr _audioSystem;

public:
	/** @name Constructors / Destructor. */
	//@{
	SceneApplication();
	virtual ~SceneApplication();
	//@}

	/** Gets the log mode. */
	LogMode GetLogMode() const { return _logMode; }

	virtual bool Create();
	virtual bool Destroy();
	virtual bool CreateMainWindow();

protected:
	virtual bool CreateLogHandlers();
	virtual bool CreateRenderSystem();
	virtual bool CreateShaderSystem();
	virtual bool CreateInputSystem();
	virtual bool CreateAudioSystem();

	virtual bool CreateScene();
	virtual bool CreateCamera();
	virtual bool CreateScreenCamera();

	virtual Image* GetImage(const String& name);
	virtual Texture* GetTexture(const String& name);
	virtual Font* GetFont(const String& name);
	virtual Sprite* GetSprite(const String& name);
	virtual Scene* GetScene(const String& name);
	virtual Model* GetModel(const String& name, real scale = 1.0);
	virtual EffectShader* GetEffectShader(const String& name);

private:
	ManagerPlugin* FindRenderSystemPlugin();
	ManagerPlugin* FindShaderSystemPlugin();
	ManagerPlugin* FindInputSystemPlugin();
	ManagerPlugin* FindAudioSystemPlugin();
};

}

#endif
