#!/bin/bash
# kill -9 `pgrep -f ros`

echo "[shell] nw_task stop. "
#complete the mapping process.

rosnode kill amcl &
rosnode kill arduino &
rosnode kill base_link_to_laser4 &

rosnode kill draw_points_and_nav &
rosnode kill edit_nav_task &

rosnode kill joint_state_publisher &
rosnode kill map_server &
rosnode kill move_base &
rosnode kill nodelet_manager &

rosnode kill picture_frame_broadcaster &
rosnode kill pose_in_picture_frame_node &
rosnode kill lasers_in_picture_frame_node &


rosnode kill robot_pose_publisher &
rosnode kill robot_state_publisher &
rosnode kill sick_tim551_2050001 &

rosnode kill velocity_smoother &
cd /home/relaybot/api_ws/src/shell_rclocal/
./killnode.sh 
echo "rosnode kill finished."
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 7


