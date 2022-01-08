#
# SAMEBOY_TEMPLATE_SOURCES

find_package(SameBoy REQUIRED)
set(SAMEBOY_QUICKLOOK_PATH ${SAMEBOY_PATH}/QuickLook)

list(APPEND
    TEMPLATE_LIST
    CartridgeTemplate
    ColorCartridgeTemplate
    UniversalCartridgeTemplate
)

foreach(TEMPLATE_FILE ${TEMPLATE_LIST})
    add_custom_command(
        OUTPUT ${TEMPLATE_FILE}_png.c
        COMMAND ${CMAKE_COMMAND}
        "-Dbin_in=${SAMEBOY_QUICKLOOK_PATH}/${TEMPLATE_FILE}.png"
        -P ${CMAKE_SOURCE_DIR}/cmake/FileEmbed.cmake
    )
    list(APPEND
        SAMEBOY_TEMPLATE_SOURCES
        ${TEMPLATE_FILE}_png.c
    )
endforeach()

add_custom_target(sameboy_template DEPENDS ${SAMEBOY_TEMPLATE_SOURCES})
