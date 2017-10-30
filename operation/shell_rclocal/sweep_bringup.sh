#!/bin/bash
echo start >>~/start_log.txt
# source ~/.bashrc
HOSTIP=`ifconfig wlan0 | grep 'inet ' | cut -d ':'  -f 2 | cut -c -15`
if [ $HOSTIP  == '192.168.31.47' ];then
echo  Connect to sweepbot.
export ROS_HOSTNAME=192.168.31.47
export ROS_MASTER_URI=http://192.168.31.47:11311
export ROS_IP=192.168.31.47

elif [ $HOSTIP  == '192.168.31.11' ];then
echo  Connect to gpsbot.
export ROS_HOSTNAME=192.168.31.11
export ROS_MASTER_URI=http://192.168.31.11:11311
export ROS_IP=192.168.31.11

elif [ $HOSTIP  == '192.168.31.60' ];then
echo  Connect to laptop.
export ROS_HOSTNAME=192.168.31.60
export ROS_MASTER_URI=http://192.168.31.60:11311
export ROS_IP=192.168.31.60

else 
echo Connect to localhost
export ROS_HOSTNAME=localhost
export ROS_MASTER_URI=http://localhost:11311
export ROS_IP=localhost
fi

export API_WS=/home/relaybot/api_ws/src/
export LIBGL_ALWAYS_SOFTWARE=1
source /home/relaybot/gpsbot_ws/devel/setup.bash
source /opt/ros/indigo/setup.bash
source /home/relaybot/catkin_ws/devel/setup.bash
source /home/relaybot/api_ws/devel/setup.bash

# da=`date "+%Y-%m-%d-%H-%M-%S"`
# cd /var/www/log/main_log/
# mkdir log_$da
# cd log_$da
# touch websocket_log_$da
# touch operation_$da
# echo "[$da]" >>/var/www/log/main_log/log_$da/websocket_log_$da
# echo "[$da]" >>/var/www/log/main_log/log_$da/operation_$da 
roslaunch operation bringup.launch
echo finished
