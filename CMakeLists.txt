cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include("tools/gate/cmake/HunterGate.cmake")

huntergate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.31.tar.gz"
  SHA1 "3d94b8522541c4c8eaac606f4c47d09322a8019c"
)

project(lab)
set(EXECUTABLE_NAME "lab")
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost CONFIG REQUIRED system filesystem)
find_package(GTest CONFIG REQUIRED)

# enum your files and delete this comment
add_executable( ${EXECUTABLE_NAME}
        main.cpp
)

target_include_directories(${EXECUTABLE_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${EXECUTABLE_NAME} GTest::main
        Boost::system
        Boost::filesystem)
target_link_libraries(${EXECUTABLE_NAME}
        Boost::filesystem
        Boost::system)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
enable_testing()
add_test(NAME unit_tests COMMAND ${EXECUTABLE_NAME})
