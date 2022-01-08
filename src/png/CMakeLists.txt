
list(APPEND
    LODEPNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lodepng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lodepng.h
)

add_library(lodepng STATIC ${LODEPNG_SOURCES})