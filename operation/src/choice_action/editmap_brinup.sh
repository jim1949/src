#!/bin/bash

#map_file:=...
roslaunch gpsbot_nav gpsbot_navi.launch &
roslaunch operation edit_wall_navpoints.launch
