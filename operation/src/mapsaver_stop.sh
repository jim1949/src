#!/bin/bash
# kill -9 `pgrep -f ros`

rosnode kill map_saver &
rosnode kill convert_img_ros &
rosnode cleanup &
mkdir $1/$2 &

sleep 0.5
cp $1/now/map.pgm $1/$2/$2.pgm &

cp $1/now/map.yaml $1/$2/$2.yaml &
rosrun map_server store_img.py $1 $2 $3 &
cp -r $1/now $1/now_bk_$2
#sh /home/relaybot/api_ws/src/operation/src/choice_action/gmapping_stop.sh
#input: folder of now which can store map and related files. $1 is path name(/var/www/maps as default.), $2 is unique map id. $3 is map name.
#function: move to folder of which with the same name and same name files within. 

# rosnode kill convertimg &

# kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &
