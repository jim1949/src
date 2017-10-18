#!/bin/bash

#rosnode kill map_saver
#rosnode kill convert_img_ros
#rosnode kill operation_buildmap
#echo "finish rosnode kill"
#roslaunch map_server multi_map_saver.launch &
#sleep 5
#roslaunch operation operation_buildmap.launch

echo "[shell] nw_task bringup start. "
echo $1/$2/$2.yaml
roslaunch operation navigation.launch map_file:=$1/$2/$2.yaml &
sleep 15
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 6