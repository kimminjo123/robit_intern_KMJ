cmake_minimum_required(VERSION 3.19)
project(hw1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt_add_executable(hw1
    WIN32 MACOSX_BUNDLE
    include/mainwindow.h
    src/main.cpp
    src/mainwindow.cpp
    ui/mainwindow.ui
)

target_include_directories(hw1 PRIVATE include)

target_link_libraries(hw1
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS hw1
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
