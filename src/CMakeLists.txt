
find_package(SameBoy REQUIRED)
find_package(SameBoyBootRom REQUIRED)
find_package(SameBoyTemplate REQUIRED)

if (USE_LIBPNG)
    find_package(libpng REQUIRED)
    add_compile_definitions(USE_LIBPNG)
    set(PNG_INCLUDE_DIRS ${LIBPNG_INCLUDE_DIRS})
    set(PNG_LIBRARIES ${LIBPNG_LIBRARIES})
else()
    find_package(lodepng REQUIRED)
    add_compile_definitions(USE_LODEPNG)
    set(PNG_INCLUDE_DIRS ${LODEPNG_INCLUDE_DIRS})
    set(PNG_LIBRARIES ${LODEPNG_LIBRARIES})
endif()

include_directories(${SAMEBOY_INCLUDE_DIRS})

list(APPEND
    THUMBNAILER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/embed.h
    ${CMAKE_CURRENT_SOURCE_DIR}/get_image_for_rom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/get_image_for_rom.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${SAMEBOY_BOOTROM_SOURCES}
    ${SAMEBOY_TEMPLATE_SOURCES}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(sameboy-thumbnailer ${THUMBNAILER_SOURCES})

add_dependencies(sameboy-thumbnailer sameboy_bootrom sameboy_template)

target_include_directories(sameboy-thumbnailer PUBLIC ${PNG_INCLUDE_DIRS})

target_link_libraries(sameboy-thumbnailer LINK_PUBLIC ${PNG_LIBRARIES})
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC ${SAMEBOY_LIBRARIES})
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC m)
