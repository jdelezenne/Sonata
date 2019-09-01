/*=============================================================================
BaseArray.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BASEARRAY_H_
#define _SE_BASEARRAY_H_

#include <vector>
#include <algorithm>

#include "Core/Common.h"
#include "Core/Containers/IList.h"

namespace SonataEngine
{

/** Array for types that can't be compared. */
template <class T>
class BaseArray : public IList<T>
{
protected:
	typedef typename std::vector<T> InnerType;
	typename InnerType _internal;

	class BaseArrayIterator : public IIterator<T>
	{
	private:
		const BaseArray<T>* _array;
		typename InnerType::const_iterator _it;
		bool _isFirst;

	public:
		BaseArrayIterator(const BaseArray<T>* array);

		virtual BaseArrayIterator& operator++();
		virtual T Current() const;
		virtual bool Next();
	};

	friend BaseArrayIterator;

public:
	typedef typename BaseArrayIterator Iterator;
	typedef bool (*SortFunction) (const T& left, const T& right);

	BaseArray();
	BaseArray(int size);
	BaseArray(T* data, int size);
	BaseArray(const T* data, int size);
	BaseArray(const BaseArray<T>& value);
	virtual ~BaseArray();

	BaseArray<T>& operator=(const BaseArray<T>& value);
	virtual T& operator[](int index);
	virtual const T& operator[](int index) const;

	virtual Iterator GetIterator() const;

	virtual int GetCapacity();

	virtual void SetCapacity(int value);

	virtual void Resize(int value);

	virtual int Count() const;

	virtual bool IsEmpty() const;

	virtual void Clear();

	virtual void Add(const T& value);

	virtual void Insert(int index, const T& value) { SEthrow("NotSupportedException"); }

	virtual void Remove(const T& value) { SEthrow("NotSupportedException"); }

	virtual void RemoveAt(int index) { SEthrow("NotSupportedException"); }

	virtual bool Contains(const T& value) const { SEthrow("NotSupportedException"); return false; }

	virtual int IndexOf(const T& value) const { SEthrow("NotSupportedException"); return 0; }

	virtual T& GetItem(int index);
	virtual const T& GetItem(int index) const;

	virtual void SetItem(int index, const T& value);

	virtual void Sort(SortFunction fnSort);
};

#include "BaseArray.inl"

}

#endif
