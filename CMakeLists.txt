cmake_minimum_required(VERSION 3.16)

project(VectorGraphics LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(VectorGraphics
    main.cpp

    src/core/GraphicsObject.cpp
    src/core/Diagram.cpp
    src/core/Command.cpp
    include/ui/Canvas.h
    include/ui/MainWindow.h
    src/core/ShapeMaker.cpp
    src/shapes/Circle.cpp
    src/ui/Canvas.cpp
    src/ui/MainWindow.cpp
    src/ui/Mouse.cpp
    src/io/XMLParser.cpp
    src/io/SVGWriter.cpp
    src/shapes/Rect.cpp
    src/shapes/RoundRect.cpp
    src/shapes/Hexagon.cpp
    src/shapes/Line.cpp
    src/shapes/Text.cpp
    src/shapes/Freehand.cpp
)

target_include_directories(VectorGraphics
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(VectorGraphics PRIVATE Qt6::Widgets)