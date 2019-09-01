/*=============================================================================
Dictionary.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class TKey, class TValue>
Dictionary<TKey, TValue>::KeyValuePair::KeyValuePair(const TKey& key, const TValue& value) :
	_pair(key, value)
{
}

template <class TKey, class TValue>
TKey Dictionary<TKey, TValue>::KeyValuePair::Key() const
{
	return _pair.first;
}

template <class TKey, class TValue>
TValue Dictionary<TKey, TValue>::KeyValuePair::Value() const
{
	return _pair.second;
}


template <class TKey, class TValue>
Dictionary<TKey, TValue>::DictionaryEntry::DictionaryEntry() :
	_iterator(NULL)
{
}

template <class TKey, class TValue>
Dictionary<TKey, TValue>::DictionaryEntry::DictionaryEntry(typename InnerType::const_iterator iterator) :
	_iterator(iterator)
{
}

template <class TKey, class TValue>
TKey Dictionary<TKey, TValue>::DictionaryEntry::GetKey() const
{
	return _iterator->first;
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::DictionaryEntry::SetKey(const TKey& key)
{
}

template <class TKey, class TValue>
TValue Dictionary<TKey, TValue>::DictionaryEntry::GetValue() const
{
	return _iterator->second;
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::DictionaryEntry::SetValue(const TValue& value)
{
}


template <class TKey, class TValue>
Dictionary<TKey, TValue>::DictionaryIterator::DictionaryIterator(const Dictionary<TKey, TValue>* dictionary) :
	_dictionary(dictionary),
	_isFirst(true)
{
	_it = _dictionary->_internal.begin();
}

template <class TKey, class TValue>
typename Dictionary<TKey, TValue>::DictionaryIterator& Dictionary<TKey, TValue>::DictionaryIterator::operator++()
{
	Next();
	return *this;
}

template <class TKey, class TValue>
bool Dictionary<TKey, TValue>::DictionaryIterator::Next()
{
	if (_it == _dictionary->_internal.end())
	{
		return false;
	}
	else if (_isFirst)
	{
		_isFirst = false;
		return true;
	}
	else
	{
		_it++;
		return (_it != _dictionary->_internal.end());
	}
}

template <class TKey, class TValue>
typename IKeyValuePair<TKey, TValue> Dictionary<TKey, TValue>::DictionaryIterator::Current() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return KeyValuePair(_it->first, _it->second);
}

template <class TKey, class TValue>
typename IDictionaryEntry<TKey, TValue> Dictionary<TKey, TValue>::DictionaryIterator::Entry() const
{
	Dictionary<TKey, TValue>::DictionaryEntry entry(_it);
	return entry;
}

template <class TKey, class TValue>
TKey Dictionary<TKey, TValue>::DictionaryIterator::Key() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return _it->first;
}

template <class TKey, class TValue>
TValue Dictionary<TKey, TValue>::DictionaryIterator::Value() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return _it->second;
}


template <class TKey, class TValue>
Dictionary<TKey, TValue>::Dictionary() :
	_internal()
{
}

template <class TKey, class TValue>
Dictionary<TKey, TValue>::Dictionary(int size) :
	_internal(size)
{
}

template <class TKey, class TValue>
Dictionary<TKey, TValue>::Dictionary(const Dictionary<TKey, TValue>& value)
{
	Dictionary<TKey, TValue>::DictionaryIterator it = value.GetIterator();
	while (it.Next())
	{
		Add(it.Key(), it.Value());
	}
}

template <class TKey, class TValue>
Dictionary<TKey, TValue>::~Dictionary()
{
}

template <class TKey, class TValue>
Dictionary<TKey, TValue>& Dictionary<TKey, TValue>::operator=(const Dictionary<TKey, TValue>& value)
{
	if (this != &value)
	{
		Clear();
		DictionaryIterator it = value.GetIterator();
		while (it.Next())
		{
			_internal.insert(InnerPair(it.Key(), it.Value()));
		}
	}
	return *this;
}

template <class TKey, class TValue>
TValue& Dictionary<TKey, TValue>::operator[](const TKey& key)
{
	if (!Contains(key))
	{
		Add(key, TValue());
	}
	return GetItem(key);
}

template <class TKey, class TValue>
const TValue& Dictionary<TKey, TValue>::operator[](const TKey& key) const
{
	return GetItem(key);
}

template <class TKey, class TValue>
typename Dictionary<TKey, TValue>::DictionaryIterator Dictionary<TKey, TValue>::GetIterator() const
{
	DictionaryIterator it(this);
	return it;
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::Clear()
{
	_internal.clear();
}

template <class TKey, class TValue>
int Dictionary<TKey, TValue>::Count() const
{
	return (int) _internal.size();
}

template <class TKey, class TValue>
bool Dictionary<TKey, TValue>::IsEmpty() const
{
	return _internal.size() == 0;
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::Add(const TKey& key, const TValue& value)
{
	_internal.insert(InnerPair(key, value));
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::Remove(const TKey& key)
{
	_internal.erase(key);
}

template <class TKey, class TValue>
bool Dictionary<TKey, TValue>::Contains(const TKey& key) const
{
	return _internal.find(key) != _internal.end();
}

template <class TKey, class TValue>
bool Dictionary<TKey, TValue>::ContainsKey(const TKey& key) const
{
	return _internal.find(key) != _internal.end();
}

template <class TKey, class TValue>
bool Dictionary<TKey, TValue>::ContainsValue(const TValue& value) const
{
	//TODO
	return false;//_internal.find(value) != _internal.end();
}

template <class TKey, class TValue>
TValue& Dictionary<TKey, TValue>::GetItem(const TKey& key)
{
	return _internal.find(key)->second;
}

template <class TKey, class TValue>
const TValue& Dictionary<TKey, TValue>::GetItem(const TKey& key) const
{
	return _internal.find(key)->second;
}

template <class TKey, class TValue>
void Dictionary<TKey, TValue>::SetItem(const TKey& key, const TValue& value)
{
	_internal[key] = value;
}

template <class TKey, class TValue>
BaseArray<TKey> Dictionary<TKey, TValue>::Keys() const
{
	static BaseArray<TKey> values;

	typename Dictionary<TKey, TValue>::InnerType::const_iterator it;
	for (it = _internal.begin(); it != _internal.end(); ++it)
		values.Add(it->first);

	return values;
}

template <class TKey, class TValue>
BaseArray<TValue> Dictionary<TKey, TValue>::Values() const
{
	static BaseArray<TValue> values;

	typename Dictionary<TKey, TValue>::InnerType::const_iterator it;
	for (it = _internal.begin(); it != _internal.end(); ++it)
		values.Add(it->second);

	return values;
}
