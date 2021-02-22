/**
 * @file "config.hpp"
 * Just some macros and useful definitions
 *
 * @author Alexandre Chetafi
 */

#pragma once

#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

#define	EZ_VERSION_MAJOR 0
#define	EZ_VERSION_MINOR 0
#define	EZ_VERSION_PATCH 1

#define PREDEF_VERSION(v,r,p) (((v) << 24) + ((r) << 16) + (p))
#define EZ_VERSION PREDEF_VERSION(EZ_VERSION_MAJOR, EZ_VERSION_MINOR, EZ_VERSION_PATCH)


#if defined(_WIN32) || defined(_WIN64)
#	define EZ_OS_WINDOWS
#	define NOVTABLE  __declspec(novtable)
#	include <windows.h>
#	if defined(max)
#		undef max
#	endif
#	if defined(min)
#		undef min
#	endif
#else
#	define EZ_OS_LINUX
#	define NOVTABLE
#endif

#if defined(__cplusplus)
#	define EZ_INLINE inline
#else
#	define EZ_INLINE
#endif

#ifdef EXPORT_STATIC
#	define EZ_API_EXPORT __declspec(dllexport)
#	define EZ_API_IMPORT __declspec(dllimport)
#else
#	define EZ_API_EXPORT
#	define EZ_API_IMPORT
#endif

#ifdef	_MSV_VER
#	define	EZ_DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__)
#	define SFML_DEPRECATED __attribute__ ((deprecated))
#else
#	define SFML_DEPRECATED
#endif

namespace ez
{
	using Int8 = 	signed char;
	using Uint8 = 	unsigned char;
	using Int16 = 	signed short;
	using Uint16 = 	unsigned short;
	using Int32 = 	signed int;
	using Uint32 = 	unsigned int;
#ifdef _MSC_VER
	using Int64 = 	signed __int64;
	using Uint64 = 	unsigned __int64;
#else
	using Int64 = signed long long;
	using Uint64 = unsigned long long;
#endif
}

#ifdef EZ_OS_WINDOWS
#	define STRUCT_ALIGN_START(name) 	__pragma(push, 1); struct name {
#	define STRUCT_ALIGN_END			}; __pragma(pop);
#elif defined(EZ_OS_LINUX)
#	define STRUCT_ALIGN_START(name) struct name {
#	define STRUCT_ALIGN_END 		} __attribute__((packed));
#else
#	define STRUCT_ALIGN_START(name)
#	define STRUCT_ALIGN_END
#endif


#endif //ENGINE_CONFIG_HPP
