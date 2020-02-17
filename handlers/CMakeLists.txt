cmake_minimum_required(VERSION 3.5)

project(handlers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(handlers handlers.h handler.cpp
    media_data.h media_data.cpp data.h data.cpp)

#add tests
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

#add boost file system
find_package(Boost COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Couldn't find BOOST_ROOT, please define it")
endif()
target_include_directories(handlers PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(handlers PRIVATE ${Boost_LIBRARIES})

#add poco framework to get date support
find_package(Poco COMPONENTS Foundation JSON REQUIRED)
if(NOT Poco_FOUND)
    message(FATAL_ERROR "No Poco Libraries found")
endif()
target_link_libraries(handlers PRIVATE Poco::Foundation PRIVATE Poco::JSON)
target_include_directories(handlers PRIVATE Poco::Foundation PRIVATE Poco:JSON)

#add wtdbo for database stuff
#find it
find_package(Wt COMPONENTS Dbo DboSqlite3 REQUIRED)
target_link_libraries(handlers PRIVATE Wt::Dbo PRIVATE Wt::DboSqlite3)
target_include_directories(handlers PRIVATE Wt::Dbo PRIVATE Wt::DboSqlite3)
