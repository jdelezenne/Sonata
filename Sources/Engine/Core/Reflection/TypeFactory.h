/*=============================================================================
TypeFactory.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TYPEFACTORY_H_
#define _SE_TYPEFACTORY_H_

#include "Core/Common.h"
#include "Core/Singleton.h"
#include "Core/String.h"
#include "Core/Containers/Array.h"
#include "Core/Containers/Dictionary.h"

namespace SonataEngine
{

class TypeInfo;
class Object;

typedef BaseArray<TypeInfo*> TypeInfoList;

/** Type Factory. */
class SE_CORE_EXPORT TypeFactory : public Singleton<TypeFactory>
{
public:
	/** Destructor. */
	~TypeFactory();

	/** Registers the specified Type. */
	void RegisterType(TypeInfo* type);

	/** Unregisters the specified Type. */
	void UnregisterType(TypeInfo* type);

	/** Gets a Type by its name. */
	TypeInfo* GetType(const String& typeName);

	/** Gets a Type by its name. */
	TypeInfoList GetTypes();

	/** Creates an instance of the specified Type. */
	Object* CreateInstance(TypeInfo* type);

	/** Creates an instance of the Type whose name is specified. */
	Object* CreateInstance(const String& typeName);

private:
	typedef Dictionary<String, TypeInfo*> TypeTable;
	TypeTable _types;
};

}

#endif 
