#!/bin/bash

#rosnode kill map_saver
#rosnode kill convert_img_ros
#rosnode kill operation_buildmap
#echo "finish rosnode kill"
#roslaunch map_server multi_map_saver.launch &
#sleep 5
#roslaunch operation operation_buildmap.launch
echo "   edit_map bringup start. "                              
roslaunch operation edit_map.launch map_file:=$1/$2/$2.yaml &
sleep 3
# rostopic pub -1 /mapping_status_feedback std_msgs/Int32 4
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 4

# rosrun operation mapping_status_fb 4
