#!/bin/bash
# kill -9 `pgrep -f ros`

echo "mapsaver save, complete saving the map." &
#complete the mapping process.

mkdir $1/$2 &
cp $1/now/map.pgm $1/$2/$2.pgm &
cp $1/now/map.yaml $1/$2/$2.yaml &
rosrun map_server store_img.py $1 $2 $3 

rosnode kill arduino &
rosnode kill base_link_to_laser4 
rosnode kill joint_state_publisher 

rosnode kill move_base &
rosnode kill velocity_smoother 
rosnode kill nodelet_manager &
rosnode kill robot_pose_publisher &
rosnode kill robot_state_publisher &
rosnode kill sick_tim551_2050001 &
rosnode kill slam_gmapping &

rosnode kill map_saver &
rosnode kill convert_img_ros 
rosnode kill amcl & 

rosnode kill lasers_in_picture_frame_node 

rosnode kill picture_frame_broadcaster 
rosnode kill pose_in_picture_frame_node &
sleep 2
cd /home/relaybot/api_ws/src/shell_rclocal/
./killnode.sh
echo map saved to $1/$2/$2.pgm
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 2



#sh /home/relaybot/api_ws/src/operation/src/choice_action/gmapping_stop.sh
#input: folder of now which can store map and related files. $1 is path name(/var/www/maps as default.), $2 is unique map id. $3 is map name.
#function: move to folder of which with the same name and same name files within. 

# rosnode kill convertimg &

# kill -9 `ps -x | grep map_saver|grep -v grep |  awk '{print $1}'`
# kill -9 `ps -x | grep tf_learning|grep -v grep |  awk '{print $1}'`
# roslaunch navigation_stage move_base_gmapping_5cm.launch &
# rosrun rosbridge_server rosbridge_websocket &
