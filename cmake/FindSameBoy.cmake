#
# SameBoy
# SAMEBOY_PATH
# SAMEBOY_INCLUDE_DIRS
# SAMEBOY_LIBRARIES
# SAMEBOY_CFLAGS

if(SAMEBOY_PATH)
    return()
endif()

set(SAMEBOY_PATH ${PROJECT_SOURCE_DIR}/deps/SameBoy)
if(NOT EXISTS ${SAMEBOY_PATH}/README.md)
    message(FATAL_ERROR "Unable to find SameBoy.\n Make sure to download the submodules")
endif()


option(SAMEBOY_STATIC "" ON)
mark_as_advanced(SAMEBOY_STATIC)

file(GLOB SAMEBOY_CORE_SOURCES ${SAMEBOY_PATH}/Core/*.c)

add_compile_definitions(GB_INTERNAL)
add_compile_definitions(GB_VERSION="lib")
if(SAMEBOY_STATIC)
    add_library(SameBoy STATIC ${SAMEBOY_CORE_SOURCES})
else()
    add_library(SameBoy SHARED ${SAMEBOY_CORE_SOURCES})
endif()

set(SAMEBOY_INCLUDE_DIRS ${SAMEBOY_PATH})
set(SAMEBOY_LIBRARIES SameBoy)
set(SAMEBOY_CFLAGS)