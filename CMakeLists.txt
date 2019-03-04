cmake_minimum_required(VERSION 3.9)
PROJECT("FrameworkEnhancement")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-stdlib=libc++ -isystem /usr/include/c++/v1)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
endif()

option(OPTION_BUILD_DOCS OFF)
option(OPTION_BUILD_EXAMPLES OFF)
option(OPTION_BUILD_GPU_TESTS OFF)
option(OPTION_BUILD_TESTS OFF)
option(OPTION_BUILD_TOOLS OFF)

# this should be removed as soon as glbinding is updated.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING")

add_subdirectory(extern/glbinding)

set(VISCOM_DO_PROFILING ON CACHE BOOL "Turn on profiling.")

file(GLOB_RECURSE SHADER_FILES_ENH ${PROJECT_SOURCE_DIR}/resources/shader/*.*)
list(FILTER SHADER_FILES_ENH EXCLUDE REGEX ".*\.gen$")
file(GLOB_RECURSE SRC_FILES_ENH
    src/enh/*.h
    src/enh/*.cpp
    src/enh/*.inl)

foreach(f ${SRC_FILES_ENH})
    file(RELATIVE_PATH SRCGR ${PROJECT_SOURCE_DIR} ${f})
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

foreach(f ${SHADER_FILES_ENH})
    file(RELATIVE_PATH SRCGR ${PROJECT_SOURCE_DIR}/resources ${f})
    string(REGEX REPLACE "(shader.*)(/[^/]*\\.[^/]+)$" "\\1" SRCGR ${SRCGR})
    string(LENGTH ${SRCGR} SRCGRLEN)
    if (${SRCGRLEN} GREATER 6)
        string(REPLACE / \\ SRCGR ${SRCGR})
        string(SUBSTRING ${SRCGR} 7 -1 SRCGR)
        source_group("shader\\enhanced\\${SRCGR}" FILES ${f})
    else()
        source_group("shader\\enhanced" FILES ${f})
    endif()
endforeach()

list(APPEND ENH_INCLUDE_DIRS
    extern/glbinding/source/glbinding/include
    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/include
    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/glbinding/include
#    extern/glbinding/source/glbinding-aux/include
#    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/glbinding-aux/include
    extern/cereal/include)

list(APPEND ENH_LIBS glbinding)
# list(APPEND ENH_LIBS glbinding glbinding-aux)

add_library(FrameworkEnhancement ${SRC_FILES_ENH} ${SHADER_FILES_ENH})
set_property(TARGET FrameworkEnhancement PROPERTY CXX_STANDARD 17)
target_include_directories(FrameworkEnhancement PUBLIC src)
target_include_directories(FrameworkEnhancement SYSTEM PUBLIC ${ENH_INCLUDE_DIRS})
target_link_libraries(FrameworkEnhancement PUBLIC VISCOMCore ${ENH_LIBS})
set_build_flags(FrameworkEnhancement)

target_compile_definitions(FrameworkEnhancement PUBLIC $<$<CONFIG:DebugOpenGLCalls>:VISCOM_OGL_DEBUG_MSGS> GLFW_INCLUDE_NONE _SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING)
if (VISCOM_DO_PROFILING)
    target_compile_definitions(FrameworkEnhancement PUBLIC ENABLE_PROFILING)
endif()

