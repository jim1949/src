#!/bin/bash
# kill -9 `pgrep -f ros`
kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &