#!/bin/bash

#rosnode kill map_saver
#rosnode kill convert_img_ros
#rosnode kill operation_buildmap
#echo "finish rosnode kill"
#roslaunch map_server multi_map_saver.launch &
#sleep 5
#roslaunch operation operation_buildmap.launch

roslaunch operation edit_map.launch
