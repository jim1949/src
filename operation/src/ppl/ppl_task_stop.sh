#!/bin/bash
# kill -9 `pgrep -f ros`

echo "[shell] ppl_task stop. "

rosnode kill operation_ppl
rosnode kill robot_path_tf
rosnode kill map_server
rosnode kill picture_frame_broadcaster

cd /home/relaybot/api_ws/src/shell_rclocal/
./killnode.sh 
echo "rosnode kill finished."
rostopic pub -1 /mapping_status_feedback std_msgs/Int32 11


