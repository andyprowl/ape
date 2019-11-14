//--------------------------------------------------------------------------------------------------
// Honeywell Proprietary
// This document and all information and expression contained herein are the property of
// Honeywell International Inc., are loaned in confidence, and may not, in whole or in
// part, be used, duplicated or disclosed for any purpose without prior written permission
// of Honeywell International Inc. This document is an unpublished work.
// Copyright 2011 Honeywell International Inc. All rights reserved.
//--------------------------------------------------------------------------------------------------

#pragma once

#include <Basix/CompilerSupport/CompilerIdentification.hpp>

#ifdef COMPILER_MSVC

#   define disableDeadCodeWarnings() \
__pragma(warning(disable: 4702))

#   define restoreDeadCodeWarnings() \
__pragma(warning(default: 4702))

#   define disableCompilerWarnings() \
__pragma(warning(push, 0))

#define restoreCompilerWarnings() \
__pragma(warning(pop))

#   define maybeUnused \
__pragma(warning(suppress: 4100 4189))

#endif

#ifdef COMPILER_CLANG

#   define disableDeadCodeWarnings()

#   define restoreDeadCodeWarnings()

#   define disableCompilerWarnings() \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wall\"") \
_Pragma("clang diagnostic ignored \"-Wextra\"") \
_Pragma("clang diagnostic ignored \"-Wpedantic\"") \
_Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
_Pragma("clang diagnostic ignored \"-Wshorten-64-to-32\"")

#   define restoreCompilerWarnings() \
_Pragma("clang diagnostic pop")

#   define maybeUnused \
__attribute__((__unused__))

#endif

#ifdef COMPILER_GCC

#   define disableDeadCodeWarnings()

#   define restoreDeadCodeWarnings()

#   define disableCompilerWarnings() \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wall\"") \
_Pragma("GCC diagnostic ignored \"-Wextra\"") \
_Pragma("GCC diagnostic ignored \"-Wpedantic\"") \
_Pragma("GCC diagnostic ignored \"-Wunused-variable\"")

#   define restoreCompilerWarnings() \
_Pragma("GCC diagnostic pop")

#   define maybeUnused \
__attribute__((__unused__))

#endif

// Allows writing e.g. attribute(maybeUnused)
#define attribute(x) x
