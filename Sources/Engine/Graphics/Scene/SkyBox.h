/*=============================================================================
SkyBox.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SKYBOX_H_
#define _SE_SKYBOX_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Scene/Sky.h"
#include "Graphics/System/Texture.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

/** SkyBox Planes. */
enum SkyBoxPlane
{
	SkyBoxPlane_Left,
	SkyBoxPlane_Right,
	SkyBoxPlane_Top,
	SkyBoxPlane_Bottom,
	SkyBoxPlane_Front,
	SkyBoxPlane_Back,
	SkyBoxPlane_Count
};

/**
	@brief SkyBox.

	A skybox represents the distant environment which is rendered onto six
	sides of a cube. The camera is placed at the center of this cube.

	Two techniques can be used to render skyboxes:
		- Six 2D textures
		- A cubemap texture

	(From Gems2)
	- Skybox resolution:
		skyRes = screenRes / tan(fov / z)

	- Skybox size:
		skySize < 2 * zfar * sqrt(3) / 3

	remarks:
		The skybox should be rendered first.
		The color buffer doesn't need to be cleared.
		Depth testing and depth writing should be disabled.
		Lighting and fog should be disabled.
		To prevent seams from appearing, the textures should use the clamp filtering mode.
*/
class SE_GRAPHICS_EXPORT SkyBox : public Sky
{
	SE_DECLARE_CLASS(SkyBox, Object);
	SE_BEGIN_REFLECTION(SkyBox);
		SE_Field(_distance, real32, Public);
		//SE_Field(_planes, Texture[SkyBoxPlane_Count], Public);
	SE_END_REFLECTION(SkyBox);

protected:
	real32 _distance;
	TexturePtr _planes[SkyBoxPlane_Count];
	bool _isCubemap;
	TexturePtr _cubeTexture;
	MeshPtr _mesh;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	SkyBox();

	/** Destructor. */
	virtual ~SkyBox();
	//@}

	/** @name Properties. */
	//@{
	/** Gets or sets the distance of the sky. */
	real32 GetDistance() const { return _distance; }
	void SetDistance(real32 value) { _distance = value; }

	/** Gets or sets a plane texture. */
	Texture* GetPlaneTexture(SkyBoxPlane plane) const;
	void SetPlaneTexture(SkyBoxPlane plane, Texture* texture);
	//@}

	virtual bool Create();
	virtual void Destroy();

	virtual void Update(const TimeValue& timeValue);
	virtual void Render();
};

}

#endif 
