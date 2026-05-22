/*-----------------------------------------------------------------------------
 * eabase.h
 *
 * Copyright (c) Electronic Arts Inc. All rights reserved.
 *---------------------------------------------------------------------------*/


#ifndef INCLUDED_eabase_H
#define INCLUDED_eabase_H


// Identify the compiler and declare the EA_COMPILER_xxxx defines
#include <EABase/config/eacompiler.h>

// Identify traits which this compiler supports, or does not support
#include <EABase/config/eacompilertraits.h>

// Identify the platform and declare the EA_xxxx defines
#include <EABase/config/eaplatform.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif

// Always include version.h for backwards compatibility.
#include <EABase/version.h>


// ------------------------------------------------------------------------
// EAArrayCount
//
// Returns the count of items in a built-in C array. This is a common technique
// which is often used to help properly calculate the number of items in an
// array at runtime in order to prevent overruns, etc.
//
// Example usage:
//     int array[75];
//     size_t arrayCount = EAArrayCount(array); // arrayCount is 75.
//
#if defined(EA_COMPILER_NO_CONSTEXPR)
	#ifndef EAArrayCount
		#define EAArrayCount(x) (sizeof(x) / sizeof(x[0]))
	#endif
#else
	// This C++11 version is a little smarter than the macro version above;
	// it can tell the difference between arrays and pointers. Other simpler
	// templated versions have failed in various subtle ways.

	template <typename T, size_t N>
	char (&EAArraySizeHelper(T (&x)[N]))[N];

	template <typename T, size_t N>
	char (&EAArraySizeHelper(T (&&x)[N]))[N];

	#define EAArrayCount(x) (sizeof(EAArraySizeHelper(x)))
#endif



// ------------------------------------------------------------------------
// EA_IS_ENABLED
//
// EA_IS_ENABLED is intended to be used for detecting if compile time features are enabled or disabled.
//
// It has some advantages over using a standard #if or #ifdef tests:
//	1) Fails to compile when passes numeric macro values. Valid options are strictly enabled or disabled.
//	2) Fails to compile when passed undefined macro values rather than disabling by default
//	3) Fails to compile when the passed macro is defined to but empty
//
// To use the macro, the calling code should create a define for the feature to enable or disable.  This feature define
// must be set to either EA_ENABLED or EA_DISABLED.  (Do not try to set the feature define directly to some other
// value.)
//
// Note: These macros are analogous to the Frostbite macro FB_USING used in combination with FB_OFF / FB_ON and are
// designed to be compatible to support gradual migration.
//
// Example usage:
//
//      // The USER_PROVIDED_FEATURE_DEFINE should be defined as either
//      // EA_ENABLED or EA_DISABLED.
//      #define USER_PROVIDED_FEATURE_DEFINE EA_ENABLED
//
//      #if EA_IS_ENABLED(USER_PROVIDED_FEATURE_DEFINE)
//          // USER_PROVIDED_FEATURE_DEFINE is enabled
//      #else
//          // USER_PROVIDED_FEATURE_DEFINE is disabled
//      #endif
//
#define EA_ENABLED              111-
#define EA_DISABLED             333-
// NOTE: Numeric values for x will produce a parse error while empty values produce a divide by zero, and the test is a bool for proper negation behavior
#define EA_IS_ENABLED(x) (333 == 333 * 111 / ((x 0) * (((x 0) == 333 ? 1 : 0) + ((x 0) == 111 ? 1 : 0))))


#if !defined(EA_GET_INSTRUCTION_POINTER)
	#if defined(EA_COMPILER_CLANG) || defined(EA_COMPILER_GNUC)
		#define EA_GET_INSTRUCTION_POINTER() []()EA_NO_INLINE{ return __builtin_return_address(0); }()
	#elif defined(EA_COMPILER_MSVC) && !defined(_MANAGED)
		#include <intrin.h>
		#define EA_GET_INSTRUCTION_POINTER() []()EA_NO_INLINE{ return _ReturnAddress(); }()
	#else
		#define EA_GET_INSTRUCTION_POINTER() nullptr
	#endif
#endif


// Extra

typedef char char8_t;
#define EA_WCHAR_UNIQUE 1


#endif // Header include guard




