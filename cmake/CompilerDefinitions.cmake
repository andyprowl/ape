#---------------------------------------------------------------------------------------------------
# Honeywell Proprietary
# This document and all information and expression contained herein are the property of
# Honeywell International Inc., are loaned in confidence, and may not, in whole or in
# part, be used, duplicated or disclosed for any purpose without prior written permission
# of Honeywell International Inc. This document is an unpublished work.
# Copyright 2011 Honeywell International Inc. All rights reserved.
# --------------------------------------------------------------------------------------------------

function(AddPrivateDefinitions SYMBOLS)

    target_compile_definitions(${PROJECT_NAME} PRIVATE ${SYMBOLS})

endfunction()
