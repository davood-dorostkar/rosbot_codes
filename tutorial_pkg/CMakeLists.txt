cmake_minimum_required(VERSION 3.8)
project(tutorial_pkg)

# Set C++ standard to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)

include_directories(
  include
)

add_executable(my_first_node src/my_first_node.cpp)
ament_target_dependencies(my_first_node rclcpp)

add_executable(camera_com src/camera_com.cpp)
ament_target_dependencies(camera_com sensor_msgs std_msgs rclcpp)

add_executable(service src/service.cpp)
ament_target_dependencies(service sensor_msgs std_msgs rclcpp)

add_executable(camera_saver src/camera_saver.cpp)
ament_target_dependencies(camera_saver sensor_msgs std_msgs std_srvs rclcpp)

add_executable(driver src/driver.cpp)
ament_target_dependencies(driver geometry_msgs rclcpp)

add_executable(tracker src/tracker.cpp)
ament_target_dependencies(tracker rclcpp OpenCV cv_bridge geometry_msgs sensor_msgs std_msgs)

install(TARGETS
  my_first_node
  camera_com
  service
  camera_saver
  driver
  tracker
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
