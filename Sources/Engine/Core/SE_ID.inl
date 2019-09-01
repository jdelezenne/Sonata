/*=============================================================================
SE_ID.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE SE_ID::SE_ID(uint32 d1, uint32 d2, uint32 d3, uint32 d4) :
	Data1(d1),
	Data2(d2),
	Data3(d3),
	Data4(d4)
{
}

SE_INLINE bool operator==(const SE_ID& left, const SE_ID& right) 
{
	return (left.Data1 == right.Data1 && left.Data2 == right.Data2 &&
		left.Data3 == right.Data3 && left.Data4 == right.Data4);
}

SE_INLINE bool operator!=(const SE_ID& left, const SE_ID& right) 
{
	 return (!(left == right));
}

SE_INLINE bool operator<(const SE_ID& left, const SE_ID& right) 
{
	if (left.Data[0] != right.Data[0])
	{
		return (left.Data[0] < right.Data[0]);
	}
	else
	{
		if (left.Data[1] != right.Data[1])
		{
			return (left.Data[1] < right.Data[1]);
		}
		else
		{
			if (left.Data[2] != right.Data[2])
			{
				return (left.Data[2] < right.Data[2]);
			}
			else
			{
				return (left.Data[3] < right.Data[3]);
			}
		}
	}
}

SE_INLINE bool operator<=(const SE_ID& left, const SE_ID& right) 
{
	return (left == right || left < right);
}

SE_INLINE bool operator>(const SE_ID& left, const SE_ID& right) 
{
	if (left.Data[0] != right.Data[0])
	{
		return (left.Data[0] > right.Data[0]);
	}
	else
	{
		if (left.Data[1] != right.Data[1])
		{
			return (left.Data[1] > right.Data[1]);
		}
		else
		{
			if (left.Data[2] != right.Data[2])
			{
				return (left.Data[2] > right.Data[2]);
			}
			else
			{
				return (left.Data[3] > right.Data[3]);
			}
		}
	}
}

SE_INLINE bool operator>=(const SE_ID& left, const SE_ID& right) 
{
	return (left == right || left > right);
}

SE_INLINE void SE_ID::Clear()
{
	Data[0] = 0;
	Data[1] = 0;
	Data[2] = 0;
	Data[3] = 0;
}

SE_INLINE bool SE_ID::IsValid()
{
	return Data1 && Data2 && Data3 && Data4;
}

SE_INLINE String SE_ID::ToString()
{
	return String::Format(_T("%4X-%4X-%4X-%4X"));
}

SE_INLINE SE_ID SE_ID::FromString(const String& value)
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException(_T("value"), _T("The string is empty.")));
		return SE_ID();
	}

	Array<String> values = value.Split(_T('-'));
	if (values.Count() != 4)
	{
		SEthrow(ArgumentException(_T("value"), _T("A valid SE_ID contains 4 values separated by hyphens.")));
		return SE_ID();
	}

	SE_ID result;
	for (int i = 0; i < 4; ++i)
	{
		result.Data[i] = values[i].ToInt32();
	}
	return result;
}
