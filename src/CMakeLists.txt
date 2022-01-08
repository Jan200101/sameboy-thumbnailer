
find_package(SameBoy REQUIRED)
find_package(SameBoyBootRom REQUIRED)
find_package(SameBoyTemplate REQUIRED)

include_directories(${SAMEBOY_INCLUDE_DIRS})

add_subdirectory(png)

list(APPEND
    THUMBNAILER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/get_image_for_rom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/get_image_for_rom.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${SAMEBOY_BOOTROM_SOURCES}
    ${SAMEBOY_TEMPLATE_SOURCES}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(sameboy-thumbnailer ${THUMBNAILER_SOURCES})

add_dependencies(sameboy-thumbnailer sameboy_bootrom sameboy_template)

target_link_libraries(sameboy-thumbnailer LINK_PUBLIC lodepng)
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC ${SAMEBOY_LIBRARIES})
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(sameboy-thumbnailer LINK_PUBLIC m)
