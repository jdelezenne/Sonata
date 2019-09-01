/*=============================================================================
Operator.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATOR_H_
#define _PROCEDURAL_OPERATOR_H_

#include "Common.h"

class Operator;
class Workflow;

Variant ToVariant(const Color8& value);
Variant ToVariant(const Color32& value);
Variant ToVariant(const Vector3& value);

Color8 VariantToColor8(const Variant& value);
Color32 VariantToColor32(const Variant& value);
Vector3 VariantToVector3(const Variant& value);

Color8 ToColor8(const Color32& c);
Color32 ToColor32(const Color8& c);

class OperatorSlot
{
public:
	OperatorSlot(Operator* owner);
	virtual ~OperatorSlot();

	Operator* GetOwner() const { return _Owner; }
	void SetOwner(Operator* value) { _Owner = value; }

	const String& GetName() const { return _name; }
	void SetName(const String& value) { _name = value; }

	const TypeInfo* GetType() const { return _Type; }
	void SetType(const TypeInfo* value) { _Type = value; }

	Variant GetValue() const;
	void SetValue(const Variant& value);

protected:
	Operator* _Owner;
	String _name;
	const TypeInfo* _Type;
};

typedef Array<OperatorSlot*> OperatorSlotList;

class OperatorProperty
{
public:
	OperatorProperty(Operator* owner, const FieldInfo* field);
	virtual ~OperatorProperty();

	Operator* GetOwner() const { return _Owner; }
	void SetOwner(Operator* value) { _Owner = value; }

	const String& GetName() const { return _name; }
	void SetName(const String& value) { _name = value; }

	bool GetCanLink() const { return _CanLink; }
	void SetCanLink(bool value) { _CanLink = value; }

	const TypeInfo* GetType() const;

	Variant GetValue() const;
	void SetValue(const Variant& value);

	bool IsLinked() const;
	OperatorSlot* GetSlot() const { return _Slot; }
	void LinkTo(OperatorSlot* slot);
	void Unlink();

protected:
	Operator* _Owner;
	const FieldInfo* _Field;
	String _name;
	bool _CanLink;
	OperatorSlot* _Slot;
};

typedef Array<OperatorProperty*> OperatorPropertyList;

class Operator : public RefObject
{
	SE_DECLARE_ABSTRACT(Operator, Object);

	SE_BEGIN_REFLECTION(Operator);
		SE_Field(_ID, int32, Public);
		SE_Field(_name, String, Public);
	SE_END_REFLECTION(Operator);

public:
	Operator();
	virtual ~Operator();

	virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream);
	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	Workflow* GetWorkflow() const { return _Workflow; }
	void SetWorkflow(Workflow* value) { _Workflow = value; }

	int32 GetID() const { return _ID; }
	void SetID(int32 value) { _ID = value; }

	const String& GetName() const { return _name; }
	void SetName(const String& value) { _name = value; }

	const OperatorPropertyList& Properties() const { return _Properties; }

	const OperatorSlotList& Slots() const { return _Slots; }

	OperatorProperty* Property(const String& name);
	OperatorSlot* Slot(const String& name);

	bool IsValid() const { return _Valid; }
	void Invalidate();

	bool HasSource(Operator* op) const;

	virtual Variant GetSlotValue(const String& name) = 0;
	virtual void SetSlotValue(const String& name, const Variant& value) = 0;

	UI::TableNode* GetNode() const { return _Node; }

	virtual UI::TableNode* CreateNode();

	void Refresh();
	virtual void DoRefresh() = 0;

	virtual void Create();

	virtual void Destroy();

	virtual void Update();

protected:
	Workflow* _Workflow;
	int32 _ID;
	String _name;
	OperatorPropertyList _Properties;
	OperatorSlotList _Slots;

	bool _Valid;
	UI::TableNode* _Node;

	friend class Workflow;
	PointInt _Position;
	struct SlotRef
	{
		int32 _Index;
		int32 _ID;
		String _name;
	};
	BaseArray<SlotRef> _SlotRefs;
};

#endif 
