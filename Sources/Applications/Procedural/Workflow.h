/*=============================================================================
Workflow.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_WORKFLOW_H_
#define _PROCEDURAL_WORKFLOW_H_

#include "Common.h"
#include "Operator.h"

class Operator;

typedef Array<Operator*> OperatorList;

class Workflow : public RefObject
{
	SE_DECLARE_CLASS(Workflow, Object);

	SE_BEGIN_REFLECTION(Workflow);
		SE_Field(_IDs, int32, Public);
		SE_Field(_Width, int32, Public);
		SE_Field(_Height, int32, Public);
	SE_END_REFLECTION(Workflow);

public:
	Workflow();
	virtual ~Workflow();

	virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream);
	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	static bool IsTypeCompatible(const TypeInfo* t0, const TypeInfo* t1);

	String GetFileName() const { return _fileName; }
	void SetFileName(String value) { _fileName = value; }

	Operator* GetSelectedOperator() const;
	void SetSelectedOperator(Operator* value);

	int32 GetWidth() const { return _Width; }
	void SetWidth(int32 value) { _Width = value; }

	int32 GetHeight() const { return _Height; }
	void SetHeight(int32 value) { _Height = value; }

	void InitializeOperators();
	void AddOperator(Operator* op);
	void RemoveOperator(Operator* op);
	Operator* GetOperatorByID(int32 id) const;

	UI::Diagram* GetDiagram() const { return _Diagram; }

	void GetST(real32& s, real32& t) { s = _s; t = _t; }
	void SetST(real32 s, real32 t) { _s = s; _t = t; }

	void Update();
	void Refresh();

protected:
	int32 _IDs;
	String _fileName;
	OperatorList _Operators;
	Operator* _SelectedOperator;
	int32 _Width;
	int32 _Height;

	OperatorList _RemovedOperators;
	UI::Diagram* _Diagram;
	real32 _s, _t;
};


struct OperatorDescription
{
	String Name;
	String Description;
	String TypeName;
};

struct OperatorCategory
{
	String Name;
	String Description;
	typedef BaseArray<OperatorDescription> OperatorList;
	OperatorList Operators;
};

class OperatorLibrary : public Object
{
	SE_DECLARE_CLASS(OperatorLibrary, Object);

public:
	typedef BaseArray<OperatorCategory> CategoryList;

	OperatorLibrary();
	virtual ~OperatorLibrary();

	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	BaseArray<OperatorCategory> Categories() const;
	const OperatorCategory* GetCategoryByName(const String& name) const;

	Operator* CreateOperator(const String& name);

protected:
	OperatorCategory DeserializeCategory(XMLSerializer* context, XMLElement* element);
	OperatorDescription DeserializeDescription(XMLSerializer* context, XMLElement* element);

protected:
	CategoryList _Categories;
};

#endif 
