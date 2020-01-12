cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests main.cpp)
#include top cmake
#link with handles(library) for testing
target_link_libraries(tests PUBLIC handlers)
message("Dir" ${CMAKE_SOURCE_DIR})
target_include_directories(tests PUBLIC ../)

enable_testing()
add_test(tests COMMAND tests)

#add boost test
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Couldn't find Boost or Boost::unit_test_framework module")
endif()
target_include_directories(tests PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(tests PUBLIC ${Boost_LIBRARIES})
