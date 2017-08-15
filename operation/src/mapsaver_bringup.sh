#!/bin/bash
roslaunch map_server multi_map_saver.launch &
roslaunch tf_learning mapinfo_pub.launch