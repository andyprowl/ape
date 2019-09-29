#---------------------------------------------------------------------------------------------------
# Honeywell Proprietary
# This document and all information and expression contained herein are the property of
# Honeywell International Inc., are loaned in confidence, and may not, in whole or in
# part, be used, duplicated or disclosed for any purpose without prior written permission
# of Honeywell International Inc. This document is an unpublished work.
# Copyright 2011 Honeywell International Inc. All rights reserved.
# --------------------------------------------------------------------------------------------------

macro(SetCpp17StandardCompliance)

    set(CMAKE_CXX_STANDARD 17)

    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_CXX_EXTENSIONS OFF)

endmacro()

macro(SetStrictWarningPolicy)

    SetCpp17StandardCompliance()

    # Set sufficiently high warning level 
    if(MSVC)
    
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")

        # Force to always compile with W4
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        
        else()
        
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        
        endif()
        
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    
    else()
        
        # Update if necessary
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-long-long -pedantic")
        
    endif()

endmacro()

# This implementation currently deals with VS only
macro(
    EnableWholeProgramOptimization
    CONFIGURATION_NAME)

    string(TOUPPER ${CONFIGURATION_NAME} UPPERCASE_CONFIGURATION_NAME)

    if(MSVC)

        message("Enabling whole program optimization for configuration ${CONFIGURATION_NAME}")
        
        set(
            CMAKE_CXX_FLAGS_${UPPERCASE_CONFIGURATION_NAME}
            "${CMAKE_CXX_FLAGS_${UPPERCASE_CONFIGURATION_NAME}} /GL")

        set(
            CMAKE_EXE_LINKER_FLAGS_${UPPERCASE_CONFIGURATION_NAME} 
            "${CMAKE_EXE_LINKER_FLAGS_${UPPERCASE_CONFIGURATION_NAME}} /LTCG")

        set(
            CMAKE_STATIC_LINKER_FLAGS_${UPPERCASE_CONFIGURATION_NAME}
            "${CMAKE_STATIC_LINKER_FLAGS_${UPPERCASE_CONFIGURATION_NAME}} /LTCG")

    endif()

endmacro()

macro(SetupWholeProgramOptimization)

    foreach(WPO_CONFIG ${ENABLE_WPO_FOR_CONFIGS})
    
        EnableWholeProgramOptimization(${WPO_CONFIG})
    
    endforeach()

endmacro()
