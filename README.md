# sdl_implementation
Library implements e172 io api using SDL2 tools.

## adding sdl_implementation to your c++ project
1. 
```
git submodule add https://github.com/burbokop/sdl_implementation.git
git submodule add https://github.com/burbokop/e172.git (because sdl_implementation depends on e172 lib)
```
in `CMakeLists.txt`:
```
include(${CMAKE_CURRENT_LIST_DIR}/sdl_implementation/sdl_implementation.cmake)
target_link_libraries(your_executable_or_lib
    sdl_impl
)
```
