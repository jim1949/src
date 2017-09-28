#!/bin/bash
# kill -9 `pgrep -f ros`

echo "[shell] edit_map stop. "
#need to save the pose.

#complete the mapping process.
rosnode kill amcl
rosnode kill arduino
rosnode kill base_link_to_laser4
rosnode kill joint_state_publisher
rosnode kill map_server
rosnode kill move_base
rosnode kill nodelet_manager

 
rosnode kill operation_nw_listener
rosnode kill operation_draw_wall

rosnode kill picture_frame_broadcaster
rosnode kill pose_in_picture_frame_node
rosnode kill lasers_in_picture_frame_node

rosnode kill operation_nw_listener
rosnode kill robot_pose_publisher
rosnode kill robot_state_publisher
rosnode kill sick_tim551_2050001

rosnode kill velocity_smoother
rosnode cleanup 

echo "rosnode kill finished."


