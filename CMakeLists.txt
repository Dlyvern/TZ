cmake_minimum_required(VERSION 3.5)

project(TZ VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Network REQUIRED)

set(TS_FILES TZ_en_150.ts)

set(PROJECT_SOURCES
        src/main.cpp
        widget.cpp
        widget.h
        widget.ui
        ${TS_FILES}
        include/UdpServer.h
        src/UdpServer.cpp
        include/UdpClient.h
        src/UdpClient.cpp
        include/Message1.h
        include/Message2.h
        include/HeightIndicatorWidget.h
        src/HeightIndicatorWidget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TZ
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(TZ SHARED
            ${PROJECT_SOURCES}
        )

    else()
        add_executable(TZ
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(TZ PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt5::Network)

set_target_properties(TZ PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TZ)
endif()
