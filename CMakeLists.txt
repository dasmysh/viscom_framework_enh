cmake_minimum_required(VERSION 3.9)
PROJECT("FrameworkEnhancement")

# this should be removed as soon as glbinding is updated.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING")

add_subdirectory(extern/glbinding)

set(VISCOM_DO_PROFILING ON CACHE BOOL "Turn on profiling.")

file(GLOB_RECURSE SHADER_FILES_ENH CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/resources/shader/*.*)
list(FILTER SHADER_FILES_ENH EXCLUDE REGEX ".*\.gen$")
source_group(TREE ${PROJECT_SOURCE_DIR}/resources/shader PREFIX "shader" FILES ${SHADER_FILES_ENH})


file(GLOB_RECURSE SRC_FILES_ENH CONFIGURE_DEPENDS
    src/*.h
    src/*.cpp
    src/*.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SRC_FILES_ENH})


include(${CMAKE_BINARY_DIR}/conan.cmake)
set(CONAN_EXTRA_REQUIRES cereal/1.3.0)
conan_cmake_run(REQUIRES ${CONAN_EXTRA_REQUIRES}
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing)


list(APPEND ENH_INCLUDE_DIRS
    extern/glbinding/source/glbinding/include
    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/include
    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/glbinding/include
#    extern/glbinding/source/glbinding-aux/include
#    ${CMAKE_CURRENT_BINARY_DIR}/extern/glbinding/source/glbinding-aux/include
    )

list(APPEND ENH_LIBS glbinding)
# list(APPEND ENH_LIBS glbinding glbinding-aux)

add_library(FrameworkEnhancement ${SRC_FILES_ENH} ${SHADER_FILES_ENH})
set_property(TARGET FrameworkEnhancement PROPERTY CXX_STANDARD 17)
target_include_directories(FrameworkEnhancement PUBLIC src)
target_include_directories(FrameworkEnhancement SYSTEM PUBLIC ${ENH_INCLUDE_DIRS})
target_link_libraries(FrameworkEnhancement PUBLIC VISCOMCore ${ENH_LIBS} CONAN_PKG::cereal)
set_build_flags(FrameworkEnhancement)

target_compile_definitions(FrameworkEnhancement PUBLIC $<$<CONFIG:DebugOpenGLCalls>:VISCOM_OGL_DEBUG_MSGS> GLFW_INCLUDE_NONE _SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING)
if (VISCOM_DO_PROFILING)
    target_compile_definitions(FrameworkEnhancement PUBLIC ENABLE_PROFILING)
endif()

