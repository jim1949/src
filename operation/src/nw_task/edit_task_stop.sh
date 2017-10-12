#!/bin/bash
# kill -9 `pgrep -f ros`


#complete the mapping process.




rosnode kill edit_nav_task
echo "rosnode kill edit_nav_task"
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 9



