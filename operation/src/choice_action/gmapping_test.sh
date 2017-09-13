#!/bin/bash
roslaunch rosbridge_server rosbridge_websocket.launch &
roslaunch gpsbot_nav gpsbot_mapping.launch &
roslaunch map_server multi_map_saver.launch &
rolaunch operation operation_buildmap.launch
