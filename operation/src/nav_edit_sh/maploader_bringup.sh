#!/bin/bash
roslaunch gpsbot_nav gpsbot_navigation_demo.launch map_file:=/var/www/maps/now/map.pgm &
roslaunch tf_learning mapinfo_pub.launch &
roslaunch operation edit_wall_navpoints.launch