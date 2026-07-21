cmake_minimum_required(VERSION 3.8)
project(coresense_modelet_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(behaviortree_ros2 REQUIRED)
find_package(sensor_msgs REQUIRED)
#find_package(coresense_msgs REQUIRED)
find_package(triplestar_msgs REQUIRED)
find_package(btcpp_ros2_interfaces REQUIRED)
find_package(pluginlib REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)



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

set(DEPENDENCIES
  ${triplestar_msgs_TARGETS}
  ${coresense_msgs_TARGETS}
  ${sensor_msgs_TARGETS}
  behaviortree_ros2::behaviortree_ros2
  ${btcpp_ros2_interfaces_TARGETS}
)


add_library(get_range_modelet_plugin SHARED src/get_range_modelet.cpp)
target_compile_definitions(get_range_modelet_plugin PRIVATE  BT_PLUGIN_EXPORT )
target_link_libraries(get_range_modelet_plugin
  ${pluginlib_TARGETS}
  ${DEPENDENCIES}
  ament_index_cpp::ament_index_cpp
)

target_include_directories(get_range_modelet_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)


###############
### Install targets and directories
install(TARGETS
  get_range_modelet_plugin
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  behavior_trees
  config
  # launch
  DESTINATION share/${PROJECT_NAME}
)


# Install plugins for other packages to load
install(TARGETS
  get_range_modelet_plugin
  LIBRARY DESTINATION share/${PROJECT_NAME}/bt_plugins
  ARCHIVE DESTINATION share/${PROJECT_NAME}/bt_plugins
  RUNTIME DESTINATION share/${PROJECT_NAME}/bt_plugins
)


# Install headers for plugins
install(DIRECTORY
  include/
  DESTINATION include
)

ament_package()
