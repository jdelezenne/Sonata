/*=============================================================================
RTScene.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Scene.h"
#include "Shader.h"
#include "OBJModelReader.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTScene);
	SE_IMPLEMENT_REFLECTION(RTScene);

	RTScene::RTScene() :
		RefObject()
	{
		_BackgroundColour = Colour32::Black;
		real32 _Gamma = 1.0f;
		_Camera = NULL;
	}

	RTScene::~RTScene()
	{
	}

	void RTScene::OnSerialized(XMLSerializer* context, XMLElement* element)
	{
		super::OnSerialized(context, element);

		SonataEngine::XMLDocument* document = element->GetOwnerDocument();

		{
			XMLElement* array = document->CreateElement("Objects");
			int count = _Objects.Count();
			array->SetAttribute("Count", String::ToString(count));
			for (int i=0; i<count; i++)
			{
				XMLElement* item = document->CreateElement("Object");
				RTSceneObject* object = _Objects.GetItem(i);
				context->Serialize(document, item, object, object->GetType());
				array->AppendChild(item);
			}
			element->AppendChild(array);
		}

		{
			XMLElement* array = document->CreateElement("Lights");
			int count = _Lights.Count();
			array->SetAttribute("Count", String::ToString(count));
			for (int i=0; i<count; i++)
			{
				XMLElement* item = document->CreateElement("Light");
				RTLight* light = _Lights.GetItem(i);
				context->Serialize(document, item, light, light->GetType());
				array->AppendChild(item);
			}
			element->AppendChild(array);
		}
	}

	void RTScene::OnDeserialized(XMLSerializer* context, XMLElement* element)
	{
		super::OnDeserialized(context, element);

		SonataEngine::XMLDocument* document = element->GetOwnerDocument();

		{
			XMLElement* array = (XMLElement*)element->GetItem("Objects");
			int count = array->GetAttribute("Count").ToInt32();
			XMLElement* item = (XMLElement*)array->GetItem("Object");
			for (int i=0; i<count; i++)
			{
				XMLElement* type = (XMLElement*)item->GetFirstChild();
				RTSceneObject* object = (RTSceneObject*)context->Deserialize(document, type);
				if (object != NULL)
					_Objects.Add(object);
				item = (XMLElement*)item->GetNextSibling();
			}
		}

		{
			XMLElement* array = (XMLElement*)element->GetItem("Lights");
			int count = array->GetAttribute("Count").ToInt32();
			XMLElement* item = (XMLElement*)array->GetItem("Light");
			for (int i=0; i<count; i++)
			{
				XMLElement* type = (XMLElement*)item->GetFirstChild();
				RTLight* light = (RTLight*)context->Deserialize(document, type);
				if (light != NULL)
					_Lights.Add(light);
				item = (XMLElement*)item->GetNextSibling();
			}
		}

		{
			XMLElement* array = (XMLElement*)element->GetItem("Meshes");
			if (array != NULL)
			{
				int count = array->GetAttribute("Count").ToInt32();
				XMLElement* item = (XMLElement*)array->GetItem("Mesh");
				for (int i=0; i<count; i++)
				{
					XMLElement* elementFileName = (XMLElement*)item->GetItem("FileName");
					XMLText* valueFileName = (XMLText*)elementFileName->GetFirstChild();
					OBJModelReader reader;
					reader.LoadModel(this, "Raytracer\\" + valueFileName->GetValue());
					item = (XMLElement*)item->GetNextSibling();
				}
			}
		}
	}

	void RTScene::Update()
	{
		if (_Camera != NULL)
			_Camera->Update();
	}

	void RTScene::Raytrace(RenderState& state, Colour32& colour)
	{
		//_Camera->Raycast(Vector2(area.GetLeft(), area.GetTop()), ray);

		TraceResult result;
		RTSceneObject* minObject = NULL;
		real32 minDistance = SE_MAX_R32;

		RTSceneObject* object;
		foreach (object, _Objects, List<RTSceneObject*>)
		{
			if (object == state._Object)
				continue;

			object->Intersect(state, state._Ray, result);

			if (result._Hit && result._Distance < minDistance)
			{
				minDistance = result._Distance;
				minObject = object;
			}
		}

		// Check if there is an intersection
		if (minObject != NULL)
		{
			// Shade
			RTShader* shader = minObject->GetShader();
			if (shader == NULL)
			{
				// No shader, return white
				colour = Colour32::White;
				return;
			}

			// Point of intersection
			state._Point = state._Ray.Origin + state._Ray.Direction * result._Distance;

			// Normal at that position
			state._Normal = minObject->GetNormal(state._Point);

			// Texture coordinates at that position
			state._TexCoord = minObject->GetUV(state._Point);

			if (state._RayType == RayType_Shadow)
			{
				colour = Colour32::Black;
				return;
			}

			RTSceneObject* object = state._Object;
			state._Object = minObject;

			bool allLights = false;
			if (shader->GetLights().IsEmpty())
			{
				allLights = true;
				shader->GetLights() = _Lights;
			}
			shader->Shade(state, colour);

			if (allLights)
				shader->GetLights().Clear();

#if 1
			// Reflection
			Colour32 refl = Colour32::Black;
			shader->Reflect(state, refl);
			colour.r += refl.r;
			colour.g += refl.g;
			colour.b += refl.b;
#endif

#if 1
			// Refraction
			Colour32 refr = Colour32::Black;
			shader->Refract(state, refr);
			colour.r += refr.r;
			colour.g += refr.g;
			colour.b += refr.b;
#endif
			state._Object = object;
		}
		else
		{
			if (state._RayType == RayType_Shadow)
			{
				colour = Colour32::White;
				return;
			}

			if (state._RayType == RayType_Eye)
			{
				// No intersection, return the background colour
				colour = _BackgroundColour;
				return;
			}
		}
	}

	void RTScene::TraceShadow(RenderState& state, RTLight* light, real32& shadow)
	{
		RayType rayType = state._RayType;
		Vector3 rayOrigin = state._Ray.Origin;
		Vector3 rayDir = state._Ray.Direction;
		Vector3 point = state._Point;
		Vector3 normal = state._Normal;
		Vector2 texCoord = state._TexCoord;

		state._RayType = RayType_Shadow;
		// Compute the light direction
		Vector3 lightDiff = light->GetPosition() - state._Point;
		real32 lightDist = lightDiff.Length();
		Vector3 lightDir = lightDiff / lightDist;
		state._Ray.Origin = state._Point + lightDir * Math::Epsilon;
		state._Ray.Direction = lightDir;

		// Trace the shadow
		shadow = 1.0f;
		TraceResult result;
		RTSceneObject* object;
		foreach (object, _Objects, List<RTSceneObject*>)
		{
			if (object == state._Object)
				continue;

			object->Intersect(state, state._Ray, result);

			if (result._Hit)
			{
				shadow = 0.0f;
				break;
			}
		}

		state._Point = point;
		state._Normal = normal;
		state._TexCoord = texCoord;
		state._Ray.Direction = rayDir;
		state._Ray.Origin = rayOrigin;
		state._RayType = rayType;
	}
}
