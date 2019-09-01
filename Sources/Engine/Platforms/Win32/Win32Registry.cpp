/*=============================================================================
Win32Registry.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Registry.h"

namespace SonataEngine
{
namespace Win32
{

static String RegistryKey_KeyNames[] =
{
	"HKEY_CLASSES_ROOT",
	"HKEY_CURRENT_USER",
	"HKEY_LOCAL_MACHINE",
	"HKEY_USERS",
	"HKEY_PERFORMANCE_DATA",
	"HKEY_CURRENT_CONFIG",
	"HKEY_DYN_DATA"
};
   
static RegistryKey RegistryKey_GetBaseKey(HKEY hKey)
{
      int index = (int)hKey & 0xfffffff;
      bool isPerfData = (hKey == HKEY_PERFORMANCE_DATA);
      RegistryKey key = RegistryKey(hKey, RegistryKey_KeyNames[index], true, true, false, isPerfData);
      return key;
}


RegistryKey::RegistryKey() :
	_hKey(NULL),
	_isWritable(false),
	_isSystemKey(false),
	_isRemoteKey(false),
	_isPerfData(false)
{
}

RegistryKey::RegistryKey(HKEY hKey, const String& keyName, bool isWritable) :
	_hKey(hKey),
	_keyName(keyName),
	_isWritable(isWritable),
	_isSystemKey(false),
	_isRemoteKey(false),
	_isPerfData(false)
{
}

RegistryKey::RegistryKey(HKEY hKey, const String& keyName, bool isWritable, bool isSystemKey, bool isRemoteKey, bool isPerfData) :
	_hKey(hKey),
	_keyName(keyName),
	_isWritable(isWritable),
	_isSystemKey(isSystemKey),
	_isRemoteKey(isRemoteKey),
	_isPerfData(isPerfData)
{
}

RegistryKey::~RegistryKey()
{
	Close();
}

String RegistryKey::GetName() const
{
	return _keyName;
}

int RegistryKey::GetValueCount() const
{
	DWORD cValues = 0;
	LONG ret = ::RegQueryInfoKey(_hKey, NULL, NULL, NULL, NULL, NULL, NULL, &cValues, NULL, NULL, NULL, NULL);
	if (ret != 0)
	{
		return 0;
	}
	return cValues;
}

int RegistryKey::GetSubKeyCount() const
{
	DWORD cSubKeys = 0;
	LONG ret = ::RegQueryInfoKey(_hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	if (ret != 0)
	{
		return 0;
	}
	return cSubKeys;
}

void RegistryKey::Close()
{
	if (_hKey != NULL)
	{
		if (!_isSystemKey)
		{
			::RegCloseKey(_hKey);
		}
		_hKey = NULL;
	}
}

void RegistryKey::Flush()
{
	if (_hKey != NULL)
	{
		::RegFlushKey(_hKey);
	}
}

RegistryValueType RegistryKey::GetValueType(const String& name) const
{
	DWORD type = 0;
	DWORD cbData = 0;
	::RegQueryValueEx(_hKey, name.Data(), NULL, &type, NULL, &cbData);

	switch (type)
	{
	case REG_SZ:
	case REG_EXPAND_SZ:
	case REG_BINARY:
	case REG_DWORD_LITTLE_ENDIAN:
	case REG_DWORD_BIG_ENDIAN:
	case REG_MULTI_SZ:
	case REG_QWORD_LITTLE_ENDIAN:
		return (RegistryValueType)type;

	default:
		return RegistryValueType_Unknown;
	}
}

void RegistryKey::GetSubKeyNames(Array<String>& names)
{
	int count = GetSubKeyCount();
	if (count > 0)
	{
		names.SetCapacity(count);
		TCHAR pName[255];
		DWORD cbName;
		for (int index = 0; index < count; index++)
		{
			cbName = 255;
			if (::RegEnumKeyEx(_hKey, index, pName, &cbName, NULL, NULL, NULL, NULL) != 0)
			{
				return;
			}
			names[index] = pName;
		}
	}
}


bool RegistryKey::GetValue(const String& name, void** value, void* defaultValue)
{
	if (_hKey == NULL)
	{
		*value = defaultValue;
		return true;
	}

	DWORD type = 0;
	DWORD cbData = 0;
	if (::RegQueryValueEx(_hKey, name.Data(), NULL, &type, NULL, &cbData) != 0)
	{
		*value = defaultValue;
		return true;
	}

	BYTE* pData;
	switch (type)
	{
	case REG_NONE:
	case REG_LINK:
	case REG_RESOURCE_LIST:
	case REG_FULL_RESOURCE_DESCRIPTOR:
	case REG_RESOURCE_REQUIREMENTS_LIST:
		*value = defaultValue;
		return false;

	case REG_SZ:
	case REG_EXPAND_SZ:
	case REG_BINARY:
	case REG_DWORD_LITTLE_ENDIAN:
	case REG_DWORD_BIG_ENDIAN:
	case REG_MULTI_SZ:
	case REG_QWORD_LITTLE_ENDIAN:
		pData = new BYTE[cbData];
		::RegQueryValueEx(_hKey, name.Data(), NULL, &type, pData, &cbData);
		*value = pData;
		return true;

	default:
		*value = defaultValue;
		return false;
	}
}

bool RegistryKey::SetValue(const String& name, void* value, int32 size, RegistryValueType valueType)
{
	::RegSetValueEx(_hKey, name.Data(), 0, (DWORD)valueType, (BYTE*)value, size);
	return true;
}

void RegistryKey::DeleteValue(const String& name)
{
	if (_isWritable)
	{
		::RegDeleteValue(_hKey, name.Data());
	}
}

bool RegistryKey::OpenSubKey(const String& name, RegistryKey& subkey, bool isWritable)
{
	HKEY hKey;
	if (::RegOpenKeyEx(_hKey, name.Data(), 0, KEY_ALL_ACCESS, &hKey) != 0)
	{
		return false;
	}

	subkey = RegistryKey(hKey, name, true);
	return true;
}

bool RegistryKey::CreateSubKey(const String& name, RegistryKey& subkey)
{
	if (OpenSubKey(name, subkey))
	{
		return true;
	}

	HKEY hKey;
	if (::RegCreateKey(_hKey, name.Data(), &hKey) != 0)
	{
		return false;
	}

	subkey = RegistryKey(hKey, name, true);
	return true;
}

bool RegistryKey::DeleteSubKey(const String& name)
{
	if (::RegDeleteKey(_hKey, name.Data()) != 0)
	{
		return false;
	}

	return true;
}


const RegistryKey Registry::ClassesRoot = RegistryKey_GetBaseKey(HKEY_CLASSES_ROOT);
const RegistryKey Registry::CurrentUser = RegistryKey_GetBaseKey(HKEY_CURRENT_USER);
const RegistryKey Registry::LocalMachine = RegistryKey_GetBaseKey(HKEY_LOCAL_MACHINE);
const RegistryKey Registry::Users = RegistryKey_GetBaseKey(HKEY_USERS);
const RegistryKey Registry::PerformanceData = RegistryKey_GetBaseKey(HKEY_PERFORMANCE_DATA);
const RegistryKey Registry::CurrentConfig = RegistryKey_GetBaseKey(HKEY_CURRENT_CONFIG);
const RegistryKey Registry::DynData = RegistryKey_GetBaseKey(HKEY_DYN_DATA);

bool Registry::GetValue(const String& keyName, const String& valueName, void** value, void* defaultValue)
{
	String subKeyName;
	RegistryKey key;
	if (!Registry::GetBaseKeyFromKeyName(keyName, key, subKeyName))
	{
		return false;
	}
	RegistryKey subKey;
	if (!key.OpenSubKey(subKeyName, subKey))
	{
		return false;
	}

	if (subKey.GetValue(valueName, value, defaultValue))
	{
		subKey.Close();
		return false;
	}

	subKey.Close();
	return true;
}

bool Registry::SetValue(const String& keyName, const String& valueName, void* value, int32 size, RegistryValueType valueType)
{
	String subKeyName;
	RegistryKey key;
	if (!Registry::GetBaseKeyFromKeyName(keyName, key, subKeyName))
	{
		return false;
	}
	RegistryKey subKey;
	if (!key.OpenSubKey(subKeyName, subKey))
	{
		return false;
	}

	if (subKey.SetValue(valueName, value, size, valueType))
	{
		subKey.Close();
		return false;
	}

	subKey.Close();
	return true;
}

bool Registry::GetBaseKeyFromKeyName(const String& keyName, RegistryKey &key, String& subKeyName)
{
	String str;
	int index = keyName.IndexOf('\\');
	if (index != -1)
	{
		str = keyName.Substring(0, index).ToUpper();
	}
	else
	{
		str = keyName.ToUpper();
	}

	if (String::Compare(str, _T("HKEY_CLASSES_ROOT")) == 0)
	{
		key = Registry::ClassesRoot;
	}
	else if (String::Compare(str, _T("HKEY_CURRENT_USER")) == 0)
	{
		key = Registry::CurrentUser;
	}
	else if (String::Compare(str, _T("HKEY_LOCAL_MACHINE")) == 0)
	{
		key = Registry::LocalMachine;
	}
	else if (String::Compare(str, _T("HKEY_USERS")) == 0)
	{
		key = Registry::Users;
	}
	else if (String::Compare(str, _T("HKEY_PERFORMANCE_DATA")) == 0)
	{
		key = Registry::PerformanceData;
	}
	else if (String::Compare(str, _T("HKEY_CURRENT_CONFIG")) == 0)
	{
		key = Registry::CurrentConfig;
	}
	else if (String::Compare(str, _T("HKEY_DYN_DATA")) == 0)
	{
		key = Registry::DynData;
	}
	else
	{
		return false;
	}

	if (index == -1 || (index == keyName.Length()))
	{
		subKeyName = String::Empty;
	}
	else
	{
		subKeyName = keyName.Substring(index + 1, (keyName.Length() - index) - 1);
	}
	return true;
}

}
}
