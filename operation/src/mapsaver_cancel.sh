#!/bin/bash
# kill -9 `pgrep -f ros`


#stop all except rosbridge_websocket.
rm -r $1/$2 




#sh /home/relaybot/api_ws/src/operation/src/choice_action/gmapping_stop.sh
#input: folder of now which can store map and related files. $1 is path name(/var/www/maps as default.), $2 is unique map id. $3 is map name.
#function: move to folder of which with the same name and same name files within. 

# rosnode kill convertimg &

# kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &
