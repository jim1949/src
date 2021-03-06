#!/bin/bash
echo Connect to AI_SPEECH.>>~/startup.log
export ROS_HOSTNAME=192.168.31.47
export ROS_MASTER_URI=http://192.168.31.47:11311
export ROS_IP=192.168.31.47

source /opt/ros/indigo/setup.bash
source /home/relaybot/catkin_ws/devel/setup.bash
source /home/relaybot/gpsbot_ws/devel/setup.bash
source /home/relaybot/api_ws/devel/setup.bash

da=`date "+%Y-%m-%d-%H-%M-%S"`
cd /var/www/log/main_log/
mkdir log_$da
cd log_$da
touch websocket_log_$da
touch operation_$da
echo "[$da]" >>/var/www/log/main_log/log_$da/websocket_log_$da
echo "[$da]" >>/var/www/log/main_log/log_$da/operation_$da &
sleep 2

roslaunch rosbridge_server rosbridge_websocket.launch >> /var/www/log/main_log/log_$da/websocket_log_$da &
#sleep 5
roslaunch operation operation_mapbuild.launch >> /var/www/log/main_log/log_$da/operation_$da
