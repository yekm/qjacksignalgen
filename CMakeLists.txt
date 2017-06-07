cmake_minimum_required(VERSION 2.8)
project(qjacksignalgen)

set(CMAKE_VERBOSE_MAKEFILE 1)

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

add_definitions("-DINSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
add_definitions("-DPROJECT_NAME=${PROJECT_NAME}")

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

#set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS_DEBUG     "${COMMON_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE   "${COMMON_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG   "-std=c++14 ${COMMON_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++14 ${COMMON_FLAGS_RELEASE}")


#[[
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS filesystem regex system thread program_options timer)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
]]

# git commit hash macro
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)



include_directories(${CMAKE_SOURCE_DIR}/jackcpp/include)

file(GLOB_RECURSE PLUGIN_SRC jackcpp/src/*.cpp)
add_library(jackcpp STATIC ${PLUGIN_SRC})
#    install(TARGETS ${LIBNAME} DESTINATION lib/${PROJECT_NAME})

set(SRC
    main.cpp
    mainwindow.cpp
    signalgenerator.cpp
    freqwidget.cpp
    )

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} jackcpp jack Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

