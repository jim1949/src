#!/bin/bash

#rosnode kill map_saver
#rosnode kill convert_img_ros
#rosnode kill operation_buildmap
#echo "finish rosnode kill"
#roslaunch map_server multi_map_saver.launch &
#sleep 5
#roslaunch operation operation_buildmap.launch

echo "ppl_task_bringup start. "
echo $1/$2/$2.yaml

# param->map_path+" "+param->map_id+" "+param->map_name


roslaunch operation frame.launch map_file:=$1/$2/$2.yaml &

# produce the path in robot_path.txt.
# got the path txt(/var/www/path/$1/... ).now ppl_src/...
sleep 2
roslaunch operation ppl_task_bringup.launch map_info:="$1 $2 $3" 

# read the path in /picture_frame, and write it inside /map.


rostopic pub -1 /mapping_status_feedback std_msgs/Int32 10