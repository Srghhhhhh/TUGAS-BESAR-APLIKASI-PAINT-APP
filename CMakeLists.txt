cmake_minimum_required(VERSION 3.16)

project(PAINT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Mencari package Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport LinguistTools)

# Definisikan sumber file dengan rapi (Hanya satu kali saja)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scribblearea.cpp
    scribblearea.h
)

# Tambahkan eksekusi aplikasi
qt_add_executable(PAINT
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resource.qrc
)

# Link library agar PrintSupport dan Widgets aktif
target_link_libraries(PAINT PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
)
target_include_directories(PAINT PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Header\ Files
    ${CMAKE_CURRENT_SOURCE_DIR}/Source\ Files
)

# Pengaturan khusus Windows/Bundle
set_target_properties(PAINT PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PAINT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(PAINT)