cmake_minimum_required(VERSION 3.28)

project(MarchingCubesQT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Widgets
    3DCore
    3DRender
    3DInput
    3DExtras
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    3DCore
    3DRender
    3DInput
    3DExtras
)
find_package(Eigen3 CONFIG REQUIRED)
find_package(OpenMP)



set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)


    qt_add_executable(MarchingCubesQT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mc3dviewer.hpp mc3dviewer.cpp
        inputwidget.hpp inputwidget.cpp
        outputwidget.hpp outputwidget.cpp
        scalarfield.hpp scalarfield.cpp
        grid.hpp grid.cpp
        marchingcubes.hpp marchingcubes.cpp
        mesh.hpp mesh.cpp
        marchingcontroller.hpp marchingcontroller.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MarchingCubesQT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MarchingCubesQT SHARED
            ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(MarchingCubesQT
        ${PROJECT_SOURCES}
    )
endif()
endif()

target_link_libraries(MarchingCubesQT PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::3DCore
    Qt${QT_VERSION_MAJOR}::3DRender
    Qt${QT_VERSION_MAJOR}::3DInput
    Qt${QT_VERSION_MAJOR}::3DExtras
)
target_link_libraries(MarchingCubesQT PRIVATE Eigen3::Eigen)
if(OpenMP_CXX_FOUND)
    target_link_libraries(MarchingCubesQT PUBLIC OpenMP::OpenMP_CXX)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MarchingCubesQT)
endif()
set_target_properties(MarchingCubesQT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MarchingCubesQT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MarchingCubesQT)
endif()
