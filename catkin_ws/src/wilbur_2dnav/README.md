# 2D Nav using GMapping

To start a map:

```
roscore
rosparam set use_sim_time true
rosrun gmapping slam_gmapping scan:=/scan _odom_frame:=/odom

```
