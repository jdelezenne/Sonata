/*=============================================================================
Plugins.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLUGINS_H_
#define _SE_PLUGINS_H_

// Render Systems
#define SE_ID_RENDERSYSTEM_D3D8 SonataEngine::SE_ID(0x6DC4B2C2,0x8B1C40d2)
#define SE_ID_RENDERSYSTEM_D3D9 SonataEngine::SE_ID(0x37e123b3,0x60805177)
#define SE_ID_RENDERSYSTEM_GL SonataEngine::SE_ID(0xa8b7dfb,0x7a6a6432)

// Shader Systems
#define SE_ID_SHADERSYSTEM_D3D8CG SonataEngine::SE_ID(0xe6def879,0xb5d04c92)
#define SE_ID_SHADERSYSTEM_D3D9CG SonataEngine::SE_ID(0x1f85335a,0xee6e41a1)
#define SE_ID_SHADERSYSTEM_D3D8 SonataEngine::SE_ID(0x9AE96625,0xC33D455a)
#define SE_ID_SHADERSYSTEM_GLCG SonataEngine::SE_ID(0xb97030f3,0x263b4916)
#define SE_ID_SHADERSYSTEM_GLSL SonataEngine::SE_ID(0xef803744,0x8bfb42fd)
#define SE_ID_SHADERSYSTEM_HLSL9 SonataEngine::SE_ID(0x9c4b3cec,0x6b884d97)

// Input Systems
#define SE_ID_INPUTSYSTEM_DI8 SonataEngine::SE_ID(0x3cf17ff4,0x4667694e)
#define SE_ID_INPUTSYSTEM_WIN32 SonataEngine::SE_ID(0xec066c5a,0x155d4675)

// Audio Systems
#define SE_ID_AUDIOSYSTEM_AL SonataEngine::SE_ID(0xc7769051,0xb31840b5)
#define SE_ID_AUDIOSYSTEM_DS8 SonataEngine::SE_ID(0xc85e9ee1,0x36c84abe)

// Physics Systems
#define SE_ID_PHYSICSSYSTEM_NULL SonataEngine::SE_ID(0x513264a7,0x57c5414e)
#define SE_ID_PHYSICSSYSTEM_NX SonataEngine::SE_ID(0xd20af632,0x66754bdd)
#define SE_ID_PHYSICSSYSTEM_ODE SonataEngine::SE_ID(0x44628672,0x8b404788)
#define SE_ID_PHYSICSSYSTEM_RW SonataEngine::SE_ID(0xb8d5fef2,0x238c4cfc)
#define SE_ID_PHYSICSSYSTEM_SOFTWARE SonataEngine::SE_ID(0xdf45a724,0x984a4c42)

// Scripting Systems

// Data Image
#define SE_ID_DATAIMAGE_BMP SonataEngine::SE_ID(0x642f2227,0x329b48cc)
#define SE_ID_DATAIMAGE_DDS SonataEngine::SE_ID(0xfe915f7a,0xe91b46ca)
#define SE_ID_DATAIMAGE_DEVIL SonataEngine::SE_ID(0x125aaecb,0x13384c46)
#define SE_ID_DATAIMAGE_SDL SonataEngine::SE_ID(0x652a9ef5,0x63c34bae)
#define SE_ID_DATAIMAGE_TGA SonataEngine::SE_ID(0x514c6b30,0xe0f74db1)

// Data Model
#define SE_ID_DATAMODEL_3DS SonataEngine::SE_ID(0x99fb4c44,0xa2d8422a)
#define SE_ID_DATAMODEL_EMPEROR SonataEngine::SE_ID(0xe4a91d96,0xbe624d3a)
#define SE_ID_DATAMODEL_MD2 SonataEngine::SE_ID(0x124b422e,0x33ca4533)
#define SE_ID_DATAMODEL_MD3 SonataEngine::SE_ID(0xf37080b3,0x4c7a4305)
#define SE_ID_DATAMODEL_MDL SonataEngine::SE_ID(0x79e9f8f7,0xc8004c29)
#define SE_ID_DATAMODEL_OBJ SonataEngine::SE_ID(0x6caebfc6,0x39c94530)
#define SE_ID_DATAMODEL_PSK SonataEngine::SE_ID(0x8910527f,0x9b4c41a2)
#define SE_ID_DATAMODEL_SDKMESH SonataEngine::SE_ID(0xa7bef061, 0x5e784100)
#define SE_ID_DATAMODEL_W3D SonataEngine::SE_ID(0x8628129b,0x5e314bfa)
#define SE_ID_DATAMODEL_X SonataEngine::SE_ID(0x83cb826b,0xd37041c1)
#define SE_ID_DATAMODEL_XATG SonataEngine::SE_ID(0xed605bd6,0x3ca648d2)

// Data Scene
#define SE_ID_DATASCENE_BSP SonataEngine::SE_ID(0x5a2bb369,0x6ad94ae8)

// Data Sound
#define SE_ID_DATASOUND_WAV SonataEngine::SE_ID(0x9e143608,0xc3d1424d)

#endif
