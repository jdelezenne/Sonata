/*=============================================================================
SymbolTable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_SYMBOLTABLE_H_
#define _SE_AI_SYMBOLTABLE_H_

#include "AI/Common.h"
#include "AI/StateMachines/FSMObject.h"

namespace SonataEngine
{
	namespace AI
	{
		struct Symbol
		{
			String Name;
			bool Value;

			Symbol() :
				Value(false)
			{
			}

			Symbol(const String& name, bool value = false) :
				Name(name),
				Value(value)
			{
			}
		};

		class SymbolTable : public FSMObject
		{
		public:
			SymbolTable();
			virtual ~SymbolTable();

			void AddSymbol(const Symbol& symbol);

			void RemoveSymbol(const String& name);

			bool GetSymbol(const String& name);

			void SetSymbol(const String& name, bool value);

		protected:
			typedef Dictionary<String, Symbol> SymbolList;
			SymbolList _Symbols;
		};

		typedef SmartPtr<SymbolTable> SymbolTablePtr;
		}
}

#endif 
