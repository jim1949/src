#!/bin/bash
roslaunch map_server multi_map_saver.launch &
roslaunch operation operation_buildmap.launch
