cmake_minimum_required(VERSION 3.10)
project(MyOwnWorld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# ImGui and ImPlot sources
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imgui)
set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/implot)

# Check if ImGui files exist
if(NOT EXISTS "${IMGUI_DIR}/imgui.cpp")
    message(FATAL_ERROR "ImGui not found at ${IMGUI_DIR}. Please ensure ImGui is cloned in thirdparty/imgui.")
endif()

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# Project sources
set(SOURCE_FILES
    src/Main.cpp
    src/Field.cpp
    src/Organism.cpp
)

# Include directories
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    src
)

# Executable
add_executable(MyOwnWorld ${SOURCE_FILES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES})

# Link libraries
target_link_libraries(MyOwnWorld ${SDL2_LIBRARIES})
