#!/bin/bash
echo start >>~/start_log.txt
source ~/.bashrc
roscore &
da=`date "+%Y-%m-%d-%H-%M-%S"`
cd /var/www/log/main_log/
mkdir log_$da
cd log_$da
touch websocket_log_$da
touch operation_$da
echo "[$da]" >>/var/www/log/main_log/log_$da/websocket_log_$da
echo "[$da]" >>/var/www/log/main_log/log_$da/operation_$da 
sleep 5
roslaunch rosbridge_server rosbridge_websocket.launch >> /var/www/log/main_log/log_$da/websocket_log_$da &
sleep 5
roslaunch operation operation_mapbuild.launch >> /var/www/log/main_log/log_$da/operation_$da
echo finished
