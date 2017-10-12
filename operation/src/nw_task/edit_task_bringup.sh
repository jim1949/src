#!/bin/bash

# edit task.
echo "task starts to be edited. "
echo $1/$2/$2.yaml
roslaunch operation edit_task.launch map_file:=$1/$2/$2.yaml &
sleep 2
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 8