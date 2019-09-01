/*=============================================================================
Attribute.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_Attribute_H_
#define _SE_Attribute_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Variant.h"

namespace SonataEngine
{

class Object;

/** Specifies the elements on which it is valid to apply an attribute. */
enum AttributeTargets
{
	AttributeTargets_namespace = (1<<0),
	AttributeTargets_Class = (1<<1),
	AttributeTargets_Struct = (1<<2),
	AttributeTargets_Enum = (1<<3),
	AttributeTargets_Constructor = (1<<4),
	AttributeTargets_Method = (1<<5),
	AttributeTargets_Property = (1<<6),
	AttributeTargets_Field = (1<<7),
	AttributeTargets_Event = (1<<8),
	AttributeTargets_Parameter = (1<<9),
	AttributeTargets_All = (1<<10)-1
};

/** Represents the base class for metadata attributes. */
class SE_CORE_EXPORT Attribute
{
public:
	/** Destructor. */
	virtual ~Attribute()
	{}

	/// Gets which elements the attribute can be applied to.
	virtual AttributeTargets GetTargets() const { return AttributeTargets_All; }

	/// Gets a value indicating whether the attribute can be inherited by derived classes and overriding members.
	virtual bool GetInherited() const { return true; }
};

/** Specifies the default value for a property. */
class SE_CORE_EXPORT DefaultValueAttribute : public Attribute
{
	static const DefaultValueAttribute Default;

	/** Constructor. */
	DefaultValueAttribute() :
		_Value(Variant::Invalid)
	{}

	/// Constructor with a value.
	DefaultValueAttribute(const Variant& value) :
		 _Value(value)
	{}

	/** Destructor. */
	virtual ~DefaultValueAttribute()
	{}

	/// Gets the default value of the property this attribute is bound to.
	const Variant GetValue() const { return _Value; }

protected:
	Variant _Value;
};

/** Specifies the display name for a property. */
class SE_CORE_EXPORT DisplayNameAttribute : public Attribute
{
public:
	static const DisplayNameAttribute Default;

	/** Constructor. */
	DisplayNameAttribute()
	{}

	/// Constructor with a value.
	DisplayNameAttribute(const String& value) :
		 _DisplayName(value)
	{}

	/** Destructor. */
	virtual ~DisplayNameAttribute()
	{}

	/// Gets the category stored in this attribute.
	String GetDisplayName() const { return _DisplayName; }

protected:
	String _DisplayName;
};

/** Specifies a category for a property. */
class SE_CORE_EXPORT CategoryAttribute : public Attribute
{
public:
	static const CategoryAttribute Default;

	/** Constructor. */
	CategoryAttribute()
	{}

	/// Constructor with a value.
	CategoryAttribute(const String& value) :
		 _Category(value)
	{}

	/** Destructor. */
	virtual ~CategoryAttribute()
	{}

	/// Gets the category stored in this attribute.
	String GetCategory() const { return _Category; }

protected:
	String _Category;
};

/** Specifies a description for a property. */
class SE_CORE_EXPORT DescriptionAttribute : public Attribute
{
public:
	static const DescriptionAttribute Default;

	/** Constructor. */
	DescriptionAttribute()
	{}

	/// Constructor with a value.
	DescriptionAttribute(const String& value) :
		 _Description(value)
	{}

	/** Destructor. */
	virtual ~DescriptionAttribute()
	{}

	/// Gets the description stored in this attribute.
	String GetDescription() const { return _Description; }

protected:
	String _Description;
};

/** Specifies whether the property this attribute is bound to is read-only. */
class SE_CORE_EXPORT ReadOnlyAttribute : public Attribute
{
	static const ReadOnlyAttribute Default;
	static const ReadOnlyAttribute No;
	static const ReadOnlyAttribute Yes;

	/** Constructor. */
	ReadOnlyAttribute() :
		_ReadOnly(ReadOnlyAttribute::Default.IsReadOnly())
	{}

	/// Constructor with a value.
	ReadOnlyAttribute(bool value) :
		 _ReadOnly(value)
	{}

	/** Destructor. */
	virtual ~ReadOnlyAttribute()
	{}

	/// Gets a value indicating whether the property this attribute is bound to is read-only.
	bool IsReadOnly() const { return _ReadOnly; }

protected:
	bool _ReadOnly;
};

/** Specifies the display name for a property. */
class SE_CORE_EXPORT PasswordTextAttribute : public Attribute
{
public:
	static const PasswordTextAttribute Default;
	static const PasswordTextAttribute No;
	static const PasswordTextAttribute Yes;

	/** Constructor. */
	PasswordTextAttribute()
	{}

	/// Constructor with a value.
	PasswordTextAttribute(bool value) :
		 _IsPassword(value)
	{}

	/** Destructor. */
	virtual ~PasswordTextAttribute()
	{}

	/// Gets a value indicating if the property this attribute is bound to should be shown as password text.
	bool IsPassword() const { return _IsPassword; }

protected:
	bool _IsPassword;
};

/** Indicates that an enumeration can be treated as a set of flags. */
class SE_CORE_EXPORT FlagsAttribute : public Attribute
{
public:
	/** Constructor. */
	FlagsAttribute()
	{}

	/** Destructor. */
	virtual ~FlagsAttribute()
	{}

	virtual AttributeTargets GetTargets() const { return AttributeTargets_Enum; }
	virtual bool GetInherited() const { return false; }
};

/** Specifies the range of values for a property. */
class SE_CORE_EXPORT RangeAttribute : public Attribute
{
	static const DefaultValueAttribute Default;

	/** Constructor. */
	RangeAttribute() :
		_Minimum(Variant::Invalid),
		_Maximum(Variant::Invalid)
	{}

	/// Constructor with a range.
	RangeAttribute(const Variant& minimum, const Variant& maximum) :
		 _Minimum(minimum),
		 _Maximum(maximum)
	{}

	/** Destructor. */
	virtual ~RangeAttribute()
	{}

	/** Gets the minimum value of the property this attribute is bound to. */
	const Variant GetMinimum() const { return _Minimum; }

	/** Gets the maximum value of the property this attribute is bound to. */
	const Variant GetMaximum() const { return _Maximum; }

protected:
	Variant _Minimum;
	Variant _Maximum;
};

#define SE_Attribute(attribute) \
	builder.SetAttribute(new attribute);

}

#endif 
