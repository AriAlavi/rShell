cmake_minimum_required(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

SET(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(rshell
    src/main.cpp
    src/commands.cpp
    src/connectors.cpp
    src/supVec.cpp
)

ADD_EXECUTABLE(test
    unit_tests/tests.cpp
    src/commands.cpp
    src/connectors.cpp
    src/supVec.cpp
)

TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
