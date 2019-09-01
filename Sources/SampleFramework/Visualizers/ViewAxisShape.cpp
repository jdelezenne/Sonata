/*=============================================================================
ViewAxisShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ViewAxisShape.h"

namespace SampleFramework
{

ViewAxisShape::ViewAxisShape() :
	_shape(NULL),
	_camera(NULL),
	_axisLocation(AxisLocation_BottomLeft),
	_screenSize(64)
{
}

ViewAxisShape::~ViewAxisShape()
{
}

void ViewAxisShape::Render()
{
	if (_shape == NULL || _camera == NULL)
		return;

	Mesh* mesh = _shape->GetMesh();
	if (mesh == NULL)
		return;

	MeshPart* meshPart = mesh->GetMeshPart(0);

	RenderSystem* renderSystem = RenderSystem::Current();

    Viewport oldViewport = renderSystem->GetViewport();
    Matrix4 oldProjection = renderSystem->GetProjectionTransform();
    Matrix4 oldView = renderSystem->GetViewTransform();
    Matrix4 oldWorld = renderSystem->GetWorldTransform();

    int area = _screenSize;
    Viewport viewport;
    switch (_axisLocation)
    {
        case AxisLocation_BottomLeft:
            viewport.SetLeft(0);
            viewport.SetTop(oldViewport.GetHeight() - area);
            break;
        case AxisLocation_BottomRight:
            viewport.SetLeft(oldViewport.GetWidth() - area);
            viewport.SetTop(oldViewport.GetHeight() - area);
            break;
        case AxisLocation_TopLeft:
            viewport.SetLeft(0);
            viewport.SetTop(0);
            break;
        case AxisLocation_TopRight:
            viewport.SetLeft(oldViewport.GetWidth() - area);
            viewport.SetTop(0);
            break;
    }
    viewport.SetWidth(area);
    viewport.SetHeight(area);
    renderSystem->SetViewport(viewport);

	meshPart->GetShader()->BeginMaterial();
	meshPart->GetShader()->BeginPass(0);

	//renderSystem->SetProjectionTransform(Matrix4::CreateOrthographic(
	//	oldViewport.GetWidth(), oldViewport.GetHeight(), 0.0f, 1000.0f));

	renderSystem->SetViewTransform(Matrix4::CreateLookAt(
        Vector3(0.0, 0.0, 1.0),
        Vector3(0.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0)));

	Matrix4 cameraView = Matrix4::CreateLookAt(_camera->GetWorldPosition(),
		(_camera->GetWorldPosition() + _camera->GetWorldDirection()), _camera->GetWorldUp());
	cameraView.SetTranslation(Vector3::Zero);

    renderSystem->SetWorldTransform(cameraView);

	RenderData renderData;
	meshPart->GetRenderData(renderData);
	renderSystem->Render(&renderData);

	meshPart->GetShader()->EndPass();
	meshPart->GetShader()->EndMaterial();

    renderSystem->SetWorldTransform(oldWorld);
    renderSystem->SetViewTransform(oldView);
    renderSystem->SetProjectionTransform(oldProjection);
    renderSystem->SetViewport(oldViewport);
}

}
