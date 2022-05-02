cmake_minimum_required(VERSION 3.0.2)
project(beginner_tutorials)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

# catkin_python_setup()

## Generate messages in the 'msg' folder
 add_message_files( FILES Num.msg )

## Generate services in the 'srv' folder
add_service_files(  FILES AddTwoInts.srv)


## Generate added messages and services with any dependencies listed here
 generate_messages( DEPENDENCIES std_msgs )


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES beginner_tutorials
  CATKIN_DEPENDS roscpp rospy std_msgs
  DEPENDS system_lib
  message_runtime
)

###########
## Build ##
###########

include_directories(include ${catkin_INCLUDE_DIRS} )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(${PROJECT_NAME}_talkernode src/talkernode.cpp)
add_executable(${PROJECT_NAME}_listenernode src/listenernode.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
set_target_properties(${PROJECT_NAME}_talkernode PROPERTIES OUTPUT_NAME node_name_ITalkalotHeHe PREFIX "")
set_target_properties(${PROJECT_NAME}_listenernode PROPERTIES OUTPUT_NAME node_name_IlistenAlotHeHe PREFIX "")
## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(${PROJECT_NAME}_talkernode  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_listenernode  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# somehow pass dependecies i guess for future packages based on this i guesss...



## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_talkernode ${catkin_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_listenernode ${catkin_LIBRARIES})





##executable by default will go into package directory of your devel space, 
##located by default at ~/catkin_ws/devel/lib/<package name>.
