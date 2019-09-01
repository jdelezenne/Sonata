/*=============================================================================
Core.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CORE_H_
#define _SE_CORE_H_

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>

// Base headers
#include "Core/Common.h"
#include "Platforms/Platforms.h"

// Interfaces
#include "Core/IRenderable.h"
#include "Core/IUpdatable.h"
#include "Core/IUserData.h"

// Misc
#include "Core/RefCounter.h"
#include "Core/SmartPtr.h"
#include "Core/Singleton.h"
#include "Core/Event.h"

// Containers
#include "Core/Containers/Array.h"
#include "Core/Containers/BaseArray.h"
#include "Core/Containers/BaseList.h"
#include "Core/Containers/Dictionary.h"
#include "Core/Containers/Graph.h"
#include "Core/Containers/Hashtable.h"
#include "Core/Containers/List.h"
#include "Core/Containers/PriorityQueue.h"
#include "Core/Containers/Queue.h"
#include "Core/Containers/Stack.h"
#include "Core/Containers/Tree.h"

// Debug
#include "Core/Debug/Debug.h"
#include "Core/Debug/StackTrace.h"
#include "Core/Debug/StackFrame.h"

// Exception
#include "Core/Exception/AccessViolationException.h"
#include "Core/Exception/IndexOutOfRangeException.h"
#include "Core/Exception/DivideByZeroException.h"
#include "Core/Exception/OutOfMemoryException.h"
#include "Core/Exception/OverflowException.h"
#include "Core/Exception/StackOverflowException.h"
#include "Core/Exception/UnderflowException.h"
#include "Core/Exception/ArgumentException.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"
#include "Core/Exception/ArithmeticException.h"
#include "Core/Exception/FormatException.h"
#include "Core/Exception/IndexOutOfRangeException.h"
#include "Core/Exception/InvalidOperationException.h"
#include "Core/Exception/NotImplementedException.h"
#include "Core/Exception/NotSupportedException.h"

// IO
#include "Core/IO/Archive.h"
#include "Core/IO/ArchiveDataPlugin.h"
#include "Core/IO/BinaryStream.h"
#include "Core/IO/ConsoleStream.h"
#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/IOException.h"
#include "Core/IO/MemoryStream.h"
#include "Core/IO/Path.h"
#include "Core/IO/Stream.h"
#include "Core/IO/TextStream.h"

// Logging
#include "Core/Logging/LogFilter.h"
#include "Core/Logging/LogFormatter.h"
#include "Core/Logging/Logger.h"
#include "Core/Logging/LogHandler.h"
#include "Core/Logging/LogLevel.h"
#include "Core/Logging/LogRecord.h"
#include "Core/Logging/TextStreamLogHandler.h"

// Math
#include "Core/Math/AABB.h"
#include "Core/Math/AxisAngle.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Math/BoundingSphere.h"
#include "Core/Math/Capsule.h"
#include "Core/Math/Complex.h"
#include "Core/Math/Cylinder.h"
#include "Core/Math/EulerAngles.h"
#include "Core/Math/Frustum.h"
#include "Core/Math/Interpolator.h"
#include "Core/Math/Math.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/OBB.h"
#include "Core/Math/ODESolver.h"
#include "Core/Math/Point3.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Ray2.h"
#include "Core/Math/Ray3.h"
#include "Core/Math/Segment.h"
#include "Core/Math/Transform3.h"
#include "Core/Math/Triangle.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

// Plugins
#include "Core/Plugins/DataPlugin.h"
#include "Core/Plugins/ManagerPlugin.h"
#include "Core/Plugins/Plugin.h"
#include "Core/Plugins/PluginManager.h"

// Reflection
#include "Core/Reflection/Reflection.h"

// Resource
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceHandler.h"
#include "Core/Resource/ResourceHelper.h"
#include "Core/Resource/ResourceManager.h"

// Serialization
#include "Core/Serialization/BinarySerializer.h"
#include "Core/Serialization/ISerializable.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/XMLSerializer.h"

// System
#include "Core/System/Console.h"
#include "Core/System/DateTime.h"
#include "Core/System/Environment.h"
#include "Core/System/Library.h"
#include "Core/System/Memory.h"
#include "Core/System/Timer.h"
#include "Core/System/TimeValue.h"
#include "Core/System/Window.h"

// Random
#include "Core/System/Random/Random.h"
#include "Core/System/Random/RandomLCG.h"
#include "Core/System/Random/RandomMT.h"
#include "Core/System/Random/RandomR250.h"
#include "Core/System/Random/RandomStd.h"

// Threading
#include "Core/Threading/Threading.h"

// Types
#include "Core/Char.h"
#include "Core/Color32.h"
#include "Core/Color8.h"
#include "Core/Flags.h"
#include "Core/Point.h"
#include "Core/Range.h"
#include "Core/Rectangle.h"
#include "Core/Scale.h"
#include "Core/Size.h"
#include "Core/String.h"
#include "Core/SE_ID.h"

// XML
#include "Core/XML/XML.h"

#endif
