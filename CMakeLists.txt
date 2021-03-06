cmake_minimum_required(VERSION 3.10)

project(PlaylistPlayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(PlaylistPlayer SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    handlers.cpp
    handlers.h
  )
else()
  add_executable(PlaylistPlayer
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

target_link_libraries(PlaylistPlayer PRIVATE Qt5::Widgets)
#for multithreading on linux
target_link_libraries(PlaylistPlayer PRIVATE -lpthread)
#for using boost library
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.65 COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "It appears no valid BOOST_ROOT was passed, please retry")
else()
    target_include_directories(PlaylistPlayer PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(PlaylistPlayer PRIVATE ${Boost_LIBRARIES})
endif()

#add handlerslibrary
add_subdirectory(${CMAKE_SOURCE_DIR}/handlers)
target_link_libraries(PlaylistPlayer PRIVATE handlers)
