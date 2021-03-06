#include "ros/ros.h"  
#include <gpsbot_navigation/edit_nav_task.h>
#include <gpsbot_navigation/nav_flag.h>
#include <gpsbot_navigation/execute_nav_task.h>
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
  ros::init(argc, argv, "service_edit_nav_task");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<gpsbot_navigation::edit_nav_task>("/edit_nav_task");  
  ros::ServiceClient client1 = n.serviceClient<gpsbot_navigation::nav_flag>("/nav_flag");  
  ros::ServiceClient client2 = n.serviceClient<gpsbot_navigation::execute_nav_task>("/execute_nav_task");  

  gpsbot_navigation::edit_nav_task srv;
  gpsbot_navigation::execute_nav_task srv2;
  gpsbot_navigation::nav_flag srv1;
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
  string name="company";
  srv1.request.map_id=3;
  srv1.request.map_name=name.c_str();
  srv1.request.nav_flag=1;
  
  srv2.request.map_id=3;
  srv2.request.map_name=name.c_str();
  srv2.request.task_id=2;
  srv2.request.rate=3;
  srv2.request.type=1;
  
//   if (client.call(srv))  
//   {  
// //     ROS_INFO("errormsg: %s", srv.response.errormsg);  
//     ROS_INFO("successed:%d",(int)srv.response.successed);
//   }  
//   else  
//   {  
//     ROS_ERROR("Failed to call service /edit_nav_task");  
//     return 1;  
//   }  
    if (client1.call(srv1))  
  {  
//     ROS_INFO("errormsg: %s", srv.response.errormsg);  
    ROS_INFO("successed:%d",(int)srv1.response.successed);
  }  
  else  
  {  
    ROS_ERROR("Failed to call service /execute_nav_task");  
    return 1;  
  }  
    if (client2.call(srv2))  
  {  
//     ROS_INFO("errormsg: %s", srv.response.errormsg);  
    ROS_INFO("successed:%d",(int)srv2.response.successed);
  }  
  else  
  {  
    ROS_ERROR("Failed to call service /nav_flag");  
    return 1;  
  }  
  
  return 0;  
}  