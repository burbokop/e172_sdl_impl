include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(sdl_impl SHARED
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.h
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.h
    ${CMAKE_CURRENT_LIST_DIR}/src/rotozoom.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/rotozoom.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlaudioprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlaudioprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sdleventhandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sdleventhandler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlgraphicsprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlgraphicsprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlrenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sdlrenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/spm.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/spm.h
    )

if($ENV{WITH_MAGURA})
    find_package(magura REQUIRED)
    target_connect_magura(sdl_impl)
    target_link_libraries(sdl_impl
        e172
        )
else($ENV{WITH_MAGURA})
    target_link_libraries(sdl_impl
        SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        e172
        )
endif($ENV{WITH_MAGURA})


