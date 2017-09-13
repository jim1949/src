#!/bin/bash
roslaunch gpsbot_nav gpsbot_mapping.launch &
#sleep 2
#roslaunch tf_learning mapinfo_pub.launch &
#roslaunch map_server multi_map_saver.launch &
sleep 2
roslaunch operation operation_buildmap.launch &
