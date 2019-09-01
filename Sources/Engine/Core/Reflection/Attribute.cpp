/*=============================================================================
Attribute.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Attribute.h"

namespace SonataEngine
{

const DefaultValueAttribute DefaultValueAttribute::Default;

const DisplayNameAttribute DisplayNameAttribute::Default;

const CategoryAttribute CategoryAttribute::Default;

const DescriptionAttribute DescriptionAttribute::Default;

const ReadOnlyAttribute ReadOnlyAttribute::Default = ReadOnlyAttribute::No;
const ReadOnlyAttribute ReadOnlyAttribute::No = ReadOnlyAttribute(false);
const ReadOnlyAttribute ReadOnlyAttribute::Yes = ReadOnlyAttribute(true);

const PasswordTextAttribute PasswordTextAttribute::Default = PasswordTextAttribute::No;
const PasswordTextAttribute PasswordTextAttribute::No = PasswordTextAttribute(false);
const PasswordTextAttribute PasswordTextAttribute::Yes = PasswordTextAttribute(true);

}
