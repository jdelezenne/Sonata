/*=============================================================================
String.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE String::String() :
	_string()
{
}

SE_INLINE String::String(const SEchar* value)
{
	if (value != NULL)
	{
		_string = value;
	}
}

SE_INLINE String::String(const SEchar value) :
	_string(1, value)
{
}

SE_INLINE String::String(const Char* value)
{
	if (value != NULL)
	{
		_string = (const SEchar*)value;
	}
}

SE_INLINE String::String(const Char value) :
	_string(1, value)
{
}

SE_INLINE String::String(const String& value) :
	_string(value.Data())
{
}

/*SE_INLINE String::operator const SEchar*() const
{
	return _string.c_str();
}

SE_INLINE String::operator const Char*() const
{
	return (const Char*)_string.c_str();
}*/

SE_INLINE Char String::operator[](int index) const
{
	SE_ASSERT(0 <= index && index <= (int)_string.length());
	return _string[index];
}

SE_INLINE SEchar& String::operator[](int index)
{
	SE_ASSERT(0 <= index && index <= (int)_string.length());
	return _string[index];
}

SE_INLINE String& String::operator+=(const String& value)
{
	_string.append(value.Data());
	return *this;
}

SE_INLINE String& String::operator+=(const SEchar* value)
{
	_string.append(value);
	return *this;
}

#if SE_USE_UNICODE
SE_INLINE String& String::operator+=(const SEchar value)
{
	_string.append(1, value);
	return *this;
}
#endif

/*SE_INLINE String String::operator+(const String& value) const
{
	return (_string + value.Data()).c_str();
}

SE_INLINE String String::operator+(const SEchar* value) const
{
	return (_string + value).c_str();
}

#if SE_USE_UNICODE
SE_INLINE String String::operator+(const SEchar value) const
{
	return (_string + value).c_str();
}
#endif*/

SE_INLINE bool String::operator==(const String& value) const
{
	return _string.compare(value.Data()) == 0;
}

SE_INLINE bool String::operator!=(const String& value) const
{
	return _string.compare(value.Data()) != 0;
}

SE_INLINE bool String::operator<(const String& value) const
{
	return _string.compare(value.Data()) < 0;
}

SE_INLINE bool String::operator>(const String& value) const
{
	return _string.compare(value.Data()) > 0;
}

SE_INLINE bool String::operator<=(const String& value) const
{
	return _string.compare(value.Data()) <= 0;
}

SE_INLINE bool String::operator>=(const String& value) const
{
	return _string.compare(value.Data()) >= 0;
}

SE_INLINE const SEchar* String::Data() const
{
	return _string.c_str();
}

SE_INLINE int String::Length() const
{
	return (int) _string.length();
}

SE_INLINE Char String::Chars(int index) const
{
	SE_ASSERT(0 <= index && index <= (int) _string.length());
	return _string[index];
}

SE_INLINE bool String::IsEmpty() const
{
	return _string.size() == 0;
}

SE_INLINE int String::IndexOf(Char value) const
{
	return (int) _string.find(value);
}

SE_INLINE int String::LastIndexOf(Char value) const
{
	return (int) _string.rfind(value);
}

SE_INLINE int String::Compare(const String& left, const String& right, bool ignoreCase)
{
	if (!ignoreCase)
	{
		return strcmp(left.Data(), right.Data());
	}
	else
	{
		return stricmp(left.Data(), right.Data());
	}
}

SE_INLINE int String::CompareTo(const String& str, bool ignoreCase) const
{
	return String::Compare(*this, str, ignoreCase);
}

SE_INLINE String String::Replace(const Char& oldValue, const Char& newValue)
{
	size_t length = _string.length();
	for (size_t i = 0; i < length; ++i)
	{
		if (_string[i] == oldValue)
		{
			_string.replace(i, 1, (const SEchar*) &newValue);
		}
	}
	return *this;
}

SE_INLINE String String::Replace(const String& oldValue, const String& newValue)
{
	//TODO
	return *this;
}

SE_INLINE String String::Remove(int start, int length)
{
	//InnerType res = _string.substr(0, start-1) + _string.substr(start+length);
	//return res.c_str();
	_string.replace(start, length, _T(""));
	return *this;
}

SE_INLINE String String::Substring(int start) const
{
	InnerType res = _string.substr(start);
	return res.c_str();
}

SE_INLINE String String::Substring(int start, int length) const
{
	InnerType res = _string.substr(start, length);
	return res.c_str();
}
