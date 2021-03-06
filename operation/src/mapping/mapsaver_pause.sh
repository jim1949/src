#!/bin/bash
#rosnode kill picture_frame_broadcaster
#rosnode kill pose_in_picture_frame_node 
#rosnode kill lasers_in_picture_frame_node 

echo "mapsaver pause."
mkdir $1/$2 

sleep 0.5
cp $1/now/map.pgm $1/$2/$2.pgm 

cp $1/now/map.yaml $1/$2/$2.yaml 
rosrun map_server store_img.py $1 $2 $3 
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
echo map saved to $1/$2/$2.pgm
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 1