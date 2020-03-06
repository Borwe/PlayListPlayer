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
find_package(Boost COMPONENTS unit_test_framework filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Couldn't find Boost or Boost::unit_test_framework module")
endif()
target_include_directories(tests PUBLIC Boost::unit_test_framework PUBLIC Boost::filesystem)
target_link_libraries(tests PUBLIC Boost::unit_test_framework PUBLIC Boost::filesystem)

if(WIN32)
    #for executing via IDE eg: qtcreator
    find_package(Poco COMPONENTS Foundation JSON REQUIRED)
    find_package(Wt COMPONENTS Dbo DboSqlite3 REQUIRED)
    add_custom_command(TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Boost::filesystem> $<TARGET_FILE_DIR:tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Boost::unit_test_framework> $<TARGET_FILE_DIR:tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Poco::Foundation> $<TARGET_FILE_DIR:tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Wt::Dbo> $<TARGET_FILE_DIR:tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Wt::DboSqlite3> $<TARGET_FILE_DIR:tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Poco::JSON> $<TARGET_FILE_DIR:tests>
    )
endif()

