cmake_minimum_required(VERSION 3.5)

project(mocking CXX)

message(STATUS "Using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS false)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-compare -Werror -Wno-four-char-constants -Wno-multichar -std=c++14 -pedantic")

# don't do this, libc++ is incomplete
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     # use libc++ with clang
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
# endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_LINK_FLAGS_DEBUG "${CMAKE_LINK_FLAGS_DEBUG} -fsanitize=address")

### Boost

find_package(Boost 1.67 REQUIRED unit_test_framework)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions("-DBOOST_TEST_DYN_LINK")

### GTest
#list(APPEND CMAKE_PREFIX_PATH "/usr/src/googletest")
find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

# FindGTest doesn't find gmock apparently, sigh
find_library(GMock_LIBRARY REQUIRED NAMES gmock)

# Gtest needs this :(
find_package(Threads REQUIRED)

add_custom_target(run-tests)

macro(add_test srcfile)
    get_filename_component(target ${srcfile} NAME_WE)
    add_executable(${target} ${srcfile})
    target_link_libraries(${target} ${Boost_LIBRARIES})
    add_custom_target(run-${target} ${target} WORKING_DIRECTORY ${CMAKE_CURRENT_BUILD_DIR} )
    add_dependencies(run-tests run-${target})
endmacro()
macro(add_gtest srcfile)
    get_filename_component(target ${srcfile} NAME_WE)
    add_executable(${target} ${srcfile})
    target_link_libraries(${target} GTest::GTest GTest::Main Threads::Threads ${GMock_LIBRARY})
    add_custom_target(run-${target} ${target} WORKING_DIRECTORY ${CMAKE_CURRENT_BUILD_DIR} )
    add_dependencies(run-tests ${target})
endmacro()

add_gtest(tokenizer1_gtest.cpp)

add_test(tokenizer1_test1.cpp)
add_test(tokenizer1_test2.cpp)
add_test(unique_tokenizer_test.cpp)
add_test(tokenizer2_test1.cpp)
add_test(tokenizer2_test2.cpp)

