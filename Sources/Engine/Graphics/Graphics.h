/*=============================================================================
Graphics.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GRAPHICS_H_
#define _SE_GRAPHICS_H_

// Graphics
#include "Graphics/Common.h"
#include "Graphics/Image.h"
#include "Graphics/ImageHelper.h"
#include "Graphics/Palette.h"
#include "Graphics/PixelFormat.h"
#include "Graphics/Palette.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Viewport.h"
#include "Graphics/VertexFormats/VertexFormats.h"

// Animation
#include "Graphics/Animation/Animation.h"

// Font
#include "Graphics/Font/BitmapFontProvider.h"
#include "Graphics/Font/Font.h"
#include "Graphics/Font/FontFamily.h"
#include "Graphics/Font/FontProvider.h"
#include "Graphics/Font/SystemFonts.h"
#include "Graphics/Font/Text.h"

// IO
#include "Graphics/IO/ImageDataPlugin.h"
#include "Graphics/IO/ImageReader.h"
#include "Graphics/IO/ImageWriter.h"
#include "Graphics/IO/ModelDataPlugin.h"
#include "Graphics/IO/ModelReader.h"
#include "Graphics/IO/ModelWriter.h"
#include "Graphics/IO/SceneDataPlugin.h"
#include "Graphics/IO/SceneReader.h"
#include "Graphics/IO/SceneWriter.h"

// Lighting
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Lighting/Light.h"
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/SpotLight.h"

// Material
#include "Graphics/Materials/ShaderMaterial.h"
#include "Graphics/Materials/DefaultMaterial.h"
#include "Graphics/Materials/EffectMaterial.h"
#include "Graphics/Materials/SasEffectMaterial.h"
#include "Graphics/Materials/UberMaterial.h"

// Model
#include "Graphics/Model/Bone.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Model/MeshPart.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Model/Skeleton.h"
#include "Graphics/Model/Skin.h"
#include "Graphics/Model/Socket.h"
#include "Graphics/Model/VertexElement.h"
#include "Graphics/Model/VertexLayout.h"

// Scene
#include "Graphics/Scene/Camera.h"
#include "Graphics/Scene/ModelNode.h"
#include "Graphics/Scene/Scene.h"
#include "Graphics/Scene/SceneNode.h"
#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Scene/Sky.h"
#include "Graphics/Scene/SkyBox.h"
#include "Graphics/Scene/SkyDome.h"
#include "Graphics/Scene/TerrainNode.h"

// Shader
#include "Graphics/Shader/EffectAnnotation.h"
#include "Graphics/Shader/EffectFunction.h"
#include "Graphics/Shader/EffectParameter.h"
#include "Graphics/Shader/EffectPass.h"
#include "Graphics/Shader/EffectShader.h"
#include "Graphics/Shader/EffectTechnique.h"
#include "Graphics/Shader/FFPEffectShader.h"
#include "Graphics/Shader/FFPPass.h"
#include "Graphics/Shader/FFPTechnique.h"
#include "Graphics/Shader/GeometryShaderProgram.h"
#include "Graphics/Shader/PixelShaderProgram.h"
#include "Graphics/Shader/ShaderParameter.h"
#include "Graphics/Shader/ShaderParameterDesc.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Shader/ShaderState.h"
#include "Graphics/Shader/ShaderSystem.h"
#include "Graphics/Shader/VertexShaderProgram.h"

// States
#include "Graphics/States/States.h"

// Shapes
#include "Graphics/Shapes/BoxShape.h"
#include "Graphics/Shapes/PlaneShape.h"
#include "Graphics/Shapes/Shape.h"
#include "Graphics/Shapes/SphereShape.h"

// System
#include "Graphics/System/DisplayAdapter.h"
#include "Graphics/System/DisplayCapabilities.h"
#include "Graphics/System/DisplayMonitor.h"
#include "Graphics/System/DisplayMode.h"
#include "Graphics/System/HardwareBuffer.h"
#include "Graphics/System/RenderContext.h"
#include "Graphics/System/RenderData.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/System/RenderTarget.h"
#include "Graphics/System/RenderTexture.h"
#include "Graphics/System/Texture.h"

// Terrain
#include "Graphics/Terrain/Terrain.h"
#include "Graphics/Terrain/HeightFieldGenerator.h"
#include "Graphics/Terrain/HeightFieldFilter.h"
#include "Graphics/Terrain/QuadTerrainRenderer.h"
#include "Graphics/Terrain/StaticTerrainRenderer.h"
#include "Graphics/Terrain/TerrainTextureGenerator.h"

#endif
