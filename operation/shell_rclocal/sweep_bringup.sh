#!/bin/bash

HOSTIP=`ifconfig wlan0 | grep 'inet ' | cut -d ':'  -f 2 | cut -c -15`
ETHIP=`ifconfig eth1 | grep 'inet ' | cut -d ':'  -f 2 | cut -c -15`
if [ $ETHIP  == '192.168.31.130' ];then
echo Eth1: Connect to gps-agvbot.
export ROS_HOSTNAME=192.168.31.130
export ROS_MASTER_URI=http://192.168.31.130:11311
export ROS_IP=192.168.31.130
else
if [ $HOSTIP  == '192.168.2.133' ];then
echo Connect to TP_LINK_GPS.
export ROS_HOSTNAME=192.168.2.133
export ROS_MASTER_URI=http://192.168.2.133:11311
export ROS_IP=192.168.2.133

elif [ $HOSTIP  == '192.168.31.11' ];then
echo Connect to AI_SPEECH.
export ROS_HOSTNAME=192.168.31.11
export ROS_MASTER_URI=http://192.168.31.11:11311
export ROS_IP=192.168.31.11
else
echo Connect to AGV
export ROS_HOSTNAME=ubuntu
export ROS_MASTER_URI=http://ubuntu:11311
export ROS_IP=ubuntu
fi
fi

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
sleep 5
roslaunch operation operation_mapbuild.launch >> /var/www/log/main_log/log_$da/operation_$da
