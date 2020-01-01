cmake_minimum_required(VERSION 2.8.3)
project(differential_drive)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS rospy)
catkin_package()

catkin_python_setup()

catkin_install_python(
    PROGRAMS scripts/diff_tf.py scripts/pid_velocity.py scripts/twist_to_motors.py scripts/virtual_joystick.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
