#!/bin/bash

#map_file:=...
#map_saver

set timeout 30 
rosnode kill map_server &

#mapping 
rosnode kill arduino &
rosnode kill base_link_to_laser4 &
rosnode kill joint_state_publisher &
rosnode kill move_base &
rosnode kill robot_pose_publisher &
rosnode kill robot_state_publisher &
rosnode kill sick_tim551_2050001 &
rosnode kill slam_gmapping &
rosnode kill nodelet_manager &

#tf
rosnode kill picture_frame_broadcaster &
rosnode kill pose_in_picture_frame_node &
rosnode kill lasers_in_picture_frame_node &

#edit_wall_navpoints
rosnode kill operation_nw_listener
rosnode kill operation_draw_wall

sleep 2
rosnode cleanup

