#
# LIBPNG_INCLUDE_DIRS
# LIBPNG_LIBRARIES
# LIBPNG_CFLAGS
# LIBPNG_FOUND


find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(_LIBPNG libpng)

	if (BUILD_STATIC AND NOT _LIBPNG_FOUND)
		 message(FATAL_ERROR "Cannot find static build information")
	endif()
endif()
set(LIBPNG_FOUND ${_LIBPNG_FOUND})

if (_LIBPNG_FOUND) # we can rely on pkg-config

	if (NOT BUILD_STATIC)
		set(LIBPNG_LIBRARIES ${_LIBPNG_LIBRARIES})
		set(LIBPNG_INCLUDE_DIRS ${_LIBPNG_INCLUDE_DIRS})
		set(LIBPNG_CFLAGS ${_LIBPNG_CFLAGS_OTHER})
	else()
		set(LIBPNG_LIBRARIES ${_LIBPNG_STATIC_LIBRARIES})
		set(LIBPNG_INCLUDE_DIRS ${_LIBPNG_STATIC_INCLUDE_DIRS})
		set(LIBPNG_CFLAGS ${_LIBPNG_STATIC_CFLAGS_OTHER})
	endif()
else()
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_lib_suffix 64)
	else()
		set(_lib_suffix 32)
	endif()

	find_path(LIBPNG_INC
		NAMES png.h
		HINTS
			ENV LIBPNGPath${_lib_suffix}
			ENV LIBPNGPath
			${_LIBPNG_INCLUDE_DIRS}
		PATHS
			/usr/include/libpng16 /usr/local/include/libpng16
			/usr/include/libpng /usr/local/include/libpng)

	find_library(LIBPNG_LIB
		NAMES ${_LIBPNG_LIBRARIES} libpng.so libpng16.so
		HINTS
			ENV LIBPNGPath${_lib_suffix}
			ENV LIBPNGPath
			${_LIBPNG_LIBRARY_DIRS}
			${_LIBPNG_STATIC_LIBRARY_DIRS}
		PATHS
			/usr/lib${_lib_suffix} /usr/local/lib${_lib_suffix}
			/usr/lib /usr/local/lib)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(LIBPNG DEFAULT_MSG LIBPNG_LIB LIBPNG_INC)
	mark_as_advanced(LIBPNG_INC LIBPNG_LIB)

	if(LIBPNG_FOUND)
		set(LIBPNG_INCLUDE_DIRS ${LIBPNG_INC})
		set(LIBPNG_LIBRARIES ${LIBPNG_LIB})
		if (BUILD_STATIC)
			set(LIBPNG_LIBRARIES ${LIBPNG_LIBRARIES} ${_LIBPNG_STATIC_LIBRARIES})
		endif()
	endif()
endif()