/*=============================================================================
RenderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RenderSystem.h"

namespace SonataEngine
{

RenderSystem::RenderSystem() :
	Manager()
{
}

RenderSystem::~RenderSystem()
{
}

bool RenderSystem::Create()
{
	return true;
}

void RenderSystem::Update(const TimeValue& timeValue)
{
}

}
