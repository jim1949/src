#!/bin/bash
# kill -9 `pgrep -f ros`


#complete the mapping process.




rosnode kill edit_nav_task
echo "rosnode kill edit_nav_task"
rostopic pub -1 /status std_msgs/Int32 9



