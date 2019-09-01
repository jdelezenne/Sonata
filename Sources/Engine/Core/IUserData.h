/*=============================================================================
IUserData.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IUSERDATA_H_
#define _SE_IUSERDATA_H_

#include "Core/Common.h"
#include "Core/Variant.h"

namespace SonataEngine
{

/** Interface for user data containers. */
class SE_CORE_EXPORT IUserData
{
protected:
	Variant _userData;

public:
	/** Gets or sets user data associated with the object. */
	const Variant& GetUserData() const { return _userData; }
	void SetUserData(const Variant& value) { _userData = value; }

};

}

#endif 
