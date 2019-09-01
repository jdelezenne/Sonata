/*=============================================================================
SasEffectMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SasEffectMaterial.h"

namespace SonataEngine
{

static String SemanticNames[] =
{
	"unknown",
	"position",
	"direction",
	"diffuse",
	"specular",
	"ambient",
	"emissive",
	"specularpower",
	"refraction",
	"opacity",
	"environment",
	"environmentnormal",
	"normal",
	"height",
	"attenuation",
	"rendercolortarget",
	"renderdepthstenciltarget",
	"viewportpixelsize",
	"cameraposition",
	"time",
	"elapsedtime",
	"animationtime",
	"animationtick",
	"mouseposition",
	"leftmousedown",
	"resetpulse",
	"world",
	"view",
	"projection",
	"worldtranspose",
	"viewtranspose",
	"projectiontranspose",
	"worldview",
	"worldviewprojection",
	"worldinverse",
	"viewinverse",
	"projectioninverse",
	"worldinversetranspose",
	"viewinversetranspose",
	"projectioninversetranspose",
	"worldviewinverse",
	"worldviewtranspose",
	"worldviewinversetranspose",
	"worldviewprojectioninverse",
	"worldviewprojectiontranspose",
	"worldviewprojectioninversetranspose",
	"viewprojection",
	"viewprojectiontranspose",
	"viewprojectioninverse",
	"viewprojectioninversetranspose"
};

SasEffectMaterial::SasEffectMaterial() :
	EffectMaterial()
{
}

SasEffectMaterial::~SasEffectMaterial()
{
}

void SasEffectMaterial::Initialize()
{
	EffectMaterial::Initialize();

	if (_effectShader == NULL)
	{
		return;
	}

	LoadParameters();
	LoadScript();

    EffectParameter* sasTimeNow = GetParameterBySasBind("Sas.Time.Now");
	EffectParameter* sasTimeLast = GetParameterBySasBind("Sas.Time.Last");
	EffectParameter* sasProjection = GetParameterBySasBind("Sas.Camera.Projection");
	EffectParameter* sasView = GetParameterBySasBind("Sas.Camera.WorldToView");
	EffectParameter* sasWorld = GetParameterBySasBind("Sas.Skeleton.MeshToJointToWorld[0]");
}

void SasEffectMaterial::SetupMaterial(SceneState* sceneState)
{
	EffectMaterial::SetupMaterial(sceneState);

	int parameterCount = _parameterInfos.Count();
	for (int index = 0; index < parameterCount; ++index)
	{
		real32 scalar;
		Vector3 vector3;
		Matrix4 matrix;
		SemanticID semanticID = _parameterInfos[index].semanticID;
		EffectParameter* handle = _parameterInfos[index].handle;
		switch (semanticID)
		{
		case SemanticID_cameraposition:
			vector3 = sceneState->Camera->GetWorldPosition();
			handle->SetValue(&vector3);
			break;

		case SemanticID_time:
			scalar = sceneState->CurrentTime;
			handle->SetValue(&scalar);
			break;

		case SemanticID_elapsedtime:
			scalar = sceneState->ElapsedTime;
			handle->SetValue(&scalar);
			break;

		case SemanticID_world:
			matrix = sceneState->World;
			handle->SetValue(&matrix);
			break;

		case SemanticID_view:
			matrix = sceneState->View;
			handle->SetValue(&matrix);
			break;

		case SemanticID_projection:
			matrix = sceneState->Projection;
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldtranspose:
			matrix = Matrix4::Transpose(sceneState->World);
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewtranspose:
			matrix = Matrix4::Transpose(sceneState->View);
			handle->SetValue(&matrix);
			break;

		case SemanticID_projectiontranspose:
			matrix = Matrix4::Transpose(sceneState->Projection);
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldview:
			matrix = sceneState->WorldView;
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewprojection:
			matrix = sceneState->WorldViewProjection;
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldinverse:
			matrix = Matrix4::Invert(sceneState->World);
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewinverse:
			matrix = Matrix4::Invert(sceneState->View);
			handle->SetValue(&matrix);
			break;

		case SemanticID_projectioninverse:
			matrix = Matrix4::Invert(sceneState->Projection);
			handle->SetValue(&matrix);
			break;



		case SemanticID_worldinversetranspose:
			matrix = Matrix4::Transpose(Matrix4::Invert(sceneState->World));
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewinversetranspose:
			matrix = Matrix4::Transpose(Matrix4::Invert(sceneState->View));
			handle->SetValue(&matrix);
			break;

		case SemanticID_projectioninversetranspose:
			matrix = Matrix4::Transpose(Matrix4::Invert(sceneState->Projection));
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewinverse:
			matrix = Matrix4::Invert(sceneState->WorldView);
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewtranspose:
			matrix = Matrix4::Transpose(sceneState->WorldView);
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewinversetranspose:
			matrix = Matrix4::Invert(Matrix4::Transpose(sceneState->WorldView));
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewprojectioninverse:
			matrix = Matrix4::Invert(sceneState->WorldViewProjection);
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewprojectiontranspose:
			matrix = Matrix4::Transpose(sceneState->WorldViewProjection);
			handle->SetValue(&matrix);
			break;

		case SemanticID_worldviewprojectioninversetranspose:
			matrix = Matrix4::Invert(Matrix4::Transpose(sceneState->WorldViewProjection));
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewprojection:
			matrix = sceneState->ViewProjection;
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewprojectiontranspose:
			matrix = Matrix4::Transpose(sceneState->ViewProjection);
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewprojectioninverse:
			matrix = Matrix4::Invert(sceneState->ViewProjection);
			handle->SetValue(&matrix);
			break;

		case SemanticID_viewprojectioninversetranspose:
			matrix = Matrix4::Transpose(Matrix4::Invert(sceneState->ViewProjection));
			handle->SetValue(&matrix);
			break;
		}
	}
}

void SasEffectMaterial::SetupGeometry(MeshPart* meshPart)
{
	EffectMaterial::SetupGeometry(meshPart);
}

void SasEffectMaterial::SetupPass(SceneState* sceneState, MeshPart* meshPart)
{
	EffectMaterial::SetupPass(sceneState, meshPart);
}

EffectParameter* SasEffectMaterial::GetParameterBySasBind(const String& bind)
{
	int parameterCount = _effectShader->GetParameterCount();
	for (int index = 0; index < parameterCount; ++index)
	{
		EffectParameter* parameter = _effectShader->GetParameterByIndex(index);
		EffectAnnotation* annotation = parameter->GetAnnotationByName("SasBindAddress");
		if (annotation != NULL)
		{
			String value = annotation->GetValueString();
			if (String::Compare(value, bind, true) == 0)
			{
				return parameter;
			}
		}
	}
	return NULL;
}

SasEffectMaterial::SemanticID SasEffectMaterial::GetSemanticID(const String& name)
{
	for (int index = 0; index < SemanticID_max; ++index)
	{
		if (String::Compare(SemanticNames[index], name, true) == 0)
		{
			return (SemanticID)index;
		}
	}
	return SemanticID_unknown;
}

void SasEffectMaterial::LoadParameters()
{
	_parameterInfos.Clear();

	int parameterCount = _effectShader->GetParameterCount();
	_parameterInfos.SetCapacity(parameterCount);
	for (int index = 0; index < parameterCount; ++index)
	{
		EffectParameter* parameter = _effectShader->GetParameterByIndex(index);
		if (parameter != NULL && !parameter->GetName().IsEmpty())
		{
			const ShaderParameterDesc& desc = parameter->GetParameterDesc();
			if (!desc.Semantic.IsEmpty())
			{
				ParameterInfo info;
				info.semanticID = GetSemanticID(desc.Semantic);
				info.handle = parameter;
				_parameterInfos.Add(info);
			}
		}
	}
}

void SasEffectMaterial::LoadScript()
{
}

}
