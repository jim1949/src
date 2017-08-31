#include "ros/ros.h"  
#include <gpsbot_navigation/edit_nav_task.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#pragma comment(lib, "json_mtd.lib")
#include <cstdio>
using namespace std;  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "edit_nav_task");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<gpsbot_navigation::edit_nav_task>("/edit_nav_task");  
  gpsbot_navigation::edit_nav_task srv;
// int32 map_id
// string map_name
// int32 type
// int32[] nav_id
// int32 task_id
  
  // srv.request.mapid = argv[1]; 
  ROS_INFO("arg in second:%d",argc);
  // if (srv.request.mapid==2){
  // // srv.request.map_name=argv[2];
  // // srv.request.type=argv[3];
  // // srv.request.task_id=argv[4];
  // // srv.request.nav_id=[1,2,3];
  // // ROS_INFO(srv.request.map_path);
 
  // }
  
  if (client.call(srv))  
  {  
//     ROS_INFO("errormsg: %s", srv.response.errormsg);  
    ROS_INFO("successed:%d",(int)srv.response.successed);
  }  
  else  
  {  
    ROS_ERROR("Failed to call service /edit_nav_task");  
    return 1;  
  }  
  
  return 0;  
}  