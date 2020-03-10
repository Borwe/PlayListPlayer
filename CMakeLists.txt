cmake_minimum_required(VERSION 3.15)

project(PlaylistPlayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
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

find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)

add_executable(PlaylistPlayer
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    #for the types dialog
    typesshow.cpp
    typesshow.h
    typesshow.ui

    #for handling single inputs
    singleinput.h
    singleinput.cpp
    singleinput.ui
)

target_link_libraries(PlaylistPlayer PRIVATE Qt5::Widgets Qt5::Gui)
target_include_directories(PlaylistPlayer PRIVATE Qt5::Widgets Qt5::Gui)
#for multithreading on linux
if(UNIX)
    target_link_libraries(PlaylistPlayer PRIVATE -lpthread)
endif()
#for using boost library
#setup boost_cmake_loc variable to be used
#set(Boost_NO_BOOST_CMAKE ON)
#set up to use dynamic libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
	message(FATAL_ERROR "It appears no valid Boost_ROOT was passed, please retry")
else()
    target_include_directories(PlaylistPlayer PRIVATE Boost::filesystem)
    target_link_libraries(PlaylistPlayer PRIVATE Boost::filesystem)
endif()

#add handlerslibrary
add_subdirectory(${CMAKE_SOURCE_DIR}/handlers)
target_link_libraries(PlaylistPlayer PUBLIC handlers)
target_include_directories(PlaylistPlayer PUBLIC handlers)

##add_custom_command(TARGET PlaylistPlayer POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:Boost::filesystem>/$<TARGET_FILE_:Boost::filesystem> $<TARGET_FILE_DIR:PlaylistPlayer>
    #)


get_target_property(BFILESYSTEM Boost::filesystem LOCATION)
get_filename_component(BFILESYSTEM_LOC ${BFILESYSTEM} NAME_WE)
message("FILESYSTEM NAME: " ${BFILESYSTEM_LOC})

#setup install prefix
message("-- Install location ${CMAKE_BINARY_DIR}/build")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/build)

#for installing
install(TARGETS PlaylistPlayer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    )

install(FILES $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Widgets>
    $<TARGET_FILE_DIR:Boost::filesystem>/${BFILESYSTEM_LOC}.dll
    DESTINATION bin)

if(WIN32)
    #for other dlls when running via IDE eg: qtcreator
    find_package(Wt COMPONENTS Dbo DboSqlite3 REQUIRED)
    find_package(Poco COMPONENTS Foundation JSON REQUIRED)
    add_custom_command(TARGET PlaylistPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:Boost::filesystem>/${BFILESYSTEM_LOC}.dll $<TARGET_FILE_DIR:PlaylistPlayer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:handlers> $<TARGET_FILE_DIR:PlaylistPlayer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Wt::Dbo> $<TARGET_FILE_DIR:PlaylistPlayer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Wt::DboSqlite3> $<TARGET_FILE_DIR:PlaylistPlayer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Poco::Foundation> $<TARGET_FILE_DIR:PlaylistPlayer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Poco::JSON> $<TARGET_FILE_DIR:PlaylistPlayer>
    #disabled since using qtcreator
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:PlaylistPlayer>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:PlaylistPlayer>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:PlaylistPlayer>
     )
endif()
