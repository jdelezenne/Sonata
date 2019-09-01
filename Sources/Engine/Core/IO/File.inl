/*=============================================================================
File.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE bool File::Exists(const String& fileName)
{
	return File(fileName).Exists();
}

SE_INLINE bool Delete(const String& fileName)
{
	return File(fileName).Delete();
}

SE_INLINE bool Copy(const String& source, const String& destination, bool overwrite)
{
	return File(source).Copy(destination, overwrite);
}

SE_INLINE bool Move(const String& source, const String& destination)
{
	return File(source).Move(destination);
}
