/*=============================================================================
Flags.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FLAGS_H_
#define _SE_FLAGS_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Flags.
	Flags enumerations are used for masking bit fields and doing bitwise comparisons.
*/
template <class Enum>
class Flag
{
public:
    typedef Enum EnumType;

	/** @name Constructors */
	//@{
	/** Creates an empty flag. */
	Flag();

	/** Creates an empty flag. */
	Flag(Enum value);

	/** Copy constructor. */
	Flag(const Flag& value);
	//@}

	SE_INLINE Flag& operator=(const Flag& f) { _Value = f._Value; return *this; }
	SE_INLINE Flag& operator&=(int32 mask) {  _Value &= mask; return *this; }
	SE_INLINE Flag& operator&=(uint32 mask) {  _Value &= mask; return *this; }
	SE_INLINE Flag& operator|=(Flag f) {  _Value |= f._Value; return *this; }
	SE_INLINE Flag& operator|=(Enum f) {  _Value |= f; return *this; }
	SE_INLINE Flag& operator^=(Flag f) {  _Value ^= f._Value; return *this; }
	SE_INLINE Flag& operator^=(Enum f) {  _Value ^= f; return *this; }

	SE_INLINE operator int() const { return _Value;}

	SE_INLINE bool operator!() const { return !_Value; }

	SE_INLINE Flag operator|(Flag f) const { Flag g; g._Value = _Value | f._Value; return g; }
	SE_INLINE Flag operator|(Enum f) const { Flag g; g._Value = _Value | f; return g; }
	SE_INLINE Flag operator^(Flag f) const { Flag g; g._Value = _Value ^ f._Value; return g; }
	SE_INLINE Flag operator^(Enum f) const { Flag g; g._Value = _Value ^ f; return g; }
	SE_INLINE Flag operator&(int32 mask) const { Flag g; g._Value = _Value & mask; return g; }
	SE_INLINE Flag operator&(uint32 mask) const { Flag g; g._Value = _Value & mask; return g; }
	SE_INLINE Flag operator&(Enum f) const { Flag g; g._Value = _Value & f; return g; }
	SE_INLINE Flag operator~() const { Flag g; g._Value = ~_Value; return g; }

	//TODO
	//void Add(Enum e);
	//void Remove(Enum e);
	//bool IsSet(Enum e) const;

protected:
	int32 _Value;
};

#include "Flags.inl"

#define SE_DECLARE_FLAGS(f, e) \
	typedef Flag<e> Flags;

#define SE_DECLARE_OPERATORS_FOR_FLAGS(f) \
	SE_INLINE Flag<f::EnumType> operator|(f::EnumType f1, f::EnumType f2) \
	{ \
		return Flags<Flags::enum_type>(f1) | f2; \
	} \
	SE_INLINE Flag<f::EnumType> operator|(f::EnumType f1, Flag<f::EnumType> f2) \
	{ \
		return f2 | f1; \
	}

}

#endif 
