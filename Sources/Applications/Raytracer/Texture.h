/*=============================================================================
Texture.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_TEXTURE_H_
#define _RAYTRACER_TEXTURE_H_

#include "Common.h"
#include "Image.h"

namespace Raytracer
{
	class RTTexture : public RefObject
	{
		SE_DECLARE_ABSTRACT(RTTexture, Object);

		SE_BEGIN_REFLECTION(RTTexture);
			SE_Field(_FileName, String, Public);
		SE_END_REFLECTION(RTTexture);

	public:
		//@{
		RTTexture();

		virtual ~RTTexture();
		//@}

		//@{
		String GetFileName() const { return _FileName; }
		void SetFileName(const String& value) { _FileName = value; }

		RTImage* GetImage() const { return _Image; }
		void SetImage(RTImage* value) { _Image = value; }
		//@}

		//@{
		//@}

	protected:
		String _FileName;
		RTImage* _Image;
	};
}

#endif 
