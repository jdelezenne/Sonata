/*=============================================================================
SymbolTable.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SymbolTable.h"

namespace SonataEngine
{
	namespace AI
	{
		SymbolTable::SymbolTable() :
			FSMObject()
		{
		}

		SymbolTable::~SymbolTable()
		{
		}

		void SymbolTable::AddSymbol(const Symbol& symbol)
		{
			_Symbols.Add(symbol.Name, symbol);
		}

		void SymbolTable::RemoveSymbol(const String& name)
		{
			_Symbols.Remove(name);
		}

		bool SymbolTable::GetSymbol(const String& name)
		{
			if (!_Symbols.Contains(name))
				return false;

			return _Symbols[name].Value;
		}

		void SymbolTable::SetSymbol(const String& name, bool value)
		{
			Symbol symbol(name, value);
			_Symbols[name] = symbol;
		}
	}
}
