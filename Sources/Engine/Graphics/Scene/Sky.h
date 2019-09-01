/*=============================================================================
Sky.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SKY_H_
#define _SE_SKY_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Base class for Sky implementations.
*/
class SE_GRAPHICS_EXPORT Sky : public NamedObject
{
	SE_DECLARE_ABSTRACT(Sky, NamedObject);

public:
	/** Destructor. */
	virtual ~Sky();

	virtual bool Create() = 0;
	virtual void Destroy() = 0;

	virtual void Update(const TimeValue& timeValue) = 0;
	virtual void Render() = 0;

protected:
	/** Constructor. */
	Sky();
};

SEPointer(Sky);

}

#endif 
