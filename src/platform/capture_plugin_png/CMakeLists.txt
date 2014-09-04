# include opencv library
include_directories(${OpenCV_INCLUDE_DIRS})

# include alvar library
include_directories(${ALVAR_INCLUDE_DIRS})

# ensure plugin api is exported when building library
add_definitions(-DALVAR_Capture_Plugin_Png_BUILD)

# define plugin library source files to compile
set(ALVARCapturePluginPng_HEADERS
    CapturePluginPng.h
)
set(ALVARCapturePluginPng_SOURCES
    CapturePluginPng.cpp
)

# add source files to plugin library and set name and properties
add_library(ALVARCapturePluginPng MODULE
    ${ALVARCapturePluginPng_HEADERS}
    ${ALVARCapturePluginPng_SOURCES}
)
set_target_properties(ALVARCapturePluginPng PROPERTIES OUTPUT_NAME alvarcapturepluginpng${ALVAR_VERSION_NODOTS})
set_target_properties(ALVARCapturePluginPng PROPERTIES DEBUG_POSTFIX d)

# link plugin library target to required libraries
target_link_libraries(ALVARCapturePluginPng
    ALVAR
    ALVARPlatform
    ${OpenCV_LIBRARIES}
)

# install headers
if(ALVAR_PACKAGE MATCHES sdk)
    install(FILES ${ALVARCapturePluginFile_HEADERS} DESTINATION include/platform/capture_plugin_png)
endif(ALVAR_PACKAGE MATCHES sdk)

# install target
install(TARGETS ALVARCapturePluginPng DESTINATION bin/alvarplugins)

# hack to setup environment runtime path
if(NOT ${MSVC_IDE})
    list(APPEND ALVAR_RUNTIME
        ${CMAKE_CURRENT_BINARY_DIR}\;
    )
else(NOT ${MSVC_IDE})
    list(APPEND ALVAR_RUNTIME
        ${CMAKE_CURRENT_BINARY_DIR}/Debug\;
        ${CMAKE_CURRENT_BINARY_DIR}/Release\;
    )
endif(NOT ${MSVC_IDE})
set(ALVAR_RUNTIME ${ALVAR_RUNTIME} PARENT_SCOPE)
