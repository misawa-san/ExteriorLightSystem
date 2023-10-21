cmake_minimum_required(VERSION 3.5)
project(pubsub)
add_subdirectory(./src/app)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(rclc REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_msg REQUIRED)

include_directories(src/app src/mclient/core src/mclient/user src/ros2 src/stub)

file(GLOB MCLIENT src/mclient/core/*.c src/mclient/user/*.c src/mclient/core/*.h src/mclient/user/*.h)
file(GLOB ROS2    src/ros2/*.cpp  src/ros2/*.hpp)
file(GLOB STUB    src/stub/*.c    src/stub/*.h)

add_executable(ros_task
  ${MCLIENT}  
  ${ROS2}
  ${STUB}
)

# Compiler Flags
target_compile_options(ros_task PUBLIC -coverage -g -fsanitize=address -fno-omit-frame-pointer)

# Linker Flags
set(ld_flags "-fsanitize=address -coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")

target_link_libraries(ros_task libapp)

ament_target_dependencies(ros_task
  rcl
  rclc
  rclcpp
  rclcpp_components
  std_msgs
  custom_msg
)

install(TARGETS
  ros_task
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

