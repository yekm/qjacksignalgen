cmake_minimum_required(VERSION 3.1.0)
project(qjacksignalgen)

set(CMAKE_VERBOSE_MAKEFILE 1)

#[=[
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-DDEBUG")
    #set(CC clang)
    #set(CXX clang++)
    #set(_CMAKE_TOOLCHAIN_PREFIX llvm-)
    message("Debug build. Install prefix CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build. Install prefix ${CMAKE_INSTALL_PREFIX}")
ELSE()
    message("Some other build type.")
ENDIF()



if(REPLACE_FLAGS)
    set(COMMON_FLAGS_DEBUG      ${REPLACE_FLAGS})
    set(COMMON_FLAGS_RELEASE    ${REPLACE_FLAGS})
else(REPLACE_FLAGS)
    set(COMMON_FLAGS_DEBUG      "-march=native -O0 -ggdb3 -Wall -Wextra -Wno-attributes")
    #set(COMMON_FLAGS_DEBUG      "-fsanitize=address,memory,thread -fsanitize-memory-track-origins -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes")
    #set(COMMON_FLAGS_DEBUG      "-fsanitize=memory -fsanitize-memory-track-origins -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes")
    #set(COMMON_FLAGS_DEBUG      "-fsanitize=thread -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes")
    set(COMMON_FLAGS_RELEASE    "-march=native -Ofast -Wall -Wextra -Wno-attributes")
endif(REPLACE_FLAGS)

#]=]

#[=[
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS filesystem regex system thread program_options timer)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) # TODO
endif()

# git commit hash macro
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

#]=]

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)

#file(GLOB_RECURSE PLUGIN_SRC jackcpp/src/*.cpp)
add_library(jackcpp STATIC
    jackcpp/src/jackaudioio.cpp
    jackcpp/src/jackblockingaudioio.cpp
    jackcpp/src/jackmidiport.cpp
    )
#install(TARGETS ${LIBNAME} DESTINATION lib/${PROJECT_NAME})

target_include_directories(jackcpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jackcpp/include>
    $<INSTALL_INTERFACE:include/jackcpp>
    )

add_executable(qjacksignalgen
    main.cpp
    mainwindow.cpp
    signalgenerator.cpp
    freqwidget.cpp
    )


set_target_properties(qjacksignalgen PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    )

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BUILD_FLAGS -O0 -ggdb3 -Wall -Wextra)
    #-fsanitize=address,memory,thread -fsanitize-memory-track-origins -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes
    #-fsanitize=memory -fsanitize-memory-track-origins -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes
    #-fsanitize=thread -O1 -fno-omit-frame-pointer -ggdb3 -Wall -Wextra -Wno-attributes
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    set(BUILD_FLAGS -O3 -Wall -Wextra)
ELSE()
    set(BUILD_FLAGS -O3 -Wall -Wextra)
    message("Some other build type.")
ENDIF()

target_compile_options(qjacksignalgen PRIVATE
    ${BUILD_FLAGS}
    )
#   target_compile_definitions(qjacksignalgen PRIVATE
#    "GIT_COMMIT_HASH=${GIT_COMMIT_HASH}"
#    "INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
#    "PROJECT_NAME=qjacksignalgen"
#    "DBGHPP=$<IF:$<CONFIG:Debug>,1,0>"
#    )

target_include_directories(qjacksignalgen
    PRIVATE
    "${CMAKE_SOURCE_DIR}/jackcpp/include"
    )

target_link_libraries(qjacksignalgen jackcpp jack Qt5::Widgets)

install(TARGETS qjacksignalgen DESTINATION bin)

