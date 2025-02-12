cmake_minimum_required(VERSION 3.14)

project(ROS2_UnrealEngine_GUI LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ROS2_UnrealEngine_GUI
  main.cpp
  ros_nodes.h
  ros_nodes.cpp
  main.qml
  qml.qrc
  ros_image_provider.h ros_image_provider.cpp



)
target_link_libraries(ROS2_UnrealEngine_GUI
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Quick
                Qt${QT_VERSION_MAJOR}::Widgets
)

ament_target_dependencies(ROS2_UnrealEngine_GUI rclcpp std_msgs
                        example_interfaces geometry_msgs sensor_msgs OpenCV cv_bridge )

include(GNUInstallDirs)
install(TARGETS ROS2_UnrealEngine_GUI
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
