cmake_minimum_required(VERSION 3.1)
project(SSM LANGUAGES CXX)

message("Stock Market helper utility")

set(DEPS_CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

message(status "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
if(MSVS)
    add_compile_options( /wd4127 /wd4250)
else()
add_compile_options(-Wall -Wextra -Wno-unused-function) #-Werror -Wpedantic
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
endif()

add_subdirectory(lib/common)

#find Package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core Gui Multimedia Charts)

find_package(Threads REQUIRED)

#for QT
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

#Standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -pthread)


#Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 3)
set(SO_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})





#Build Library
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})


#include/set files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common/)

#file( GLOB_RECURSE SOURCE_FILES *.cpp)
#file( GLOB_RECURSE HEADER_FILES *.h)
#file( GLOB_RECURSE MODEL_FILES Model/*.cpp)
#file( GLOB_RECURSE API_FILES Model/*.h)

SET( SOURCE_FILES
        Model/acsmodel.cpp
        common/mainwindow.cpp
        common/avgcoststocks.cpp
        common/chart.cpp
        common/csvmodel.cpp
        common/tradelog.cpp
        main.cpp)
SET( HEADER_FILES
        Model/acsmodel.h
        common/mainwindow.h
        common/avgcoststocks.h
        common/chart.h
        common/threadpool.h
        common/timer.h
        common/csvreader.h
        common/csvmodel.h
        common/tradelog.h)

#file(GLOB CLIENT_FILE *.cpp *.h)

set(UIFORM common/mainwindow.ui)

#set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resource.qrc )
QT5_WRAP_UI(UI_HEADERS ${UIFORM} )

add_executable(${PROJECT_NAME}  ${HEADER_FILES} ${SOURCE_FILES} ${UI_HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Charts  Threads::Threads stdc++fs lib::common)

install (TARGETS ${PROJECT_NAME} DESTINATION bin)

set(CPACK_SYSTEM_NAME ${SO_VERSION})

#Generate debian package with cpack [or alternative make package]
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Postnikov Dmitry")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Stock Market helper utility")
    set(CPACK_PACKAGE_EXECUTABLES "SMH" "SMH")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS} cmake (>=3.5)")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libqt5gui5 (>=5.3.0),libqt5core5a (>=5.3.0),libqt5widgets5 (>=5.3.0),libqt5network5 (>=5.3.0)")
    set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
endif()

include(CPack)