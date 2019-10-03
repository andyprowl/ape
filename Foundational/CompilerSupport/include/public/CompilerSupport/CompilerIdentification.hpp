//--------------------------------------------------------------------------------------------------
// Honeywell Proprietary
// This document and all information and expression contained herein are the property of
// Honeywell International Inc., are loaned in confidence, and may not, in whole or in
// part, be used, duplicated or disclosed for any purpose without prior written permission
// of Honeywell International Inc. This document is an unpublished work.
// Copyright 2011 Honeywell International Inc. All rights reserved.
//--------------------------------------------------------------------------------------------------

#pragma once

#if defined(_MSC_VER)

    #define COMPILER_MSVC

#endif

#if defined(__clang__)
    
#   define COMPILER_CLANG

#endif

#if (defined(__GNUC__) && !defined(__clang__))

#   define COMPILER_GCC

#endif
