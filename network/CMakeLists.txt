cmake_minimum_required(VERSION 3.16)
project(r-type_network LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if (NOT MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SRC_R_TYPE_NETWORK_CORE "core/*.cpp")
add_library(r-type_network_core STATIC ${SRC_R_TYPE_NETWORK_CORE})
target_include_directories(r-type_network_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

if (MSVC)
  file(GLOB_RECURSE SRC_R_TYPE_NETWORK_IOCP "iocp/*.cpp")
  add_library(r-type_network_platform STATIC ${SRC_R_TYPE_NETWORK_IOCP})
  target_include_directories(r-type_network_platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/iocp/include
  )
  target_link_libraries(r-type_network_platform PUBLIC r-type_network_core)
else()
  file(GLOB_RECURSE SRC_R_TYPE_NETWORK_POSIX "posix/*.cpp")
  add_library(r-type_network_platform STATIC ${SRC_R_TYPE_NETWORK_POSIX})
  target_include_directories(r-type_network_platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/posix/include
  )
  target_link_libraries(r-type_network_platform PUBLIC r-type_network_core)
endif()

file(GLOB_RECURSE SRC_R_TYPE_NETWORK "api/*.cpp")
add_library(r-type_network SHARED ${SRC_R_TYPE_NETWORK})
target_include_directories(r-type_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(r-type_network PRIVATE R_TYPE_NETWORK_BUILD=1)
target_link_libraries(r-type_network PUBLIC r-type_network_platform)

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
  else()
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
  endif()
endfunction()

enable_strict_warnings(r-type_network)

option(ENABLE_DEBUG "Enable debug macros and flags" OFF)
if(ENABLE_DEBUG)
  target_compile_definitions(r-type_network PRIVATE r-type_network_DEBUG=1)
endif()
