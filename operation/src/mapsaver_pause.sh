#!/bin/bash
rosnode kill picture_frame_node &
rosnode kill robot_pose_in_picture_frame_node &
rosnode kill laser_points_in_picture_frame_node &
rosnode cleanup
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
