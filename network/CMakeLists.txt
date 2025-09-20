cmake_minimum_required(VERSION 3.16)
project(r-type_network LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if (UNIX AND NOT APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_SKIP_INSTALL_RPATH FALSE)
    set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_BUILD_RPATH "\$ORIGIN")
    set(CMAKE_INSTALL_RPATH "\$ORIGIN")
elseif (APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_SKIP_INSTALL_RPATH FALSE)
    set(CMAKE_MACOSX_RPATH TRUE)
    set(CMAKE_BUILD_RPATH "@loader_path")
    set(CMAKE_INSTALL_RPATH "@loader_path")
endif ()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif ()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(ProcessorCount)
ProcessorCount(N)
if (NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SRC_R_TYPE_NETWORK_CORE "src/Core/*.cpp")
add_library(r-type_network_core SHARED ${SRC_R_TYPE_NETWORK_CORE})
target_include_directories(r-type_network_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    file(GLOB_RECURSE SRC_R_TYPE_NETWORK_WINDOWS "src/Platform/Windows/*.cpp")
    add_library(r-type_network SHARED ${SRC_R_TYPE_NETWORK_WINDOWS})
    target_link_libraries(r-type_network PUBLIC r-type_network_core ws2_32)

else ()
    file(GLOB_RECURSE SRC_R_TYPE_NETWORK_POSIX "src/Subplatform/POSIX/*.cpp")
    add_library(r-type_network_subplatform SHARED ${SRC_R_TYPE_NETWORK_POSIX})
    target_include_directories(r-type_network_subplatform PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(r-type_network_subplatform PUBLIC r-type_network_core)

    if (APPLE)
        file(GLOB_RECURSE SRC_R_TYPE_NETWORK_MACOS "src/Platform/MacOS/*.cpp")
        add_library(r-type_network SHARED ${SRC_R_TYPE_NETWORK_MACOS})
    else ()
        file(GLOB_RECURSE SRC_R_TYPE_NETWORK_LINUX "src/Platform/Linux/*.cpp")
        add_library(r-type_network SHARED ${SRC_R_TYPE_NETWORK_LINUX})
    endif ()

    target_link_libraries(r-type_network PUBLIC r-type_network_subplatform r-type_network_core)
    set_target_properties(r-type_network_core r-type_network_subplatform r-type_network PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()

target_include_directories(r-type_network PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(r-type_network PRIVATE R_TYPE_NETWORK_BUILD=1)
target_compile_definitions(r-type_network_core PRIVATE R_TYPE_NETWORK_BUILD=1)

if (NOT MSVC)
    target_compile_definitions(r-type_network_subplatform PRIVATE R_TYPE_NETWORK_BUILD=1)
endif ()

function(enable_strict_warnings tgt)
    if (MSVC)
        target_compile_options(${tgt} PRIVATE
                /W4
                /WX
                /permissive-
                /EHsc
                /Zc:__cplusplus
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        target_compile_options(${tgt} PRIVATE
                -Wall -Wextra -Werror -Wpedantic
                -Wconversion -Wsign-conversion
                -Wshadow -Wnull-dereference
                -Wundef -Wuninitialized
                -Wcast-align -Wcast-qual
                -Wswitch-default
                -Wdouble-promotion
                -Wformat=2
                -Wwrite-strings
        )
    else ()
        target_compile_options(${tgt} PRIVATE
                -Wall -Wextra -Werror -Wpedantic
                -Wconversion -Wsign-conversion
                -Wshadow -Wnull-dereference
                -Wundef -Wuninitialized -Winit-self
                -Wredundant-decls
                -Wcast-align -Wcast-qual
                -Wmissing-declarations -Wswitch-default
                -Wdouble-promotion -Wformat=2 -Wwrite-strings
        )
    endif ()
endfunction()

enable_strict_warnings(r-type_network)
enable_strict_warnings(r-type_network_core)

if (NOT MSVC)
    enable_strict_warnings(r-type_network_subplatform)
endif ()

option(ENABLE_DEBUG "Enable debug macros and flags" OFF)
if (ENABLE_DEBUG)
    target_compile_definitions(r-type_network PRIVATE DEBUG=1)
    target_compile_definitions(r-type_network_core PRIVATE DEBUG=1)
    if (NOT MSVC)
        target_compile_definitions(r-type_network_subplatform PRIVATE DEBUG=1)
    endif ()

endif ()
