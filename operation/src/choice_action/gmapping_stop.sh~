#!/bin/bash

#map_saver

set timeout 30 
rosnode kill map_saver &
rosnode kill convert_img_ros &

#mapping 
rosnode kill arduino &
rosnode kill base_link_to_laser4 &
rosnode kill joint_state_publisher &
rosnode kill move_base &
rosnode kill robot_pose_publisher &
rosnode kill robot_state_publisher &
rosnode kill sick_tim551_2050001 &
rosnode kill slam_gmapping &
rosnode kill velocity_smoother &
rosnode kill nodelet_manager &

#tf
rosnode kill picture_frame_broadcaster &
rosnode kill pose_in_picture_frame_node &
rosnode kill lasers_in_picture_frame_node &

rosnode kill operation_buildmap

#sh /home/relaybot/Desktop/shell.sh

