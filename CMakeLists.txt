cmake_minimum_required(VERSION 3.26.3)

project(
  e172_sdl_impl
  LANGUAGES CXX
  VERSION 0.7.0
  DESCRIPTION "SDL2 implementation of e172"
  HOMEPAGE_URL "https://github.com/burbokop/e172_sdl_impl")

option(ENABLE_FIND_E172_PACKAGE
       "Find e172 package (searches for link and include directories if OFF)"
       ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED)

set(INSTALL_NAMESPACE e172)
set(INSTALL_IMPL_NAMESPACE sdl)

set(INSTALLDIR
    "${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAMESPACE}/impl/${INSTALL_IMPL_NAMESPACE}"
)

target_sources(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/effects/anaglyph.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/effects/visualeffect.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/audioprovider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/audioprovider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/eventprovider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/eventprovider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/graphicsprovider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/renderer.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/renderer.h>
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/effects/anaglyph.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/effects/visualeffect.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/audioprovider.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/eventprovider.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/renderer.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/private/rotozoom.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/private/rotozoom.h
          ${CMAKE_CURRENT_LIST_DIR}/src/private/spm.cpp
          ${CMAKE_CURRENT_LIST_DIR}/src/private/spm.h)

if(ENABLE_FIND_E172_PACKAGE)
  find_package(e172 REQUIRED)
endif()

if($ENV{WITH_MAGURA})
  find_package(magura REQUIRED)
  target_connect_magura(${PROJECT_NAME})
else($ENV{WITH_MAGURA})
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_ttf
                                                SDL2_mixer)
endif($ENV{WITH_MAGURA})

if(ENABLE_FIND_E172_PACKAGE)
  target_link_libraries(${PROJECT_NAME} PRIVATE e172::e172)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE e172)
endif()

if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL
                                                "GNU"))
  target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
  DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAMESPACE}/impl/${INSTALL_IMPL_NAMESPACE}"
  FILES_MATCHING
  PATTERN "*.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  EXPORT ${PROJECT_NAME}_targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${INSTALL_NAMESPACE}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
