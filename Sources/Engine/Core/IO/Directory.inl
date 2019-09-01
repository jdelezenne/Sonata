/*=============================================================================
Directory.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE bool Directory::Exists(const String& path)
{
	return Directory(path).Exists();
}

SE_INLINE bool Directory::Create(const String& path)
{
	return Directory(path).Create();
}

SE_INLINE bool Directory::Delete(const String& path)
{
	return Directory(path).Delete();
}

SE_INLINE bool Directory::Move(const String& source, const String& destination)
{
	return Directory(source).Move(destination);
}

SE_INLINE const Array<String> Directory::GetDirectories(const String& path)
{
	return Directory(path).GetDirectories();
}

SE_INLINE const Array<String> Directory::GetFiles(const String& path)
{
	return Directory(path).GetFiles();
}
