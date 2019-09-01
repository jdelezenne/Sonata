/*=============================================================================
OGPR.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPR_H_
#define _OGPR_H_

#include "GPR.h"
#include "GPRX.h"

#include "OGPRRenderer.h"
#include "OGPRBuffer.h"
#include "OGPRVertexFuncEnv.h"
#include "OGPRFragmentFuncEnv.h"
#include "OGPRPixelFuncEnv.h"

GPRRESULT OGPR_CreateRenderer(DWORD version, OGPRRenderer **renderer);

#endif
