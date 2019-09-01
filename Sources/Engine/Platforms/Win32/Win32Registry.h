/*=============================================================================
Win32Registry.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32REGISTRY_H_
#define _SE_WIN32REGISTRY_H_

#include "Win32Platform.h"
#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{
namespace Win32
{

/** Specifies the data types to use when storing values in the registry, or identifies the data type of a value in the registry. */
enum RegistryValueType
{
	/// Indicates an unsupported registry data type.
	RegistryValueType_Unknown = 0,

	/// Specifies a null-terminated string.
	RegistryValueType_String = 1,

	/// Specifies a null-terminated string that contains unexpanded references to environment variables.
	RegistryValueType_ExpandString = 2,

	/// Specifies binary data in any form.
	RegistryValueType_Binary = 3,

	/// Specifies a 32-bit binary number.
	RegistryValueType_DWord = 4,

	/// Specifies an array of null-terminated strings, terminated by two null characters.
	RegistryValueType_MultiString = 7,

	/// Specifies a 64-bit binary number.
	RegistryValueType_QWord = 11
};


/**
	@brief Win32 Registry Key.
	Represents a key-level node in the Windows registry.
*/
class SE_CORE_EXPORT RegistryKey
{
private:
	HKEY _hKey;
	String _keyName;
	bool _isWritable;
	bool _isSystemKey;
	bool _isRemoteKey;
	bool _isPerfData;

public:
	RegistryKey();
	RegistryKey(HKEY hKey, const String& keyName, bool isWritable);
	RegistryKey(HKEY hKey, const String& keyName, bool isWritable, bool isSystemKey, bool isRemoteKey, bool isPerfData);
	~RegistryKey();

	/** Retrieves the name of the key. */
	String GetName() const;

	/** Retrieves the count of values in the key. */
	int GetValueCount() const;

	/** Retrieves the count of subkeys of the current key. */
	int GetSubKeyCount() const;

	/** Closes the key and flushes it to disk if its contents have been modified. */
	void Close();

	/** Writes all the attributes of the specified open registry key into the registry. */
	void Flush();

	/** Retrieves the registry data type of the value associated with the specified name. */
	RegistryValueType GetValueType(const String& name) const;

	/** Retrieves an array of strings that contains all the subkey names. */
	void GetSubKeyNames(Array<String>& names);

	/** Retrieves the value associated with the specified name. */
	bool GetValue(const String& name, void** value, void* defaultValue = NULL);

	/** Sets the value of a name/value pair in the registry key, using the specified registry data type. */
	bool SetValue(const String& name, void* value, int32 size, RegistryValueType valueType);

	/** Deletes the specified value from this key. */
	void DeleteValue(const String& name);

	/** Retrieves a specified subkey. */
	bool OpenSubKey(const String& name, RegistryKey& subkey, bool isWritable = false);

	/** Creates a new subkey or opens an existing subkey. */
	bool CreateSubKey(const String& name, RegistryKey& subkey);

	/** Deletes the specified subkey. */
	bool DeleteSubKey(const String& name);
};


/**
	@brief Win32 Registry.
	Provides RegistryKey objects that represent the root keys in the Windows registry, and static methods to access key/value pairs.
*/
class SE_CORE_EXPORT Registry
{
public:
	/// Defines the types (or classes) of documents and the properties associated with those types.
	static const RegistryKey ClassesRoot;

	/// Contains information about the current user preferences.
	static const RegistryKey CurrentUser;

	/// Contains the configuration data for the local machine.
	static const RegistryKey LocalMachine;

	/// Contains information about the default user configuration.
	static const RegistryKey Users;

	/// Contains performance information for software components.
	static const RegistryKey PerformanceData;

	/// Contains configuration information pertaining to the hardware that is not specific to the user.
	static const RegistryKey CurrentConfig;

	/// Contains dynamic registry data.
	static const RegistryKey DynData;

public:
	/** Retrieves the value associated with the specified name, in the specified registry key. */
	static bool GetValue(const String& keyName, const String& valueName, void** value, void* defaultValue = NULL);

	/** Sets the name/value pair on the specified registry key, using the specified registry data type. If the specified key does not exist, it is created. */
	static bool SetValue(const String& keyName, const String& valueName, void* value, int32 size, RegistryValueType valueType);

private:
	static bool GetBaseKeyFromKeyName(const String& keyName, RegistryKey &key, String& subKeyName);
};

}
}

#endif 
