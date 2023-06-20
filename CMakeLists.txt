cmake_minimum_required(VERSION 2.8.12)
project (nemoapi C CXX)

option(ASYNC "Build async" OFF)

include_directories(./include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(rapidjson REQUIRED)

include_directories(${RAPIDJSON_INCLUDE_DIRS})

if (UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
    set (CXX_FLAGS
        -g
        -fPIC
        -Wall
        -D__STDC_FORMAT_MACROS
        -Wno-unused-parameter
        -Wno-unused-function
        -Wunused-variable
        -Wunused-value
        -Wcast-qual
        -Wcast-align
        -Wwrite-strings
        -Wsign-compare
        -Winvalid-pch
        -fms-extensions
        -Wfloat-equal
        -Wextra
        -std=c++11
    )
    include_directories (${PROJECT_SOURCE_DIR})
else (UNIX)
set (CXX_FLAGS -DWIN32 -D_WIN32 -EHsc)
    set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
    include_directories(${VcpkgRoot}/include
                        ${PROJECT_SOURCE_DIR})
    link_directories(${VcpkgRoot}/debug/lib ${VcpkgRoot}/debug/lib/manual-link)
endif (UNIX)

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if (UNIX)
    SET(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb -D_DEBUG")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -ggdb -DNDEBUG")
    SET(DEPENDENT_LIBRARIES curl ssl crypto sodium)
endif (UNIX)

if (ASYNC)
    list(APPEND DEPENDENT_LIBRARIES uv)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNEMOAPI_ASYNC")
endif (ASYNC)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set (MACOSX_RPATH ${LIBRARY_OUTPUT_PATH})
set (CMAKE_MACOSX_RPATH 0)

message (STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message (STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message (STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message (STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})

add_subdirectory (src)
add_subdirectory (examples)