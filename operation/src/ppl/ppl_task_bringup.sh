#!/bin/bash


echo "ppl_task_bringup start. "
echo $1/$2/$2.yaml

# param->map_path+" "+param->map_id+" "+param->map_name


roslaunch operation frame.launch map_file:=$1/$2/$2.yaml &

# produce the path in robot_path.txt.
# got the path txt(/var/www/path/$1/... ).now ppl_src/...
sleep 1
#=====================there is some bug with this ppl.cpp
roslaunch operation ppl_task_bringup.launch map_info:="$1 $2 $3" 

# read the path in /picture_frame, and write it inside /map.
sleep 1
roslaunch operation robot_path_tf.launch map_info:="$1 $2 $3" 

rosnode kill map_server
rosnode kill picture_frame_broadcaster
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 10

# =======进度条
# b=''
# i=0
# while [ $i -le  100 ]
# do
#     printf "progress:[%-50s]%d%%\r" $b $i
#     sleep 0.1
#     i=`expr 2 + $i`       
#     b=#$b
# done
# echo