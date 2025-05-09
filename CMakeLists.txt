cmake_minimum_required(VERSION 3.28)

project(MarchingCubesQT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    3DCore
    3DRender
    3DInput
    3DExtras
)

find_package(Eigen3 CONFIG REQUIRED)
find_package(OpenMP)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/mainwindow.ui
)
qt_add_executable(MarchingCubesQT
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    src/mc3dviewer.hpp src/mc3dviewer.cpp
    src/inputwidget.hpp src/inputwidget.cpp
    src/outputwidget.hpp src/outputwidget.cpp
    src/scalarfield.hpp src/scalarfield.cpp
    src/grid.hpp src/grid.cpp
    src/marchingcubes.hpp src/marchingcubes.cpp
    src/mesh.hpp src/mesh.cpp
    src/marchingcontroller.hpp src/marchingcontroller.cpp
    src/bvh.hpp src/bvh.cpp
    shaders/wireframe.vert
    shaders/wireframe.frag
    shaders/wireframe.geom
    resource.qrc
    src/wireframematerial.hpp src/wireframematerial.cpp
)

target_link_libraries(MarchingCubesQT PRIVATE
    Qt6::Widgets
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DInput
    Qt6::3DExtras
    Eigen3::Eigen
)
if(OpenMP_CXX_FOUND)
    target_link_libraries(MarchingCubesQT PUBLIC OpenMP::OpenMP_CXX)
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
