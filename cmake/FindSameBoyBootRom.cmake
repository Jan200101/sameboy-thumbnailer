#
# SAMEBOY_BOOTROM_SOURCES

find_package(SameBoy REQUIRED)
set(SAMEBOY_BOOTROM_PATH ${SAMEBOY_PATH}/BootROMs)

set(SAMEBOY_BOOTROM_VERSION "cgb_boot" CACHE STRING "")
set(SAMEBOY_BOOTROM_SIZE "2304" CACHE STRING "")
mark_as_advanced(SAMEBOY_BOOTROM_VERSION SAMEBOY_BOOTROM_SIZE)

add_executable(pb12 ${SAMEBOY_BOOTROM_PATH}/pb12.c)

find_program(RGBASM rgbasm REQUIRED)
find_program(RGBLINK rgblink REQUIRED)
find_program(RGBGFX rgbgfx REQUIRED)
mark_as_advanced(RGBASM RGBLINK RGBGFX)

# custom build step
add_custom_command(
    OUTPUT SameBoyLogo.pb12
    COMMAND
        ${RGBGFX} -h -u -o SameBoyLogo.2bpp ${SAMEBOY_BOOTROM_PATH}/SameBoyLogo.png
    COMMAND
        pb12 < SameBoyLogo.2bpp > SameBoyLogo.pb12
    MAIN_DEPENDENCY ${SAMEBOY_BOOTROM_PATH}/SameBoyLogo.png
    DEPENDS pb12
    VERBATIM
)

add_custom_command(
    OUTPUT ${SAMEBOY_BOOTROM_VERSION}.bin
    COMMAND
        ${RGBASM} -o ${SAMEBOY_BOOTROM_VERSION}.tmp ${SAMEBOY_BOOTROM_PATH}/${SAMEBOY_BOOTROM_VERSION}.asm
    COMMAND
        ${RGBLINK} -o ${SAMEBOY_BOOTROM_VERSION}.tmp2 ${SAMEBOY_BOOTROM_VERSION}.tmp
    COMMAND
        dd if=${SAMEBOY_BOOTROM_VERSION}.tmp2 of=${SAMEBOY_BOOTROM_VERSION}.bin count=1 bs=${SAMEBOY_BOOTROM_SIZE}
    DEPENDS SameBoyLogo.pb12
    VERBATIM
)

add_custom_target(sameboy_bootrom DEPENDS ${SAMEBOY_BOOTROM_VERSION}.bin)

add_custom_command(
    OUTPUT ${SAMEBOY_BOOTROM_VERSION}_bin.c
    COMMAND ${CMAKE_COMMAND}
    "-Dbin_in=${SAMEBOY_BOOTROM_VERSION}.bin"
    -P ${CMAKE_SOURCE_DIR}/cmake/FileEmbed.cmake
    DEPENDS ${SAMEBOY_BOOTROM_VERSION}.bin
    VERBATIM
)

list(APPEND
    SAMEBOY_BOOTROM_SOURCES
    ${SAMEBOY_BOOTROM_VERSION}_bin.c
)
