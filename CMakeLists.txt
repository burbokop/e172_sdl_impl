cmake_minimum_required(VERSION 3.26.3)
project(e172_sdl_impl LANGUAGES CXX VERSION 0.7.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

add_library(e172_sdl_impl SHARED)

target_sources(e172_sdl_impl
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.h
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audioprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audioprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/eventprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/eventprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/renderer.h
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/private/rotozoom.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/private/rotozoom.h
    ${CMAKE_CURRENT_LIST_DIR}/src/private/spm.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/private/spm.h)

if($ENV{WITH_MAGURA})
    find_package(magura REQUIRED)
    target_connect_magura(e172_sdl_impl)
    target_link_libraries(e172_sdl_impl
        e172
        )
else($ENV{WITH_MAGURA})
    target_link_libraries(e172_sdl_impl
        SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        e172
        )
endif($ENV{WITH_MAGURA})
