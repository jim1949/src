#!/bin/bash
log_file=/tmp/minimal-upstart.log
DATE=`date`


source /opt/ros/indigo/setup.bash
source /home/relaybot/catkin_ws/devel/setup.bash
source /home/relaybot/gpsbot_ws/devel/setup.bash
source /home/relaybot/api_ws/devel/setup.bash >>$log_file

interface=wlan0
echo "$DATE: minimal:gpsbot-start on interface $interface" >> $log_file

#export ROS_IP=`ifconfig $interface | grep -o 'inet addr:[^ ]*' | cut -d: -f2`

ROS_IP=192.168.31.11
echo "$DATE: minimal:gpsbot-start setting ROS_IP=$ROS_IP" >> $log_file

if [ "$ROS_IP" = "" ]; then
    echo "$DATE: minimal:gpsbot-start can't run with empty ROS_IP." >> $log_file
    exit 1
fi

roslaunch operation bringup.launch



