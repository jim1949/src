#!/bin/bash

#map_file:=...
roslaunch gpsbot_nav gpsbot_navi.launch &
roslaunch gpsbot_navigation operation_navigation.launch
