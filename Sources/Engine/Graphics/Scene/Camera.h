/*=============================================================================
Camera.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CAMERA_H_
#define _SE_CAMERA_H_

#include "Graphics/Common.h"
#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Viewport.h"

namespace SonataEngine
{

class Scene;

/** Projection Type. */
enum ProjectionType
{
	/// Orthographic.
	ProjectionType_Orthographic,

	/// Perspective.
	ProjectionType_Perspective
};

SE_BEGIN_ENUM(ProjectionType);
	SE_Enum(Orthographic);
	SE_Enum(Perspective);
SE_END_ENUM(ProjectionType);

/**
	@brief Camera.
*/
class SE_GRAPHICS_EXPORT Camera : public SceneObject
{
	SE_DECLARE_CLASS(Camera, SceneObject);
	SE_BEGIN_REFLECTION(Camera);
		SE_Field(_projectionType, ProjectionType, Public);
		SE_Field(_viewport, Viewport, Public);
		SE_Field(_fov, real32, Public);
		SE_Field(_aspectRatio, real32, Public);
		SE_Field(_leftPlane, int32, Public);
		SE_Field(_topPlane, int32, Public);
		SE_Field(_rightPlane, int32, Public);
		SE_Field(_bottomPlane, int32, Public);
		SE_Field(_nearPlane, real32, Public);
		SE_Field(_farPlane, real32, Public);
	SE_END_REFLECTION(Camera);

protected:
	Scene* _scene;
	Viewport _viewport;
	ProjectionType _projectionType;
	real32 _fov;
	real32 _aspectRatio;
	int32 _leftPlane;
	int32 _topPlane;
	int32 _rightPlane;
	int32 _bottomPlane;
	real32 _nearPlane;
	real32 _farPlane;

	bool _needProjectionUpdate;

	Matrix4 _projection;
	Matrix4 _view;
	Frustum _frustum;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Camera();

	/** Destructor. */
	virtual ~Camera();
	//@}

	/** @name Projection. */
	//@{
	/**
		Gets the scene of the camera.
		@return Scene.
	*/
	Scene* GetScene() const { return _scene; }

	/**
		Sets the scene of the camera.
		@param value Scene.
	*/
	void SetScene(Scene* value);

	/**
		Gets the viewport of the camera.
		@return Viewport.
	*/
	const Viewport& GetViewport() const { return _viewport; }

	/**
		Sets the viewport of the camera.
		@param value Viewport.
	*/
	void SetViewport(const Viewport& value);

	/**
		Gets the projection type of the camera.
		@return Projection type.
	*/
	ProjectionType GetProjectionType() const { return _projectionType; }

	/**
		Sets the projection type of the camera.
		@param value Projection type.
	*/
	void SetProjectionType(ProjectionType value);

	/**
		Gets the vertical Field Of View of the camera.
		@return Vertical Field Of View.
	*/
	real32 GetFOV() const { return _fov; }

	/**
		Sets the vertical Field Of View of the camera.
		@param value Field Of View.
	*/
	void SetFOV(real32 value);

	/**
		Gets the horizontal ratio of the camera.
		@return Horizontal ratio.
	*/
	real32 GetAspectRatio() const { return _aspectRatio; }

	/**
		Sets the horizontal ratio of the camera.
		@param value Horizontal ratio.
	*/
	void SetAspectRatio(real32 value);

	/** Gets or sets the clipping planes. */
	int32 GetLeftPlane() const { return _leftPlane; }
	void SetLeftPlane(int32 value);

	int32 GetTopPlane() const { return _topPlane; }
	void SetTopPlane(int32 value);

	int32 GetRightPlane() const { return _rightPlane; }
	void SetRightPlane(int32 value);

	int32 GetBottomPlane() const { return _bottomPlane; }
	void SetBottomPlane(int32 value);

	/**
		Gets the near clipping plane of the camera.
		@return Near clipping plane.
	*/
	real32 GetNearPlane() const { return _nearPlane; }

	/**
		Sets the near clipping plane of the camera.
		@param value Near clipping plane.
	*/
	void SetNearPlane(real32 value);

	/**
		Gets the far clipping plane of the camera.
		@return Far clipping plane.
	*/

	real32 GetFarPlane() const { return _farPlane; }

	/**
		Sets the far clipping plane of the camera.
		@param value Far clipping plane.
	*/
	void SetFarPlane(real32 value);

	/**
		Sets the camera projection to perspective mode with the required parameters.
		@param fov Field Of View.
		@param aspect Horizontal Ratio.
		@param near Near clipping plane.
		@param far Far clipping plane.
	*/
	void SetPerspective(real32 fov, real32 aspect, real32 nearPlane, real32 farPlane);

	/**
		Sets the camera projection to orthographic mode with the required parameters.
		@param leftPlane Left clipping plane.
		@param topPlane Top clipping plane.
		@param rightPlane Right clipping plane.
		@param bottomPlane Bottom clipping plane.
		@param nearPlane Near clipping plane.
		@param farPlane Far clipping plane.
	*/
	void SetOrthographic(int32 leftPlane, int32 topPlane, int32 rightPlane, int32 bottomPlane, real32 nearPlane, real32 farPlane);

	/**
		Sets the camera projection to orthographic mode with the required parameters.
		@param width Width of the clipping plane.
		@param height Height of the clipping plane.
		@param nearPlane Near clipping plane.
		@param farPlane Far clipping plane.
	*/
	void SetOrthographic(int32 width, int32 height, real32 nearPlane, real32 farPlane);
	//@}

	/** @name Properties. */
	//@{
	/**
		Sets the direction of the camera.
		@param value Direction of the camera.
	*/
	void SetDirection(const Vector3& value);

	/**
		Changes the target position of the camera.
		@param value Target position of the camera.
	*/
	void LookAt(const Vector3& value);

	/**
		Gets the projection matrix of the camera.
		@return Projection matrix of the camera.
	*/
	const Matrix4& GetProjection();

	/**
		Gets the view matrix of the camera.
		@return View matrix of the camera.
	*/
	const Matrix4& GetView();

	/**
		Gets the view frustum of the camera.
		@return View frustum of the camera.
	*/
	const Frustum& GetFrustum();
	//@}

	/** Updates the camera. */
	virtual void Update(const TimeValue& timeValue);

protected:
	virtual void _NotifyProjectionUpdate();

	/** Computes the view matrix. */
	virtual void _UpdateWorldTransform();

	/** Computes the projection matrix. */
	virtual void _UpdateProjection();
};

SEPointer(Camera);

}

#endif
