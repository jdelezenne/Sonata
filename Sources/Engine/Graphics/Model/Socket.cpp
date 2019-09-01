/*=============================================================================
Socket.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Socket.h"
#include "Graphics/Model/Bone.h"

namespace SonataEngine
{

Socket::Socket() :
	NamedObject()
{
}

Socket::~Socket()
{
}

const Matrix4& Socket::GetTransform()
{
	//TODO: Update
	return _transform;
}

void Socket::Update()
{
	if (_bone == NULL)
	{
		return;
	}
	Matrix4 transform = _bone->GetGlobalTransform() * _transform;

	AttachmentList::Iterator it = Attachments.GetIterator();
	while (it.Next())
	{
		it.Current()->SetLocalPosition(transform.GetTranslation());
	}
}

}
