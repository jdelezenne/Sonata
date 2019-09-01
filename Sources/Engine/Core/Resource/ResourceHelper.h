/*=============================================================================
ResourceHelper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RESOURCEHELPER_H_
#define _SE_RESOURCEHELPER_H_

#include "Core/Common.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource.h"

namespace SonataEngine
{

/**
	@brief Resource helper.
	Helper class for resource access.
*/
class SE_CORE_EXPORT ResourceHelper
{
public:
	static Resource* LoadFromFile(const String& fileName, const SE_ID& type);

	static bool SaveToFile(Resource* resource, const String& fileName);
};

}

#endif
