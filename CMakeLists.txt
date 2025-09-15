cmake_minimum_required(VERSION 3.16)
project(r-type_server LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SRC_R-TYPE_SERVER "src/*.cpp")

add_executable(r-type_server ${SRC_R-TYPE_SERVER})
target_include_directories(r-type_server PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_compile_options(r-type_server PRIVATE
    -Wall -Wextra -Werror -pedantic
    -Wconversion -Wsign-conversion
    -Wshadow -Wnull-dereference
    -Wundef -Wuninitialized
    -Winit-self -Wredundant-decls
    -Wcast-align -Wcast-qual
    -Wmissing-declarations -Wswitch-default
    -Wdouble-promotion -Wformat=2 -Wwrite-strings
)

option(ENABLE_DEBUG "Enable debug macros and flags" OFF)
if(ENABLE_DEBUG)
    target_compile_definitions(r-type_server PRIVATE r-type_server_DEBUG=1)
endif()

