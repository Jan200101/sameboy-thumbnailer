cmake_minimum_required(VERSION 3.18)

if(CMAKE_POLICY_DEFAULT_CMP0017 OR CMAKE_POLICY_DEFAULT_CMP0020)
	# touch these to remove warnings
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

project(sameboy-thumbnailer VERSION 0.0.0 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(libpng)
option(USE_LIBPNG "Use libpng instead of lodepng" ${LIBPNG_FOUND})

add_compile_definitions(VERSION=${CMAKE_PROJECT_VERSION})


add_subdirectory(src)