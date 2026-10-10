cmake_minimum_required(VERSION 3.20)
project(Anx VERSION 0.1.0 LANGUAGES CXX)

# Establecemos C++20 de forma estricta
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Directorios de inclusión globales para que 'src' encuentre los headers en 'include'
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
)

# Archivos fuentes y de cabecera de nuestro primer módulo base
add_executable(
    
    ${PROJECT_NAME}
    src/main.cpp
    ${IMGUI_SOURCES}
)


set(IMGUI_SOURCES
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_impl_win32.cpp
    third_party/imgui/imgui_impl_dx11.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d11
    d3dcompiler
    dwmapi
)


