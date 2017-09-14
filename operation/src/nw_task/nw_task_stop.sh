#!/bin/bash
# kill -9 `pgrep -f ros`

echo "[shell] nw_task stop. "
#complete the mapping process.

rosnode kill amcl
rosnode kill arduino
rosnode kill base_link_to_laser4

rosnode kill draw_points_and_nav
rosnode kill edit_nav_task

rosnode kill joint_state_publisher
rosnode kill map_server
rosnode kill move_base
rosnode kill nodelet_manager

rosnode kill picture_frame_broadcaster
rosnode kill pose_in_picture_frame_node
rosnode kill lasers_in_picture_frame_node


rosnode kill robot_pose_publisher
rosnode kill robot_state_publisher
rosnode kill sick_tim551_2050001

rosnode kill velocity_smoother
rosnode cleanup 



#sh /home/relaybot/api_ws/src/operation/src/choice_action/gmapping_stop.sh
#input: folder of now which can store map and related files. $1 is path name(/var/www/maps as default.), $2 is unique map id. $3 is map name.
#function: move to folder of which with the same name and same name files within. 

# rosnode kill convertimg &

# kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &
