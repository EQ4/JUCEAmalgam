/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/


/*** Start of inlined file: juce_audio_processors.h ***/
#ifndef __JUCE_AUDIO_PROCESSORS_JUCEHEADER__
#define __JUCE_AUDIO_PROCESSORS_JUCEHEADER__

#include "juce_gui_basics_amalgam.h"
#include "juce_audio_basics_amalgam.h"

/** Config: JUCE_PLUGINHOST_VST
	Enables the VST audio plugin hosting classes. This requires the Steinberg VST SDK to be
	installed on your machine.

	@see VSTPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_AU
*/
#ifndef JUCE_PLUGINHOST_VST
 #define JUCE_PLUGINHOST_VST 0
#endif

/** Config: JUCE_PLUGINHOST_AU
	Enables the AudioUnit plugin hosting classes. This is Mac-only, of course.

	@see AudioUnitPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_VST
*/
#ifndef JUCE_PLUGINHOST_AU
 #define JUCE_PLUGINHOST_AU 0
#endif

#if ! (JUCE_PLUGINHOST_AU || JUCE_PLUGINHOST_VST)
// #error "You need to set either the JUCE_PLUGINHOST_AU anr/or JUCE_PLUGINHOST_VST flags if you're using this module!"
#endif

#if ! (defined (JUCE_SUPPORT_CARBON) || JUCE_64BIT)
 #define JUCE_SUPPORT_CARBON 1
#endif


/*** Start of inlined file: juce_StandardHeader.h ***/
#ifndef __JUCE_STANDARDHEADER_JUCEHEADER__
#define __JUCE_STANDARDHEADER_JUCEHEADER__

/** Current JUCE version number.

	See also SystemStats::getJUCEVersion() for a string version.
*/
#define JUCE_MAJOR_VERSION      2
#define JUCE_MINOR_VERSION      0
#define JUCE_BUILDNUMBER        21

/** Current Juce version number.

	Bits 16 to 32 = major version.
	Bits 8 to 16 = minor version.
	Bits 0 to 8 = point release.

	See also SystemStats::getJUCEVersion() for a string version.
*/
#define JUCE_VERSION            ((JUCE_MAJOR_VERSION << 16) + (JUCE_MINOR_VERSION << 8) + JUCE_BUILDNUMBER)


/*** Start of inlined file: juce_TargetPlatform.h ***/
#ifndef __JUCE_TARGETPLATFORM_JUCEHEADER__
#define __JUCE_TARGETPLATFORM_JUCEHEADER__

/*  This file figures out which platform is being built, and defines some macros
	that the rest of the code can use for OS-specific compilation.

	Macros that will be set here are:

	- One of JUCE_WINDOWS, JUCE_MAC JUCE_LINUX, JUCE_IOS, JUCE_ANDROID, etc.
	- Either JUCE_32BIT or JUCE_64BIT, depending on the architecture.
	- Either JUCE_LITTLE_ENDIAN or JUCE_BIG_ENDIAN.
	- Either JUCE_INTEL or JUCE_PPC
	- Either JUCE_GCC or JUCE_MSVC
*/

#if (defined (_WIN32) || defined (_WIN64))
  #define       JUCE_WIN32 1
  #define       JUCE_WINDOWS 1
#elif defined (JUCE_ANDROID)
  #undef        JUCE_ANDROID
  #define       JUCE_ANDROID 1
#elif defined (LINUX) || defined (__linux__)
  #define     JUCE_LINUX 1
#elif defined (__APPLE_CPP__) || defined(__APPLE_CC__)
  #define Point CarbonDummyPointName // (workaround to avoid definition of "Point" by old Carbon headers)
  #define Component CarbonDummyCompName
  #include <CoreFoundation/CoreFoundation.h> // (needed to find out what platform we're using)
  #undef Point
  #undef Component

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	#define     JUCE_IPHONE 1
	#define     JUCE_IOS 1
  #else
	#define     JUCE_MAC 1
  #endif
#else
  #error "Unknown platform!"
#endif

#if JUCE_WINDOWS
  #ifdef _MSC_VER
	#ifdef _WIN64
	  #define JUCE_64BIT 1
	#else
	  #define JUCE_32BIT 1
	#endif
  #endif

  #ifdef _DEBUG
	#define JUCE_DEBUG 1
  #endif

  #ifdef __MINGW32__
	#define JUCE_MINGW 1
  #endif

  /** If defined, this indicates that the processor is little-endian. */
  #define JUCE_LITTLE_ENDIAN 1

  #define JUCE_INTEL 1
#endif

#if JUCE_MAC || JUCE_IOS

  #if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
	#define JUCE_DEBUG 1
  #endif

  #if ! (defined (DEBUG) || defined (_DEBUG) || defined (NDEBUG) || defined (_NDEBUG))
	#warning "Neither NDEBUG or DEBUG has been defined - you should set one of these to make it clear whether this is a release build,"
  #endif

  #ifdef __LITTLE_ENDIAN__
	#define JUCE_LITTLE_ENDIAN 1
  #else
	#define JUCE_BIG_ENDIAN 1
  #endif
#endif

#if JUCE_MAC

  #if defined (__ppc__) || defined (__ppc64__)
	#define JUCE_PPC 1
  #else
	#define JUCE_INTEL 1
  #endif

  #ifdef __LP64__
	#define JUCE_64BIT 1
  #else
	#define JUCE_32BIT 1
  #endif

  #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_4
	#error "Building for OSX 10.3 is no longer supported!"
  #endif

  #ifndef MAC_OS_X_VERSION_10_5
	#error "To build with 10.4 compatibility, use a 10.5 or 10.6 SDK and set the deployment target to 10.4"
  #endif

#endif

#if JUCE_LINUX || JUCE_ANDROID

  #ifdef _DEBUG
	#define JUCE_DEBUG 1
  #endif

  // Allow override for big-endian Linux platforms
  #if defined (__LITTLE_ENDIAN__) || ! defined (JUCE_BIG_ENDIAN)
	#define JUCE_LITTLE_ENDIAN 1
	#undef JUCE_BIG_ENDIAN
  #else
	#undef JUCE_LITTLE_ENDIAN
	#define JUCE_BIG_ENDIAN 1
  #endif

  #if defined (__LP64__) || defined (_LP64)
	#define JUCE_64BIT 1
  #else
	#define JUCE_32BIT 1
  #endif

  #if __MMX__ || __SSE__ || __amd64__
	#define JUCE_INTEL 1
  #endif
#endif

// Compiler type macros.

#ifdef __GNUC__
  #define JUCE_GCC 1
#elif defined (_MSC_VER)
  #define JUCE_MSVC 1

  #if _MSC_VER < 1500
	#define JUCE_VC8_OR_EARLIER 1

	#if _MSC_VER < 1400
	  #define JUCE_VC7_OR_EARLIER 1

	  #if _MSC_VER < 1300
		#warning "MSVC 6.0 is no longer supported!"
	  #endif
	#endif
  #endif

  #if JUCE_64BIT || ! JUCE_VC7_OR_EARLIER
	#define JUCE_USE_INTRINSICS 1
  #endif
#else
  #error unknown compiler
#endif

#endif   // __JUCE_TARGETPLATFORM_JUCEHEADER__

/*** End of inlined file: juce_TargetPlatform.h ***/

#ifndef DOXYGEN
 // These are old macros that are now deprecated: you should just use the juce namespace directly.
 #define JUCE_NAMESPACE juce
 #define BEGIN_JUCE_NAMESPACE    namespace juce {
 #define END_JUCE_NAMESPACE      }
#endif


/*** Start of inlined file: juce_PlatformDefs.h ***/
#ifndef __JUCE_PLATFORMDEFS_JUCEHEADER__
#define __JUCE_PLATFORMDEFS_JUCEHEADER__

/*  This file defines miscellaneous macros for debugging, assertions, etc.
*/

#ifdef JUCE_FORCE_DEBUG
 #undef JUCE_DEBUG

 #if JUCE_FORCE_DEBUG
  #define JUCE_DEBUG 1
 #endif
#endif

/** This macro defines the C calling convention used as the standard for Juce calls. */
#if JUCE_MSVC
 #define JUCE_CALLTYPE   __stdcall
 #define JUCE_CDECL      __cdecl
#else
 #define JUCE_CALLTYPE
 #define JUCE_CDECL
#endif

// Debugging and assertion macros

#if JUCE_LOG_ASSERTIONS
 #define juce_LogCurrentAssertion    juce::logAssertion (__FILE__, __LINE__);
#elif JUCE_DEBUG
 #define juce_LogCurrentAssertion    std::cerr << "JUCE Assertion failure in " << __FILE__ << ", line " << __LINE__ << std::endl;
#else
 #define juce_LogCurrentAssertion
#endif

#if JUCE_IOS || JUCE_LINUX || JUCE_ANDROID || JUCE_PPC
  /** This will try to break into the debugger if the app is currently being debugged.
	  If called by an app that's not being debugged, the behaiour isn't defined - it may crash or not, depending
	  on the platform.
	  @see jassert()
  */
  #define juce_breakDebugger        { ::kill (0, SIGTRAP); }
#elif JUCE_USE_INTRINSICS
  #ifndef __INTEL_COMPILER
	#pragma intrinsic (__debugbreak)
  #endif
  #define juce_breakDebugger        { __debugbreak(); }
#elif JUCE_GCC || JUCE_MAC
  #define juce_breakDebugger        { asm ("int $3"); }
#else
  #define juce_breakDebugger        { __asm int 3 }
#endif

#if JUCE_DEBUG || DOXYGEN
  /** Writes a string to the standard error stream.
	  This is only compiled in a debug build.
	  @see Logger::outputDebugString
  */
  #define DBG(dbgtext)              { juce::String tempDbgBuf; tempDbgBuf << dbgtext; juce::Logger::outputDebugString (tempDbgBuf); }

  /** This will always cause an assertion failure.
	  It is only compiled in a debug build, (unless JUCE_LOG_ASSERTIONS is enabled for your build).
	  @see jassert
  */
  #define jassertfalse              { juce_LogCurrentAssertion; if (juce::juce_isRunningUnderDebugger()) juce_breakDebugger; }

  /** Platform-independent assertion macro.

	  This macro gets turned into a no-op when you're building with debugging turned off, so be
	  careful that the expression you pass to it doesn't perform any actions that are vital for the
	  correct behaviour of your program!
	  @see jassertfalse
  */
  #define jassert(expression)       { if (! (expression)) jassertfalse; }

#else

  // If debugging is disabled, these dummy debug and assertion macros are used..

  #define DBG(dbgtext)
  #define jassertfalse              { juce_LogCurrentAssertion }

  #if JUCE_LOG_ASSERTIONS
   #define jassert(expression)      { if (! (expression)) jassertfalse; }
  #else
   #define jassert(a)               {}
  #endif

#endif

#ifndef DOXYGEN
namespace juce
{
	template <bool b> struct JuceStaticAssert;
	template <> struct JuceStaticAssert <true> { static void dummy() {} };
}
#endif

/** A compile-time assertion macro.
	If the expression parameter is false, the macro will cause a compile error. (The actual error
	message that the compiler generates may be completely bizarre and seem to have no relation to
	the place where you put the static_assert though!)
*/
#define static_jassert(expression)      juce::JuceStaticAssert<expression>::dummy();

/** This is a shorthand macro for declaring stubs for a class's copy constructor and operator=.

	For example, instead of
	@code
	class MyClass
	{
		etc..

	private:
		MyClass (const MyClass&);
		MyClass& operator= (const MyClass&);
	};@endcode

	..you can just write:

	@code
	class MyClass
	{
		etc..

	private:
		JUCE_DECLARE_NON_COPYABLE (MyClass);
	};@endcode
*/
#define JUCE_DECLARE_NON_COPYABLE(className) \
	className (const className&);\
	className& operator= (const className&)

/** This is a shorthand way of writing both a JUCE_DECLARE_NON_COPYABLE and
	JUCE_LEAK_DETECTOR macro for a class.
*/
#define JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(className) \
	JUCE_DECLARE_NON_COPYABLE(className);\
	JUCE_LEAK_DETECTOR(className)

/** This macro can be added to class definitions to disable the use of new/delete to
	allocate the object on the heap, forcing it to only be used as a stack or member variable.
*/
#define JUCE_PREVENT_HEAP_ALLOCATION \
   private: \
	static void* operator new (size_t); \
	static void operator delete (void*);

#if ! DOXYGEN
 #define JUCE_JOIN_MACRO_HELPER(a, b) a ## b
 #define JUCE_STRINGIFY_MACRO_HELPER(a) #a
#endif

/** A good old-fashioned C macro concatenation helper.
	This combines two items (which may themselves be macros) into a single string,
	avoiding the pitfalls of the ## macro operator.
*/
#define JUCE_JOIN_MACRO(item1, item2)  JUCE_JOIN_MACRO_HELPER (item1, item2)

/** A handy C macro for stringifying any symbol, rather than just a macro parameter.
*/
#define JUCE_STRINGIFY(item)  JUCE_STRINGIFY_MACRO_HELPER (item)

#if JUCE_CATCH_UNHANDLED_EXCEPTIONS

  #define JUCE_TRY try

  #define JUCE_CATCH_ALL            catch (...) {}
  #define JUCE_CATCH_ALL_ASSERT     catch (...) { jassertfalse; }

  #if ! JUCE_MODULE_AVAILABLE_juce_gui_basics
	#define JUCE_CATCH_EXCEPTION    JUCE_CATCH_ALL
  #else
	/** Used in try-catch blocks, this macro will send exceptions to the JUCEApplication
		object so they can be logged by the application if it wants to.
	*/
	#define JUCE_CATCH_EXCEPTION \
	  catch (const std::exception& e)  \
	  { \
		  JUCEApplication::sendUnhandledException (&e, __FILE__, __LINE__); \
	  } \
	  catch (...) \
	  { \
		  JUCEApplication::sendUnhandledException (nullptr, __FILE__, __LINE__); \
	  }
  #endif

#else

  #define JUCE_TRY
  #define JUCE_CATCH_EXCEPTION
  #define JUCE_CATCH_ALL
  #define JUCE_CATCH_ALL_ASSERT

#endif

#if JUCE_DEBUG || DOXYGEN
  /** A platform-independent way of forcing an inline function.
	  Use the syntax: @code
	  forcedinline void myfunction (int x)
	  @endcode
  */
  #define forcedinline  inline
#else
  #if JUCE_MSVC
   #define forcedinline       __forceinline
  #else
   #define forcedinline       inline __attribute__((always_inline))
  #endif
#endif

#if JUCE_MSVC || DOXYGEN
  /** This can be placed before a stack or member variable declaration to tell the compiler
	  to align it to the specified number of bytes. */
  #define JUCE_ALIGN(bytes)   __declspec (align (bytes))
#else
  #define JUCE_ALIGN(bytes)   __attribute__ ((aligned (bytes)))
#endif

// Cross-compiler deprecation macros..
#if DOXYGEN || (JUCE_MSVC && ! JUCE_NO_DEPRECATION_WARNINGS)
 /** This can be used to wrap a function which has been deprecated. */
 #define JUCE_DEPRECATED(functionDef)     __declspec(deprecated) functionDef
#elif JUCE_GCC  && ! JUCE_NO_DEPRECATION_WARNINGS
 #define JUCE_DEPRECATED(functionDef)     functionDef __attribute__ ((deprecated))
#else
 #define JUCE_DEPRECATED(functionDef)     functionDef
#endif

#if JUCE_ANDROID && ! DOXYGEN
 #define JUCE_MODAL_LOOPS_PERMITTED 0
#else
 /** Some operating environments don't provide a modal loop mechanism, so this flag can be
	 used to disable any functions that try to run a modal loop. */
 #define JUCE_MODAL_LOOPS_PERMITTED 1
#endif

// Here, we'll check for C++11 compiler support, and if it's not available, define
// a few workarounds, so that we can still use some of the newer language features.
#if defined (__GXX_EXPERIMENTAL_CXX0X__) && defined (__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
 #define JUCE_COMPILER_SUPPORTS_NOEXCEPT 1
 #define JUCE_COMPILER_SUPPORTS_NULLPTR 1
 #define JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS 1
#endif

#if defined (__clang__) && defined (__has_feature)
 #if __has_feature (cxx_nullptr)
  #define JUCE_COMPILER_SUPPORTS_NULLPTR 1
 #endif

 #if __has_feature (cxx_noexcept)
  #define JUCE_COMPILER_SUPPORTS_NOEXCEPT 1
 #endif

 #if __has_feature (cxx_rvalue_references)
  #define JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS 1
 #endif
#endif

#if defined (_MSC_VER) && _MSC_VER >= 1600
 #if _MSC_VER >= 1700
  #define JUCE_COMPILER_SUPPORTS_NOEXCEPT 1
 #else
  #define JUCE_COMPILER_SUPPORTS_NOEXCEPT 0
 #endif
 #define JUCE_COMPILER_SUPPORTS_NULLPTR 1
 #define JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS 1
#endif

// Declare some fake versions of nullptr and noexcept, for older compilers:
#if ! (DOXYGEN || JUCE_COMPILER_SUPPORTS_NOEXCEPT)
 #define noexcept  throw()
#endif

#if ! (DOXYGEN || JUCE_COMPILER_SUPPORTS_NULLPTR)
 #define nullptr (0)
#endif

#endif   // __JUCE_PLATFORMDEFS_JUCEHEADER__

/*** End of inlined file: juce_PlatformDefs.h ***/

// Now we'll include any OS headers we need.. (at this point we are outside the Juce namespace).
#if JUCE_MSVC
 #pragma warning (push)
 #pragma warning (disable: 4514 4245 4100)
#endif

#include <cstdlib>
#include <cstdarg>
#include <climits>
#include <limits>
#include <cmath>
#include <cwchar>
#include <stdexcept>
#include <typeinfo>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>

#if JUCE_USE_INTRINSICS
 #include <intrin.h>
#endif

#if JUCE_MAC || JUCE_IOS
 #include <libkern/OSAtomic.h>
#endif

#if JUCE_LINUX
 #include <signal.h>

 #if __INTEL_COMPILER
  #if __ia64__
   #include <ia64intrin.h>
  #else
   #include <ia32intrin.h>
  #endif
 #endif
#endif

#if JUCE_MSVC && JUCE_DEBUG
 #include <crtdbg.h>
#endif

#if JUCE_MSVC
 #pragma warning (pop)
#endif

#if JUCE_ANDROID
 #include <sys/atomics.h>
 #include <byteswap.h>
#endif

// DLL building settings on Windows
#if JUCE_MSVC
 #ifdef JUCE_DLL_BUILD
  #define JUCE_API __declspec (dllexport)
  #pragma warning (disable: 4251)
 #elif defined (JUCE_DLL)
  #define JUCE_API __declspec (dllimport)
  #pragma warning (disable: 4251)
 #endif
 #ifdef __INTEL_COMPILER
  #pragma warning (disable: 1125) // (virtual override warning)
 #endif
#elif defined (__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
 #ifdef JUCE_DLL_BUILD
  #define JUCE_API __attribute__ ((visibility("default")))
 #endif
#endif

#ifndef JUCE_API
 /** This macro is added to all juce public class declarations. */
 #define JUCE_API
#endif

/** This macro is added to all juce public function declarations. */
#define JUCE_PUBLIC_FUNCTION        JUCE_API JUCE_CALLTYPE

/** This turns on some non-essential bits of code that should prevent old code from compiling
	in cases where method signatures have changed, etc.
*/
#if (! defined (JUCE_CATCH_DEPRECATED_CODE_MISUSE)) && JUCE_DEBUG && ! DOXYGEN
 #define JUCE_CATCH_DEPRECATED_CODE_MISUSE 1
#endif

// Now include some basics that are needed by most of the Juce classes...
BEGIN_JUCE_NAMESPACE

extern JUCE_API bool JUCE_CALLTYPE juce_isRunningUnderDebugger();

#if JUCE_LOG_ASSERTIONS
 extern JUCE_API void logAssertion (const char* filename, int lineNum) noexcept;
#endif

#undef max
#undef min


/*** Start of inlined file: juce_Memory.h ***/
#ifndef __JUCE_MEMORY_JUCEHEADER__
#define __JUCE_MEMORY_JUCEHEADER__

#if JUCE_MINGW
 /** This allocator is not defined in mingw gcc. */
 #define alloca              __builtin_alloca
#endif

/** Fills a block of memory with zeros. */
inline void zeromem (void* memory, size_t numBytes) noexcept        { memset (memory, 0, numBytes); }

/** Overwrites a structure or object with zeros. */
template <typename Type>
inline void zerostruct (Type& structure) noexcept                   { memset (&structure, 0, sizeof (structure)); }

/** Delete an object pointer, and sets the pointer to null.

	Remember that it's not good c++ practice to use delete directly - always try to use a ScopedPointer
	or other automatic lifetime-management system rather than resorting to deleting raw pointers!
*/
template <typename Type>
inline void deleteAndZero (Type& pointer)                           { delete pointer; pointer = nullptr; }

/** A handy function which adds a number of bytes to any type of pointer and returns the result.
	This can be useful to avoid casting pointers to a char* and back when you want to move them by
	a specific number of bytes,
*/
template <typename Type>
inline Type* addBytesToPointer (Type* pointer, int bytes) noexcept  { return (Type*) (((char*) pointer) + bytes); }

/** A handy function which returns the difference between any two pointers, in bytes.
	The address of the second pointer is subtracted from the first, and the difference in bytes is returned.
*/
template <typename Type1, typename Type2>
inline int getAddressDifference (Type1* pointer1, Type2* pointer2) noexcept  { return (int) (((const char*) pointer1) - (const char*) pointer2); }

/** If a pointer is non-null, this returns a new copy of the object that it points to, or safely returns
	nullptr if the pointer is null.
*/
template <class Type>
inline Type* createCopyIfNotNull (Type* pointer)     { return pointer != nullptr ? new Type (*pointer) : nullptr; }

/* In a Windows DLL build, we'll expose some malloc/free functions that live inside the DLL, and use these for
   allocating all the objects - that way all juce objects in the DLL and in the host will live in the same heap,
   avoiding problems when an object is created in one module and passed across to another where it is deleted.
   By piggy-backing on the JUCE_LEAK_DETECTOR macro, these allocators can be injected into most juce classes.
*/
#if JUCE_MSVC && defined (JUCE_DLL) && ! DOXYGEN
 extern JUCE_API void* juceDLL_malloc (size_t);
 extern JUCE_API void  juceDLL_free (void*);

 #define JUCE_LEAK_DETECTOR(OwnerClass)  public:\
			  static void* operator new (size_t sz)           { return juce::juceDLL_malloc ((int) sz); } \
			  static void* operator new (size_t, void* p)     { return p; } \
			  static void operator delete (void* p)           { juce::juceDLL_free (p); } \
			  static void operator delete (void*, void*)      {}
#endif

/** (Deprecated) This was a Windows-specific way of checking for object leaks - now please
	use the JUCE_LEAK_DETECTOR instead.
*/
#ifndef juce_UseDebuggingNewOperator
 #define juce_UseDebuggingNewOperator
#endif

#endif   // __JUCE_MEMORY_JUCEHEADER__

/*** End of inlined file: juce_Memory.h ***/


/*** Start of inlined file: juce_MathsFunctions.h ***/
#ifndef __JUCE_MATHSFUNCTIONS_JUCEHEADER__
#define __JUCE_MATHSFUNCTIONS_JUCEHEADER__

/*
	This file sets up some handy mathematical typdefs and functions.
*/

// Definitions for the int8, int16, int32, int64 and pointer_sized_int types.

/** A platform-independent 8-bit signed integer type. */
typedef signed char                 int8;
/** A platform-independent 8-bit unsigned integer type. */
typedef unsigned char               uint8;
/** A platform-independent 16-bit signed integer type. */
typedef signed short                int16;
/** A platform-independent 16-bit unsigned integer type. */
typedef unsigned short              uint16;
/** A platform-independent 32-bit signed integer type. */
typedef signed int                  int32;
/** A platform-independent 32-bit unsigned integer type. */
typedef unsigned int                uint32;

#if JUCE_MSVC
  /** A platform-independent 64-bit integer type. */
  typedef __int64                   int64;
  /** A platform-independent 64-bit unsigned integer type. */
  typedef unsigned __int64          uint64;
  /** A platform-independent macro for writing 64-bit literals, needed because
	  different compilers have different syntaxes for this.

	  E.g. writing literal64bit (0x1000000000) will translate to 0x1000000000LL for
	  GCC, or 0x1000000000 for MSVC.
  */
  #define literal64bit(longLiteral)     ((__int64) longLiteral)
#else
  /** A platform-independent 64-bit integer type. */
  typedef long long                 int64;
  /** A platform-independent 64-bit unsigned integer type. */
  typedef unsigned long long        uint64;
  /** A platform-independent macro for writing 64-bit literals, needed because
	  different compilers have different syntaxes for this.

	  E.g. writing literal64bit (0x1000000000) will translate to 0x1000000000LL for
	  GCC, or 0x1000000000 for MSVC.
  */
  #define literal64bit(longLiteral)     (longLiteral##LL)
#endif

#if JUCE_64BIT
  /** A signed integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef int64                     pointer_sized_int;
  /** An unsigned integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef uint64                    pointer_sized_uint;
#elif JUCE_MSVC
  /** A signed integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef _W64 int                  pointer_sized_int;
  /** An unsigned integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef _W64 unsigned int         pointer_sized_uint;
#else
  /** A signed integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef int                       pointer_sized_int;
  /** An unsigned integer type that's guaranteed to be large enough to hold a pointer without truncating it. */
  typedef unsigned int              pointer_sized_uint;
#endif

#if JUCE_MSVC
  typedef pointer_sized_int ssize_t;
#endif

// Some indispensible min/max functions

/** Returns the larger of two values. */
template <typename Type>
inline Type jmax (const Type a, const Type b)                                               { return (a < b) ? b : a; }

/** Returns the larger of three values. */
template <typename Type>
inline Type jmax (const Type a, const Type b, const Type c)                                 { return (a < b) ? ((b < c) ? c : b) : ((a < c) ? c : a); }

/** Returns the larger of four values. */
template <typename Type>
inline Type jmax (const Type a, const Type b, const Type c, const Type d)                   { return jmax (a, jmax (b, c, d)); }

/** Returns the smaller of two values. */
template <typename Type>
inline Type jmin (const Type a, const Type b)                                               { return (b < a) ? b : a; }

/** Returns the smaller of three values. */
template <typename Type>
inline Type jmin (const Type a, const Type b, const Type c)                                 { return (b < a) ? ((c < b) ? c : b) : ((c < a) ? c : a); }

/** Returns the smaller of four values. */
template <typename Type>
inline Type jmin (const Type a, const Type b, const Type c, const Type d)                   { return jmin (a, jmin (b, c, d)); }

/** Scans an array of values, returning the minimum value that it contains. */
template <typename Type>
const Type findMinimum (const Type* data, int numValues)
{
	if (numValues <= 0)
		return Type();

	Type result (*data++);

	while (--numValues > 0) // (> 0 rather than >= 0 because we've already taken the first sample)
	{
		const Type& v = *data++;
		if (v < result)  result = v;
	}

	return result;
}

/** Scans an array of values, returning the maximum value that it contains. */
template <typename Type>
const Type findMaximum (const Type* values, int numValues)
{
	if (numValues <= 0)
		return Type();

	Type result (*values++);

	while (--numValues > 0) // (> 0 rather than >= 0 because we've already taken the first sample)
	{
		const Type& v = *values++;
		if (result < v)  result = v;
	}

	return result;
}

/** Scans an array of values, returning the minimum and maximum values that it contains. */
template <typename Type>
void findMinAndMax (const Type* values, int numValues, Type& lowest, Type& highest)
{
	if (numValues <= 0)
	{
		lowest = Type();
		highest = Type();
	}
	else
	{
		Type mn (*values++);
		Type mx (mn);

		while (--numValues > 0) // (> 0 rather than >= 0 because we've already taken the first sample)
		{
			const Type& v = *values++;

			if (mx < v)  mx = v;
			if (v < mn)  mn = v;
		}

		lowest = mn;
		highest = mx;
	}
}

/** Constrains a value to keep it within a given range.

	This will check that the specified value lies between the lower and upper bounds
	specified, and if not, will return the nearest value that would be in-range. Effectively,
	it's like calling jmax (lowerLimit, jmin (upperLimit, value)).

	Note that it expects that lowerLimit <= upperLimit. If this isn't true,
	the results will be unpredictable.

	@param lowerLimit           the minimum value to return
	@param upperLimit           the maximum value to return
	@param valueToConstrain     the value to try to return
	@returns    the closest value to valueToConstrain which lies between lowerLimit
				and upperLimit (inclusive)
	@see jlimit0To, jmin, jmax
*/
template <typename Type>
inline Type jlimit (const Type lowerLimit,
					const Type upperLimit,
					const Type valueToConstrain) noexcept
{
	jassert (lowerLimit <= upperLimit); // if these are in the wrong order, results are unpredictable..

	return (valueToConstrain < lowerLimit) ? lowerLimit
										   : ((upperLimit < valueToConstrain) ? upperLimit
																			  : valueToConstrain);
}

/** Returns true if a value is at least zero, and also below a specified upper limit.
	This is basically a quicker way to write:
	@code valueToTest >= 0 && valueToTest < upperLimit
	@endcode
*/
template <typename Type>
inline bool isPositiveAndBelow (Type valueToTest, Type upperLimit) noexcept
{
	jassert (Type() <= upperLimit); // makes no sense to call this if the upper limit is itself below zero..
	return Type() <= valueToTest && valueToTest < upperLimit;
}

template <>
inline bool isPositiveAndBelow (const int valueToTest, const int upperLimit) noexcept
{
	jassert (upperLimit >= 0); // makes no sense to call this if the upper limit is itself below zero..
	return static_cast <unsigned int> (valueToTest) < static_cast <unsigned int> (upperLimit);
}

/** Returns true if a value is at least zero, and also less than or equal to a specified upper limit.
	This is basically a quicker way to write:
	@code valueToTest >= 0 && valueToTest <= upperLimit
	@endcode
*/
template <typename Type>
inline bool isPositiveAndNotGreaterThan (Type valueToTest, Type upperLimit) noexcept
{
	jassert (Type() <= upperLimit); // makes no sense to call this if the upper limit is itself below zero..
	return Type() <= valueToTest && valueToTest <= upperLimit;
}

template <>
inline bool isPositiveAndNotGreaterThan (const int valueToTest, const int upperLimit) noexcept
{
	jassert (upperLimit >= 0); // makes no sense to call this if the upper limit is itself below zero..
	return static_cast <unsigned int> (valueToTest) <= static_cast <unsigned int> (upperLimit);
}

/** Handy function to swap two values. */
template <typename Type>
inline void swapVariables (Type& variable1, Type& variable2)
{
	std::swap (variable1, variable2);
}

/** Handy function for getting the number of elements in a simple const C array.
	E.g.
	@code
	static int myArray[] = { 1, 2, 3 };

	int numElements = numElementsInArray (myArray) // returns 3
	@endcode
*/
template <typename Type, int N>
inline int numElementsInArray (Type (&array)[N])
{
	(void) array; // (required to avoid a spurious warning in MS compilers)
	(void) sizeof (0[array]); // This line should cause an error if you pass an object with a user-defined subscript operator
	return N;
}

// Some useful maths functions that aren't always present with all compilers and build settings.

/** Using juce_hypot is easier than dealing with the different types of hypot function
	that are provided by the various platforms and compilers. */
template <typename Type>
inline Type juce_hypot (Type a, Type b) noexcept
{
   #if JUCE_WINDOWS
	return static_cast <Type> (_hypot (a, b));
   #else
	return static_cast <Type> (hypot (a, b));
   #endif
}

/** 64-bit abs function. */
inline int64 abs64 (const int64 n) noexcept
{
	return (n >= 0) ? n : -n;
}

/** A predefined value for Pi, at double-precision.

	@see float_Pi
*/
const double  double_Pi  = 3.1415926535897932384626433832795;

/** A predefined value for Pi, at sngle-precision.

	@see double_Pi
*/
const float   float_Pi   = 3.14159265358979323846f;

/** The isfinite() method seems to vary between platforms, so this is a
	platform-independent function for it.
*/
template <typename FloatingPointType>
inline bool juce_isfinite (FloatingPointType value)
{
   #if JUCE_WINDOWS
	return _finite (value);
   #elif JUCE_ANDROID
	return isfinite (value);
   #else
	return std::isfinite (value);
   #endif
}

#if JUCE_MSVC
 #pragma optimize ("t", off)
 #pragma float_control (precise, on, push)
#endif

/** Fast floating-point-to-integer conversion.

	This is faster than using the normal c++ cast to convert a float to an int, and
	it will round the value to the nearest integer, rather than rounding it down
	like the normal cast does.

	Note that this routine gets its speed at the expense of some accuracy, and when
	rounding values whose floating point component is exactly 0.5, odd numbers and
	even numbers will be rounded up or down differently.
*/
template <typename FloatType>
inline int roundToInt (const FloatType value) noexcept
{
	union { int asInt[2]; double asDouble; } n;
	n.asDouble = ((double) value) + 6755399441055744.0;

   #if JUCE_BIG_ENDIAN
	return n.asInt [1];
   #else
	return n.asInt [0];
   #endif
}

#if JUCE_MSVC
 #pragma float_control (pop)
 #pragma optimize ("", on)  // resets optimisations to the project defaults
#endif

/** Fast floating-point-to-integer conversion.

	This is a slightly slower and slightly more accurate version of roundDoubleToInt(). It works
	fine for values above zero, but negative numbers are rounded the wrong way.
*/
inline int roundToIntAccurate (const double value) noexcept
{
	return roundToInt (value + 1.5e-8);
}

/** Fast floating-point-to-integer conversion.

	This is faster than using the normal c++ cast to convert a double to an int, and
	it will round the value to the nearest integer, rather than rounding it down
	like the normal cast does.

	Note that this routine gets its speed at the expense of some accuracy, and when
	rounding values whose floating point component is exactly 0.5, odd numbers and
	even numbers will be rounded up or down differently. For a more accurate conversion,
	see roundDoubleToIntAccurate().
*/
inline int roundDoubleToInt (const double value) noexcept
{
	return roundToInt (value);
}

/** Fast floating-point-to-integer conversion.

	This is faster than using the normal c++ cast to convert a float to an int, and
	it will round the value to the nearest integer, rather than rounding it down
	like the normal cast does.

	Note that this routine gets its speed at the expense of some accuracy, and when
	rounding values whose floating point component is exactly 0.5, odd numbers and
	even numbers will be rounded up or down differently.
*/
inline int roundFloatToInt (const float value) noexcept
{
	return roundToInt (value);
}

/** Returns true if the specified integer is a power-of-two.
*/
template <typename IntegerType>
bool isPowerOfTwo (IntegerType value)
{
   return (value & (value - 1)) == 0;
}

/** Returns the smallest power-of-two which is equal to or greater than the given integer.
*/
inline int nextPowerOfTwo (int n) noexcept
{
	--n;
	n |= (n >> 1);
	n |= (n >> 2);
	n |= (n >> 4);
	n |= (n >> 8);
	n |= (n >> 16);
	return n + 1;
}

/** Performs a modulo operation, but can cope with the dividend being negative.
	The divisor must be greater than zero.
*/
template <typename IntegerType>
int negativeAwareModulo (IntegerType dividend, const IntegerType divisor) noexcept
{
	jassert (divisor > 0);
	dividend %= divisor;
	return (dividend < 0) ? (dividend + divisor) : dividend;
}

#if (JUCE_INTEL && JUCE_32BIT) || defined (DOXYGEN)
 /** This macro can be applied to a float variable to check whether it contains a denormalised
	 value, and to normalise it if necessary.
	 On CPUs that aren't vulnerable to denormalisation problems, this will have no effect.
 */
 #define JUCE_UNDENORMALISE(x)   x += 1.0f; x -= 1.0f;
#else
 #define JUCE_UNDENORMALISE(x)
#endif

/** This namespace contains a few template classes for helping work out class type variations.
*/
namespace TypeHelpers
{
   #if JUCE_VC8_OR_EARLIER
	#define PARAMETER_TYPE(type) const type&
   #else
	/** The ParameterType struct is used to find the best type to use when passing some kind
		of object as a parameter.

		Of course, this is only likely to be useful in certain esoteric template situations.

		Because "typename TypeHelpers::ParameterType<SomeClass>::type" is a bit of a mouthful, there's
		a PARAMETER_TYPE(SomeClass) macro that you can use to get the same effect.

		E.g. "myFunction (PARAMETER_TYPE (int), PARAMETER_TYPE (MyObject))"
		would evaluate to "myfunction (int, const MyObject&)", keeping any primitive types as
		pass-by-value, but passing objects as a const reference, to avoid copying.
	*/
	template <typename Type> struct ParameterType                   { typedef const Type& type; };

   #if ! DOXYGEN
	template <typename Type> struct ParameterType <Type&>           { typedef Type& type; };
	template <typename Type> struct ParameterType <Type*>           { typedef Type* type; };
	template <>              struct ParameterType <char>            { typedef char type; };
	template <>              struct ParameterType <unsigned char>   { typedef unsigned char type; };
	template <>              struct ParameterType <short>           { typedef short type; };
	template <>              struct ParameterType <unsigned short>  { typedef unsigned short type; };
	template <>              struct ParameterType <int>             { typedef int type; };
	template <>              struct ParameterType <unsigned int>    { typedef unsigned int type; };
	template <>              struct ParameterType <long>            { typedef long type; };
	template <>              struct ParameterType <unsigned long>   { typedef unsigned long type; };
	template <>              struct ParameterType <int64>           { typedef int64 type; };
	template <>              struct ParameterType <uint64>          { typedef uint64 type; };
	template <>              struct ParameterType <bool>            { typedef bool type; };
	template <>              struct ParameterType <float>           { typedef float type; };
	template <>              struct ParameterType <double>          { typedef double type; };
   #endif

	/** A helpful macro to simplify the use of the ParameterType template.
		@see ParameterType
	*/
	#define PARAMETER_TYPE(a)    typename TypeHelpers::ParameterType<a>::type
   #endif

	/** These templates are designed to take a type, and if it's a double, they return a double
		type; for anything else, they return a float type.
	*/
	template <typename Type> struct SmallestFloatType             { typedef float  type; };
	template <>              struct SmallestFloatType <double>    { typedef double type; };
}

#endif   // __JUCE_MATHSFUNCTIONS_JUCEHEADER__

/*** End of inlined file: juce_MathsFunctions.h ***/


/*** Start of inlined file: juce_ByteOrder.h ***/
#ifndef __JUCE_BYTEORDER_JUCEHEADER__
#define __JUCE_BYTEORDER_JUCEHEADER__

/** Contains static methods for converting the byte order between different
	endiannesses.
*/
class JUCE_API  ByteOrder
{
public:

	/** Swaps the upper and lower bytes of a 16-bit integer. */
	static uint16 swap (uint16 value);

	/** Reverses the order of the 4 bytes in a 32-bit integer. */
	static uint32 swap (uint32 value);

	/** Reverses the order of the 8 bytes in a 64-bit integer. */
	static uint64 swap (uint64 value);

	/** Swaps the byte order of a 16-bit int if the CPU is big-endian */
	static uint16 swapIfBigEndian (uint16 value);

	/** Swaps the byte order of a 32-bit int if the CPU is big-endian */
	static uint32 swapIfBigEndian (uint32 value);

	/** Swaps the byte order of a 64-bit int if the CPU is big-endian */
	static uint64 swapIfBigEndian (uint64 value);

	/** Swaps the byte order of a 16-bit int if the CPU is little-endian */
	static uint16 swapIfLittleEndian (uint16 value);

	/** Swaps the byte order of a 32-bit int if the CPU is little-endian */
	static uint32 swapIfLittleEndian (uint32 value);

	/** Swaps the byte order of a 64-bit int if the CPU is little-endian */
	static uint64 swapIfLittleEndian (uint64 value);

	/** Turns 4 bytes into a little-endian integer. */
	static uint32 littleEndianInt (const void* bytes);

	/** Turns 2 bytes into a little-endian integer. */
	static uint16 littleEndianShort (const void* bytes);

	/** Turns 4 bytes into a big-endian integer. */
	static uint32 bigEndianInt (const void* bytes);

	/** Turns 2 bytes into a big-endian integer. */
	static uint16 bigEndianShort (const void* bytes);

	/** Converts 3 little-endian bytes into a signed 24-bit value (which is sign-extended to 32 bits). */
	static int littleEndian24Bit (const char* bytes);

	/** Converts 3 big-endian bytes into a signed 24-bit value (which is sign-extended to 32 bits). */
	static int bigEndian24Bit (const char* bytes);

	/** Copies a 24-bit number to 3 little-endian bytes. */
	static void littleEndian24BitToChars (int value, char* destBytes);

	/** Copies a 24-bit number to 3 big-endian bytes. */
	static void bigEndian24BitToChars (int value, char* destBytes);

	/** Returns true if the current CPU is big-endian. */
	static bool isBigEndian();

private:
	ByteOrder();

	JUCE_DECLARE_NON_COPYABLE (ByteOrder);
};

#if JUCE_USE_INTRINSICS && ! defined (__INTEL_COMPILER)
 #pragma intrinsic (_byteswap_ulong)
#endif

inline uint16 ByteOrder::swap (uint16 n)
{
   #if JUCE_USE_INTRINSICSxxx // agh - the MS compiler has an internal error when you try to use this intrinsic!
	return static_cast <uint16> (_byteswap_ushort (n));
   #else
	return static_cast <uint16> ((n << 8) | (n >> 8));
   #endif
}

inline uint32 ByteOrder::swap (uint32 n)
{
   #if JUCE_MAC || JUCE_IOS
	return OSSwapInt32 (n);
   #elif JUCE_GCC && JUCE_INTEL
	asm("bswap %%eax" : "=a"(n) : "a"(n));
	return n;
   #elif JUCE_USE_INTRINSICS
	return _byteswap_ulong (n);
   #elif JUCE_MSVC
	__asm {
		mov eax, n
		bswap eax
		mov n, eax
	}
	return n;
   #elif JUCE_ANDROID
	return bswap_32 (n);
   #else
	return (n << 24) | (n >> 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8);
   #endif
}

inline uint64 ByteOrder::swap (uint64 value)
{
   #if JUCE_MAC || JUCE_IOS
	return OSSwapInt64 (value);
   #elif JUCE_USE_INTRINSICS
	return _byteswap_uint64 (value);
   #else
	return (((int64) swap ((uint32) value)) << 32) | swap ((uint32) (value >> 32));
   #endif
}

#if JUCE_LITTLE_ENDIAN
 inline uint16 ByteOrder::swapIfBigEndian (const uint16 v)                                  { return v; }
 inline uint32 ByteOrder::swapIfBigEndian (const uint32 v)                                  { return v; }
 inline uint64 ByteOrder::swapIfBigEndian (const uint64 v)                                  { return v; }
 inline uint16 ByteOrder::swapIfLittleEndian (const uint16 v)                               { return swap (v); }
 inline uint32 ByteOrder::swapIfLittleEndian (const uint32 v)                               { return swap (v); }
 inline uint64 ByteOrder::swapIfLittleEndian (const uint64 v)                               { return swap (v); }
 inline uint32 ByteOrder::littleEndianInt (const void* const bytes)                         { return *static_cast <const uint32*> (bytes); }
 inline uint16 ByteOrder::littleEndianShort (const void* const bytes)                       { return *static_cast <const uint16*> (bytes); }
 inline uint32 ByteOrder::bigEndianInt (const void* const bytes)                            { return swap (*static_cast <const uint32*> (bytes)); }
 inline uint16 ByteOrder::bigEndianShort (const void* const bytes)                          { return swap (*static_cast <const uint16*> (bytes)); }
 inline bool ByteOrder::isBigEndian()                                                       { return false; }
#else
 inline uint16 ByteOrder::swapIfBigEndian (const uint16 v)                                  { return swap (v); }
 inline uint32 ByteOrder::swapIfBigEndian (const uint32 v)                                  { return swap (v); }
 inline uint64 ByteOrder::swapIfBigEndian (const uint64 v)                                  { return swap (v); }
 inline uint16 ByteOrder::swapIfLittleEndian (const uint16 v)                               { return v; }
 inline uint32 ByteOrder::swapIfLittleEndian (const uint32 v)                               { return v; }
 inline uint64 ByteOrder::swapIfLittleEndian (const uint64 v)                               { return v; }
 inline uint32 ByteOrder::littleEndianInt (const void* const bytes)                         { return swap (*static_cast <const uint32*> (bytes)); }
 inline uint16 ByteOrder::littleEndianShort (const void* const bytes)                       { return swap (*static_cast <const uint16*> (bytes)); }
 inline uint32 ByteOrder::bigEndianInt (const void* const bytes)                            { return *static_cast <const uint32*> (bytes); }
 inline uint16 ByteOrder::bigEndianShort (const void* const bytes)                          { return *static_cast <const uint16*> (bytes); }
 inline bool ByteOrder::isBigEndian()                                                       { return true; }
#endif

inline int  ByteOrder::littleEndian24Bit (const char* const bytes)                          { return (((int) bytes[2]) << 16) | (((int) (uint8) bytes[1]) << 8) | ((int) (uint8) bytes[0]); }
inline int  ByteOrder::bigEndian24Bit (const char* const bytes)                             { return (((int) bytes[0]) << 16) | (((int) (uint8) bytes[1]) << 8) | ((int) (uint8) bytes[2]); }
inline void ByteOrder::littleEndian24BitToChars (const int value, char* const destBytes)    { destBytes[0] = (char)(value & 0xff); destBytes[1] = (char)((value >> 8) & 0xff); destBytes[2] = (char)((value >> 16) & 0xff); }
inline void ByteOrder::bigEndian24BitToChars (const int value, char* const destBytes)       { destBytes[0] = (char)((value >> 16) & 0xff); destBytes[1] = (char)((value >> 8) & 0xff); destBytes[2] = (char)(value & 0xff); }

#endif   // __JUCE_BYTEORDER_JUCEHEADER__

/*** End of inlined file: juce_ByteOrder.h ***/


/*** Start of inlined file: juce_Logger.h ***/
#ifndef __JUCE_LOGGER_JUCEHEADER__
#define __JUCE_LOGGER_JUCEHEADER__


/*** Start of inlined file: juce_String.h ***/
#ifndef __JUCE_STRING_JUCEHEADER__
#define __JUCE_STRING_JUCEHEADER__


/*** Start of inlined file: juce_CharacterFunctions.h ***/
#ifndef __JUCE_CHARACTERFUNCTIONS_JUCEHEADER__
#define __JUCE_CHARACTERFUNCTIONS_JUCEHEADER__

#if JUCE_WINDOWS && ! DOXYGEN
 #define JUCE_NATIVE_WCHAR_IS_UTF8      0
 #define JUCE_NATIVE_WCHAR_IS_UTF16     1
 #define JUCE_NATIVE_WCHAR_IS_UTF32     0
#else
 /** This macro will be set to 1 if the compiler's native wchar_t is an 8-bit type. */
 #define JUCE_NATIVE_WCHAR_IS_UTF8      0
 /** This macro will be set to 1 if the compiler's native wchar_t is a 16-bit type. */
 #define JUCE_NATIVE_WCHAR_IS_UTF16     0
 /** This macro will be set to 1 if the compiler's native wchar_t is a 32-bit type. */
 #define JUCE_NATIVE_WCHAR_IS_UTF32     1
#endif

#if JUCE_NATIVE_WCHAR_IS_UTF32 || DOXYGEN
 /** A platform-independent 32-bit unicode character type. */
 typedef wchar_t        juce_wchar;
#else
 typedef uint32         juce_wchar;
#endif

#ifndef DOXYGEN
 /** This macro is deprecated, but preserved for compatibility with old code. */
 #define JUCE_T(stringLiteral)   (L##stringLiteral)
#endif

#if JUCE_DEFINE_T_MACRO
 /** The 'T' macro is an alternative for using the "L" prefix in front of a string literal.

	 This macro is deprecated, but available for compatibility with old code if you set
	 JUCE_DEFINE_T_MACRO = 1. The fastest, most portable and best way to write your string
	 literals is as standard char strings, using escaped utf-8 character sequences for extended
	 characters, rather than trying to store them as wide-char strings.
 */
 #define T(stringLiteral)   JUCE_T(stringLiteral)
#endif

/**
	A collection of functions for manipulating characters and character strings.

	Most of these methods are designed for internal use by the String and CharPointer
	classes, but some of them may be useful to call directly.

	@see String, CharPointer_UTF8, CharPointer_UTF16, CharPointer_UTF32
*/
class JUCE_API  CharacterFunctions
{
public:

	/** Converts a character to upper-case. */
	static juce_wchar toUpperCase (juce_wchar character) noexcept;
	/** Converts a character to lower-case. */
	static juce_wchar toLowerCase (juce_wchar character) noexcept;

	/** Checks whether a unicode character is upper-case. */
	static bool isUpperCase (juce_wchar character) noexcept;
	/** Checks whether a unicode character is lower-case. */
	static bool isLowerCase (juce_wchar character) noexcept;

	/** Checks whether a character is whitespace. */
	static bool isWhitespace (char character) noexcept;
	/** Checks whether a character is whitespace. */
	static bool isWhitespace (juce_wchar character) noexcept;

	/** Checks whether a character is a digit. */
	static bool isDigit (char character) noexcept;
	/** Checks whether a character is a digit. */
	static bool isDigit (juce_wchar character) noexcept;

	/** Checks whether a character is alphabetic. */
	static bool isLetter (char character) noexcept;
	/** Checks whether a character is alphabetic. */
	static bool isLetter (juce_wchar character) noexcept;

	/** Checks whether a character is alphabetic or numeric. */
	static bool isLetterOrDigit (char character) noexcept;
	/** Checks whether a character is alphabetic or numeric. */
	static bool isLetterOrDigit (juce_wchar character) noexcept;

	/** Returns 0 to 16 for '0' to 'F", or -1 for characters that aren't a legal hex digit. */
	static int getHexDigitValue (juce_wchar digit) noexcept;

	/** Parses a character string to read a floating-point number.
		Note that this will advance the pointer that is passed in, leaving it at
		the end of the number.
	*/
	template <typename CharPointerType>
	static double readDoubleValue (CharPointerType& text) noexcept
	{
		double result[3] = { 0 }, accumulator[2] = { 0 };
		int exponentAdjustment[2] = { 0 }, exponentAccumulator[2] = { -1, -1 };
		int exponent = 0, decPointIndex = 0, digit = 0;
		int lastDigit = 0, numSignificantDigits = 0;
		bool isNegative = false, digitsFound = false;
		const int maxSignificantDigits = 15 + 2;

		text = text.findEndOfWhitespace();
		juce_wchar c = *text;

		switch (c)
		{
			case '-':   isNegative = true; // fall-through..
			case '+':   c = *++text;
		}

		switch (c)
		{
			case 'n':
			case 'N':
				if ((text[1] == 'a' || text[1] == 'A') && (text[2] == 'n' || text[2] == 'N'))
					return std::numeric_limits<double>::quiet_NaN();
				break;

			case 'i':
			case 'I':
				if ((text[1] == 'n' || text[1] == 'N') && (text[2] == 'f' || text[2] == 'F'))
					return std::numeric_limits<double>::infinity();
				break;
		}

		for (;;)
		{
			if (text.isDigit())
			{
				lastDigit = digit;
				digit = (int) text.getAndAdvance() - '0';
				digitsFound = true;

				if (decPointIndex != 0)
					exponentAdjustment[1]++;

				if (numSignificantDigits == 0 && digit == 0)
					continue;

				if (++numSignificantDigits > maxSignificantDigits)
				{
					if (digit > 5)
						++accumulator [decPointIndex];
					else if (digit == 5 && (lastDigit & 1) != 0)
						++accumulator [decPointIndex];

					if (decPointIndex > 0)
						exponentAdjustment[1]--;
					else
						exponentAdjustment[0]++;

					while (text.isDigit())
					{
						++text;
						if (decPointIndex == 0)
							exponentAdjustment[0]++;
					}
				}
				else
				{
					const double maxAccumulatorValue = (double) ((std::numeric_limits<unsigned int>::max() - 9) / 10);
					if (accumulator [decPointIndex] > maxAccumulatorValue)
					{
						result [decPointIndex] = mulexp10 (result [decPointIndex], exponentAccumulator [decPointIndex])
													+ accumulator [decPointIndex];
						accumulator [decPointIndex] = 0;
						exponentAccumulator [decPointIndex] = 0;
					}

					accumulator [decPointIndex] = accumulator[decPointIndex] * 10 + digit;
					exponentAccumulator [decPointIndex]++;
				}
			}
			else if (decPointIndex == 0 && *text == '.')
			{
				++text;
				decPointIndex = 1;

				if (numSignificantDigits > maxSignificantDigits)
				{
					while (text.isDigit())
						++text;
					break;
				}
			}
			else
			{
				break;
			}
		}

		result[0] = mulexp10 (result[0], exponentAccumulator[0]) + accumulator[0];

		if (decPointIndex != 0)
			result[1] = mulexp10 (result[1], exponentAccumulator[1]) + accumulator[1];

		c = *text;
		if ((c == 'e' || c == 'E') && digitsFound)
		{
			bool negativeExponent = false;

			switch (*++text)
			{
				case '-':   negativeExponent = true; // fall-through..
				case '+':   ++text;
			}

			while (text.isDigit())
				exponent = (exponent * 10) + ((int) text.getAndAdvance() - '0');

			if (negativeExponent)
				exponent = -exponent;
		}

		double r = mulexp10 (result[0], exponent + exponentAdjustment[0]);
		if (decPointIndex != 0)
			r += mulexp10 (result[1], exponent - exponentAdjustment[1]);

		return isNegative ? -r : r;
	}

	/** Parses a character string, to read a floating-point value. */
	template <typename CharPointerType>
	static double getDoubleValue (CharPointerType text) noexcept
	{
		return readDoubleValue (text);
	}

	/** Parses a character string, to read an integer value. */
	template <typename IntType, typename CharPointerType>
	static IntType getIntValue (const CharPointerType& text) noexcept
	{
		IntType v = 0;
		CharPointerType s (text.findEndOfWhitespace());

		const bool isNeg = *s == '-';
		if (isNeg)
			++s;

		for (;;)
		{
			const juce_wchar c = s.getAndAdvance();

			if (c >= '0' && c <= '9')
				v = v * 10 + (IntType) (c - '0');
			else
				break;
		}

		return isNeg ? -v : v;
	}

	/** Counts the number of characters in a given string, stopping if the count exceeds
		a specified limit. */
	template <typename CharPointerType>
	static size_t lengthUpTo (CharPointerType text, const size_t maxCharsToCount) noexcept
	{
		size_t len = 0;

		while (len < maxCharsToCount && text.getAndAdvance() != 0)
			++len;

		return len;
	}

	/** Counts the number of characters in a given string, stopping if the count exceeds
		a specified end-pointer. */
	template <typename CharPointerType>
	static size_t lengthUpTo (CharPointerType start, const CharPointerType& end) noexcept
	{
		size_t len = 0;

		while (start < end && start.getAndAdvance() != 0)
			++len;

		return len;
	}

	/** Copies null-terminated characters from one string to another. */
	template <typename DestCharPointerType, typename SrcCharPointerType>
	static void copyAll (DestCharPointerType& dest, SrcCharPointerType src) noexcept
	{
		for (;;)
		{
			const juce_wchar c = src.getAndAdvance();

			if (c == 0)
				break;

			dest.write (c);
		}

		dest.writeNull();
	}

	/** Copies characters from one string to another, up to a null terminator
		or a given byte size limit. */
	template <typename DestCharPointerType, typename SrcCharPointerType>
	static int copyWithDestByteLimit (DestCharPointerType& dest, SrcCharPointerType src, int maxBytes) noexcept
	{
		typename DestCharPointerType::CharType const* const startAddress = dest.getAddress();
		maxBytes -= sizeof (typename DestCharPointerType::CharType); // (allow for a terminating null)

		for (;;)
		{
			const juce_wchar c = src.getAndAdvance();
			const int bytesNeeded = (int) DestCharPointerType::getBytesRequiredFor (c);

			maxBytes -= bytesNeeded;
			if (c == 0 || maxBytes < 0)
				break;

			dest.write (c);
		}

		dest.writeNull();

		return (int) (getAddressDifference (dest.getAddress(), startAddress) + sizeof (typename DestCharPointerType::CharType));
	}

	/** Copies characters from one string to another, up to a null terminator
		or a given maximum number of characters. */
	template <typename DestCharPointerType, typename SrcCharPointerType>
	static void copyWithCharLimit (DestCharPointerType& dest, SrcCharPointerType src, int maxChars) noexcept
	{
		while (--maxChars > 0)
		{
			const juce_wchar c = src.getAndAdvance();
			if (c == 0)
				break;

			dest.write (c);
		}

		dest.writeNull();
	}

	/** Compares two null-terminated character strings. */
	template <typename CharPointerType1, typename CharPointerType2>
	static int compare (CharPointerType1 s1, CharPointerType2 s2) noexcept
	{
		for (;;)
		{
			const int c1 = (int) s1.getAndAdvance();
			const int c2 = (int) s2.getAndAdvance();

			const int diff = c1 - c2;
			if (diff != 0)
				return diff < 0 ? -1 : 1;
			else if (c1 == 0)
				break;
		}

		return 0;
	}

	/** Compares two null-terminated character strings, up to a given number of characters. */
	template <typename CharPointerType1, typename CharPointerType2>
	static int compareUpTo (CharPointerType1 s1, CharPointerType2 s2, int maxChars) noexcept
	{
		while (--maxChars >= 0)
		{
			const int c1 = (int) s1.getAndAdvance();
			const int c2 = (int) s2.getAndAdvance();

			const int diff = c1 - c2;
			if (diff != 0)
				return diff < 0 ? -1 : 1;
			else if (c1 == 0)
				break;
		}

		return 0;
	}

	/** Compares two null-terminated character strings, using a case-independant match. */
	template <typename CharPointerType1, typename CharPointerType2>
	static int compareIgnoreCase (CharPointerType1 s1, CharPointerType2 s2) noexcept
	{
		for (;;)
		{
			int c1 = (int) s1.toUpperCase();
			int c2 = (int) s2.toUpperCase();
			++s1;
			++s2;

			const int diff = c1 - c2;
			if (diff != 0)
				return diff < 0 ? -1 : 1;
			else if (c1 == 0)
				break;
		}

		return 0;
	}

	/** Compares two null-terminated character strings, using a case-independent match. */
	template <typename CharPointerType1, typename CharPointerType2>
	static int compareIgnoreCaseUpTo (CharPointerType1 s1, CharPointerType2 s2, int maxChars) noexcept
	{
		while (--maxChars >= 0)
		{
			int c1 = s1.toUpperCase();
			int c2 = s2.toUpperCase();
			++s1;
			++s2;

			const int diff = c1 - c2;
			if (diff != 0)
				return diff < 0 ? -1 : 1;
			else if (c1 == 0)
				break;
		}

		return 0;
	}

	/** Finds the character index of a given substring in another string.
		Returns -1 if the substring is not found.
	*/
	template <typename CharPointerType1, typename CharPointerType2>
	static int indexOf (CharPointerType1 haystack, const CharPointerType2& needle) noexcept
	{
		int index = 0;
		const int needleLength = (int) needle.length();

		for (;;)
		{
			if (haystack.compareUpTo (needle, needleLength) == 0)
				return index;

			if (haystack.getAndAdvance() == 0)
				return -1;

			++index;
		}
	}

	/** Finds the character index of a given substring in another string, using
		a case-independent match.
		Returns -1 if the substring is not found.
	*/
	template <typename CharPointerType1, typename CharPointerType2>
	static int indexOfIgnoreCase (CharPointerType1 haystack, const CharPointerType2& needle) noexcept
	{
		int index = 0;
		const int needleLength = (int) needle.length();

		for (;;)
		{
			if (haystack.compareIgnoreCaseUpTo (needle, needleLength) == 0)
				return index;

			if (haystack.getAndAdvance() == 0)
				return -1;

			++index;
		}
	}

	/** Finds the character index of a given character in another string.
		Returns -1 if the character is not found.
	*/
	template <typename Type>
	static int indexOfChar (Type text, const juce_wchar charToFind) noexcept
	{
		int i = 0;

		while (! text.isEmpty())
		{
			if (text.getAndAdvance() == charToFind)
				return i;

			++i;
		}

		return -1;
	}

	/** Finds the character index of a given character in another string, using
		a case-independent match.
		Returns -1 if the character is not found.
	*/
	template <typename Type>
	static int indexOfCharIgnoreCase (Type text, juce_wchar charToFind) noexcept
	{
		charToFind = CharacterFunctions::toLowerCase (charToFind);
		int i = 0;

		while (! text.isEmpty())
		{
			if (text.toLowerCase() == charToFind)
				return i;

			++text;
			++i;
		}

		return -1;
	}

	/** Returns a pointer to the first non-whitespace character in a string.
		If the string contains only whitespace, this will return a pointer
		to its null terminator.
	*/
	template <typename Type>
	static Type findEndOfWhitespace (const Type& text) noexcept
	{
		Type p (text);

		while (p.isWhitespace())
			++p;

		return p;
	}

	/** Returns a pointer to the first character in the string which is found in
		the breakCharacters string.
	*/
	template <typename Type>
	static Type findEndOfToken (const Type& text, const Type& breakCharacters, const Type& quoteCharacters)
	{
		Type t (text);
		juce_wchar currentQuoteChar = 0;

		while (! t.isEmpty())
		{
			const juce_wchar c = t.getAndAdvance();

			if (currentQuoteChar == 0 && breakCharacters.indexOf (c) >= 0)
			{
				--t;
				break;
			}

			if (quoteCharacters.indexOf (c) >= 0)
			{
				if (currentQuoteChar == 0)
					currentQuoteChar = c;
				else if (currentQuoteChar == c)
					currentQuoteChar = 0;
			}
		}

		return t;
	}

private:
	static double mulexp10 (const double value, int exponent) noexcept;
};

#endif   // __JUCE_CHARACTERFUNCTIONS_JUCEHEADER__

/*** End of inlined file: juce_CharacterFunctions.h ***/

#ifndef JUCE_STRING_UTF_TYPE
 #define JUCE_STRING_UTF_TYPE 8
#endif

#if JUCE_MSVC
 #pragma warning (push)
 #pragma warning (disable: 4514 4996)
#endif


/*** Start of inlined file: juce_Atomic.h ***/
#ifndef __JUCE_ATOMIC_JUCEHEADER__
#define __JUCE_ATOMIC_JUCEHEADER__

/**
	Simple class to hold a primitive value and perform atomic operations on it.

	The type used must be a 32 or 64 bit primitive, like an int, pointer, etc.
	There are methods to perform most of the basic atomic operations.
*/
template <typename Type>
class Atomic
{
public:
	/** Creates a new value, initialised to zero. */
	inline Atomic() noexcept
		: value (0)
	{
	}

	/** Creates a new value, with a given initial value. */
	inline Atomic (const Type initialValue) noexcept
		: value (initialValue)
	{
	}

	/** Copies another value (atomically). */
	inline Atomic (const Atomic& other) noexcept
		: value (other.get())
	{
	}

	/** Destructor. */
	inline ~Atomic() noexcept
	{
		// This class can only be used for types which are 32 or 64 bits in size.
		static_jassert (sizeof (Type) == 4 || sizeof (Type) == 8);
	}

	/** Atomically reads and returns the current value. */
	Type get() const noexcept;

	/** Copies another value onto this one (atomically). */
	inline Atomic& operator= (const Atomic& other) noexcept         { exchange (other.get()); return *this; }

	/** Copies another value onto this one (atomically). */
	inline Atomic& operator= (const Type newValue) noexcept         { exchange (newValue); return *this; }

	/** Atomically sets the current value. */
	void set (Type newValue) noexcept                               { exchange (newValue); }

	/** Atomically sets the current value, returning the value that was replaced. */
	Type exchange (Type value) noexcept;

	/** Atomically adds a number to this value, returning the new value. */
	Type operator+= (Type amountToAdd) noexcept;

	/** Atomically subtracts a number from this value, returning the new value. */
	Type operator-= (Type amountToSubtract) noexcept;

	/** Atomically increments this value, returning the new value. */
	Type operator++() noexcept;

	/** Atomically decrements this value, returning the new value. */
	Type operator--() noexcept;

	/** Atomically compares this value with a target value, and if it is equal, sets
		this to be equal to a new value.

		This operation is the atomic equivalent of doing this:
		@code
		bool compareAndSetBool (Type newValue, Type valueToCompare)
		{
			if (get() == valueToCompare)
			{
				set (newValue);
				return true;
			}

			return false;
		}
		@endcode

		@returns true if the comparison was true and the value was replaced; false if
				 the comparison failed and the value was left unchanged.
		@see compareAndSetValue
	*/
	bool compareAndSetBool (Type newValue, Type valueToCompare) noexcept;

	/** Atomically compares this value with a target value, and if it is equal, sets
		this to be equal to a new value.

		This operation is the atomic equivalent of doing this:
		@code
		Type compareAndSetValue (Type newValue, Type valueToCompare)
		{
			Type oldValue = get();
			if (oldValue == valueToCompare)
				set (newValue);

			return oldValue;
		}
		@endcode

		@returns the old value before it was changed.
		@see compareAndSetBool
	*/
	Type compareAndSetValue (Type newValue, Type valueToCompare) noexcept;

	/** Implements a memory read/write barrier. */
	static void memoryBarrier() noexcept;

   #if JUCE_64BIT
	JUCE_ALIGN (8)
   #else
	JUCE_ALIGN (4)
   #endif

	/** The raw value that this class operates on.
		This is exposed publically in case you need to manipulate it directly
		for performance reasons.
	*/
	volatile Type value;

private:
	static inline Type castFrom32Bit (int32 value) noexcept   { return *(Type*) &value; }
	static inline Type castFrom64Bit (int64 value) noexcept   { return *(Type*) &value; }
	static inline int32 castTo32Bit (Type value) noexcept     { return *(int32*) &value; }
	static inline int64 castTo64Bit (Type value) noexcept     { return *(int64*) &value; }

	Type operator++ (int); // better to just use pre-increment with atomics..
	Type operator-- (int);

	/** This templated negate function will negate pointers as well as integers */
	template <typename ValueType>
	inline ValueType negateValue (ValueType n) noexcept
	{
		return sizeof (ValueType) == 1 ? (ValueType) -(signed char) n
			: (sizeof (ValueType) == 2 ? (ValueType) -(short) n
			: (sizeof (ValueType) == 4 ? (ValueType) -(int) n
			: ((ValueType) -(int64) n)));
	}

	/** This templated negate function will negate pointers as well as integers */
	template <typename PointerType>
	inline PointerType* negateValue (PointerType* n) noexcept
	{
		return reinterpret_cast <PointerType*> (-reinterpret_cast <pointer_sized_int> (n));
	}
};

/*
	The following code is in the header so that the atomics can be inlined where possible...
*/
#if JUCE_IOS || (JUCE_MAC && (JUCE_PPC || defined (__clang__) || __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2)))
  #define JUCE_ATOMICS_MAC 1        // Older OSX builds using gcc4.1 or earlier

  #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
	#define JUCE_MAC_ATOMICS_VOLATILE
  #else
	#define JUCE_MAC_ATOMICS_VOLATILE volatile
  #endif

  #if JUCE_PPC || JUCE_IOS
	// None of these atomics are available for PPC or for iPhoneOS 3.1 or earlier!!
	template <typename Type> static Type OSAtomicAdd64Barrier (Type b, JUCE_MAC_ATOMICS_VOLATILE Type* a) noexcept  { jassertfalse; return *a += b; }
	template <typename Type> static Type OSAtomicIncrement64Barrier (JUCE_MAC_ATOMICS_VOLATILE Type* a) noexcept    { jassertfalse; return ++*a; }
	template <typename Type> static Type OSAtomicDecrement64Barrier (JUCE_MAC_ATOMICS_VOLATILE Type* a) noexcept    { jassertfalse; return --*a; }
	template <typename Type> static bool OSAtomicCompareAndSwap64Barrier (Type old, Type newValue, JUCE_MAC_ATOMICS_VOLATILE Type* value) noexcept
		{ jassertfalse; if (old == *value) { *value = newValue; return true; } return false; }
	#define JUCE_64BIT_ATOMICS_UNAVAILABLE 1
  #endif

#elif JUCE_GCC
  #define JUCE_ATOMICS_GCC 1        // GCC with intrinsics

  #if JUCE_IOS || JUCE_ANDROID // (64-bit ops will compile but not link on these mobile OSes)
	#define JUCE_64BIT_ATOMICS_UNAVAILABLE 1
  #endif

#else
  #define JUCE_ATOMICS_WINDOWS 1    // Windows with intrinsics

  #if JUCE_USE_INTRINSICS
	#ifndef __INTEL_COMPILER
	 #pragma intrinsic (_InterlockedExchange, _InterlockedIncrement, _InterlockedDecrement, _InterlockedCompareExchange, \
						_InterlockedCompareExchange64, _InterlockedExchangeAdd, _ReadWriteBarrier)
	#endif
	#define juce_InterlockedExchange(a, b)              _InterlockedExchange(a, b)
	#define juce_InterlockedIncrement(a)                _InterlockedIncrement(a)
	#define juce_InterlockedDecrement(a)                _InterlockedDecrement(a)
	#define juce_InterlockedExchangeAdd(a, b)           _InterlockedExchangeAdd(a, b)
	#define juce_InterlockedCompareExchange(a, b, c)    _InterlockedCompareExchange(a, b, c)
	#define juce_InterlockedCompareExchange64(a, b, c)  _InterlockedCompareExchange64(a, b, c)
	#define juce_MemoryBarrier _ReadWriteBarrier
  #else
	long juce_InterlockedExchange (volatile long* a, long b) noexcept;
	long juce_InterlockedIncrement (volatile long* a) noexcept;
	long juce_InterlockedDecrement (volatile long* a) noexcept;
	long juce_InterlockedExchangeAdd (volatile long* a, long b) noexcept;
	long juce_InterlockedCompareExchange (volatile long* a, long b, long c) noexcept;
	__int64 juce_InterlockedCompareExchange64 (volatile __int64* a, __int64 b, __int64 c) noexcept;
	inline void juce_MemoryBarrier() noexcept  { long x = 0; juce_InterlockedIncrement (&x); }
  #endif

  #if JUCE_64BIT
	#ifndef __INTEL_COMPILER
	 #pragma intrinsic (_InterlockedExchangeAdd64, _InterlockedExchange64, _InterlockedIncrement64, _InterlockedDecrement64)
	#endif
	#define juce_InterlockedExchangeAdd64(a, b)     _InterlockedExchangeAdd64(a, b)
	#define juce_InterlockedExchange64(a, b)        _InterlockedExchange64(a, b)
	#define juce_InterlockedIncrement64(a)          _InterlockedIncrement64(a)
	#define juce_InterlockedDecrement64(a)          _InterlockedDecrement64(a)
  #else
	// None of these atomics are available in a 32-bit Windows build!!
	template <typename Type> static Type juce_InterlockedExchangeAdd64 (volatile Type* a, Type b) noexcept  { jassertfalse; Type old = *a; *a += b; return old; }
	template <typename Type> static Type juce_InterlockedExchange64 (volatile Type* a, Type b) noexcept     { jassertfalse; Type old = *a; *a = b; return old; }
	template <typename Type> static Type juce_InterlockedIncrement64 (volatile Type* a) noexcept            { jassertfalse; return ++*a; }
	template <typename Type> static Type juce_InterlockedDecrement64 (volatile Type* a) noexcept            { jassertfalse; return --*a; }
	#define JUCE_64BIT_ATOMICS_UNAVAILABLE 1
  #endif
#endif

#if JUCE_MSVC
  #pragma warning (push)
  #pragma warning (disable: 4311)  // (truncation warning)
#endif

template <typename Type>
inline Type Atomic<Type>::get() const noexcept
{
  #if JUCE_ATOMICS_MAC
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) OSAtomicAdd32Barrier ((int32_t) 0, (JUCE_MAC_ATOMICS_VOLATILE int32_t*) &value))
							  : castFrom64Bit ((int64) OSAtomicAdd64Barrier ((int64_t) 0, (JUCE_MAC_ATOMICS_VOLATILE int64_t*) &value));
  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) juce_InterlockedExchangeAdd ((volatile long*) &value, (long) 0))
							  : castFrom64Bit ((int64) juce_InterlockedExchangeAdd64 ((volatile __int64*) &value, (__int64) 0));
  #elif JUCE_ATOMICS_GCC
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) __sync_add_and_fetch ((volatile int32*) &value, 0))
							  : castFrom64Bit ((int64) __sync_add_and_fetch ((volatile int64*) &value, 0));
  #endif
}

template <typename Type>
inline Type Atomic<Type>::exchange (const Type newValue) noexcept
{
  #if JUCE_ATOMICS_MAC || JUCE_ATOMICS_GCC
	Type currentVal = value;
	while (! compareAndSetBool (newValue, currentVal)) { currentVal = value; }
	return currentVal;
  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) juce_InterlockedExchange ((volatile long*) &value, (long) castTo32Bit (newValue)))
							  : castFrom64Bit ((int64) juce_InterlockedExchange64 ((volatile __int64*) &value, (__int64) castTo64Bit (newValue)));
  #endif
}

template <typename Type>
inline Type Atomic<Type>::operator+= (const Type amountToAdd) noexcept
{
  #if JUCE_ATOMICS_MAC
	return sizeof (Type) == 4 ? (Type) OSAtomicAdd32Barrier ((int32_t) castTo32Bit (amountToAdd), (JUCE_MAC_ATOMICS_VOLATILE int32_t*) &value)
							  : (Type) OSAtomicAdd64Barrier ((int64_t) amountToAdd, (JUCE_MAC_ATOMICS_VOLATILE int64_t*) &value);
  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? (Type) (juce_InterlockedExchangeAdd ((volatile long*) &value, (long) amountToAdd) + (long) amountToAdd)
							  : (Type) (juce_InterlockedExchangeAdd64 ((volatile __int64*) &value, (__int64) amountToAdd) + (__int64) amountToAdd);
  #elif JUCE_ATOMICS_GCC
	return (Type) __sync_add_and_fetch (&value, amountToAdd);
  #endif
}

template <typename Type>
inline Type Atomic<Type>::operator-= (const Type amountToSubtract) noexcept
{
	return operator+= (negateValue (amountToSubtract));
}

template <typename Type>
inline Type Atomic<Type>::operator++() noexcept
{
  #if JUCE_ATOMICS_MAC
	return sizeof (Type) == 4 ? (Type) OSAtomicIncrement32Barrier ((JUCE_MAC_ATOMICS_VOLATILE int32_t*) &value)
							  : (Type) OSAtomicIncrement64Barrier ((JUCE_MAC_ATOMICS_VOLATILE int64_t*) &value);
  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? (Type) juce_InterlockedIncrement ((volatile long*) &value)
							  : (Type) juce_InterlockedIncrement64 ((volatile __int64*) &value);
  #elif JUCE_ATOMICS_GCC
	return (Type) __sync_add_and_fetch (&value, 1);
  #endif
}

template <typename Type>
inline Type Atomic<Type>::operator--() noexcept
{
  #if JUCE_ATOMICS_MAC
	return sizeof (Type) == 4 ? (Type) OSAtomicDecrement32Barrier ((JUCE_MAC_ATOMICS_VOLATILE int32_t*) &value)
							  : (Type) OSAtomicDecrement64Barrier ((JUCE_MAC_ATOMICS_VOLATILE int64_t*) &value);
  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? (Type) juce_InterlockedDecrement ((volatile long*) &value)
							  : (Type) juce_InterlockedDecrement64 ((volatile __int64*) &value);
  #elif JUCE_ATOMICS_GCC
	return (Type) __sync_add_and_fetch (&value, -1);
  #endif
}

template <typename Type>
inline bool Atomic<Type>::compareAndSetBool (const Type newValue, const Type valueToCompare) noexcept
{
  #if JUCE_ATOMICS_MAC
	return sizeof (Type) == 4 ? OSAtomicCompareAndSwap32Barrier ((int32_t) castTo32Bit (valueToCompare), (int32_t) castTo32Bit (newValue), (JUCE_MAC_ATOMICS_VOLATILE int32_t*) &value)
							  : OSAtomicCompareAndSwap64Barrier ((int64_t) castTo64Bit (valueToCompare), (int64_t) castTo64Bit (newValue), (JUCE_MAC_ATOMICS_VOLATILE int64_t*) &value);
  #elif JUCE_ATOMICS_WINDOWS
	return compareAndSetValue (newValue, valueToCompare) == valueToCompare;
  #elif JUCE_ATOMICS_GCC
	return sizeof (Type) == 4 ? __sync_bool_compare_and_swap ((volatile int32*) &value, castTo32Bit (valueToCompare), castTo32Bit (newValue))
							  : __sync_bool_compare_and_swap ((volatile int64*) &value, castTo64Bit (valueToCompare), castTo64Bit (newValue));
  #endif
}

template <typename Type>
inline Type Atomic<Type>::compareAndSetValue (const Type newValue, const Type valueToCompare) noexcept
{
  #if JUCE_ATOMICS_MAC
	for (;;) // Annoying workaround for only having a bool CAS operation..
	{
		if (compareAndSetBool (newValue, valueToCompare))
			return valueToCompare;

		const Type result = value;
		if (result != valueToCompare)
			return result;
	}

  #elif JUCE_ATOMICS_WINDOWS
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) juce_InterlockedCompareExchange ((volatile long*) &value, (long) castTo32Bit (newValue), (long) castTo32Bit (valueToCompare)))
							  : castFrom64Bit ((int64) juce_InterlockedCompareExchange64 ((volatile __int64*) &value, (__int64) castTo64Bit (newValue), (__int64) castTo64Bit (valueToCompare)));
  #elif JUCE_ATOMICS_GCC
	return sizeof (Type) == 4 ? castFrom32Bit ((int32) __sync_val_compare_and_swap ((volatile int32*) &value, castTo32Bit (valueToCompare), castTo32Bit (newValue)))
							  : castFrom64Bit ((int64) __sync_val_compare_and_swap ((volatile int64*) &value, castTo64Bit (valueToCompare), castTo64Bit (newValue)));
  #endif
}

template <typename Type>
inline void Atomic<Type>::memoryBarrier() noexcept
{
  #if JUCE_ATOMICS_MAC
	OSMemoryBarrier();
  #elif JUCE_ATOMICS_GCC
	__sync_synchronize();
  #elif JUCE_ATOMICS_WINDOWS
	juce_MemoryBarrier();
  #endif
}

#if JUCE_MSVC
  #pragma warning (pop)
#endif

#endif   // __JUCE_ATOMIC_JUCEHEADER__

/*** End of inlined file: juce_Atomic.h ***/


/*** Start of inlined file: juce_CharPointer_UTF8.h ***/
#ifndef __JUCE_CHARPOINTER_UTF8_JUCEHEADER__
#define __JUCE_CHARPOINTER_UTF8_JUCEHEADER__

/**
	Wraps a pointer to a null-terminated UTF-8 character string, and provides
	various methods to operate on the data.
	@see CharPointer_UTF16, CharPointer_UTF32
*/
class CharPointer_UTF8
{
public:
	typedef char CharType;

	inline explicit CharPointer_UTF8 (const CharType* const rawPointer) noexcept
		: data (const_cast <CharType*> (rawPointer))
	{
	}

	inline CharPointer_UTF8 (const CharPointer_UTF8& other) noexcept
		: data (other.data)
	{
	}

	inline CharPointer_UTF8& operator= (const CharPointer_UTF8& other) noexcept
	{
		data = other.data;
		return *this;
	}

	inline CharPointer_UTF8& operator= (const CharType* text) noexcept
	{
		data = const_cast <CharType*> (text);
		return *this;
	}

	/** This is a pointer comparison, it doesn't compare the actual text. */
	inline bool operator== (const CharPointer_UTF8& other) const noexcept { return data == other.data; }
	inline bool operator!= (const CharPointer_UTF8& other) const noexcept { return data != other.data; }
	inline bool operator<= (const CharPointer_UTF8& other) const noexcept { return data <= other.data; }
	inline bool operator<  (const CharPointer_UTF8& other) const noexcept { return data <  other.data; }
	inline bool operator>= (const CharPointer_UTF8& other) const noexcept { return data >= other.data; }
	inline bool operator>  (const CharPointer_UTF8& other) const noexcept { return data >  other.data; }

	/** Returns the address that this pointer is pointing to. */
	inline CharType* getAddress() const noexcept        { return data; }

	/** Returns the address that this pointer is pointing to. */
	inline operator const CharType*() const noexcept    { return data; }

	/** Returns true if this pointer is pointing to a null character. */
	inline bool isEmpty() const noexcept                { return *data == 0; }

	/** Returns the unicode character that this pointer is pointing to. */
	juce_wchar operator*() const noexcept
	{
		const signed char byte = (signed char) *data;

		if (byte >= 0)
			return (juce_wchar) (uint8) byte;

		uint32 n = (uint32) (uint8) byte;
		uint32 mask = 0x7f;
		uint32 bit = 0x40;
		size_t numExtraValues = 0;

		while ((n & bit) != 0 && bit > 0x10)
		{
			mask >>= 1;
			++numExtraValues;
			bit >>= 1;
		}

		n &= mask;

		for (size_t i = 1; i <= numExtraValues; ++i)
		{
			const uint8 nextByte = (uint8) data [i];

			if ((nextByte & 0xc0) != 0x80)
				break;

			n <<= 6;
			n |= (nextByte & 0x3f);
		}

		return (juce_wchar) n;
	}

	/** Moves this pointer along to the next character in the string. */
	CharPointer_UTF8& operator++() noexcept
	{
		const signed char n = (signed char) *data++;

		if (n < 0)
		{
			juce_wchar bit = 0x40;

			while ((n & bit) != 0 && bit > 0x8)
			{
				++data;
				bit >>= 1;
			}
		}

		return *this;
	}

	/** Moves this pointer back to the previous character in the string. */
	CharPointer_UTF8& operator--() noexcept
	{
		const char n = *--data;

		if ((n & 0xc0) == 0xc0)
		{
			int count = 3;

			do
			{
				--data;
			}
			while ((*data & 0xc0) == 0xc0 && --count >= 0);
		}

		return *this;
	}

	/** Returns the character that this pointer is currently pointing to, and then
		advances the pointer to point to the next character. */
	juce_wchar getAndAdvance() noexcept
	{
		const signed char byte = (signed char) *data++;

		if (byte >= 0)
			return (juce_wchar) (uint8) byte;

		uint32 n = (uint32) (uint8) byte;
		uint32 mask = 0x7f;
		uint32 bit = 0x40;
		int numExtraValues = 0;

		while ((n & bit) != 0 && bit > 0x8)
		{
			mask >>= 1;
			++numExtraValues;
			bit >>= 1;
		}

		n &= mask;

		while (--numExtraValues >= 0)
		{
			const uint32 nextByte = (uint32) (uint8) *data++;

			if ((nextByte & 0xc0) != 0x80)
				break;

			n <<= 6;
			n |= (nextByte & 0x3f);
		}

		return (juce_wchar) n;
	}

	/** Moves this pointer along to the next character in the string. */
	CharPointer_UTF8 operator++ (int) noexcept
	{
		CharPointer_UTF8 temp (*this);
		++*this;
		return temp;
	}

	/** Moves this pointer forwards by the specified number of characters. */
	void operator+= (int numToSkip) noexcept
	{
		if (numToSkip < 0)
		{
			while (++numToSkip <= 0)
				--*this;
		}
		else
		{
			while (--numToSkip >= 0)
				++*this;
		}
	}

	/** Moves this pointer backwards by the specified number of characters. */
	void operator-= (int numToSkip) noexcept
	{
		operator+= (-numToSkip);
	}

	/** Returns the character at a given character index from the start of the string. */
	juce_wchar operator[] (int characterIndex) const noexcept
	{
		CharPointer_UTF8 p (*this);
		p += characterIndex;
		return *p;
	}

	/** Returns a pointer which is moved forwards from this one by the specified number of characters. */
	CharPointer_UTF8 operator+ (int numToSkip) const noexcept
	{
		CharPointer_UTF8 p (*this);
		p += numToSkip;
		return p;
	}

	/** Returns a pointer which is moved backwards from this one by the specified number of characters. */
	CharPointer_UTF8 operator- (int numToSkip) const noexcept
	{
		CharPointer_UTF8 p (*this);
		p += -numToSkip;
		return p;
	}

	/** Returns the number of characters in this string. */
	size_t length() const noexcept
	{
		const CharType* d = data;
		size_t count = 0;

		for (;;)
		{
			const uint32 n = (uint32) (uint8) *d++;

			if ((n & 0x80) != 0)
			{
				uint32 bit = 0x40;

				while ((n & bit) != 0)
				{
					++d;
					bit >>= 1;

					if (bit == 0)
						break; // illegal utf-8 sequence
				}
			}
			else if (n == 0)
				break;

			++count;
		}

		return count;
	}

	/** Returns the number of characters in this string, or the given value, whichever is lower. */
	size_t lengthUpTo (const size_t maxCharsToCount) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, maxCharsToCount);
	}

	/** Returns the number of characters in this string, or up to the given end pointer, whichever is lower. */
	size_t lengthUpTo (const CharPointer_UTF8& end) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, end);
	}

	/** Returns the number of bytes that are used to represent this string.
		This includes the terminating null character.
	*/
	size_t sizeInBytes() const noexcept
	{
		jassert (data != nullptr);
		return strlen (data) + 1;
	}

	/** Returns the number of bytes that would be needed to represent the given
		unicode character in this encoding format.
	*/
	static size_t getBytesRequiredFor (const juce_wchar charToWrite) noexcept
	{
		size_t num = 1;
		const uint32 c = (uint32) charToWrite;

		if (c >= 0x80)
		{
			++num;
			if (c >= 0x800)
			{
				++num;
				if (c >= 0x10000)
					++num;
			}
		}

		return num;
	}

	/** Returns the number of bytes that would be needed to represent the given
		string in this encoding format.
		The value returned does NOT include the terminating null character.
	*/
	template <class CharPointer>
	static size_t getBytesRequiredFor (CharPointer text) noexcept
	{
		size_t count = 0;
		juce_wchar n;

		while ((n = text.getAndAdvance()) != 0)
			count += getBytesRequiredFor (n);

		return count;
	}

	/** Returns a pointer to the null character that terminates this string. */
	CharPointer_UTF8 findTerminatingNull() const noexcept
	{
		return CharPointer_UTF8 (data + strlen (data));
	}

	/** Writes a unicode character to this string, and advances this pointer to point to the next position. */
	void write (const juce_wchar charToWrite) noexcept
	{
		const uint32 c = (uint32) charToWrite;

		if (c >= 0x80)
		{
			int numExtraBytes = 1;
			if (c >= 0x800)
			{
				++numExtraBytes;
				if (c >= 0x10000)
					++numExtraBytes;
			}

			*data++ = (CharType) ((uint32) (0xff << (7 - numExtraBytes)) | (c >> (numExtraBytes * 6)));

			while (--numExtraBytes >= 0)
				*data++ = (CharType) (0x80 | (0x3f & (c >> (numExtraBytes * 6))));
		}
		else
		{
			*data++ = (CharType) c;
		}
	}

	/** Writes a null character to this string (leaving the pointer's position unchanged). */
	inline void writeNull() const noexcept
	{
		*data = 0;
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	template <typename CharPointer>
	void writeAll (const CharPointer& src) noexcept
	{
		CharacterFunctions::copyAll (*this, src);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	void writeAll (const CharPointer_UTF8& src) noexcept
	{
		const CharType* s = src.data;

		while ((*data = *s) != 0)
		{
			++data;
			++s;
		}
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxDestBytes parameter specifies the maximum number of bytes that can be written
		to the destination buffer before stopping.
	*/
	template <typename CharPointer>
	int writeWithDestByteLimit (const CharPointer& src, const int maxDestBytes) noexcept
	{
		return CharacterFunctions::copyWithDestByteLimit (*this, src, maxDestBytes);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxChars parameter specifies the maximum number of characters that can be
		written to the destination buffer before stopping (including the terminating null).
	*/
	template <typename CharPointer>
	void writeWithCharLimit (const CharPointer& src, const int maxChars) noexcept
	{
		CharacterFunctions::copyWithCharLimit (*this, src, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compare (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compare (*this, other);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareUpTo (*this, other, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compareIgnoreCase (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compareIgnoreCase (*this, other);
	}

	/** Compares this string with another one. */
	int compareIgnoreCase (const CharPointer_UTF8& other) const noexcept
	{
	   #if JUCE_WINDOWS
		return stricmp (data, other.data);
	   #else
		return strcasecmp (data, other.data);
	   #endif
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareIgnoreCaseUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareIgnoreCaseUpTo (*this, other, maxChars);
	}

	/** Returns the character index of a substring, or -1 if it isn't found. */
	template <typename CharPointer>
	int indexOf (const CharPointer& stringToFind) const noexcept
	{
		return CharacterFunctions::indexOf (*this, stringToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind) const noexcept
	{
		return CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind, const bool ignoreCase) const noexcept
	{
		return ignoreCase ? CharacterFunctions::indexOfCharIgnoreCase (*this, charToFind)
						  : CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns true if the first character of this string is whitespace. */
	bool isWhitespace() const noexcept      { return *data == ' ' || (*data <= 13 && *data >= 9); }
	/** Returns true if the first character of this string is a digit. */
	bool isDigit() const noexcept           { return *data >= '0' && *data <= '9'; }
	/** Returns true if the first character of this string is a letter. */
	bool isLetter() const noexcept          { return CharacterFunctions::isLetter (operator*()) != 0; }
	/** Returns true if the first character of this string is a letter or digit. */
	bool isLetterOrDigit() const noexcept   { return CharacterFunctions::isLetterOrDigit (operator*()) != 0; }
	/** Returns true if the first character of this string is upper-case. */
	bool isUpperCase() const noexcept       { return CharacterFunctions::isUpperCase (operator*()) != 0; }
	/** Returns true if the first character of this string is lower-case. */
	bool isLowerCase() const noexcept       { return CharacterFunctions::isLowerCase (operator*()) != 0; }

	/** Returns an upper-case version of the first character of this string. */
	juce_wchar toUpperCase() const noexcept { return CharacterFunctions::toUpperCase (operator*()); }
	/** Returns a lower-case version of the first character of this string. */
	juce_wchar toLowerCase() const noexcept { return CharacterFunctions::toLowerCase (operator*()); }

	/** Parses this string as a 32-bit integer. */
	int getIntValue32() const noexcept      { return atoi (data); }

	/** Parses this string as a 64-bit integer. */
	int64 getIntValue64() const noexcept
	{
	   #if JUCE_LINUX || JUCE_ANDROID
		return atoll (data);
	   #elif JUCE_WINDOWS
		return _atoi64 (data);
	   #else
		return CharacterFunctions::getIntValue <int64, CharPointer_UTF8> (*this);
	   #endif
	}

	/** Parses this string as a floating point double. */
	double getDoubleValue() const noexcept  { return CharacterFunctions::getDoubleValue (*this); }

	/** Returns the first non-whitespace character in the string. */
	CharPointer_UTF8 findEndOfWhitespace() const noexcept   { return CharacterFunctions::findEndOfWhitespace (*this); }

	/** Returns true if the given unicode character can be represented in this encoding. */
	static bool canRepresent (juce_wchar character) noexcept
	{
		return ((unsigned int) character) < (unsigned int) 0x10ffff;
	}

	/** Returns true if this data contains a valid string in this encoding. */
	static bool isValidString (const CharType* dataToTest, int maxBytesToRead)
	{
		while (--maxBytesToRead >= 0 && *dataToTest != 0)
		{
			const signed char byte = (signed char) *dataToTest;

			if (byte < 0)
			{
				uint32 n = (uint32) (uint8) byte;
				uint32 mask = 0x7f;
				uint32 bit = 0x40;
				int numExtraValues = 0;

				while ((n & bit) != 0)
				{
					if (bit <= 0x10)
						return false;

					mask >>= 1;
					++numExtraValues;
					bit >>= 1;
				}

				n &= mask;

				while (--numExtraValues >= 0)
				{
					const uint32 nextByte = (uint32) (uint8) *dataToTest++;

					if ((nextByte & 0xc0) != 0x80)
						return false;
				}
			}
		}

		return true;
	}

	/** Atomically swaps this pointer for a new value, returning the previous value. */
	CharPointer_UTF8 atomicSwap (const CharPointer_UTF8& newValue)
	{
		return CharPointer_UTF8 (reinterpret_cast <Atomic<CharType*>&> (data).exchange (newValue.data));
	}

	/** These values are the byte-order-mark (BOM) values for a UTF-8 stream. */
	enum
	{
		byteOrderMark1 = 0xef,
		byteOrderMark2 = 0xbb,
		byteOrderMark3 = 0xbf
	};

private:
	CharType* data;
};

#endif   // __JUCE_CHARPOINTER_UTF8_JUCEHEADER__

/*** End of inlined file: juce_CharPointer_UTF8.h ***/


/*** Start of inlined file: juce_CharPointer_UTF16.h ***/
#ifndef __JUCE_CHARPOINTER_UTF16_JUCEHEADER__
#define __JUCE_CHARPOINTER_UTF16_JUCEHEADER__

/**
	Wraps a pointer to a null-terminated UTF-16 character string, and provides
	various methods to operate on the data.
	@see CharPointer_UTF8, CharPointer_UTF32
*/
class CharPointer_UTF16
{
public:
   #if JUCE_NATIVE_WCHAR_IS_UTF16
	typedef wchar_t CharType;
   #else
	typedef int16 CharType;
   #endif

	inline explicit CharPointer_UTF16 (const CharType* const rawPointer) noexcept
		: data (const_cast <CharType*> (rawPointer))
	{
	}

	inline CharPointer_UTF16 (const CharPointer_UTF16& other) noexcept
		: data (other.data)
	{
	}

	inline CharPointer_UTF16& operator= (const CharPointer_UTF16& other) noexcept
	{
		data = other.data;
		return *this;
	}

	inline CharPointer_UTF16& operator= (const CharType* text) noexcept
	{
		data = const_cast <CharType*> (text);
		return *this;
	}

	/** This is a pointer comparison, it doesn't compare the actual text. */
	inline bool operator== (const CharPointer_UTF16& other) const noexcept { return data == other.data; }
	inline bool operator!= (const CharPointer_UTF16& other) const noexcept { return data != other.data; }
	inline bool operator<= (const CharPointer_UTF16& other) const noexcept { return data <= other.data; }
	inline bool operator<  (const CharPointer_UTF16& other) const noexcept { return data <  other.data; }
	inline bool operator>= (const CharPointer_UTF16& other) const noexcept { return data >= other.data; }
	inline bool operator>  (const CharPointer_UTF16& other) const noexcept { return data >  other.data; }

	/** Returns the address that this pointer is pointing to. */
	inline CharType* getAddress() const noexcept        { return data; }

	/** Returns the address that this pointer is pointing to. */
	inline operator const CharType*() const noexcept    { return data; }

	/** Returns true if this pointer is pointing to a null character. */
	inline bool isEmpty() const noexcept                { return *data == 0; }

	/** Returns the unicode character that this pointer is pointing to. */
	juce_wchar operator*() const noexcept
	{
		uint32 n = (uint32) (uint16) *data;

		if (n >= 0xd800 && n <= 0xdfff && ((uint32) (uint16) data[1]) >= 0xdc00)
			n = 0x10000 + (((n - 0xd800) << 10) | (((uint32) (uint16) data[1]) - 0xdc00));

		return (juce_wchar) n;
	}

	/** Moves this pointer along to the next character in the string. */
	CharPointer_UTF16& operator++() noexcept
	{
		const juce_wchar n = *data++;

		if (n >= 0xd800 && n <= 0xdfff && ((uint32) (uint16) *data) >= 0xdc00)
			++data;

		return *this;
	}

	/** Moves this pointer back to the previous character in the string. */
	CharPointer_UTF16& operator--() noexcept
	{
		const juce_wchar n = *--data;

		if (n >= 0xdc00 && n <= 0xdfff)
			--data;

		return *this;
	}

	/** Returns the character that this pointer is currently pointing to, and then
		advances the pointer to point to the next character. */
	juce_wchar getAndAdvance() noexcept
	{
		uint32 n = (uint32) (uint16) *data++;

		if (n >= 0xd800 && n <= 0xdfff && ((uint32) (uint16) *data) >= 0xdc00)
			n = 0x10000 + ((((n - 0xd800) << 10) | (((uint32) (uint16) *data++) - 0xdc00)));

		return (juce_wchar) n;
	}

	/** Moves this pointer along to the next character in the string. */
	CharPointer_UTF16 operator++ (int) noexcept
	{
		CharPointer_UTF16 temp (*this);
		++*this;
		return temp;
	}

	/** Moves this pointer forwards by the specified number of characters. */
	void operator+= (int numToSkip) noexcept
	{
		if (numToSkip < 0)
		{
			while (++numToSkip <= 0)
				--*this;
		}
		else
		{
			while (--numToSkip >= 0)
				++*this;
		}
	}

	/** Moves this pointer backwards by the specified number of characters. */
	void operator-= (int numToSkip) noexcept
	{
		operator+= (-numToSkip);
	}

	/** Returns the character at a given character index from the start of the string. */
	juce_wchar operator[] (const int characterIndex) const noexcept
	{
		CharPointer_UTF16 p (*this);
		p += characterIndex;
		return *p;
	}

	/** Returns a pointer which is moved forwards from this one by the specified number of characters. */
	CharPointer_UTF16 operator+ (const int numToSkip) const noexcept
	{
		CharPointer_UTF16 p (*this);
		p += numToSkip;
		return p;
	}

	/** Returns a pointer which is moved backwards from this one by the specified number of characters. */
	CharPointer_UTF16 operator- (const int numToSkip) const noexcept
	{
		CharPointer_UTF16 p (*this);
		p += -numToSkip;
		return p;
	}

	/** Writes a unicode character to this string, and advances this pointer to point to the next position. */
	void write (juce_wchar charToWrite) noexcept
	{
		if (charToWrite >= 0x10000)
		{
			charToWrite -= 0x10000;
			*data++ = (CharType) (0xd800 + (charToWrite >> 10));
			*data++ = (CharType) (0xdc00 + (charToWrite & 0x3ff));
		}
		else
		{
			*data++ = (CharType) charToWrite;
		}
	}

	/** Writes a null character to this string (leaving the pointer's position unchanged). */
	inline void writeNull() const noexcept
	{
		*data = 0;
	}

	/** Returns the number of characters in this string. */
	size_t length() const noexcept
	{
		const CharType* d = data;
		size_t count = 0;

		for (;;)
		{
			const int n = *d++;

			if (n >= 0xd800 && n <= 0xdfff)
			{
				if (*d++ == 0)
					break;
			}
			else if (n == 0)
				break;

			++count;
		}

		return count;
	}

	/** Returns the number of characters in this string, or the given value, whichever is lower. */
	size_t lengthUpTo (const size_t maxCharsToCount) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, maxCharsToCount);
	}

	/** Returns the number of characters in this string, or up to the given end pointer, whichever is lower. */
	size_t lengthUpTo (const CharPointer_UTF16& end) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, end);
	}

	/** Returns the number of bytes that are used to represent this string.
		This includes the terminating null character.
	*/
	size_t sizeInBytes() const noexcept
	{
		return sizeof (CharType) * (findNullIndex (data) + 1);
	}

	/** Returns the number of bytes that would be needed to represent the given
		unicode character in this encoding format.
	*/
	static size_t getBytesRequiredFor (const juce_wchar charToWrite) noexcept
	{
		return (charToWrite >= 0x10000) ? (sizeof (CharType) * 2) : sizeof (CharType);
	}

	/** Returns the number of bytes that would be needed to represent the given
		string in this encoding format.
		The value returned does NOT include the terminating null character.
	*/
	template <class CharPointer>
	static size_t getBytesRequiredFor (CharPointer text) noexcept
	{
		size_t count = 0;
		juce_wchar n;

		while ((n = text.getAndAdvance()) != 0)
			count += getBytesRequiredFor (n);

		return count;
	}

	/** Returns a pointer to the null character that terminates this string. */
	CharPointer_UTF16 findTerminatingNull() const noexcept
	{
		const CharType* t = data;

		while (*t != 0)
			++t;

		return CharPointer_UTF16 (t);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	template <typename CharPointer>
	void writeAll (const CharPointer& src) noexcept
	{
		CharacterFunctions::copyAll (*this, src);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	void writeAll (const CharPointer_UTF16& src) noexcept
	{
		const CharType* s = src.data;

		while ((*data = *s) != 0)
		{
			++data;
			++s;
		}
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxDestBytes parameter specifies the maximum number of bytes that can be written
		to the destination buffer before stopping.
	*/
	template <typename CharPointer>
	int writeWithDestByteLimit (const CharPointer& src, const int maxDestBytes) noexcept
	{
		return CharacterFunctions::copyWithDestByteLimit (*this, src, maxDestBytes);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxChars parameter specifies the maximum number of characters that can be
		written to the destination buffer before stopping (including the terminating null).
	*/
	template <typename CharPointer>
	void writeWithCharLimit (const CharPointer& src, const int maxChars) noexcept
	{
		CharacterFunctions::copyWithCharLimit (*this, src, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compare (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compare (*this, other);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareUpTo (*this, other, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compareIgnoreCase (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compareIgnoreCase (*this, other);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareIgnoreCaseUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareIgnoreCaseUpTo (*this, other, maxChars);
	}

   #if JUCE_WINDOWS && ! DOXYGEN
	int compareIgnoreCase (const CharPointer_UTF16& other) const noexcept
	{
		return _wcsicmp (data, other.data);
	}

	int compareIgnoreCaseUpTo (const CharPointer_UTF16& other, int maxChars) const noexcept
	{
		return _wcsnicmp (data, other.data, (size_t) maxChars);
	}

	int indexOf (const CharPointer_UTF16& stringToFind) const noexcept
	{
		const CharType* const t = wcsstr (data, stringToFind.getAddress());
		return t == nullptr ? -1 : (int) (t - data);
	}
   #endif

	/** Returns the character index of a substring, or -1 if it isn't found. */
	template <typename CharPointer>
	int indexOf (const CharPointer& stringToFind) const noexcept
	{
		return CharacterFunctions::indexOf (*this, stringToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind) const noexcept
	{
		return CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind, const bool ignoreCase) const noexcept
	{
		return ignoreCase ? CharacterFunctions::indexOfCharIgnoreCase (*this, charToFind)
						  : CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns true if the first character of this string is whitespace. */
	bool isWhitespace() const noexcept      { return CharacterFunctions::isWhitespace (operator*()) != 0; }
	/** Returns true if the first character of this string is a digit. */
	bool isDigit() const noexcept           { return CharacterFunctions::isDigit (operator*()) != 0; }
	/** Returns true if the first character of this string is a letter. */
	bool isLetter() const noexcept          { return CharacterFunctions::isLetter (operator*()) != 0; }
	/** Returns true if the first character of this string is a letter or digit. */
	bool isLetterOrDigit() const noexcept   { return CharacterFunctions::isLetterOrDigit (operator*()) != 0; }
	/** Returns true if the first character of this string is upper-case. */
	bool isUpperCase() const noexcept       { return CharacterFunctions::isUpperCase (operator*()) != 0; }
	/** Returns true if the first character of this string is lower-case. */
	bool isLowerCase() const noexcept       { return CharacterFunctions::isLowerCase (operator*()) != 0; }

	/** Returns an upper-case version of the first character of this string. */
	juce_wchar toUpperCase() const noexcept { return CharacterFunctions::toUpperCase (operator*()); }
	/** Returns a lower-case version of the first character of this string. */
	juce_wchar toLowerCase() const noexcept { return CharacterFunctions::toLowerCase (operator*()); }

	/** Parses this string as a 32-bit integer. */
	int getIntValue32() const noexcept
	{
	   #if JUCE_WINDOWS
		return _wtoi (data);
	   #else
		return CharacterFunctions::getIntValue <int, CharPointer_UTF16> (*this);
	   #endif
	}

	/** Parses this string as a 64-bit integer. */
	int64 getIntValue64() const noexcept
	{
	   #if JUCE_WINDOWS
		return _wtoi64 (data);
	   #else
		return CharacterFunctions::getIntValue <int64, CharPointer_UTF16> (*this);
	   #endif
	}

	/** Parses this string as a floating point double. */
	double getDoubleValue() const noexcept  { return CharacterFunctions::getDoubleValue (*this); }

	/** Returns the first non-whitespace character in the string. */
	CharPointer_UTF16 findEndOfWhitespace() const noexcept   { return CharacterFunctions::findEndOfWhitespace (*this); }

	/** Returns true if the given unicode character can be represented in this encoding. */
	static bool canRepresent (juce_wchar character) noexcept
	{
		return ((unsigned int) character) < (unsigned int) 0x10ffff
				 && (((unsigned int) character) < 0xd800 || ((unsigned int) character) > 0xdfff);
	}

	/** Returns true if this data contains a valid string in this encoding. */
	static bool isValidString (const CharType* dataToTest, int maxBytesToRead)
	{
		maxBytesToRead /= sizeof (CharType);

		while (--maxBytesToRead >= 0 && *dataToTest != 0)
		{
			const uint32 n = (uint32) (uint16) *dataToTest++;

			if (n >= 0xd800)
			{
				if (n > 0x10ffff)
					return false;

				if (n <= 0xdfff)
				{
					if (n > 0xdc00)
						return false;

					const uint32 nextChar = (uint32) (uint16) *dataToTest++;

					if (nextChar < 0xdc00 || nextChar > 0xdfff)
						return false;
				}
			}
		}

		return true;
	}

	/** Atomically swaps this pointer for a new value, returning the previous value. */
	CharPointer_UTF16 atomicSwap (const CharPointer_UTF16& newValue)
	{
		return CharPointer_UTF16 (reinterpret_cast <Atomic<CharType*>&> (data).exchange (newValue.data));
	}

	/** These values are the byte-order-mark (BOM) values for a UTF-16 stream. */
	enum
	{
		byteOrderMarkBE1 = 0xfe,
		byteOrderMarkBE2 = 0xff,
		byteOrderMarkLE1 = 0xff,
		byteOrderMarkLE2 = 0xfe
	};

private:
	CharType* data;

	static unsigned int findNullIndex (const CharType* const t) noexcept
	{
		unsigned int n = 0;

		while (t[n] != 0)
			++n;

		return n;
	}
};

#endif   // __JUCE_CHARPOINTER_UTF16_JUCEHEADER__

/*** End of inlined file: juce_CharPointer_UTF16.h ***/


/*** Start of inlined file: juce_CharPointer_UTF32.h ***/
#ifndef __JUCE_CHARPOINTER_UTF32_JUCEHEADER__
#define __JUCE_CHARPOINTER_UTF32_JUCEHEADER__

/**
	Wraps a pointer to a null-terminated UTF-32 character string, and provides
	various methods to operate on the data.
	@see CharPointer_UTF8, CharPointer_UTF16
*/
class CharPointer_UTF32
{
public:
	typedef juce_wchar CharType;

	inline explicit CharPointer_UTF32 (const CharType* const rawPointer) noexcept
		: data (const_cast <CharType*> (rawPointer))
	{
	}

	inline CharPointer_UTF32 (const CharPointer_UTF32& other) noexcept
		: data (other.data)
	{
	}

	inline CharPointer_UTF32& operator= (const CharPointer_UTF32& other) noexcept
	{
		data = other.data;
		return *this;
	}

	inline CharPointer_UTF32& operator= (const CharType* text) noexcept
	{
		data = const_cast <CharType*> (text);
		return *this;
	}

	/** This is a pointer comparison, it doesn't compare the actual text. */
	inline bool operator== (const CharPointer_UTF32& other) const noexcept { return data == other.data; }
	inline bool operator!= (const CharPointer_UTF32& other) const noexcept { return data != other.data; }
	inline bool operator<= (const CharPointer_UTF32& other) const noexcept { return data <= other.data; }
	inline bool operator<  (const CharPointer_UTF32& other) const noexcept { return data <  other.data; }
	inline bool operator>= (const CharPointer_UTF32& other) const noexcept { return data >= other.data; }
	inline bool operator>  (const CharPointer_UTF32& other) const noexcept { return data >  other.data; }

	/** Returns the address that this pointer is pointing to. */
	inline CharType* getAddress() const noexcept        { return data; }

	/** Returns the address that this pointer is pointing to. */
	inline operator const CharType*() const noexcept    { return data; }

	/** Returns true if this pointer is pointing to a null character. */
	inline bool isEmpty() const noexcept                { return *data == 0; }

	/** Returns the unicode character that this pointer is pointing to. */
	inline juce_wchar operator*() const noexcept        { return *data; }

	/** Moves this pointer along to the next character in the string. */
	inline CharPointer_UTF32& operator++() noexcept
	{
		++data;
		return *this;
	}

	/** Moves this pointer to the previous character in the string. */
	inline CharPointer_UTF32& operator--() noexcept
	{
		--data;
		return *this;
	}

	/** Returns the character that this pointer is currently pointing to, and then
		advances the pointer to point to the next character. */
	inline juce_wchar getAndAdvance() noexcept  { return *data++; }

	/** Moves this pointer along to the next character in the string. */
	CharPointer_UTF32 operator++ (int) noexcept
	{
		CharPointer_UTF32 temp (*this);
		++data;
		return temp;
	}

	/** Moves this pointer forwards by the specified number of characters. */
	inline void operator+= (const int numToSkip) noexcept
	{
		data += numToSkip;
	}

	inline void operator-= (const int numToSkip) noexcept
	{
		data -= numToSkip;
	}

	/** Returns the character at a given character index from the start of the string. */
	inline juce_wchar& operator[] (const int characterIndex) const noexcept
	{
		return data [characterIndex];
	}

	/** Returns a pointer which is moved forwards from this one by the specified number of characters. */
	CharPointer_UTF32 operator+ (const int numToSkip) const noexcept
	{
		return CharPointer_UTF32 (data + numToSkip);
	}

	/** Returns a pointer which is moved backwards from this one by the specified number of characters. */
	CharPointer_UTF32 operator- (const int numToSkip) const noexcept
	{
		return CharPointer_UTF32 (data - numToSkip);
	}

	/** Writes a unicode character to this string, and advances this pointer to point to the next position. */
	inline void write (const juce_wchar charToWrite) noexcept
	{
		*data++ = charToWrite;
	}

	inline void replaceChar (const juce_wchar newChar) noexcept
	{
		*data = newChar;
	}

	/** Writes a null character to this string (leaving the pointer's position unchanged). */
	inline void writeNull() const noexcept
	{
		*data = 0;
	}

	/** Returns the number of characters in this string. */
	size_t length() const noexcept
	{
	   #if JUCE_NATIVE_WCHAR_IS_UTF32 && ! JUCE_ANDROID
		return wcslen (data);
	   #else
		size_t n = 0;
		while (data[n] != 0)
			++n;
		return n;
	   #endif
	}

	/** Returns the number of characters in this string, or the given value, whichever is lower. */
	size_t lengthUpTo (const size_t maxCharsToCount) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, maxCharsToCount);
	}

	/** Returns the number of characters in this string, or up to the given end pointer, whichever is lower. */
	size_t lengthUpTo (const CharPointer_UTF32& end) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, end);
	}

	/** Returns the number of bytes that are used to represent this string.
		This includes the terminating null character.
	*/
	size_t sizeInBytes() const noexcept
	{
		return sizeof (CharType) * (length() + 1);
	}

	/** Returns the number of bytes that would be needed to represent the given
		unicode character in this encoding format.
	*/
	static inline size_t getBytesRequiredFor (const juce_wchar) noexcept
	{
		return sizeof (CharType);
	}

	/** Returns the number of bytes that would be needed to represent the given
		string in this encoding format.
		The value returned does NOT include the terminating null character.
	*/
	template <class CharPointer>
	static size_t getBytesRequiredFor (const CharPointer& text) noexcept
	{
		return sizeof (CharType) * text.length();
	}

	/** Returns a pointer to the null character that terminates this string. */
	CharPointer_UTF32 findTerminatingNull() const noexcept
	{
		return CharPointer_UTF32 (data + length());
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	template <typename CharPointer>
	void writeAll (const CharPointer& src) noexcept
	{
		CharacterFunctions::copyAll (*this, src);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	void writeAll (const CharPointer_UTF32& src) noexcept
	{
		const CharType* s = src.data;

		while ((*data = *s) != 0)
		{
			++data;
			++s;
		}
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxDestBytes parameter specifies the maximum number of bytes that can be written
		to the destination buffer before stopping.
	*/
	template <typename CharPointer>
	int writeWithDestByteLimit (const CharPointer& src, const int maxDestBytes) noexcept
	{
		return CharacterFunctions::copyWithDestByteLimit (*this, src, maxDestBytes);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxChars parameter specifies the maximum number of characters that can be
		written to the destination buffer before stopping (including the terminating null).
	*/
	template <typename CharPointer>
	void writeWithCharLimit (const CharPointer& src, const int maxChars) noexcept
	{
		CharacterFunctions::copyWithCharLimit (*this, src, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compare (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compare (*this, other);
	}

   #if JUCE_NATIVE_WCHAR_IS_UTF32 && ! JUCE_ANDROID
	/** Compares this string with another one. */
	int compare (const CharPointer_UTF32& other) const noexcept
	{
		return wcscmp (data, other.data);
	}
   #endif

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareUpTo (*this, other, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compareIgnoreCase (const CharPointer& other) const
	{
		return CharacterFunctions::compareIgnoreCase (*this, other);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareIgnoreCaseUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareIgnoreCaseUpTo (*this, other, maxChars);
	}

	/** Returns the character index of a substring, or -1 if it isn't found. */
	template <typename CharPointer>
	int indexOf (const CharPointer& stringToFind) const noexcept
	{
		return CharacterFunctions::indexOf (*this, stringToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind) const noexcept
	{
		int i = 0;

		while (data[i] != 0)
		{
			if (data[i] == charToFind)
				return i;

			++i;
		}

		return -1;
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind, const bool ignoreCase) const noexcept
	{
		return ignoreCase ? CharacterFunctions::indexOfCharIgnoreCase (*this, charToFind)
						  : CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns true if the first character of this string is whitespace. */
	bool isWhitespace() const               { return CharacterFunctions::isWhitespace (*data) != 0; }
	/** Returns true if the first character of this string is a digit. */
	bool isDigit() const                    { return CharacterFunctions::isDigit (*data) != 0; }
	/** Returns true if the first character of this string is a letter. */
	bool isLetter() const                   { return CharacterFunctions::isLetter (*data) != 0; }
	/** Returns true if the first character of this string is a letter or digit. */
	bool isLetterOrDigit() const            { return CharacterFunctions::isLetterOrDigit (*data) != 0; }
	/** Returns true if the first character of this string is upper-case. */
	bool isUpperCase() const                { return CharacterFunctions::isUpperCase (*data) != 0; }
	/** Returns true if the first character of this string is lower-case. */
	bool isLowerCase() const                { return CharacterFunctions::isLowerCase (*data) != 0; }

	/** Returns an upper-case version of the first character of this string. */
	juce_wchar toUpperCase() const noexcept { return CharacterFunctions::toUpperCase (*data); }
	/** Returns a lower-case version of the first character of this string. */
	juce_wchar toLowerCase() const noexcept { return CharacterFunctions::toLowerCase (*data); }

	/** Parses this string as a 32-bit integer. */
	int getIntValue32() const noexcept      { return CharacterFunctions::getIntValue <int, CharPointer_UTF32> (*this); }
	/** Parses this string as a 64-bit integer. */
	int64 getIntValue64() const noexcept    { return CharacterFunctions::getIntValue <int64, CharPointer_UTF32> (*this); }

	/** Parses this string as a floating point double. */
	double getDoubleValue() const noexcept  { return CharacterFunctions::getDoubleValue (*this); }

	/** Returns the first non-whitespace character in the string. */
	CharPointer_UTF32 findEndOfWhitespace() const noexcept   { return CharacterFunctions::findEndOfWhitespace (*this); }

	/** Returns true if the given unicode character can be represented in this encoding. */
	static bool canRepresent (juce_wchar character) noexcept
	{
		return ((unsigned int) character) < (unsigned int) 0x10ffff;
	}

	/** Returns true if this data contains a valid string in this encoding. */
	static bool isValidString (const CharType* dataToTest, int maxBytesToRead)
	{
		maxBytesToRead /= sizeof (CharType);

		while (--maxBytesToRead >= 0 && *dataToTest != 0)
			if (! canRepresent (*dataToTest++))
				return false;

		return true;
	}

	/** Atomically swaps this pointer for a new value, returning the previous value. */
	CharPointer_UTF32 atomicSwap (const CharPointer_UTF32& newValue)
	{
		return CharPointer_UTF32 (reinterpret_cast <Atomic<CharType*>&> (data).exchange (newValue.data));
	}

private:
	CharType* data;
};

#endif   // __JUCE_CHARPOINTER_UTF32_JUCEHEADER__

/*** End of inlined file: juce_CharPointer_UTF32.h ***/


/*** Start of inlined file: juce_CharPointer_ASCII.h ***/
#ifndef __JUCE_CHARPOINTER_ASCII_JUCEHEADER__
#define __JUCE_CHARPOINTER_ASCII_JUCEHEADER__

/**
	Wraps a pointer to a null-terminated ASCII character string, and provides
	various methods to operate on the data.

	A valid ASCII string is assumed to not contain any characters above 127.

	@see CharPointer_UTF8, CharPointer_UTF16, CharPointer_UTF32
*/
class CharPointer_ASCII
{
public:
	typedef char CharType;

	inline explicit CharPointer_ASCII (const CharType* const rawPointer) noexcept
		: data (const_cast <CharType*> (rawPointer))
	{
	}

	inline CharPointer_ASCII (const CharPointer_ASCII& other) noexcept
		: data (other.data)
	{
	}

	inline CharPointer_ASCII& operator= (const CharPointer_ASCII& other) noexcept
	{
		data = other.data;
		return *this;
	}

	inline CharPointer_ASCII& operator= (const CharType* text) noexcept
	{
		data = const_cast <CharType*> (text);
		return *this;
	}

	/** This is a pointer comparison, it doesn't compare the actual text. */
	inline bool operator== (const CharPointer_ASCII& other) const noexcept { return data == other.data; }
	inline bool operator!= (const CharPointer_ASCII& other) const noexcept { return data != other.data; }
	inline bool operator<= (const CharPointer_ASCII& other) const noexcept { return data <= other.data; }
	inline bool operator<  (const CharPointer_ASCII& other) const noexcept { return data <  other.data; }
	inline bool operator>= (const CharPointer_ASCII& other) const noexcept { return data >= other.data; }
	inline bool operator>  (const CharPointer_ASCII& other) const noexcept { return data >  other.data; }

	/** Returns the address that this pointer is pointing to. */
	inline CharType* getAddress() const noexcept        { return data; }

	/** Returns the address that this pointer is pointing to. */
	inline operator const CharType*() const noexcept    { return data; }

	/** Returns true if this pointer is pointing to a null character. */
	inline bool isEmpty() const noexcept                { return *data == 0; }

	/** Returns the unicode character that this pointer is pointing to. */
	inline juce_wchar operator*() const noexcept        { return (juce_wchar) (uint8) *data; }

	/** Moves this pointer along to the next character in the string. */
	inline CharPointer_ASCII& operator++() noexcept
	{
		++data;
		return *this;
	}

	/** Moves this pointer to the previous character in the string. */
	inline CharPointer_ASCII& operator--() noexcept
	{
		--data;
		return *this;
	}

	/** Returns the character that this pointer is currently pointing to, and then
		advances the pointer to point to the next character. */
	inline juce_wchar getAndAdvance() noexcept  { return (juce_wchar) (uint8) *data++; }

	/** Moves this pointer along to the next character in the string. */
	CharPointer_ASCII operator++ (int) noexcept
	{
		CharPointer_ASCII temp (*this);
		++data;
		return temp;
	}

	/** Moves this pointer forwards by the specified number of characters. */
	inline void operator+= (const int numToSkip) noexcept
	{
		data += numToSkip;
	}

	inline void operator-= (const int numToSkip) noexcept
	{
		data -= numToSkip;
	}

	/** Returns the character at a given character index from the start of the string. */
	inline juce_wchar operator[] (const int characterIndex) const noexcept
	{
		return (juce_wchar) (unsigned char) data [characterIndex];
	}

	/** Returns a pointer which is moved forwards from this one by the specified number of characters. */
	CharPointer_ASCII operator+ (const int numToSkip) const noexcept
	{
		return CharPointer_ASCII (data + numToSkip);
	}

	/** Returns a pointer which is moved backwards from this one by the specified number of characters. */
	CharPointer_ASCII operator- (const int numToSkip) const noexcept
	{
		return CharPointer_ASCII (data - numToSkip);
	}

	/** Writes a unicode character to this string, and advances this pointer to point to the next position. */
	inline void write (const juce_wchar charToWrite) noexcept
	{
		*data++ = (char) charToWrite;
	}

	inline void replaceChar (const juce_wchar newChar) noexcept
	{
		*data = (char) newChar;
	}

	/** Writes a null character to this string (leaving the pointer's position unchanged). */
	inline void writeNull() const noexcept
	{
		*data = 0;
	}

	/** Returns the number of characters in this string. */
	size_t length() const noexcept
	{
		return (size_t) strlen (data);
	}

	/** Returns the number of characters in this string, or the given value, whichever is lower. */
	size_t lengthUpTo (const size_t maxCharsToCount) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, maxCharsToCount);
	}

	/** Returns the number of characters in this string, or up to the given end pointer, whichever is lower. */
	size_t lengthUpTo (const CharPointer_ASCII& end) const noexcept
	{
		return CharacterFunctions::lengthUpTo (*this, end);
	}

	/** Returns the number of bytes that are used to represent this string.
		This includes the terminating null character.
	*/
	size_t sizeInBytes() const noexcept
	{
		return length() + 1;
	}

	/** Returns the number of bytes that would be needed to represent the given
		unicode character in this encoding format.
	*/
	static inline size_t getBytesRequiredFor (const juce_wchar) noexcept
	{
		return 1;
	}

	/** Returns the number of bytes that would be needed to represent the given
		string in this encoding format.
		The value returned does NOT include the terminating null character.
	*/
	template <class CharPointer>
	static size_t getBytesRequiredFor (const CharPointer& text) noexcept
	{
		return text.length();
	}

	/** Returns a pointer to the null character that terminates this string. */
	CharPointer_ASCII findTerminatingNull() const noexcept
	{
		return CharPointer_ASCII (data + length());
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	template <typename CharPointer>
	void writeAll (const CharPointer& src) noexcept
	{
		CharacterFunctions::copyAll (*this, src);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes. */
	void writeAll (const CharPointer_ASCII& src) noexcept
	{
		strcpy (data, src.data);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxDestBytes parameter specifies the maximum number of bytes that can be written
		to the destination buffer before stopping.
	*/
	template <typename CharPointer>
	int writeWithDestByteLimit (const CharPointer& src, const int maxDestBytes) noexcept
	{
		return CharacterFunctions::copyWithDestByteLimit (*this, src, maxDestBytes);
	}

	/** Copies a source string to this pointer, advancing this pointer as it goes.
		The maxChars parameter specifies the maximum number of characters that can be
		written to the destination buffer before stopping (including the terminating null).
	*/
	template <typename CharPointer>
	void writeWithCharLimit (const CharPointer& src, const int maxChars) noexcept
	{
		CharacterFunctions::copyWithCharLimit (*this, src, maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compare (const CharPointer& other) const noexcept
	{
		return CharacterFunctions::compare (*this, other);
	}

	/** Compares this string with another one. */
	int compare (const CharPointer_ASCII& other) const noexcept
	{
		return strcmp (data, other.data);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareUpTo (*this, other, maxChars);
	}

	/** Compares this string with another one, up to a specified number of characters. */
	int compareUpTo (const CharPointer_ASCII& other, const int maxChars) const noexcept
	{
		return strncmp (data, other.data, (size_t) maxChars);
	}

	/** Compares this string with another one. */
	template <typename CharPointer>
	int compareIgnoreCase (const CharPointer& other) const
	{
		return CharacterFunctions::compareIgnoreCase (*this, other);
	}

	int compareIgnoreCase (const CharPointer_ASCII& other) const
	{
	   #if JUCE_WINDOWS
		return stricmp (data, other.data);
	   #else
		return strcasecmp (data, other.data);
	   #endif
	}

	/** Compares this string with another one, up to a specified number of characters. */
	template <typename CharPointer>
	int compareIgnoreCaseUpTo (const CharPointer& other, const int maxChars) const noexcept
	{
		return CharacterFunctions::compareIgnoreCaseUpTo (*this, other, maxChars);
	}

	/** Returns the character index of a substring, or -1 if it isn't found. */
	template <typename CharPointer>
	int indexOf (const CharPointer& stringToFind) const noexcept
	{
		return CharacterFunctions::indexOf (*this, stringToFind);
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind) const noexcept
	{
		int i = 0;

		while (data[i] != 0)
		{
			if (data[i] == (char) charToFind)
				return i;

			++i;
		}

		return -1;
	}

	/** Returns the character index of a unicode character, or -1 if it isn't found. */
	int indexOf (const juce_wchar charToFind, const bool ignoreCase) const noexcept
	{
		return ignoreCase ? CharacterFunctions::indexOfCharIgnoreCase (*this, charToFind)
						  : CharacterFunctions::indexOfChar (*this, charToFind);
	}

	/** Returns true if the first character of this string is whitespace. */
	bool isWhitespace() const               { return CharacterFunctions::isWhitespace (*data) != 0; }
	/** Returns true if the first character of this string is a digit. */
	bool isDigit() const                    { return CharacterFunctions::isDigit (*data) != 0; }
	/** Returns true if the first character of this string is a letter. */
	bool isLetter() const                   { return CharacterFunctions::isLetter (*data) != 0; }
	/** Returns true if the first character of this string is a letter or digit. */
	bool isLetterOrDigit() const            { return CharacterFunctions::isLetterOrDigit (*data) != 0; }
	/** Returns true if the first character of this string is upper-case. */
	bool isUpperCase() const                { return CharacterFunctions::isUpperCase ((juce_wchar) (uint8) *data) != 0; }
	/** Returns true if the first character of this string is lower-case. */
	bool isLowerCase() const                { return CharacterFunctions::isLowerCase ((juce_wchar) (uint8) *data) != 0; }

	/** Returns an upper-case version of the first character of this string. */
	juce_wchar toUpperCase() const noexcept { return CharacterFunctions::toUpperCase ((juce_wchar) (uint8) *data); }
	/** Returns a lower-case version of the first character of this string. */
	juce_wchar toLowerCase() const noexcept { return CharacterFunctions::toLowerCase ((juce_wchar) (uint8) *data); }

	/** Parses this string as a 32-bit integer. */
	int getIntValue32() const noexcept      { return atoi (data); }

	/** Parses this string as a 64-bit integer. */
	int64 getIntValue64() const noexcept
	{
	   #if JUCE_LINUX || JUCE_ANDROID
		return atoll (data);
	   #elif JUCE_WINDOWS
		return _atoi64 (data);
	   #else
		return CharacterFunctions::getIntValue <int64, CharPointer_ASCII> (*this);
	   #endif
	}

	/** Parses this string as a floating point double. */
	double getDoubleValue() const noexcept  { return CharacterFunctions::getDoubleValue (*this); }

	/** Returns the first non-whitespace character in the string. */
	CharPointer_ASCII findEndOfWhitespace() const noexcept   { return CharacterFunctions::findEndOfWhitespace (*this); }

	/** Returns true if the given unicode character can be represented in this encoding. */
	static bool canRepresent (juce_wchar character) noexcept
	{
		return ((unsigned int) character) < (unsigned int) 128;
	}

	/** Returns true if this data contains a valid string in this encoding. */
	static bool isValidString (const CharType* dataToTest, int maxBytesToRead)
	{
		while (--maxBytesToRead >= 0)
		{
			if (((signed char) *dataToTest) <= 0)
				return *dataToTest == 0;

			++dataToTest;
		}

		return true;
	}

private:
	CharType* data;
};

#endif   // __JUCE_CHARPOINTER_ASCII_JUCEHEADER__

/*** End of inlined file: juce_CharPointer_ASCII.h ***/

#if JUCE_MSVC
 #pragma warning (pop)
#endif

class OutputStream;

/**
	The JUCE String class!

	Using a reference-counted internal representation, these strings are fast
	and efficient, and there are methods to do just about any operation you'll ever
	dream of.

	@see StringArray, StringPairArray
*/
class JUCE_API  String
{
public:

	/** Creates an empty string.
		@see empty
	*/
	String() noexcept;

	/** Creates a copy of another string. */
	String (const String& other) noexcept;

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	String (String&& other) noexcept;
   #endif

	/** Creates a string from a zero-terminated ascii text string.

		The string passed-in must not contain any characters with a value above 127, because
		these can't be converted to unicode without knowing the original encoding that was
		used to create the string. If you attempt to pass-in values above 127, you'll get an
		assertion.

		To create strings with extended characters from UTF-8, you should explicitly call
		String (CharPointer_UTF8 ("my utf8 string..")). It's *highly* recommended that you
		use UTF-8 with escape characters in your source code to represent extended characters,
		because there's no other way to represent unicode strings in a way that isn't dependent
		on the compiler, source code editor and platform.
	*/
	String (const char* text);

	/** Creates a string from a string of 8-bit ascii characters.

		The string passed-in must not contain any characters with a value above 127, because
		these can't be converted to unicode without knowing the original encoding that was
		used to create the string. If you attempt to pass-in values above 127, you'll get an
		assertion.

		To create strings with extended characters from UTF-8, you should explicitly call
		String (CharPointer_UTF8 ("my utf8 string..")). It's *highly* recommended that you
		use UTF-8 with escape characters in your source code to represent extended characters,
		because there's no other way to represent unicode strings in a way that isn't dependent
		on the compiler, source code editor and platform.

		This will use up the the first maxChars characters of the string (or less if the string
		is actually shorter).
	*/
	String (const char* text, size_t maxChars);

	/** Creates a string from a whcar_t character string.
		Depending on the platform, this may be treated as either UTF-32 or UTF-16.
	*/
	String (const wchar_t* text);

	/** Creates a string from a whcar_t character string.
		Depending on the platform, this may be treated as either UTF-32 or UTF-16.
	*/
	String (const wchar_t* text, size_t maxChars);

	/** Creates a string from a UTF-8 character string */
	String (const CharPointer_UTF8& text);

	/** Creates a string from a UTF-8 character string */
	String (const CharPointer_UTF8& text, size_t maxChars);

	/** Creates a string from a UTF-8 character string */
	String (const CharPointer_UTF8& start, const CharPointer_UTF8& end);

	/** Creates a string from a UTF-16 character string */
	String (const CharPointer_UTF16& text);

	/** Creates a string from a UTF-16 character string */
	String (const CharPointer_UTF16& text, size_t maxChars);

	/** Creates a string from a UTF-16 character string */
	String (const CharPointer_UTF16& start, const CharPointer_UTF16& end);

	/** Creates a string from a UTF-32 character string */
	String (const CharPointer_UTF32& text);

	/** Creates a string from a UTF-32 character string */
	String (const CharPointer_UTF32& text, size_t maxChars);

	/** Creates a string from a UTF-32 character string */
	String (const CharPointer_UTF32& start, const CharPointer_UTF32& end);

	/** Creates a string from an ASCII character string */
	String (const CharPointer_ASCII& text);

	/** Creates a string from a single character. */
	static String charToString (juce_wchar character);

	/** Destructor. */
	~String() noexcept;

	/** This is an empty string that can be used whenever one is needed.

		It's better to use this than String() because it explains what's going on
		and is more efficient.
	*/
	static const String empty;

	/** This is the character encoding type used internally to store the string.

		By setting the value of JUCE_STRING_UTF_TYPE to 8, 16, or 32, you can change the
		internal storage format of the String class. UTF-8 uses the least space (if your strings
		contain few extended characters), but call operator[] involves iterating the string to find
		the required index. UTF-32 provides instant random access to its characters, but uses 4 bytes
		per character to store them. UTF-16 uses more space than UTF-8 and is also slow to index,
		but is the native wchar_t format used in Windows.

		It doesn't matter too much which format you pick, because the toUTF8(), toUTF16() and
		toUTF32() methods let you access the string's content in any of the other formats.
	*/
   #if (JUCE_STRING_UTF_TYPE == 32)
	typedef CharPointer_UTF32 CharPointerType;
   #elif (JUCE_STRING_UTF_TYPE == 16)
	typedef CharPointer_UTF16 CharPointerType;
   #elif (JUCE_STRING_UTF_TYPE == 8)
	typedef CharPointer_UTF8  CharPointerType;
   #else
	#error "You must set the value of JUCE_STRING_UTF_TYPE to be either 8, 16, or 32!"
   #endif

	/** Generates a probably-unique 32-bit hashcode from this string. */
	int hashCode() const noexcept;

	/** Generates a probably-unique 64-bit hashcode from this string. */
	int64 hashCode64() const noexcept;

	/** Returns the number of characters in the string. */
	int length() const noexcept;

	// Assignment and concatenation operators..

	/** Replaces this string's contents with another string. */
	String& operator= (const String& other) noexcept;

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	String& operator= (String&& other) noexcept;
   #endif

	/** Appends another string at the end of this one. */
	String& operator+= (const String& stringToAppend);
	/** Appends another string at the end of this one. */
	String& operator+= (const char* textToAppend);
	/** Appends another string at the end of this one. */
	String& operator+= (const wchar_t* textToAppend);
	/** Appends a decimal number at the end of this string. */
	String& operator+= (int numberToAppend);
	/** Appends a character at the end of this string. */
	String& operator+= (char characterToAppend);
	/** Appends a character at the end of this string. */
	String& operator+= (wchar_t characterToAppend);
   #if ! JUCE_NATIVE_WCHAR_IS_UTF32
	/** Appends a character at the end of this string. */
	String& operator+= (juce_wchar characterToAppend);
   #endif

	/** Appends a string to the end of this one.

		@param textToAppend     the string to add
		@param maxCharsToTake   the maximum number of characters to take from the string passed in
	*/
	void append (const String& textToAppend, size_t maxCharsToTake);

	/** Appends a string to the end of this one.

		@param textToAppend     the string to add
		@param maxCharsToTake   the maximum number of characters to take from the string passed in
	*/
	template <class CharPointer>
	void appendCharPointer (const CharPointer& textToAppend, size_t maxCharsToTake)
	{
		if (textToAppend.getAddress() != nullptr)
		{
			size_t extraBytesNeeded = 0;
			size_t numChars = 0;

			for (CharPointer t (textToAppend); numChars < maxCharsToTake && ! t.isEmpty();)
			{
				extraBytesNeeded += CharPointerType::getBytesRequiredFor (t.getAndAdvance());
				++numChars;
			}

			if (numChars > 0)
			{
				const size_t byteOffsetOfNull = getByteOffsetOfEnd();

				preallocateBytes (byteOffsetOfNull + extraBytesNeeded);
				CharPointerType (addBytesToPointer (text.getAddress(), (int) byteOffsetOfNull)).writeWithCharLimit (textToAppend, (int) (numChars + 1));
			}
		}
	}

	/** Appends a string to the end of this one. */
	template <class CharPointer>
	void appendCharPointer (const CharPointer& textToAppend)
	{
		if (textToAppend.getAddress() != nullptr)
		{
			size_t extraBytesNeeded = 0;

			for (CharPointer t (textToAppend); ! t.isEmpty();)
				extraBytesNeeded += CharPointerType::getBytesRequiredFor (t.getAndAdvance());

			if (extraBytesNeeded > 0)
			{
				const size_t byteOffsetOfNull = getByteOffsetOfEnd();

				preallocateBytes (byteOffsetOfNull + extraBytesNeeded);
				CharPointerType (addBytesToPointer (text.getAddress(), (int) byteOffsetOfNull)).writeAll (textToAppend);
			}
		}
	}

	// Comparison methods..

	/** Returns true if the string contains no characters.
		Note that there's also an isNotEmpty() method to help write readable code.
		@see containsNonWhitespaceChars()
	*/
	inline bool isEmpty() const noexcept                    { return text[0] == 0; }

	/** Returns true if the string contains at least one character.
		Note that there's also an isEmpty() method to help write readable code.
		@see containsNonWhitespaceChars()
	*/
	inline bool isNotEmpty() const noexcept                 { return text[0] != 0; }

	/** Case-insensitive comparison with another string. */
	bool equalsIgnoreCase (const String& other) const noexcept;

	/** Case-insensitive comparison with another string. */
	bool equalsIgnoreCase (const wchar_t* other) const noexcept;

	/** Case-insensitive comparison with another string. */
	bool equalsIgnoreCase (const char* other) const noexcept;

	/** Case-sensitive comparison with another string.
		@returns     0 if the two strings are identical; negative if this string comes before
					 the other one alphabetically, or positive if it comes after it.
	*/
	int compare (const String& other) const noexcept;

	/** Case-sensitive comparison with another string.
		@returns     0 if the two strings are identical; negative if this string comes before
					 the other one alphabetically, or positive if it comes after it.
	*/
	int compare (const char* other) const noexcept;

	/** Case-sensitive comparison with another string.
		@returns     0 if the two strings are identical; negative if this string comes before
					 the other one alphabetically, or positive if it comes after it.
	*/
	int compare (const wchar_t* other) const noexcept;

	/** Case-insensitive comparison with another string.
		@returns     0 if the two strings are identical; negative if this string comes before
					 the other one alphabetically, or positive if it comes after it.
	*/
	int compareIgnoreCase (const String& other) const noexcept;

	/** Lexicographic comparison with another string.

		The comparison used here is case-insensitive and ignores leading non-alphanumeric
		characters, making it good for sorting human-readable strings.

		@returns     0 if the two strings are identical; negative if this string comes before
					 the other one alphabetically, or positive if it comes after it.
	*/
	int compareLexicographically (const String& other) const noexcept;

	/** Tests whether the string begins with another string.
		If the parameter is an empty string, this will always return true.
		Uses a case-sensitive comparison.
	*/
	bool startsWith (const String& text) const noexcept;

	/** Tests whether the string begins with a particular character.
		If the character is 0, this will always return false.
		Uses a case-sensitive comparison.
	*/
	bool startsWithChar (juce_wchar character) const noexcept;

	/** Tests whether the string begins with another string.
		If the parameter is an empty string, this will always return true.
		Uses a case-insensitive comparison.
	*/
	bool startsWithIgnoreCase (const String& text) const noexcept;

	/** Tests whether the string ends with another string.
		If the parameter is an empty string, this will always return true.
		Uses a case-sensitive comparison.
	*/
	bool endsWith (const String& text) const noexcept;

	/** Tests whether the string ends with a particular character.
		If the character is 0, this will always return false.
		Uses a case-sensitive comparison.
	*/
	bool endsWithChar (juce_wchar character) const noexcept;

	/** Tests whether the string ends with another string.
		If the parameter is an empty string, this will always return true.
		Uses a case-insensitive comparison.
	*/
	bool endsWithIgnoreCase (const String& text) const noexcept;

	/** Tests whether the string contains another substring.
		If the parameter is an empty string, this will always return true.
		Uses a case-sensitive comparison.
	*/
	bool contains (const String& text) const noexcept;

	/** Tests whether the string contains a particular character.
		Uses a case-sensitive comparison.
	*/
	bool containsChar (juce_wchar character) const noexcept;

	/** Tests whether the string contains another substring.
		Uses a case-insensitive comparison.
	*/
	bool containsIgnoreCase (const String& text) const noexcept;

	/** Tests whether the string contains another substring as a distict word.

		@returns    true if the string contains this word, surrounded by
					non-alphanumeric characters
		@see indexOfWholeWord, containsWholeWordIgnoreCase
	*/
	bool containsWholeWord (const String& wordToLookFor) const noexcept;

	/** Tests whether the string contains another substring as a distict word.

		@returns    true if the string contains this word, surrounded by
					non-alphanumeric characters
		@see indexOfWholeWordIgnoreCase, containsWholeWord
	*/
	bool containsWholeWordIgnoreCase (const String& wordToLookFor) const noexcept;

	/** Finds an instance of another substring if it exists as a distict word.

		@returns    if the string contains this word, surrounded by non-alphanumeric characters,
					then this will return the index of the start of the substring. If it isn't
					found, then it will return -1
		@see indexOfWholeWordIgnoreCase, containsWholeWord
	*/
	int indexOfWholeWord (const String& wordToLookFor) const noexcept;

	/** Finds an instance of another substring if it exists as a distict word.

		@returns    if the string contains this word, surrounded by non-alphanumeric characters,
					then this will return the index of the start of the substring. If it isn't
					found, then it will return -1
		@see indexOfWholeWord, containsWholeWordIgnoreCase
	*/
	int indexOfWholeWordIgnoreCase (const String& wordToLookFor) const noexcept;

	/** Looks for any of a set of characters in the string.
		Uses a case-sensitive comparison.

		@returns    true if the string contains any of the characters from
					the string that is passed in.
	*/
	bool containsAnyOf (const String& charactersItMightContain) const noexcept;

	/** Looks for a set of characters in the string.
		Uses a case-sensitive comparison.

		@returns    Returns false if any of the characters in this string do not occur in
					the parameter string. If this string is empty, the return value will
					always be true.
	*/
	bool containsOnly (const String& charactersItMightContain) const noexcept;

	/** Returns true if this string contains any non-whitespace characters.

		This will return false if the string contains only whitespace characters, or
		if it's empty.

		It is equivalent to calling "myString.trim().isNotEmpty()".
	*/
	bool containsNonWhitespaceChars() const noexcept;

	/** Returns true if the string matches this simple wildcard expression.

		So for example String ("abcdef").matchesWildcard ("*DEF", true) would return true.

		This isn't a full-blown regex though! The only wildcard characters supported
		are "*" and "?". It's mainly intended for filename pattern matching.
	*/
	bool matchesWildcard (const String& wildcard, bool ignoreCase) const noexcept;

	// Substring location methods..

	/** Searches for a character inside this string.
		Uses a case-sensitive comparison.
		@returns    the index of the first occurrence of the character in this
					string, or -1 if it's not found.
	*/
	int indexOfChar (juce_wchar characterToLookFor) const noexcept;

	/** Searches for a character inside this string.
		Uses a case-sensitive comparison.
		@param startIndex           the index from which the search should proceed
		@param characterToLookFor   the character to look for
		@returns            the index of the first occurrence of the character in this
							string, or -1 if it's not found.
	*/
	int indexOfChar (int startIndex, juce_wchar characterToLookFor) const noexcept;

	/** Returns the index of the first character that matches one of the characters
		passed-in to this method.

		This scans the string, beginning from the startIndex supplied, and if it finds
		a character that appears in the string charactersToLookFor, it returns its index.

		If none of these characters are found, it returns -1.

		If ignoreCase is true, the comparison will be case-insensitive.

		@see indexOfChar, lastIndexOfAnyOf
	*/
	int indexOfAnyOf (const String& charactersToLookFor,
					  int startIndex = 0,
					  bool ignoreCase = false) const noexcept;

	/** Searches for a substring within this string.
		Uses a case-sensitive comparison.
		@returns    the index of the first occurrence of this substring, or -1 if it's not found.
					If textToLookFor is an empty string, this will always return 0.
	*/
	int indexOf (const String& textToLookFor) const noexcept;

	/** Searches for a substring within this string.
		Uses a case-sensitive comparison.
		@param startIndex       the index from which the search should proceed
		@param textToLookFor    the string to search for
		@returns                the index of the first occurrence of this substring, or -1 if it's not found.
								If textToLookFor is an empty string, this will always return -1.
	*/
	int indexOf (int startIndex, const String& textToLookFor) const noexcept;

	/** Searches for a substring within this string.
		Uses a case-insensitive comparison.
		@returns    the index of the first occurrence of this substring, or -1 if it's not found.
					If textToLookFor is an empty string, this will always return 0.
	*/
	int indexOfIgnoreCase (const String& textToLookFor) const noexcept;

	/** Searches for a substring within this string.
		Uses a case-insensitive comparison.
		@param startIndex       the index from which the search should proceed
		@param textToLookFor    the string to search for
		@returns                the index of the first occurrence of this substring, or -1 if it's not found.
								If textToLookFor is an empty string, this will always return -1.
	*/
	int indexOfIgnoreCase (int startIndex, const String& textToLookFor) const noexcept;

	/** Searches for a character inside this string (working backwards from the end of the string).
		Uses a case-sensitive comparison.
		@returns    the index of the last occurrence of the character in this string, or -1 if it's not found.
	*/
	int lastIndexOfChar (juce_wchar character) const noexcept;

	/** Searches for a substring inside this string (working backwards from the end of the string).
		Uses a case-sensitive comparison.
		@returns    the index of the start of the last occurrence of the substring within this string,
					or -1 if it's not found. If textToLookFor is an empty string, this will always return -1.
	*/
	int lastIndexOf (const String& textToLookFor) const noexcept;

	/** Searches for a substring inside this string (working backwards from the end of the string).
		Uses a case-insensitive comparison.
		@returns    the index of the start of the last occurrence of the substring within this string, or -1
					if it's not found. If textToLookFor is an empty string, this will always return -1.
	*/
	int lastIndexOfIgnoreCase (const String& textToLookFor) const noexcept;

	/** Returns the index of the last character in this string that matches one of the
		characters passed-in to this method.

		This scans the string backwards, starting from its end, and if it finds
		a character that appears in the string charactersToLookFor, it returns its index.

		If none of these characters are found, it returns -1.

		If ignoreCase is true, the comparison will be case-insensitive.

		@see lastIndexOf, indexOfAnyOf
	*/
	int lastIndexOfAnyOf (const String& charactersToLookFor,
						  bool ignoreCase = false) const noexcept;

	// Substring extraction and manipulation methods..

	/** Returns the character at this index in the string.
		In a release build, no checks are made to see if the index is within a valid range, so be
		careful! In a debug build, the index is checked and an assertion fires if it's out-of-range.

		Also beware that depending on the encoding format that the string is using internally, this
		method may execute in either O(1) or O(n) time, so be careful when using it in your algorithms.
		If you're scanning through a string to inspect its characters, you should never use this operator
		for random access, it's far more efficient to call getCharPointer() to return a pointer, and
		then to use that to iterate the string.
		@see getCharPointer
	*/
	const juce_wchar operator[] (int index) const noexcept;

	/** Returns the final character of the string.
		If the string is empty this will return 0.
	*/
	juce_wchar getLastCharacter() const noexcept;

	/** Returns a subsection of the string.

		If the range specified is beyond the limits of the string, as much as
		possible is returned.

		@param startIndex   the index of the start of the substring needed
		@param endIndex     all characters from startIndex up to (but not including)
							this index are returned
		@see fromFirstOccurrenceOf, dropLastCharacters, getLastCharacters, upToFirstOccurrenceOf
	*/
	String substring (int startIndex, int endIndex) const;

	/** Returns a section of the string, starting from a given position.

		@param startIndex   the first character to include. If this is beyond the end
							of the string, an empty string is returned. If it is zero or
							less, the whole string is returned.
		@returns            the substring from startIndex up to the end of the string
		@see dropLastCharacters, getLastCharacters, fromFirstOccurrenceOf, upToFirstOccurrenceOf, fromLastOccurrenceOf
	*/
	String substring (int startIndex) const;

	/** Returns a version of this string with a number of characters removed
		from the end.

		@param numberToDrop     the number of characters to drop from the end of the
								string. If this is greater than the length of the string,
								an empty string will be returned. If zero or less, the
								original string will be returned.
		@see substring, fromFirstOccurrenceOf, upToFirstOccurrenceOf, fromLastOccurrenceOf, getLastCharacter
	*/
	String dropLastCharacters (int numberToDrop) const;

	/** Returns a number of characters from the end of the string.

		This returns the last numCharacters characters from the end of the string. If the
		string is shorter than numCharacters, the whole string is returned.

		@see substring, dropLastCharacters, getLastCharacter
	*/
	String getLastCharacters (int numCharacters) const;

	/** Returns a section of the string starting from a given substring.

		This will search for the first occurrence of the given substring, and
		return the section of the string starting from the point where this is
		found (optionally not including the substring itself).

		e.g. for the string "123456", fromFirstOccurrenceOf ("34", true) would return "3456", and
									  fromFirstOccurrenceOf ("34", false) would return "56".

		If the substring isn't found, the method will return an empty string.

		If ignoreCase is true, the comparison will be case-insensitive.

		@see upToFirstOccurrenceOf, fromLastOccurrenceOf
	*/
	String fromFirstOccurrenceOf (const String& substringToStartFrom,
								  bool includeSubStringInResult,
										bool ignoreCase) const;

	/** Returns a section of the string starting from the last occurrence of a given substring.

		Similar to fromFirstOccurrenceOf(), but using the last occurrence of the substring, and
		unlike fromFirstOccurrenceOf(), if the substring isn't found, this method will
		return the whole of the original string.

		@see fromFirstOccurrenceOf, upToLastOccurrenceOf
	*/
	String fromLastOccurrenceOf (const String& substringToFind,
								 bool includeSubStringInResult,
								 bool ignoreCase) const;

	/** Returns the start of this string, up to the first occurrence of a substring.

		This will search for the first occurrence of a given substring, and then
		return a copy of the string, up to the position of this substring,
		optionally including or excluding the substring itself in the result.

		e.g. for the string "123456", upTo ("34", false) would return "12", and
									  upTo ("34", true) would return "1234".

		If the substring isn't found, this will return the whole of the original string.

		@see upToLastOccurrenceOf, fromFirstOccurrenceOf
	*/
	String upToFirstOccurrenceOf (const String& substringToEndWith,
								  bool includeSubStringInResult,
								  bool ignoreCase) const;

	/** Returns the start of this string, up to the last occurrence of a substring.

		Similar to upToFirstOccurrenceOf(), but this finds the last occurrence rather than the first.
		If the substring isn't found, this will return the whole of the original string.

		@see upToFirstOccurrenceOf, fromFirstOccurrenceOf
	*/
	String upToLastOccurrenceOf (const String& substringToFind,
								 bool includeSubStringInResult,
								 bool ignoreCase) const;

	/** Returns a copy of this string with any whitespace characters removed from the start and end. */
	String trim() const;

	/** Returns a copy of this string with any whitespace characters removed from the start. */
	String trimStart() const;

	/** Returns a copy of this string with any whitespace characters removed from the end. */
	String trimEnd() const;

	/** Returns a copy of this string, having removed a specified set of characters from its start.
		Characters are removed from the start of the string until it finds one that is not in the
		specified set, and then it stops.
		@param charactersToTrim     the set of characters to remove.
		@see trim, trimStart, trimCharactersAtEnd
	*/
	String trimCharactersAtStart (const String& charactersToTrim) const;

	/** Returns a copy of this string, having removed a specified set of characters from its end.
		Characters are removed from the end of the string until it finds one that is not in the
		specified set, and then it stops.
		@param charactersToTrim     the set of characters to remove.
		@see trim, trimEnd, trimCharactersAtStart
	*/
	String trimCharactersAtEnd (const String& charactersToTrim) const;

	/** Returns an upper-case version of this string. */
	String toUpperCase() const;

	/** Returns an lower-case version of this string. */
	String toLowerCase() const;

	/** Replaces a sub-section of the string with another string.

		This will return a copy of this string, with a set of characters
		from startIndex to startIndex + numCharsToReplace removed, and with
		a new string inserted in their place.

		Note that this is a const method, and won't alter the string itself.

		@param startIndex               the first character to remove. If this is beyond the bounds of the string,
										it will be constrained to a valid range.
		@param numCharactersToReplace   the number of characters to remove. If zero or less, no
										characters will be taken out.
		@param stringToInsert           the new string to insert at startIndex after the characters have been
										removed.
	*/
	String replaceSection (int startIndex,
						   int numCharactersToReplace,
						   const String& stringToInsert) const;

	/** Replaces all occurrences of a substring with another string.

		Returns a copy of this string, with any occurrences of stringToReplace
		swapped for stringToInsertInstead.

		Note that this is a const method, and won't alter the string itself.
	*/
	String replace (const String& stringToReplace,
					const String& stringToInsertInstead,
					bool ignoreCase = false) const;

	/** Returns a string with all occurrences of a character replaced with a different one. */
	String replaceCharacter (juce_wchar characterToReplace,
							 juce_wchar characterToInsertInstead) const;

	/** Replaces a set of characters with another set.

		Returns a string in which each character from charactersToReplace has been replaced
		by the character at the equivalent position in newCharacters (so the two strings
		passed in must be the same length).

		e.g. replaceCharacters ("abc", "def") replaces 'a' with 'd', 'b' with 'e', etc.

		Note that this is a const method, and won't affect the string itself.
	*/
	String replaceCharacters (const String& charactersToReplace,
							  const String& charactersToInsertInstead) const;

	/** Returns a version of this string that only retains a fixed set of characters.

		This will return a copy of this string, omitting any characters which are not
		found in the string passed-in.

		e.g. for "1122334455", retainCharacters ("432") would return "223344"

		Note that this is a const method, and won't alter the string itself.
	*/
	String retainCharacters (const String& charactersToRetain) const;

	/** Returns a version of this string with a set of characters removed.

		This will return a copy of this string, omitting any characters which are
		found in the string passed-in.

		e.g. for "1122334455", removeCharacters ("432") would return "1155"

		Note that this is a const method, and won't alter the string itself.
	*/
	String removeCharacters (const String& charactersToRemove) const;

	/** Returns a section from the start of the string that only contains a certain set of characters.

		This returns the leftmost section of the string, up to (and not including) the
		first character that doesn't appear in the string passed in.
	*/
	String initialSectionContainingOnly (const String& permittedCharacters) const;

	/** Returns a section from the start of the string that only contains a certain set of characters.

		This returns the leftmost section of the string, up to (and not including) the
		first character that occurs in the string passed in. (If none of the specified
		characters are found in the string, the return value will just be the original string).
	*/
	String initialSectionNotContaining (const String& charactersToStopAt) const;

	/** Checks whether the string might be in quotation marks.

		@returns    true if the string begins with a quote character (either a double or single quote).
					It is also true if there is whitespace before the quote, but it doesn't check the end of the string.
		@see unquoted, quoted
	*/
	bool isQuotedString() const;

	/** Removes quotation marks from around the string, (if there are any).

		Returns a copy of this string with any quotes removed from its ends. Quotes that aren't
		at the ends of the string are not affected. If there aren't any quotes, the original string
		is returned.

		Note that this is a const method, and won't alter the string itself.

		@see isQuotedString, quoted
	*/
	String unquoted() const;

	/** Adds quotation marks around a string.

		This will return a copy of the string with a quote at the start and end, (but won't
		add the quote if there's already one there, so it's safe to call this on strings that
		may already have quotes around them).

		Note that this is a const method, and won't alter the string itself.

		@param quoteCharacter   the character to add at the start and end
		@see isQuotedString, unquoted
	*/
	String quoted (juce_wchar quoteCharacter = '"') const;

	/** Creates a string which is a version of a string repeated and joined together.

		@param stringToRepeat         the string to repeat
		@param numberOfTimesToRepeat  how many times to repeat it
	*/
	static String repeatedString (const String& stringToRepeat,
								  int numberOfTimesToRepeat);

	/** Returns a copy of this string with the specified character repeatedly added to its
		beginning until the total length is at least the minimum length specified.
	*/
	String paddedLeft (juce_wchar padCharacter, int minimumLength) const;

	/** Returns a copy of this string with the specified character repeatedly added to its
		end until the total length is at least the minimum length specified.
	*/
	String paddedRight (juce_wchar padCharacter, int minimumLength) const;

	/** Creates a string from data in an unknown format.

		This looks at some binary data and tries to guess whether it's Unicode
		or 8-bit characters, then returns a string that represents it correctly.

		Should be able to handle Unicode endianness correctly, by looking at
		the first two bytes.
	*/
	static String createStringFromData (const void* data, int size);

	/** Creates a String from a printf-style parameter list.

		I don't like this method. I don't use it myself, and I recommend avoiding it and
		using the operator<< methods or pretty much anything else instead. It's only provided
		here because of the popular unrest that was stirred-up when I tried to remove it...

		If you're really determined to use it, at least make sure that you never, ever,
		pass any String objects to it as parameters. And bear in mind that internally, depending
		on the platform, it may be using wchar_t or char character types, so that even string
		literals can't be safely used as parameters if you're writing portable code.
	*/
	static String formatted (const String formatString, ... );

	// Numeric conversions..

	/** Creates a string containing this signed 32-bit integer as a decimal number.
		@see getIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (int decimalInteger);

	/** Creates a string containing this unsigned 32-bit integer as a decimal number.
		@see getIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (unsigned int decimalInteger);

	/** Creates a string containing this signed 16-bit integer as a decimal number.
		@see getIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (short decimalInteger);

	/** Creates a string containing this unsigned 16-bit integer as a decimal number.
		@see getIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (unsigned short decimalInteger);

	/** Creates a string containing this signed 64-bit integer as a decimal number.
		@see getLargeIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (int64 largeIntegerValue);

	/** Creates a string containing this unsigned 64-bit integer as a decimal number.
		@see getLargeIntValue, getFloatValue, getDoubleValue, toHexString
	*/
	explicit String (uint64 largeIntegerValue);

	/** Creates a string representing this floating-point number.
		@param floatValue               the value to convert to a string
		@see getDoubleValue, getIntValue
	*/
	explicit String (float floatValue);

	/** Creates a string representing this floating-point number.
		@param doubleValue              the value to convert to a string
		@see getFloatValue, getIntValue
	*/
	explicit String (double doubleValue);

	/** Creates a string representing this floating-point number.
		@param floatValue               the value to convert to a string
		@param numberOfDecimalPlaces    if this is > 0, it will format the number using that many
										decimal places, and will not use exponent notation. If 0 or
										less, it will use exponent notation if necessary.
		@see getDoubleValue, getIntValue
	*/
	String (float floatValue, int numberOfDecimalPlaces);

	/** Creates a string representing this floating-point number.
		@param doubleValue              the value to convert to a string
		@param numberOfDecimalPlaces    if this is > 0, it will format the number using that many
										decimal places, and will not use exponent notation. If 0 or
										less, it will use exponent notation if necessary.
		@see getFloatValue, getIntValue
	*/
	String (double doubleValue, int numberOfDecimalPlaces);

	/** Reads the value of the string as a decimal number (up to 32 bits in size).

		@returns the value of the string as a 32 bit signed base-10 integer.
		@see getTrailingIntValue, getHexValue32, getHexValue64
	*/
	int getIntValue() const noexcept;

	/** Reads the value of the string as a decimal number (up to 64 bits in size).

		@returns the value of the string as a 64 bit signed base-10 integer.
	*/
	int64 getLargeIntValue() const noexcept;

	/** Parses a decimal number from the end of the string.

		This will look for a value at the end of the string.
		e.g. for "321 xyz654" it will return 654; for "2 3 4" it'll return 4.

		Negative numbers are not handled, so "xyz-5" returns 5.

		@see getIntValue
	*/
	int getTrailingIntValue() const noexcept;

	/** Parses this string as a floating point number.

		@returns    the value of the string as a 32-bit floating point value.
		@see getDoubleValue
	*/
	float getFloatValue() const noexcept;

	/** Parses this string as a floating point number.

		@returns    the value of the string as a 64-bit floating point value.
		@see getFloatValue
	*/
	double getDoubleValue() const noexcept;

	/** Parses the string as a hexadecimal number.

		Non-hexadecimal characters in the string are ignored.

		If the string contains too many characters, then the lowest significant
		digits are returned, e.g. "ffff12345678" would produce 0x12345678.

		@returns    a 32-bit number which is the value of the string in hex.
	*/
	int getHexValue32() const noexcept;

	/** Parses the string as a hexadecimal number.

		Non-hexadecimal characters in the string are ignored.

		If the string contains too many characters, then the lowest significant
		digits are returned, e.g. "ffff1234567812345678" would produce 0x1234567812345678.

		@returns    a 64-bit number which is the value of the string in hex.
	*/
	int64 getHexValue64() const noexcept;

	/** Creates a string representing this 32-bit value in hexadecimal. */
	static String toHexString (int number);

	/** Creates a string representing this 64-bit value in hexadecimal. */
	static String toHexString (int64 number);

	/** Creates a string representing this 16-bit value in hexadecimal. */
	static String toHexString (short number);

	/** Creates a string containing a hex dump of a block of binary data.

		@param data         the binary data to use as input
		@param size         how many bytes of data to use
		@param groupSize    how many bytes are grouped together before inserting a
							space into the output. e.g. group size 0 has no spaces,
							group size 1 looks like: "be a1 c2 ff", group size 2 looks
							like "bea1 c2ff".
	*/
	static String toHexString (const void* data, int size, int groupSize = 1);

	/** Returns the character pointer currently being used to store this string.

		Because it returns a reference to the string's internal data, the pointer
		that is returned must not be stored anywhere, as it can be deleted whenever the
		string changes.
	*/
	inline const CharPointerType& getCharPointer() const noexcept    { return text; }

	/** Returns a pointer to a UTF-8 version of this string.

		Because it returns a reference to the string's internal data, the pointer
		that is returned must not be stored anywhere, as it can be deleted whenever the
		string changes.

		To find out how many bytes you need to store this string as UTF-8, you can call
		CharPointer_UTF8::getBytesRequiredFor (myString.getCharPointer())

		@see getCharPointer, toUTF16, toUTF32
	*/
	CharPointer_UTF8 toUTF8() const;

	/** Returns a pointer to a UTF-32 version of this string.

		Because it returns a reference to the string's internal data, the pointer
		that is returned must not be stored anywhere, as it can be deleted whenever the
		string changes.

		To find out how many bytes you need to store this string as UTF-16, you can call
		CharPointer_UTF16::getBytesRequiredFor (myString.getCharPointer())

		@see getCharPointer, toUTF8, toUTF32
	*/
	CharPointer_UTF16 toUTF16() const;

	/** Returns a pointer to a UTF-32 version of this string.

		Because it returns a reference to the string's internal data, the pointer
		that is returned must not be stored anywhere, as it can be deleted whenever the
		string changes.

		@see getCharPointer, toUTF8, toUTF16
	*/
	CharPointer_UTF32 toUTF32() const;

	/** Returns a pointer to a wchar_t version of this string.

		Because it returns a reference to the string's internal data, the pointer
		that is returned must not be stored anywhere, as it can be deleted whenever the
		string changes.

		Bear in mind that the wchar_t type is different on different platforms, so on
		Windows, this will be equivalent to calling toUTF16(), on unix it'll be the same
		as calling toUTF32(), etc.

		@see getCharPointer, toUTF8, toUTF16, toUTF32
	*/
	const wchar_t* toWideCharPointer() const;

	/** Creates a String from a UTF-8 encoded buffer.
		If the size is < 0, it'll keep reading until it hits a zero.
	*/
	static String fromUTF8 (const char* utf8buffer, int bufferSizeBytes = -1);

	/** Returns the number of bytes required to represent this string as UTF8.
		The number returned does NOT include the trailing zero.
		@see toUTF8, copyToUTF8
	*/
	int getNumBytesAsUTF8() const noexcept;

	/** Copies the string to a buffer as UTF-8 characters.

		Returns the number of bytes copied to the buffer, including the terminating null
		character.

		To find out how many bytes you need to store this string as UTF-8, you can call
		CharPointer_UTF8::getBytesRequiredFor (myString.getCharPointer())

		@param destBuffer       the place to copy it to; if this is a null pointer, the method just
								returns the number of bytes required (including the terminating null character).
		@param maxBufferSizeBytes  the size of the destination buffer, in bytes. If the string won't fit, it'll
								put in as many as it can while still allowing for a terminating null char at the
								end, and will return the number of bytes that were actually used.
		@see CharPointer_UTF8::writeWithDestByteLimit
	*/
	int copyToUTF8 (CharPointer_UTF8::CharType* destBuffer, int maxBufferSizeBytes) const noexcept;

	/** Copies the string to a buffer as UTF-16 characters.

		Returns the number of bytes copied to the buffer, including the terminating null
		character.

		To find out how many bytes you need to store this string as UTF-16, you can call
		CharPointer_UTF16::getBytesRequiredFor (myString.getCharPointer())

		@param destBuffer       the place to copy it to; if this is a null pointer, the method just
								returns the number of bytes required (including the terminating null character).
		@param maxBufferSizeBytes  the size of the destination buffer, in bytes. If the string won't fit, it'll
								put in as many as it can while still allowing for a terminating null char at the
								end, and will return the number of bytes that were actually used.
		@see CharPointer_UTF16::writeWithDestByteLimit
	*/
	int copyToUTF16 (CharPointer_UTF16::CharType* destBuffer, int maxBufferSizeBytes) const noexcept;

	/** Copies the string to a buffer as UTF-16 characters.

		Returns the number of bytes copied to the buffer, including the terminating null
		character.

		To find out how many bytes you need to store this string as UTF-32, you can call
		CharPointer_UTF32::getBytesRequiredFor (myString.getCharPointer())

		@param destBuffer       the place to copy it to; if this is a null pointer, the method just
								returns the number of bytes required (including the terminating null character).
		@param maxBufferSizeBytes  the size of the destination buffer, in bytes. If the string won't fit, it'll
								put in as many as it can while still allowing for a terminating null char at the
								end, and will return the number of bytes that were actually used.
		@see CharPointer_UTF32::writeWithDestByteLimit
	*/
	int copyToUTF32 (CharPointer_UTF32::CharType* destBuffer, int maxBufferSizeBytes) const noexcept;

	/** Increases the string's internally allocated storage.

		Although the string's contents won't be affected by this call, it will
		increase the amount of memory allocated internally for the string to grow into.

		If you're about to make a large number of calls to methods such
		as += or <<, it's more efficient to preallocate enough extra space
		beforehand, so that these methods won't have to keep resizing the string
		to append the extra characters.

		@param numBytesNeeded   the number of bytes to allocate storage for. If this
								value is less than the currently allocated size, it will
								have no effect.
	*/
	void preallocateBytes (size_t numBytesNeeded);

	/** Swaps the contents of this string with another one.
		This is a very fast operation, as no allocation or copying needs to be done.
	*/
	void swapWith (String& other) noexcept;

   #if JUCE_MAC || JUCE_IOS || DOXYGEN
	/** MAC ONLY - Creates a String from an OSX CFString. */
	static String fromCFString (CFStringRef cfString);

	/** MAC ONLY - Converts this string to a CFString.
		Remember that you must use CFRelease() to free the returned string when you're
		finished with it.
	*/
	CFStringRef toCFString() const;

	/** MAC ONLY - Returns a copy of this string in which any decomposed unicode characters have
		been converted to their precomposed equivalents. */
	String convertToPrecomposedUnicode() const;
   #endif

private:

	CharPointerType text;

	struct PreallocationBytes
	{
		explicit PreallocationBytes (size_t);
		size_t numBytes;
	};

	explicit String (const PreallocationBytes&); // This constructor preallocates a certain amount of memory
	void appendFixedLength (const char* text, int numExtraChars);
	size_t getByteOffsetOfEnd() const noexcept;
	JUCE_DEPRECATED (String (const String& stringToCopy, size_t charsToAllocate));

	// This private cast operator should prevent strings being accidentally cast
	// to bools (this is possible because the compiler can add an implicit cast
	// via a const char*)
	operator bool() const noexcept  { return false; }
};

/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (const char* string1,     const String& string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (const wchar_t* string1,  const String& string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (char string1,            const String& string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (wchar_t string1,         const String& string2);
#if ! JUCE_NATIVE_WCHAR_IS_UTF32
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (juce_wchar string1,      const String& string2);
#endif

/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, const String& string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, const char* string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, const wchar_t* string2);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, char characterToAppend);
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, wchar_t characterToAppend);
#if ! JUCE_NATIVE_WCHAR_IS_UTF32
/** Concatenates two strings. */
JUCE_API String JUCE_CALLTYPE operator+ (String string1, juce_wchar characterToAppend);
#endif

/** Appends a character at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, char characterToAppend);
/** Appends a character at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, wchar_t characterToAppend);
#if ! JUCE_NATIVE_WCHAR_IS_UTF32
/** Appends a character at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, juce_wchar characterToAppend);
#endif

/** Appends a string to the end of the first one. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, const char* string2);
/** Appends a string to the end of the first one. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, const wchar_t* string2);
/** Appends a string to the end of the first one. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, const String& string2);

/** Appends a decimal number at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, short number);
/** Appends a decimal number at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, int number);
/** Appends a decimal number at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, long number);
/** Appends a decimal number at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, float number);
/** Appends a decimal number at the end of a string. */
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, double number);

/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const String& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const char* string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const wchar_t* string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const CharPointer_UTF8& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const CharPointer_UTF16& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator== (const String& string1, const CharPointer_UTF32& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const String& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const char* string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const wchar_t* string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const CharPointer_UTF8& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const CharPointer_UTF16& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator!= (const String& string1, const CharPointer_UTF32& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator>  (const String& string1, const String& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator<  (const String& string1, const String& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator>= (const String& string1, const String& string2) noexcept;
/** Case-sensitive comparison of two strings. */
JUCE_API bool JUCE_CALLTYPE operator<= (const String& string1, const String& string2) noexcept;

/** This operator allows you to write a juce String directly to std output streams.
	This is handy for writing strings to std::cout, std::cerr, etc.
*/
template <class traits>
std::basic_ostream <char, traits>& JUCE_CALLTYPE operator<< (std::basic_ostream <char, traits>& stream, const String& stringToWrite)
{
	return stream << stringToWrite.toUTF8().getAddress();
}

/** This operator allows you to write a juce String directly to std output streams.
	This is handy for writing strings to std::wcout, std::wcerr, etc.
*/
template <class traits>
std::basic_ostream <wchar_t, traits>& JUCE_CALLTYPE operator<< (std::basic_ostream <wchar_t, traits>& stream, const String& stringToWrite)
{
	return stream << stringToWrite.toWideCharPointer();
}

/** Writes a string to an OutputStream as UTF8. */
JUCE_API OutputStream& JUCE_CALLTYPE operator<< (OutputStream& stream, const String& stringToWrite);

#endif   // __JUCE_STRING_JUCEHEADER__

/*** End of inlined file: juce_String.h ***/

/**
	Acts as an application-wide logging class.

	A subclass of Logger can be created and passed into the Logger::setCurrentLogger
	method and this will then be used by all calls to writeToLog.

	The logger class also contains methods for writing messages to the debugger's
	output stream.

	@see FileLogger
*/
class JUCE_API  Logger
{
public:

	/** Destructor. */
	virtual ~Logger();

	/** Sets the current logging class to use.

		Note that the object passed in won't be deleted when no longer needed.
		A null pointer can be passed-in to disable any logging.

		If deleteOldLogger is set to true, the existing logger will be
		deleted (if there is one).
	*/
	static void JUCE_CALLTYPE setCurrentLogger (Logger* newLogger,
												bool deleteOldLogger = false);

	/** Writes a string to the current logger.

		This will pass the string to the logger's logMessage() method if a logger
		has been set.

		@see logMessage
	*/
	static void JUCE_CALLTYPE writeToLog (const String& message);

	/** Writes a message to the standard error stream.

		This can be called directly, or by using the DBG() macro in
		juce_PlatformDefs.h (which will avoid calling the method in non-debug builds).
	*/
	static void JUCE_CALLTYPE outputDebugString (const String& text);

protected:

	Logger();

	/** This is overloaded by subclasses to implement custom logging behaviour.

		@see setCurrentLogger
	*/
	virtual void logMessage (const String& message) = 0;

private:
	static Logger* currentLogger;
};

#endif   // __JUCE_LOGGER_JUCEHEADER__

/*** End of inlined file: juce_Logger.h ***/


/*** Start of inlined file: juce_LeakedObjectDetector.h ***/
#ifndef __JUCE_LEAKEDOBJECTDETECTOR_JUCEHEADER__
#define __JUCE_LEAKEDOBJECTDETECTOR_JUCEHEADER__

/**
	Embedding an instance of this class inside another class can be used as a low-overhead
	way of detecting leaked instances.

	This class keeps an internal static count of the number of instances that are
	active, so that when the app is shutdown and the static destructors are called,
	it can check whether there are any left-over instances that may have been leaked.

	To use it, use the JUCE_LEAK_DETECTOR macro as a simple way to put one in your
	class declaration. Have a look through the juce codebase for examples, it's used
	in most of the classes.
*/
template <class OwnerClass>
class LeakedObjectDetector
{
public:

	LeakedObjectDetector() noexcept                                 { ++(getCounter().numObjects); }
	LeakedObjectDetector (const LeakedObjectDetector&) noexcept     { ++(getCounter().numObjects); }

	~LeakedObjectDetector()
	{
		if (--(getCounter().numObjects) < 0)
		{
			DBG ("*** Dangling pointer deletion! Class: " << getLeakedObjectClassName());

			/** If you hit this, then you've managed to delete more instances of this class than you've
				created.. That indicates that you're deleting some dangling pointers.

				Note that although this assertion will have been triggered during a destructor, it might
				not be this particular deletion that's at fault - the incorrect one may have happened
				at an earlier point in the program, and simply not been detected until now.

				Most errors like this are caused by using old-fashioned, non-RAII techniques for
				your object management. Tut, tut. Always, always use ScopedPointers, OwnedArrays,
				ReferenceCountedObjects, etc, and avoid the 'delete' operator at all costs!
			*/
			jassertfalse;
		}
	}

private:

	class LeakCounter
	{
	public:
		LeakCounter() noexcept {}

		~LeakCounter()
		{
			if (numObjects.value > 0)
			{
				DBG ("*** Leaked objects detected: " << numObjects.value << " instance(s) of class " << getLeakedObjectClassName());

				/** If you hit this, then you've leaked one or more objects of the type specified by
					the 'OwnerClass' template parameter - the name should have been printed by the line above.

					If you're leaking, it's probably because you're using old-fashioned, non-RAII techniques for
					your object management. Tut, tut. Always, always use ScopedPointers, OwnedArrays,
					ReferenceCountedObjects, etc, and avoid the 'delete' operator at all costs!
				*/
				jassertfalse;
			}
		}

		Atomic<int> numObjects;
	};

	static const char* getLeakedObjectClassName()
	{
		return OwnerClass::getLeakedObjectClassName();
	}

	static LeakCounter& getCounter() noexcept
	{
		static LeakCounter counter;
		return counter;
	}
};

#if DOXYGEN || ! defined (JUCE_LEAK_DETECTOR)
 #if (DOXYGEN || JUCE_CHECK_MEMORY_LEAKS)
  /** This macro lets you embed a leak-detecting object inside a class.

	  To use it, simply declare a JUCE_LEAK_DETECTOR(YourClassName) inside a private section
	  of the class declaration. E.g.

	  @code
	  class MyClass
	  {
	  public:
		  MyClass();
		  void blahBlah();

	  private:
		  JUCE_LEAK_DETECTOR (MyClass);
	  };@endcode

	  @see JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR, LeakedObjectDetector
  */
  #define JUCE_LEAK_DETECTOR(OwnerClass) \
		friend class juce::LeakedObjectDetector<OwnerClass>; \
		static const char* getLeakedObjectClassName() noexcept { return #OwnerClass; } \
		juce::LeakedObjectDetector<OwnerClass> JUCE_JOIN_MACRO (leakDetector, __LINE__);
 #else
  #define JUCE_LEAK_DETECTOR(OwnerClass)
 #endif
#endif

#endif   // __JUCE_LEAKEDOBJECTDETECTOR_JUCEHEADER__

/*** End of inlined file: juce_LeakedObjectDetector.h ***/

// unbelievably, some system headers actually use macros to define these symbols:
#undef check
#undef TYPE_BOOL

#if JUCE_MAC || JUCE_IOS || DOXYGEN

 /** A handy C++ wrapper that creates and deletes an NSAutoreleasePool object using RAII.
	 You should use the JUCE_AUTORELEASEPOOL macro to create a local auto-release pool on the stack.
 */
 class JUCE_API  ScopedAutoReleasePool
 {
 public:
	 ScopedAutoReleasePool();
	 ~ScopedAutoReleasePool();

 private:
	 void* pool;

	 JUCE_DECLARE_NON_COPYABLE (ScopedAutoReleasePool);
 };

 /** A macro that can be used to easily declare a local ScopedAutoReleasePool object for RAII-based obj-C autoreleasing. */
 #define JUCE_AUTORELEASEPOOL  const juce::ScopedAutoReleasePool JUCE_JOIN_MACRO (autoReleasePool_, __LINE__);

#else
 #define JUCE_AUTORELEASEPOOL
#endif

END_JUCE_NAMESPACE

#endif   // __JUCE_STANDARDHEADER_JUCEHEADER__

/*** End of inlined file: juce_StandardHeader.h ***/

namespace juce
{

// START_AUTOINCLUDE processors, format, format_types, scanning
#ifndef __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/*** Start of inlined file: juce_AudioPlayHead.h ***/
#ifndef __JUCE_AUDIOPLAYHEAD_JUCEHEADER__
#define __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/**
	A subclass of AudioPlayHead can supply information about the position and
	status of a moving play head during audio playback.

	One of these can be supplied to an AudioProcessor object so that it can find
	out about the position of the audio that it is rendering.

	@see AudioProcessor::setPlayHead, AudioProcessor::getPlayHead
*/
class JUCE_API  AudioPlayHead
{
protected:

	AudioPlayHead() {}

public:
	virtual ~AudioPlayHead() {}

	/** Frame rate types. */
	enum FrameRateType
	{
		fps24           = 0,
		fps25           = 1,
		fps2997         = 2,
		fps30           = 3,
		fps2997drop     = 4,
		fps30drop       = 5,
		fpsUnknown      = 99
	};

	/** This structure is filled-in by the AudioPlayHead::getCurrentPosition() method.
	*/
	struct JUCE_API  CurrentPositionInfo
	{
		/** The tempo in BPM */
		double bpm;

		/** Time signature numerator, e.g. the 3 of a 3/4 time sig */
		int timeSigNumerator;
		/** Time signature denominator, e.g. the 4 of a 3/4 time sig */
		int timeSigDenominator;

		/** The current play position, in seconds from the start of the edit. */
		double timeInSeconds;

		/** For timecode, the position of the start of the edit, in seconds from 00:00:00:00. */
		double editOriginTime;

		/** The current play position in pulses-per-quarter-note.

			This is the number of quarter notes since the edit start.
		*/
		double ppqPosition;

		/** The position of the start of the last bar, in pulses-per-quarter-note.

			This is the number of quarter notes from the start of the edit to the
			start of the current bar.

			Note - this value may be unavailable on some hosts, e.g. Pro-Tools. If
			it's not available, the value will be 0.
		*/
		double ppqPositionOfLastBarStart;

		/** The video frame rate, if applicable. */
		FrameRateType frameRate;

		/** True if the transport is currently playing. */
		bool isPlaying;

		/** True if the transport is currently recording.

			(When isRecording is true, then isPlaying will also be true).
		*/
		bool isRecording;

		/** The current cycle start position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopStart;

		/** The current cycle end position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopEnd;

		/** True if the transport is currently looping. */
		bool isLooping;

		bool operator== (const CurrentPositionInfo& other) const noexcept;
		bool operator!= (const CurrentPositionInfo& other) const noexcept;

		void resetToDefault();
	};

	/** Fills-in the given structure with details about the transport's
		position at the start of the current processing block.
	*/
	virtual bool getCurrentPosition (CurrentPositionInfo& result) = 0;
};

#endif   // __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/*** End of inlined file: juce_AudioPlayHead.h ***/


#endif
#ifndef __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__

/*** Start of inlined file: juce_AudioPluginInstance.h ***/
#ifndef __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__
#define __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessor.h ***/
#ifndef __JUCE_AUDIOPROCESSOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOR_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessorEditor.h ***/
#ifndef __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

class AudioProcessor;

/**
	Base class for the component that acts as the GUI for an AudioProcessor.

	Derive your editor component from this class, and create an instance of it
	by overriding the AudioProcessor::createEditor() method.

	@see AudioProcessor, GenericAudioProcessorEditor
*/
class JUCE_API  AudioProcessorEditor  : public Component
{
protected:

	/** Creates an editor for the specified processor.
	*/
	AudioProcessorEditor (AudioProcessor* owner);

public:
	/** Destructor. */
	~AudioProcessorEditor();

	/** Returns a pointer to the processor that this editor represents. */
	AudioProcessor* getAudioProcessor() const noexcept        { return owner; }

private:

	AudioProcessor* const owner;

	JUCE_DECLARE_NON_COPYABLE (AudioProcessorEditor);
};

#endif   // __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorEditor.h ***/


/*** Start of inlined file: juce_AudioProcessorListener.h ***/
#ifndef __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

class AudioProcessor;

/**
	Base class for listeners that want to know about changes to an AudioProcessor.

	Use AudioProcessor::addListener() to register your listener with an AudioProcessor.

	@see AudioProcessor
*/
class JUCE_API  AudioProcessorListener
{
public:

	/** Destructor. */
	virtual ~AudioProcessorListener() {}

	/** Receives a callback when a parameter is changed.

		IMPORTANT NOTE: this will be called synchronously when a parameter changes, and
		many audio processors will change their parameter during their audio callback.
		This means that not only has your handler code got to be completely thread-safe,
		but it's also got to be VERY fast, and avoid blocking. If you need to handle
		this event on your message thread, use this callback to trigger an AsyncUpdater
		or ChangeBroadcaster which you can respond to on the message thread.
	*/
	virtual void audioProcessorParameterChanged (AudioProcessor* processor,
												 int parameterIndex,
												 float newValue) = 0;

	/** Called to indicate that something else in the plugin has changed, like its
		program, number of parameters, etc.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.
	*/
	virtual void audioProcessorChanged (AudioProcessor* processor) = 0;

	/** Indicates that a parameter change gesture has started.

		E.g. if the user is dragging a slider, this would be called when they first
		press the mouse button, and audioProcessorParameterChangeGestureEnd would be
		called when they release it.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureEnd
	*/
	virtual void audioProcessorParameterChangeGestureBegin (AudioProcessor* processor,
															int parameterIndex);

	/** Indicates that a parameter change gesture has finished.

		E.g. if the user is dragging a slider, this would be called when they release
		the mouse button.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureBegin
	*/
	virtual void audioProcessorParameterChangeGestureEnd (AudioProcessor* processor,
														  int parameterIndex);
};

#endif   // __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorListener.h ***/

/**
	Base class for audio processing filters or plugins.

	This is intended to act as a base class of audio filter that is general enough to
	be wrapped as a VST, AU, RTAS, etc, or used internally.

	It is also used by the plugin hosting code as the wrapper around an instance
	of a loaded plugin.

	Derive your filter class from this base class, and if you're building a plugin,
	you should implement a global function called createPluginFilter() which creates
	and returns a new instance of your subclass.
*/
class JUCE_API  AudioProcessor
{
protected:

	/** Constructor.

		You can also do your initialisation tasks in the initialiseFilterInfo()
		call, which will be made after this object has been created.
	*/
	AudioProcessor();

public:
	/** Destructor. */
	virtual ~AudioProcessor();

	/** Returns the name of this processor.
	*/
	virtual const String getName() const = 0;

	/** Called before playback starts, to let the filter prepare itself.

		The sample rate is the target sample rate, and will remain constant until
		playback stops.

		The estimatedSamplesPerBlock value is a HINT about the typical number of
		samples that will be processed for each callback, but isn't any kind
		of guarantee. The actual block sizes that the host uses may be different
		each time the callback happens, and may be more or less than this value.
	*/
	virtual void prepareToPlay (double sampleRate,
								int estimatedSamplesPerBlock) = 0;

	/** Called after playback has stopped, to let the filter free up any resources it
		no longer needs.
	*/
	virtual void releaseResources() = 0;

	/** Renders the next block.

		When this method is called, the buffer contains a number of channels which is
		at least as great as the maximum number of input and output channels that
		this filter is using. It will be filled with the filter's input data and
		should be replaced with the filter's output.

		So for example if your filter has 2 input channels and 4 output channels, then
		the buffer will contain 4 channels, the first two being filled with the
		input data. Your filter should read these, do its processing, and replace
		the contents of all 4 channels with its output.

		Or if your filter has 5 inputs and 2 outputs, the buffer will have 5 channels,
		all filled with data, and your filter should overwrite the first 2 of these
		with its output. But be VERY careful not to write anything to the last 3
		channels, as these might be mapped to memory that the host assumes is read-only!

		Note that if you have more outputs than inputs, then only those channels that
		correspond to an input channel are guaranteed to contain sensible data - e.g.
		in the case of 2 inputs and 4 outputs, the first two channels contain the input,
		but the last two channels may contain garbage, so you should be careful not to
		let this pass through without being overwritten or cleared.

		Also note that the buffer may have more channels than are strictly necessary,
		but your should only read/write from the ones that your filter is supposed to
		be using.

		The number of samples in these buffers is NOT guaranteed to be the same for every
		callback, and may be more or less than the estimated value given to prepareToPlay().
		Your code must be able to cope with variable-sized blocks, or you're going to get
		clicks and crashes!

		If the filter is receiving a midi input, then the midiMessages array will be filled
		with the midi messages for this block. Each message's timestamp will indicate the
		message's time, as a number of samples from the start of the block.

		Any messages left in the midi buffer when this method has finished are assumed to
		be the filter's midi output. This means that your filter should be careful to
		clear any incoming messages from the array if it doesn't want them to be passed-on.

		Be very careful about what you do in this callback - it's going to be called by
		the audio thread, so any kind of interaction with the UI is absolutely
		out of the question. If you change a parameter in here and need to tell your UI to
		update itself, the best way is probably to inherit from a ChangeBroadcaster, let
		the UI components register as listeners, and then call sendChangeMessage() inside the
		processBlock() method to send out an asynchronous message. You could also use
		the AsyncUpdater class in a similar way.
	*/
	virtual void processBlock (AudioSampleBuffer& buffer,
							   MidiBuffer& midiMessages) = 0;

	/** Returns the current AudioPlayHead object that should be used to find
		out the state and position of the playhead.

		You can call this from your processBlock() method, and use the AudioPlayHead
		object to get the details about the time of the start of the block currently
		being processed.

		If the host hasn't supplied a playhead object, this will return 0.
	*/
	AudioPlayHead* getPlayHead() const noexcept               { return playHead; }

	/** Returns the current sample rate.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time, and may return 0 if it's unknown.
	*/
	double getSampleRate() const noexcept                     { return sampleRate; }

	/** Returns the current typical block size that is being used.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time.

		Remember it's not the ONLY block size that may be used when calling
		processBlock, it's just the normal one. The actual block sizes used may be
		larger or smaller than this, and will vary between successive calls.
	*/
	int getBlockSize() const noexcept                         { return blockSize; }

	/** Returns the number of input channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumInputChannels() const noexcept                  { return numInputChannels; }

	/** Returns the number of output channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumOutputChannels() const noexcept                 { return numOutputChannels; }

	/** Returns the name of one of the input channels, as returned by the host.

		The host might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getInputChannelName (int channelIndex) const = 0;

	/** Returns the name of one of the output channels, as returned by the host.

		The host might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getOutputChannelName (int channelIndex) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isInputChannelStereoPair (int index) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isOutputChannelStereoPair (int index) const = 0;

	/** This returns the number of samples delay that the filter imposes on the audio
		passing through it.

		The host will call this to find the latency - the filter itself should set this value
		by calling setLatencySamples() as soon as it can during its initialisation.
	*/
	int getLatencySamples() const noexcept                            { return latencySamples; }

	/** The filter should call this to set the number of samples delay that it introduces.

		The filter should call this as soon as it can during initialisation, and can call it
		later if the value changes.
	*/
	void setLatencySamples (int newLatency);

	/** Returns true if the processor wants midi messages. */
	virtual bool acceptsMidi() const = 0;

	/** Returns true if the processor produces midi messages. */
	virtual bool producesMidi() const = 0;

	/** This returns a critical section that will automatically be locked while the host
		is calling the processBlock() method.

		Use it from your UI or other threads to lock access to variables that are used
		by the process callback, but obviously be careful not to keep it locked for
		too long, because that could cause stuttering playback. If you need to do something
		that'll take a long time and need the processing to stop while it happens, use the
		suspendProcessing() method instead.

		@see suspendProcessing
	*/
	const CriticalSection& getCallbackLock() const noexcept             { return callbackLock; }

	/** Enables and disables the processing callback.

		If you need to do something time-consuming on a thread and would like to make sure
		the audio processing callback doesn't happen until you've finished, use this
		to disable the callback and re-enable it again afterwards.

		E.g.
		@code
		void loadNewPatch()
		{
			suspendProcessing (true);

			..do something that takes ages..

			suspendProcessing (false);
		}
		@endcode

		If the host tries to make an audio callback while processing is suspended, the
		filter will return an empty buffer, but won't block the audio thread like it would
		do if you use the getCallbackLock() critical section to synchronise access.

		If you're going to use this, your processBlock() method must call isSuspended() and
		check whether it's suspended or not. If it is, then it should skip doing any real
		processing, either emitting silence or passing the input through unchanged.

		@see getCallbackLock
	*/
	void suspendProcessing (bool shouldBeSuspended);

	/** Returns true if processing is currently suspended.
		@see suspendProcessing
	*/
	bool isSuspended() const noexcept                                   { return suspended; }

	/** A plugin can override this to be told when it should reset any playing voices.

		The default implementation does nothing, but a host may call this to tell the
		plugin that it should stop any tails or sounds that have been left running.
	*/
	virtual void reset();

	/** Returns true if the processor is being run in an offline mode for rendering.

		If the processor is being run live on realtime signals, this returns false.
		If the mode is unknown, this will assume it's realtime and return false.

		This value may be unreliable until the prepareToPlay() method has been called,
		and could change each time prepareToPlay() is called.

		@see setNonRealtime()
	*/
	bool isNonRealtime() const noexcept                                 { return nonRealtime; }

	/** Called by the host to tell this processor whether it's being used in a non-realime
		capacity for offline rendering or bouncing.

		Whatever value is passed-in will be
	*/
	void setNonRealtime (bool isNonRealtime) noexcept;

	/** Creates the filter's UI.

		This can return 0 if you want a UI-less filter, in which case the host may create
		a generic UI that lets the user twiddle the parameters directly.

		If you do want to pass back a component, the component should be created and set to
		the correct size before returning it. If you implement this method, you must
		also implement the hasEditor() method and make it return true.

		Remember not to do anything silly like allowing your filter to keep a pointer to
		the component that gets created - it could be deleted later without any warning, which
		would make your pointer into a dangler. Use the getActiveEditor() method instead.

		The correct way to handle the connection between an editor component and its
		filter is to use something like a ChangeBroadcaster so that the editor can
		register itself as a listener, and be told when a change occurs. This lets them
		safely unregister themselves when they are deleted.

		Here are a few things to bear in mind when writing an editor:

		- Initially there won't be an editor, until the user opens one, or they might
		  not open one at all. Your filter mustn't rely on it being there.
		- An editor object may be deleted and a replacement one created again at any time.
		- It's safe to assume that an editor will be deleted before its filter.

		@see hasEditor
	*/
	virtual AudioProcessorEditor* createEditor() = 0;

	/** Your filter must override this and return true if it can create an editor component.
		@see createEditor
	*/
	virtual bool hasEditor() const = 0;

	/** Returns the active editor, if there is one.

		Bear in mind this can return 0, even if an editor has previously been
		opened.
	*/
	AudioProcessorEditor* getActiveEditor() const noexcept             { return activeEditor; }

	/** Returns the active editor, or if there isn't one, it will create one.

		This may call createEditor() internally to create the component.
	*/
	AudioProcessorEditor* createEditorIfNeeded();

	/** This must return the correct value immediately after the object has been
		created, and mustn't change the number of parameters later.
	*/
	virtual int getNumParameters() = 0;

	/** Returns the name of a particular parameter. */
	virtual const String getParameterName (int parameterIndex) = 0;

	/** Called by the host to find out the value of one of the filter's parameters.

		The host will expect the value returned to be between 0 and 1.0.

		This could be called quite frequently, so try to make your code efficient.
		It's also likely to be called by non-UI threads, so the code in here should
		be thread-aware.
	*/
	virtual float getParameter (int parameterIndex) = 0;

	/** Returns the value of a parameter as a text string. */
	virtual const String getParameterText (int parameterIndex) = 0;

	/** The host will call this method to change the value of one of the filter's parameters.

		The host may call this at any time, including during the audio processing
		callback, so the filter has to process this very fast and avoid blocking.

		If you want to set the value of a parameter internally, e.g. from your
		editor component, then don't call this directly - instead, use the
		setParameterNotifyingHost() method, which will also send a message to
		the host telling it about the change. If the message isn't sent, the host
		won't be able to automate your parameters properly.

		The value passed will be between 0 and 1.0.
	*/
	virtual void setParameter (int parameterIndex,
							   float newValue) = 0;

	/** Your filter can call this when it needs to change one of its parameters.

		This could happen when the editor or some other internal operation changes
		a parameter. This method will call the setParameter() method to change the
		value, and will then send a message to the host telling it about the change.

		Note that to make sure the host correctly handles automation, you should call
		the beginParameterChangeGesture() and endParameterChangeGesture() methods to
		tell the host when the user has started and stopped changing the parameter.
	*/
	void setParameterNotifyingHost (int parameterIndex,
									float newValue);

	/** Returns true if the host can automate this parameter.

		By default, this returns true for all parameters.
	*/
	virtual bool isParameterAutomatable (int parameterIndex) const;

	/** Should return true if this parameter is a "meta" parameter.

		A meta-parameter is a parameter that changes other params. It is used
		by some hosts (e.g. AudioUnit hosts).

		By default this returns false.
	*/
	virtual bool isMetaParameter (int parameterIndex) const;

	/** Sends a signal to the host to tell it that the user is about to start changing this
		parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		If you call this, it must be matched by a later call to endParameterChangeGesture().
	*/
	void beginParameterChangeGesture (int parameterIndex);

	/** Tells the host that the user has finished changing this parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		A call to this method must follow a call to beginParameterChangeGesture().
	*/
	void endParameterChangeGesture (int parameterIndex);

	/** The filter can call this when something (apart from a parameter value) has changed.

		It sends a hint to the host that something like the program, number of parameters,
		etc, has changed, and that it should update itself.
	*/
	void updateHostDisplay();

	/** Returns the number of preset programs the filter supports.

		The value returned must be valid as soon as this object is created, and
		must not change over its lifetime.

		This value shouldn't be less than 1.
	*/
	virtual int getNumPrograms() = 0;

	/** Returns the number of the currently active program.
	*/
	virtual int getCurrentProgram() = 0;

	/** Called by the host to change the current program.
	*/
	virtual void setCurrentProgram (int index) = 0;

	/** Must return the name of a given program. */
	virtual const String getProgramName (int index) = 0;

	/** Called by the host to rename a program.
	*/
	virtual void changeProgramName (int index, const String& newName) = 0;

	/** The host will call this method when it wants to save the filter's internal state.

		This must copy any info about the filter's state into the block of memory provided,
		so that the host can store this and later restore it using setStateInformation().

		Note that there's also a getCurrentProgramStateInformation() method, which only
		stores the current program, not the state of the entire filter.

		See also the helper function copyXmlToBinary() for storing settings as XML.

		@see getCurrentProgramStateInformation
	*/
	virtual void getStateInformation (juce::MemoryBlock& destData) = 0;

	/** The host will call this method if it wants to save the state of just the filter's
		current program.

		Unlike getStateInformation, this should only return the current program's state.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls getStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see getStateInformation, setCurrentProgramStateInformation
	*/
	virtual void getCurrentProgramStateInformation (juce::MemoryBlock& destData);

	/** This must restore the filter's state from a block of data previously created
		using getStateInformation().

		Note that there's also a setCurrentProgramStateInformation() method, which tries
		to restore just the current program, not the state of the entire filter.

		See also the helper function getXmlFromBinary() for loading settings as XML.

		@see setCurrentProgramStateInformation
	*/
	virtual void setStateInformation (const void* data, int sizeInBytes) = 0;

	/** The host will call this method if it wants to restore the state of just the filter's
		current program.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls setStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see setStateInformation, getCurrentProgramStateInformation
	*/
	virtual void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

	/** Adds a listener that will be called when an aspect of this processor changes. */
	void addListener (AudioProcessorListener* newListener);

	/** Removes a previously added listener. */
	void removeListener (AudioProcessorListener* listenerToRemove);

	/** Tells the processor to use this playhead object.
		The processor will not take ownership of the object, so the caller must delete it when
		it is no longer being used.
	*/
	void setPlayHead (AudioPlayHead* newPlayHead) noexcept;

	/** Not for public use - this is called before deleting an editor component. */
	void editorBeingDeleted (AudioProcessorEditor* editor) noexcept;

	/** Not for public use - this is called to initialise the processor before playing. */
	void setPlayConfigDetails (int numIns, int numOuts,
							   double sampleRate,
							   int blockSize) noexcept;

protected:

	/** Helper function that just converts an xml element into a binary blob.

		Use this in your filter's getStateInformation() method if you want to
		store its state as xml.

		Then use getXmlFromBinary() to reverse this operation and retrieve the XML
		from a binary blob.
	*/
	static void copyXmlToBinary (const XmlElement& xml,
								 juce::MemoryBlock& destData);

	/** Retrieves an XML element that was stored as binary with the copyXmlToBinary() method.

		This might return 0 if the data's unsuitable or corrupted. Otherwise it will return
		an XmlElement object that the caller must delete when no longer needed.
	*/
	static XmlElement* getXmlFromBinary (const void* data, int sizeInBytes);

	/** @internal */
	AudioPlayHead* playHead;

	/** @internal */
	void sendParamChangeMessageToListeners (int parameterIndex, float newValue);

private:
	Array <AudioProcessorListener*> listeners;
	Component::SafePointer<AudioProcessorEditor> activeEditor;
	double sampleRate;
	int blockSize, numInputChannels, numOutputChannels, latencySamples;
	bool suspended, nonRealtime;
	CriticalSection callbackLock, listenerLock;

   #if JUCE_DEBUG
	BigInteger changingParams;
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor);
};

#endif   // __JUCE_AUDIOPROCESSOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessor.h ***/


/*** Start of inlined file: juce_PluginDescription.h ***/
#ifndef __JUCE_PLUGINDESCRIPTION_JUCEHEADER__
#define __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

/**
	A small class to represent some facts about a particular type of plugin.

	This class is for storing and managing the details about a plugin without
	actually having to load an instance of it.

	A KnownPluginList contains a list of PluginDescription objects.

	@see KnownPluginList
*/
class JUCE_API  PluginDescription
{
public:

	PluginDescription();
	PluginDescription (const PluginDescription& other);
	PluginDescription& operator= (const PluginDescription& other);
	~PluginDescription();

	/** The name of the plugin. */
	String name;

	/** A more descriptive name for the plugin.
		This may be the same as the 'name' field, but some plugins may provide an
		alternative name.
	*/
	String descriptiveName;

	/** The plugin format, e.g. "VST", "AudioUnit", etc.
	*/
	String pluginFormatName;

	/** A category, such as "Dynamics", "Reverbs", etc.
	*/
	String category;

	/** The manufacturer. */
	String manufacturerName;

	/** The version. This string doesn't have any particular format. */
	String version;

	/** Either the file containing the plugin module, or some other unique way
		of identifying it.

		E.g. for an AU, this would be an ID string that the component manager
		could use to retrieve the plugin. For a VST, it's the file path.
	*/
	String fileOrIdentifier;

	/** The last time the plugin file was changed.
		This is handy when scanning for new or changed plugins.
	*/
	Time lastFileModTime;

	/** A unique ID for the plugin.

		Note that this might not be unique between formats, e.g. a VST and some
		other format might actually have the same id.

		@see createIdentifierString
	*/
	int uid;

	/** True if the plugin identifies itself as a synthesiser. */
	bool isInstrument;

	/** The number of inputs. */
	int numInputChannels;

	/** The number of outputs. */
	int numOutputChannels;

	/** Returns true if the two descriptions refer the the same plugin.

		This isn't quite as simple as them just having the same file (because of
		shell plugins).
	*/
	bool isDuplicateOf (const PluginDescription& other) const;

	/** Returns a string that can be saved and used to uniquely identify the
		plugin again.

		This contains less info than the XML encoding, and is independent of the
		plugin's file location, so can be used to store a plugin ID for use
		across different machines.
	*/
	String createIdentifierString() const;

	/** Creates an XML object containing these details.

		@see loadFromXml
	*/
	XmlElement* createXml() const;

	/** Reloads the info in this structure from an XML record that was previously
		saved with createXML().

		Returns true if the XML was a valid plugin description.
	*/
	bool loadFromXml (const XmlElement& xml);

private:

	JUCE_LEAK_DETECTOR (PluginDescription);
};

#endif   // __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

/*** End of inlined file: juce_PluginDescription.h ***/

/**
	Base class for an active instance of a plugin.

	This derives from the AudioProcessor class, and adds some extra functionality
	that helps when wrapping dynamically loaded plugins.

	@see AudioProcessor, AudioPluginFormat
*/
class JUCE_API  AudioPluginInstance   : public AudioProcessor
{
public:

	/** Destructor.

		Make sure that you delete any UI components that belong to this plugin before
		deleting the plugin.
	*/
	virtual ~AudioPluginInstance() {}

	/** Fills-in the appropriate parts of this plugin description object.
	*/
	virtual void fillInPluginDescription (PluginDescription& description) const = 0;

	/** Returns a pointer to some kind of platform-specific data about the plugin.

		E.g. For a VST, this value can be cast to an AEffect*. For an AudioUnit, it can be
		cast to an AudioUnit handle.
	*/
	virtual void* getPlatformSpecificData()         { return nullptr; }

protected:

	AudioPluginInstance() {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginInstance);
};

#endif   // __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginInstance.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSOR_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__

/*** Start of inlined file: juce_AudioProcessorGraph.h ***/
#ifndef __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__


/*** Start of inlined file: juce_AudioPluginFormatManager.h ***/
#ifndef __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__
#define __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__


/*** Start of inlined file: juce_AudioPluginFormat.h ***/
#ifndef __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__
#define __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

class PluginDescription;

/**
	The base class for a type of plugin format, such as VST, AudioUnit, LADSPA, etc.

	Use the static getNumFormats() and getFormat() calls to find the types
	of format that are available.
*/
class JUCE_API  AudioPluginFormat
{
public:

	/** Destructor. */
	virtual ~AudioPluginFormat();

	/** Returns the format name.

		E.g. "VST", "AudioUnit", etc.
	*/
	virtual String getName() const = 0;

	/** This tries to create descriptions for all the plugin types available in
		a binary module file.

		The file will be some kind of DLL or bundle.

		Normally there will only be one type returned, but some plugins
		(e.g. VST shells) can use a single DLL to create a set of different plugin
		subtypes, so in that case, each subtype is returned as a separate object.
	*/
	virtual void findAllTypesForFile (OwnedArray <PluginDescription>& results,
									  const String& fileOrIdentifier) = 0;

	/** Tries to recreate a type from a previously generated PluginDescription.

		@see PluginDescription::createInstance
	*/
	virtual AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc) = 0;

	/** Should do a quick check to see if this file or directory might be a plugin of
		this format.

		This is for searching for potential files, so it shouldn't actually try to
		load the plugin or do anything time-consuming.
	*/
	virtual bool fileMightContainThisPluginType (const String& fileOrIdentifier) = 0;

	/** Returns a readable version of the name of the plugin that this identifier refers to.
	*/
	virtual String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) = 0;

	/** Checks whether this plugin could possibly be loaded.

		It doesn't actually need to load it, just to check whether the file or component
		still exists.
	*/
	virtual bool doesPluginStillExist (const PluginDescription& desc) = 0;

	/** Searches a suggested set of directories for any plugins in this format.

		The path might be ignored, e.g. by AUs, which are found by the OS rather
		than manually.
	*/
	virtual StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
											   bool recursive) = 0;

	/** Returns the typical places to look for this kind of plugin.

		Note that if this returns no paths, it means that the format can't be scanned-for
		(i.e. it's an internal format that doesn't live in files)
	*/
	virtual FileSearchPath getDefaultLocationsToSearch() = 0;

protected:

	AudioPluginFormat() noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginFormat);
};

#endif   // __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginFormat.h ***/

/**
	This maintains a list of known AudioPluginFormats.

	@see AudioPluginFormat
*/
class JUCE_API  AudioPluginFormatManager  : public DeletedAtShutdown
{
public:

	AudioPluginFormatManager();

	/** Destructor. */
	~AudioPluginFormatManager();

	juce_DeclareSingleton_SingleThreaded (AudioPluginFormatManager, false);

	/** Adds any formats that it knows about, e.g. VST.
	*/
	void addDefaultFormats();

	/** Returns the number of types of format that are available.

		Use getFormat() to get one of them.
	*/
	int getNumFormats();

	/** Returns one of the available formats.

		@see getNumFormats
	*/
	AudioPluginFormat* getFormat (int index);

	/** Adds a format to the list.

		The object passed in will be owned and deleted by the manager.
	*/
	void addFormat (AudioPluginFormat* format);

	/** Tries to load the type for this description, by trying all the formats
		that this manager knows about.

		The caller is responsible for deleting the object that is returned.

		If it can't load the plugin, it returns 0 and leaves a message in the
		errorMessage string.
	*/
	AudioPluginInstance* createPluginInstance (const PluginDescription& description,
											   String& errorMessage) const;

	/** Checks that the file or component for this plugin actually still exists.

		(This won't try to load the plugin)
	*/
	bool doesPluginStillExist (const PluginDescription& description) const;

private:

	OwnedArray <AudioPluginFormat> formats;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginFormatManager);
};

#endif   // __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginFormatManager.h ***/


/*** Start of inlined file: juce_KnownPluginList.h ***/
#ifndef __JUCE_KNOWNPLUGINLIST_JUCEHEADER__
#define __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

/**
	Manages a list of plugin types.

	This can be easily edited, saved and loaded, and used to create instances of
	the plugin types in it.

	@see PluginListComponent
*/
class JUCE_API  KnownPluginList   : public ChangeBroadcaster
{
public:

	/** Creates an empty list.
	*/
	KnownPluginList();

	/** Destructor. */
	~KnownPluginList();

	/** Clears the list. */
	void clear();

	/** Returns the number of types currently in the list.
		@see getType
	*/
	int getNumTypes() const noexcept                                { return types.size(); }

	/** Returns one of the types.
		@see getNumTypes
	*/
	PluginDescription* getType (int index) const noexcept           { return types [index]; }

	/** Looks for a type in the list which comes from this file.
	*/
	PluginDescription* getTypeForFile (const String& fileOrIdentifier) const;

	/** Looks for a type in the list which matches a plugin type ID.

		The identifierString parameter must have been created by
		PluginDescription::createIdentifierString().
	*/
	PluginDescription* getTypeForIdentifierString (const String& identifierString) const;

	/** Adds a type manually from its description. */
	bool addType (const PluginDescription& type);

	/** Removes a type. */
	void removeType (int index);

	/** Looks for all types that can be loaded from a given file, and adds them
		to the list.

		If dontRescanIfAlreadyInList is true, then the file will only be loaded and
		re-tested if it's not already in the list, or if the file's modification
		time has changed since the list was created. If dontRescanIfAlreadyInList is
		false, the file will always be reloaded and tested.

		Returns true if any new types were added, and all the types found in this
		file (even if it was already known and hasn't been re-scanned) get returned
		in the array.
	*/
	bool scanAndAddFile (const String& possiblePluginFileOrIdentifier,
						 bool dontRescanIfAlreadyInList,
						 OwnedArray <PluginDescription>& typesFound,
						 AudioPluginFormat& formatToUse);

	/** Returns true if the specified file is already known about and if it
		hasn't been modified since our entry was created.
	*/
	bool isListingUpToDate (const String& possiblePluginFileOrIdentifier) const;

	/** Scans and adds a bunch of files that might have been dragged-and-dropped.

		If any types are found in the files, their descriptions are returned in the array.
	*/
	void scanAndAddDragAndDroppedFiles (const StringArray& filenames,
										OwnedArray <PluginDescription>& typesFound);

	/** Sort methods used to change the order of the plugins in the list.
	*/
	enum SortMethod
	{
		defaultOrder = 0,
		sortAlphabetically,
		sortByCategory,
		sortByManufacturer,
		sortByFileSystemLocation
	};

	/** Adds all the plugin types to a popup menu so that the user can select one.

		Depending on the sort method, it may add sub-menus for categories,
		manufacturers, etc.

		Use getIndexChosenByMenu() to find out the type that was chosen.
	*/
	void addToMenu (PopupMenu& menu,
					const SortMethod sortMethod) const;

	/** Converts a menu item index that has been chosen into its index in this list.

		Returns -1 if it's not an ID that was used.

		@see addToMenu
	*/
	int getIndexChosenByMenu (int menuResultCode) const;

	/** Sorts the list. */
	void sort (const SortMethod method);

	/** Creates some XML that can be used to store the state of this list. */
	XmlElement* createXml() const;

	/** Recreates the state of this list from its stored XML format. */
	void recreateFromXml (const XmlElement& xml);

private:

	OwnedArray <PluginDescription> types;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnownPluginList);
};

#endif   // __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

/*** End of inlined file: juce_KnownPluginList.h ***/

/**
	A type of AudioProcessor which plays back a graph of other AudioProcessors.

	Use one of these objects if you want to wire-up a set of AudioProcessors
	and play back the result.

	Processors can be added to the graph as "nodes" using addNode(), and once
	added, you can connect any of their input or output channels to other
	nodes using addConnection().

	To play back a graph through an audio device, you might want to use an
	AudioProcessorPlayer object.
*/
class JUCE_API  AudioProcessorGraph   : public AudioProcessor,
										public AsyncUpdater
{
public:

	/** Creates an empty graph.
	*/
	AudioProcessorGraph();

	/** Destructor.
		Any processor objects that have been added to the graph will also be deleted.
	*/
	~AudioProcessorGraph();

	/** Represents one of the nodes, or processors, in an AudioProcessorGraph.

		To create a node, call AudioProcessorGraph::addNode().
	*/
	class JUCE_API  Node   : public ReferenceCountedObject
	{
	public:

		/** The ID number assigned to this node.
			This is assigned by the graph that owns it, and can't be changed.
		*/
		const uint32 nodeId;

		/** The actual processor object that this node represents. */
		AudioProcessor* getProcessor() const noexcept           { return processor; }

		/** A set of user-definable properties that are associated with this node.

			This can be used to attach values to the node for whatever purpose seems
			useful. For example, you might store an x and y position if your application
			is displaying the nodes on-screen.
		*/
		NamedValueSet properties;

		/** A convenient typedef for referring to a pointer to a node object. */
		typedef ReferenceCountedObjectPtr <Node> Ptr;

	private:

		friend class AudioProcessorGraph;

		const ScopedPointer<AudioProcessor> processor;
		bool isPrepared;

		Node (uint32 nodeId, AudioProcessor*) noexcept;

		void setParentGraph (AudioProcessorGraph*) const;
		void prepare (double sampleRate, int blockSize, AudioProcessorGraph*);
		void unprepare();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node);
	};

	/** Represents a connection between two channels of two nodes in an AudioProcessorGraph.

		To create a connection, use AudioProcessorGraph::addConnection().
	*/
	struct JUCE_API  Connection
	{
	public:

		Connection (uint32 sourceNodeId, int sourceChannelIndex,
					uint32 destNodeId, int destChannelIndex) noexcept;

		/** The ID number of the node which is the input source for this connection.
			@see AudioProcessorGraph::getNodeForId
		*/
		uint32 sourceNodeId;

		/** The index of the output channel of the source node from which this
			connection takes its data.

			If this value is the special number AudioProcessorGraph::midiChannelIndex, then
			it is referring to the source node's midi output. Otherwise, it is the zero-based
			index of an audio output channel in the source node.
		*/
		int sourceChannelIndex;

		/** The ID number of the node which is the destination for this connection.
			@see AudioProcessorGraph::getNodeForId
		*/
		uint32 destNodeId;

		/** The index of the input channel of the destination node to which this
			connection delivers its data.

			If this value is the special number AudioProcessorGraph::midiChannelIndex, then
			it is referring to the destination node's midi input. Otherwise, it is the zero-based
			index of an audio input channel in the destination node.
		*/
		int destChannelIndex;

	private:

		JUCE_LEAK_DETECTOR (Connection);
	};

	/** Deletes all nodes and connections from this graph.

		Any processor objects in the graph will be deleted.
	*/
	void clear();

	/** Returns the number of nodes in the graph. */
	int getNumNodes() const                                         { return nodes.size(); }

	/** Returns a pointer to one of the nodes in the graph.
		This will return nullptr if the index is out of range.
		@see getNodeForId
	*/
	Node* getNode (const int index) const                           { return nodes [index]; }

	/** Searches the graph for a node with the given ID number and returns it.
		If no such node was found, this returns nullptr.
		@see getNode
	*/
	Node* getNodeForId (const uint32 nodeId) const;

	/** Adds a node to the graph.

		This creates a new node in the graph, for the specified processor. Once you have
		added a processor to the graph, the graph owns it and will delete it later when
		it is no longer needed.

		The optional nodeId parameter lets you specify an ID to use for the node, but
		if the value is already in use, this new node will overwrite the old one.

		If this succeeds, it returns a pointer to the newly-created node.
	*/
	Node* addNode (AudioProcessor* newProcessor, uint32 nodeId = 0);

	/** Deletes a node within the graph which has the specified ID.

		This will also delete any connections that are attached to this node.
	*/
	bool removeNode (uint32 nodeId);

	/** Returns the number of connections in the graph. */
	int getNumConnections() const                                       { return connections.size(); }

	/** Returns a pointer to one of the connections in the graph. */
	const Connection* getConnection (int index) const                   { return connections [index]; }

	/** Searches for a connection between some specified channels.

		If no such connection is found, this returns nullptr.
	*/
	const Connection* getConnectionBetween (uint32 sourceNodeId,
											int sourceChannelIndex,
											uint32 destNodeId,
											int destChannelIndex) const;

	/** Returns true if there is a connection between any of the channels of
		two specified nodes.
	*/
	bool isConnected (uint32 possibleSourceNodeId,
					  uint32 possibleDestNodeId) const;

	/** Returns true if it would be legal to connect the specified points.
	*/
	bool canConnect (uint32 sourceNodeId, int sourceChannelIndex,
					 uint32 destNodeId, int destChannelIndex) const;

	/** Attempts to connect two specified channels of two nodes.

		If this isn't allowed (e.g. because you're trying to connect a midi channel
		to an audio one or other such nonsense), then it'll return false.
	*/
	bool addConnection (uint32 sourceNodeId, int sourceChannelIndex,
						uint32 destNodeId, int destChannelIndex);

	/** Deletes the connection with the specified index.

		Returns true if a connection was actually deleted.
	*/
	void removeConnection (int index);

	/** Deletes any connection between two specified points.

		Returns true if a connection was actually deleted.
	*/
	bool removeConnection (uint32 sourceNodeId, int sourceChannelIndex,
						   uint32 destNodeId, int destChannelIndex);

	/** Removes all connections from the specified node.
	*/
	bool disconnectNode (uint32 nodeId);

	/** Returns true if the given connection's channel numbers map on to valid
		channels at each end.
		Even if a connection is valid when created, its status could change if
		a node changes its channel config.
	*/
	bool isConnectionLegal (const Connection* connection) const;

	/** Performs a sanity checks of all the connections.

		This might be useful if some of the processors are doing things like changing
		their channel counts, which could render some connections obsolete.
	*/
	bool removeIllegalConnections();

	/** A special number that represents the midi channel of a node.

		This is used as a channel index value if you want to refer to the midi input
		or output instead of an audio channel.
	*/
	static const int midiChannelIndex;

	/** A special type of AudioProcessor that can live inside an AudioProcessorGraph
		in order to use the audio that comes into and out of the graph itself.

		If you create an AudioGraphIOProcessor in "input" mode, it will act as a
		node in the graph which delivers the audio that is coming into the parent
		graph. This allows you to stream the data to other nodes and process the
		incoming audio.

		Likewise, one of these in "output" mode can be sent data which it will add to
		the sum of data being sent to the graph's output.

		@see AudioProcessorGraph
	*/
	class JUCE_API  AudioGraphIOProcessor     : public AudioPluginInstance
	{
	public:
		/** Specifies the mode in which this processor will operate.
		*/
		enum IODeviceType
		{
			audioInputNode,     /**< In this mode, the processor has output channels
									 representing all the audio input channels that are
									 coming into its parent audio graph. */
			audioOutputNode,    /**< In this mode, the processor has input channels
									 representing all the audio output channels that are
									 going out of its parent audio graph. */
			midiInputNode,      /**< In this mode, the processor has a midi output which
									 delivers the same midi data that is arriving at its
									 parent graph. */
			midiOutputNode      /**< In this mode, the processor has a midi input and
									 any data sent to it will be passed out of the parent
									 graph. */
		};

		/** Returns the mode of this processor. */
		IODeviceType getType() const                                { return type; }

		/** Returns the parent graph to which this processor belongs, or nullptr if it
			hasn't yet been added to one. */
		AudioProcessorGraph* getParentGraph() const                 { return graph; }

		/** True if this is an audio or midi input. */
		bool isInput() const;
		/** True if this is an audio or midi output. */
		bool isOutput() const;

		AudioGraphIOProcessor (const IODeviceType type);
		~AudioGraphIOProcessor();

		const String getName() const;
		void fillInPluginDescription (PluginDescription&) const;

		void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
		void releaseResources();
		void processBlock (AudioSampleBuffer&, MidiBuffer&);

		const String getInputChannelName (int channelIndex) const;
		const String getOutputChannelName (int channelIndex) const;
		bool isInputChannelStereoPair (int index) const;
		bool isOutputChannelStereoPair (int index) const;
		bool acceptsMidi() const;
		bool producesMidi() const;

		bool hasEditor() const;
		AudioProcessorEditor* createEditor();

		int getNumParameters();
		const String getParameterName (int);
		float getParameter (int);
		const String getParameterText (int);
		void setParameter (int, float);

		int getNumPrograms();
		int getCurrentProgram();
		void setCurrentProgram (int);
		const String getProgramName (int);
		void changeProgramName (int, const String&);

		void getStateInformation (juce::MemoryBlock& destData);
		void setStateInformation (const void* data, int sizeInBytes);

		/** @internal */
		void setParentGraph (AudioProcessorGraph*);

	private:
		const IODeviceType type;
		AudioProcessorGraph* graph;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioGraphIOProcessor);
	};

	// AudioProcessor methods:

	const String getName() const;

	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void processBlock (AudioSampleBuffer&, MidiBuffer&);

	const String getInputChannelName (int channelIndex) const;
	const String getOutputChannelName (int channelIndex) const;
	bool isInputChannelStereoPair (int index) const;
	bool isOutputChannelStereoPair (int index) const;

	bool acceptsMidi() const;
	bool producesMidi() const;

	bool hasEditor() const                          { return false; }
	AudioProcessorEditor* createEditor()            { return nullptr; }

	int getNumParameters()                          { return 0; }
	const String getParameterName (int)             { return String::empty; }
	float getParameter (int)                        { return 0; }
	const String getParameterText (int)             { return String::empty; }
	void setParameter (int, float)                  { }

	int getNumPrograms()                            { return 0; }
	int getCurrentProgram()                         { return 0; }
	void setCurrentProgram (int)                    { }
	const String getProgramName (int)               { return String::empty; }
	void changeProgramName (int, const String&)     { }

	void getStateInformation (juce::MemoryBlock& destData);
	void setStateInformation (const void* data, int sizeInBytes);

	/** @internal */
	void handleAsyncUpdate();

private:

	ReferenceCountedArray <Node> nodes;
	OwnedArray <Connection> connections;
	uint32 lastNodeId;
	AudioSampleBuffer renderingBuffers;
	OwnedArray <MidiBuffer> midiBuffers;

	CriticalSection renderLock;
	Array<void*> renderingOps;

	friend class AudioGraphIOProcessor;
	AudioSampleBuffer* currentAudioInputBuffer;
	AudioSampleBuffer currentAudioOutputBuffer;
	MidiBuffer* currentMidiInputBuffer;
	MidiBuffer currentMidiOutputBuffer;

	void clearRenderingSequence();
	void buildRenderingSequence();

	bool isAnInputTo (uint32 possibleInputId, uint32 possibleDestinationId, int recursionCheck) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorGraph);
};

#endif   // __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorGraph.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/*** Start of inlined file: juce_GenericAudioProcessorEditor.h ***/
#ifndef __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__
#define __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/**
	A type of UI component that displays the parameters of an AudioProcessor as
	a simple list of sliders.

	This can be used for showing an editor for a processor that doesn't supply
	its own custom editor.

	@see AudioProcessor
*/
class JUCE_API  GenericAudioProcessorEditor      : public AudioProcessorEditor
{
public:

	GenericAudioProcessorEditor (AudioProcessor* owner);
	~GenericAudioProcessorEditor();

	void paint (Graphics& g);
	void resized();

private:

	PropertyPanel panel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericAudioProcessorEditor);
};

#endif   // __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/*** End of inlined file: juce_GenericAudioProcessorEditor.h ***/


#endif
#ifndef __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_AudioUnitPluginFormat.h ***/
#ifndef __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__
#define __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_AU && JUCE_MAC

/**
	Implements a plugin format manager for AudioUnits.
*/
class JUCE_API  AudioUnitPluginFormat   : public AudioPluginFormat
{
public:

	AudioUnitPluginFormat();
	~AudioUnitPluginFormat();

	String getName() const                { return "AudioUnit"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier);
	StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, bool recursive);
	bool doesPluginStillExist (const PluginDescription& desc);
	FileSearchPath getDefaultLocationsToSearch();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioUnitPluginFormat);
};

#endif

#endif   // __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_AudioUnitPluginFormat.h ***/


#endif
#ifndef __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_DirectXPluginFormat.h ***/
#ifndef __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__
#define __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_DX && JUCE_WINDOWS

//   Sorry, this file is just a placeholder at the moment!...

/**
	Implements a plugin format manager for DirectX plugins.
*/
class JUCE_API  DirectXPluginFormat   : public AudioPluginFormat
{
public:

	DirectXPluginFormat();
	~DirectXPluginFormat();

	String getName() const                { return "DirectX"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier)  { return fileOrIdentifier; }
	FileSearchPath getDefaultLocationsToSearch();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectXPluginFormat);
};

#endif

#endif   // __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_DirectXPluginFormat.h ***/


#endif
#ifndef __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_LADSPAPluginFormat.h ***/
#ifndef __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__
#define __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_LADSPA && JUCE_LINUX

//   Sorry, this file is just a placeholder at the moment!...

/**
	Implements a plugin format manager for DirectX plugins.
*/
class JUCE_API  LADSPAPluginFormat   : public AudioPluginFormat
{
public:

	LADSPAPluginFormat();
	~LADSPAPluginFormat();

	String getName() const                { return "LADSPA"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier)  { return fileOrIdentifier; }
	FileSearchPath getDefaultLocationsToSearch();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LADSPAPluginFormat);
};

#endif

#endif   // __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_LADSPAPluginFormat.h ***/


#endif

/*** Start of inlined file: juce_VSTMidiEventList.h ***/
#ifdef __aeffect__

#ifndef __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__
#define __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__

/** Holds a set of VSTMidiEvent objects and makes it easy to add
	events to the list.

	This is used by both the VST hosting code and the plugin wrapper.
*/
class VSTMidiEventList
{
public:

	VSTMidiEventList()
		: numEventsUsed (0), numEventsAllocated (0)
	{
	}

	~VSTMidiEventList()
	{
		freeEvents();
	}

	void clear()
	{
		numEventsUsed = 0;

		if (events != nullptr)
			events->numEvents = 0;
	}

	void addEvent (const void* const midiData, const int numBytes, const int frameOffset)
	{
		ensureSize (numEventsUsed + 1);

		VstMidiEvent* const e = (VstMidiEvent*) (events->events [numEventsUsed]);
		events->numEvents = ++numEventsUsed;

		if (numBytes <= 4)
		{
			if (e->type == kVstSysExType)
			{
				delete[] (((VstMidiSysexEvent*) e)->sysexDump);
				e->type = kVstMidiType;
				e->byteSize = sizeof (VstMidiEvent);
				e->noteLength = 0;
				e->noteOffset = 0;
				e->detune = 0;
				e->noteOffVelocity = 0;
			}

			e->deltaFrames = frameOffset;
			memcpy (e->midiData, midiData, numBytes);
		}
		else
		{
			VstMidiSysexEvent* const se = (VstMidiSysexEvent*) e;

			if (se->type == kVstSysExType)
				delete[] se->sysexDump;

			se->sysexDump = new char [numBytes];
			memcpy (se->sysexDump, midiData, numBytes);

			se->type = kVstSysExType;
			se->byteSize = sizeof (VstMidiSysexEvent);
			se->deltaFrames = frameOffset;
			se->flags = 0;
			se->dumpBytes = numBytes;
			se->resvd1 = 0;
			se->resvd2 = 0;
		}
	}

	// Handy method to pull the events out of an event buffer supplied by the host
	// or plugin.
	static void addEventsToMidiBuffer (const VstEvents* events, MidiBuffer& dest)
	{
		for (int i = 0; i < events->numEvents; ++i)
		{
			const VstEvent* const e = events->events[i];

			if (e != nullptr)
			{
				if (e->type == kVstMidiType)
				{
					dest.addEvent ((const juce::uint8*) ((const VstMidiEvent*) e)->midiData,
								   4, e->deltaFrames);
				}
				else if (e->type == kVstSysExType)
				{
					dest.addEvent ((const juce::uint8*) ((const VstMidiSysexEvent*) e)->sysexDump,
								   (int) ((const VstMidiSysexEvent*) e)->dumpBytes,
								   e->deltaFrames);
				}
			}
		}
	}

	void ensureSize (int numEventsNeeded)
	{
		if (numEventsNeeded > numEventsAllocated)
		{
			numEventsNeeded = (numEventsNeeded + 32) & ~31;

			const int size = 20 + sizeof (VstEvent*) * numEventsNeeded;

			if (events == nullptr)
				events.calloc (size, 1);
			else
				events.realloc (size, 1);

			for (int i = numEventsAllocated; i < numEventsNeeded; ++i)
				events->events[i] = allocateVSTEvent();

			numEventsAllocated = numEventsNeeded;
		}
	}

	void freeEvents()
	{
		if (events != nullptr)
		{
			for (int i = numEventsAllocated; --i >= 0;)
				freeVSTEvent (events->events[i]);

			events.free();
			numEventsUsed = 0;
			numEventsAllocated = 0;
		}
	}

	HeapBlock <VstEvents> events;

private:
	int numEventsUsed, numEventsAllocated;

	static VstEvent* allocateVSTEvent()
	{
		VstEvent* const e = (VstEvent*) std::calloc (1, sizeof (VstMidiEvent) > sizeof (VstMidiSysexEvent) ? sizeof (VstMidiEvent)
																										   : sizeof (VstMidiSysexEvent));
		e->type = kVstMidiType;
		e->byteSize = sizeof (VstMidiEvent);
		return e;
	}

	static void freeVSTEvent (VstEvent* e)
	{
		if (e->type == kVstSysExType)
			delete[] (((VstMidiSysexEvent*) e)->sysexDump);

		std::free (e);
	}
};

#endif   // __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__
#endif   // __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__

/*** End of inlined file: juce_VSTMidiEventList.h ***/


#ifndef __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_VSTPluginFormat.h ***/
#ifndef __JUCE_VSTPLUGINFORMAT_JUCEHEADER__
#define __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_VST

/**
	Implements a plugin format manager for VSTs.
*/
class JUCE_API  VSTPluginFormat   : public AudioPluginFormat
{
public:

	VSTPluginFormat();
	~VSTPluginFormat();

	String getName() const                { return "VST"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier);
	StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, bool recursive);
	bool doesPluginStillExist (const PluginDescription& desc);
	FileSearchPath getDefaultLocationsToSearch();

private:

	void recursiveFileSearch (StringArray& results, const File& dir, const bool recursive);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginFormat);
};

#endif
#endif   // __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_VSTPluginFormat.h ***/


#endif
#ifndef __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

#endif
#ifndef __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/*** Start of inlined file: juce_PluginDirectoryScanner.h ***/
#ifndef __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__
#define __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/**
	Scans a directory for plugins, and adds them to a KnownPluginList.

	To use one of these, create it and call scanNextFile() repeatedly, until
	it returns false.
*/
class JUCE_API  PluginDirectoryScanner
{
public:

	/**
		Creates a scanner.

		@param listToAddResultsTo       this will get the new types added to it.
		@param formatToLookFor          this is the type of format that you want to look for
		@param directoriesToSearch      the path to search
		@param searchRecursively        true to search recursively
		@param deadMansPedalFile        if this isn't File::nonexistent, then it will
										be used as a file to store the names of any plugins
										that crash during initialisation. If there are
										any plugins listed in it, then these will always
										be scanned after all other possible files have
										been tried - in this way, even if there's a few
										dodgy plugins in your path, then a couple of rescans
										will still manage to find all the proper plugins.
										It's probably best to choose a file in the user's
										application data directory (alongside your app's
										settings file) for this. The file format it uses
										is just a list of filenames of the modules that
										failed.
	*/
	PluginDirectoryScanner (KnownPluginList& listToAddResultsTo,
							AudioPluginFormat& formatToLookFor,
							FileSearchPath directoriesToSearch,
							bool searchRecursively,
							const File& deadMansPedalFile);

	/** Destructor. */
	~PluginDirectoryScanner();

	/** Tries the next likely-looking file.

		If dontRescanIfAlreadyInList is true, then the file will only be loaded and
		re-tested if it's not already in the list, or if the file's modification
		time has changed since the list was created. If dontRescanIfAlreadyInList is
		false, the file will always be reloaded and tested.

		Returns false when there are no more files to try.
	*/
	bool scanNextFile (bool dontRescanIfAlreadyInList);

	/** Skips over the next file without scanning it.
		Returns false when there are no more files to try.
	*/
	bool skipNextFile();

	/** Returns the description of the plugin that will be scanned during the next
		call to scanNextFile().

		This is handy if you want to show the user which file is currently getting
		scanned.
	*/
	const String getNextPluginFileThatWillBeScanned() const;

	/** Returns the estimated progress, between 0 and 1.
	*/
	float getProgress() const                                       { return progress; }

	/** This returns a list of all the filenames of things that looked like being
		a plugin file, but which failed to open for some reason.
	*/
	const StringArray& getFailedFiles() const noexcept              { return failedFiles; }

private:

	KnownPluginList& list;
	AudioPluginFormat& format;
	StringArray filesOrIdentifiersToScan;
	File deadMansPedalFile;
	StringArray failedFiles;
	int nextIndex;
	float progress;

	StringArray getDeadMansPedalFile();
	void setDeadMansPedalFile (const StringArray& newContents);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginDirectoryScanner);
};

#endif   // __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/*** End of inlined file: juce_PluginDirectoryScanner.h ***/


#endif
#ifndef __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_PluginListComponent.h ***/
#ifndef __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__
#define __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/**
	A component displaying a list of plugins, with options to scan for them,
	add, remove and sort them.
*/
class JUCE_API  PluginListComponent   : public Component,
										public FileDragAndDropTarget,
										public ListBoxModel,
										private ChangeListener,
										private ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
										private Timer
{
public:

	/**
		Creates the list component.

		For info about the deadMansPedalFile, see the PluginDirectoryScanner constructor.

		The properties file, if supplied, is used to store the user's last search paths.
	*/
	PluginListComponent (KnownPluginList& listToRepresent,
						 const File& deadMansPedalFile,
						 PropertiesFile* propertiesToUse);

	/** Destructor. */
	~PluginListComponent();

	/** @internal */
	void resized();
	/** @internal */
	bool isInterestedInFileDrag (const StringArray&);
	/** @internal */
	void filesDropped (const StringArray&, int, int);
	/** @internal */
	int getNumRows();
	/** @internal */
	void paintListBoxItem (int row, Graphics&, int width, int height, bool rowIsSelected);
	/** @internal */
	void deleteKeyPressed (int lastRowSelected);

private:

	KnownPluginList& list;
	File deadMansPedalFile;
	ListBox listBox;
	TextButton optionsButton;
	PropertiesFile* propertiesToUse;
	int typeToScan;

	void scanFor (AudioPluginFormat*);
	static void optionsMenuStaticCallback (int result, PluginListComponent*);
	void optionsMenuCallback (int result);
	void updateList();

	void buttonClicked (Button*);
	void changeListenerCallback (ChangeBroadcaster*);
	void timerCallback();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListComponent);
};

#endif   // __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_PluginListComponent.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_PROCESSORS_JUCEHEADER__

/*** End of inlined file: juce_audio_processors.h ***/

