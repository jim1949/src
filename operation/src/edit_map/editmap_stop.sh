#!/bin/bash
# kill -9 `pgrep -f ros`


#need to save the pose.

# need to kill these nodes

# /map_server
# /operation_draw_wall
# /operation_nw_listener
# /picture_frame_broadcaster


#complete the mapping process.

rosnode kill operation_nw_listener &
rosnode kill operation_draw_wall &
rosnode kill picture_frame_broadcaster &
rosnode kill map_server &
sleep 1
cd /home/relaybot/api_ws/src/shell_rclocal/
./killnode.sh

echo "rosnode kill finished."
echo "edit_map stop... "


