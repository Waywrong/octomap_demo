cmake_minimum_required(VERSION 2.8.3)
project(octomap_test)

SET(CMAKE_CXX_FLAGS -std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  octomap_msgs
  octomap_ros
  pcl_conversions
  pcl_ros
  sensor_msgs
  nav_msgs
  std_msgs
  roscpp
  tf
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES pub_pcl
#  CATKIN_DEPENDS actionlib actionlib_msgs geometry_msgs roscpp tf
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(pub_pcl src/pub_pcl.cpp)
add_executable(octomap_test src/octomap_test.cpp)

target_link_libraries(pub_pcl
 ${catkin_LIBRARIES}
)

target_link_libraries(octomap_test
 ${catkin_LIBRARIES}
)

install(TARGETS pub_pcl octomap_test
 ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
