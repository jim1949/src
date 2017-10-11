#!/bin/bash

echo "mapsaver bringup start."
roslaunch operation mapping.launch &
sleep 15
#This method is not a good way. because actually, we don't know whether the progress is finished or not. Still need to be fixed in the future.
rostopic pub -1 /status std_msgs/Int32 0


