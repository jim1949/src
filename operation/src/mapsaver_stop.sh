#!/bin/bash
# kill -9 `pgrep -f ros`

rosnode kill map_saver &
rosnode kill convert_img_ros &
rosnode cleanup &
mkdir /var/www/maps/test_0809 &
cp -r /var/www/maps /var/www/maps/test_0809 &
# kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &