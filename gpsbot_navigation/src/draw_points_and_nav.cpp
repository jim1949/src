/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

excute the navigation task of nav poses in the task (vector) service called from android.
input: 
1.nav_flag is ready.//set as a global flag in the service.
2.nav_task name. nav_task executation rate,nav_task start,pause,stop.

functions:
1.ready to 
2.draw the nave poses in the task on the map.
3.execute the task.

*/

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>
#include <gpsbot_navigation/execute_nav_task.h>
#include <gpsbot_navigation/nav_flag.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#pragma comment(lib, "json_mtd.lib")

#include <cstdio>

using namespace std;
// class nav_info{
//   public:
//     int nav_num;
//     int waypoints[];
//     bool execute_server(gpsbot_navigation::execute_nav_task::Request& req,gpsbot_navigation::execute_nav_task::Response& res);
//     bool nav_flag_server(gpsbot_navigation::nav_flag::Request& req,gpsbot_navigation::nav_flag::Response& res);
// };
//0：not ready,1:start,2:pause;3:stop;
//global parameter from navigation
int nav_flag=0;
int nav_map_id;
string nav_map_name;

//global parameter from execution.
int exe_map_id;
string exe_map_name;
int exe_task_id;
int exe_rate;
// type:1:start,2:pause,3:stop
int exe_type;

//global paramter store the navigation array
vector<int> nav_id_vec;



bool nav_flag_server(gpsbot_navigation::nav_flag::Request& req,gpsbot_navigation::nav_flag::Response& res){
  ROS_INFO("navigation is ready! Localized already!");
  nav_flag=req.nav_flag;
  nav_map_id=req.map_id;
  nav_map_name=req.map_name;
  if (nav_flag==1){
    ROS_INFO("Localized.");
  }
  else if(nav_flag==2){
    ROS_INFO("Haven't localized.");
  }
  else{
    ROS_INFO("Wrong nav_flag.")

  }
  
  res.successed=1;

  return true;
}

bool execute_server(gpsbot_navigation::execute_nav_task::Request& req,gpsbot_navigation::execute_nav_task::Response& res){
  ROS_INFO("execute the service.");

  string path;

  if (exe_map_id!=nav_map_id){ROS_ERROR("exe_map_id:%d and nav_map_id:%d not matched.",exe_map_id,nav_map_id);}
  if (exe_map_name!=nav_map_name){ROS_ERROR("exe_map_name:%d and nav_map_name:%d not matched.",exe_map_name,nav_map_name);}
  if (nav_flag==1){
    ROS_INFO("localized already,start to navigate for task %d",exe_task_id);
    exe_map_id=req.map_id;
    exe_map_name=req.map_name;
    exe_task_id=req.task_id;
    exe_rate=req.rate;
    exe_type=req.type;
  
  }
  res.successed=1;
  return true;
}


void draw_nav_pose(vector<int> nav_id_vec){

        visualization_msgs::Marker points, line_strip, line_list;
        points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/map";
        points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
        points.ns = line_strip.ns = line_list.ns = "points_and_lines";
        points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
        points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;

        points.id = 0;
        line_strip.id = 1;
        line_list.id = 2;

        points.type = visualization_msgs::Marker::POINTS;
        line_strip.type = visualization_msgs::Marker::LINE_STRIP;
        line_list.type = visualization_msgs::Marker::LINE_LIST;

        // POINTS markers use x and y scale for width/height respectively
        points.scale.x = 0.2;
        points.scale.y = 0.2;

        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        line_strip.scale.x = 0.1;
        line_list.scale.x = 0.1;

        // Points are green
        points.color.g = 1.0f;
        points.color.a = 1.0;

        // Line strip is blue
        line_strip.color.b = 1.0;
        line_strip.color.a = 1.0;

        // Line list is red
        line_list.color.r = 1.0;
        line_list.color.a = 1.0;

        // Create the vertices for the points and lines
        for (uint32_t i = 0; i < nav_id_vec.size(); i++)
        {
          // ROS_INFO("path:%s",path[i].c_str());
          string path;
          char map_id_path[8],;
          sprintf(map_id_path,"%d",exe_map_id);
          sprintf(nav_id_path,"%d",nav_id_vec[i]);
          // path="http://192.168.31.130:82/nav_manager/3/nav_pose/4.json"
          path="/var/www/nav_manager/"+exe_map_id+"/nav_pose/"+nav_id_path;
          ifstream is;
          is.open(path.c_str()); 
          if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of json path ERROR.");} 

          geometry_msgs::Point p;
          p.x = root["nav_pose"]["position"]["x"].asDouble();
          p.y = root["nav_pose"]["position"]["y"].asDouble();
          p.z = root["nav_pose"]["position"]["z"].asDouble();
          // ROS_INFO("x:%f,y:%f,z:%f",p.x,p.y,p.z);

          points.points.push_back(p);
          line_strip.points.push_back(p);

          // The line list needs two points for each line
          line_list.points.push_back(p);
          p.z += 1.0;
          line_list.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(line_list);

        return p;
    
}

//task_id->task->pose_id
int main( int argc, char** argv )
{
  ros::init(argc, argv, "points_and_lines");
  ros::NodeHandle n;
  // nav_info nav;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);//
  ros::ServiceServer execute_task=n.advertiseService("/execute_nav_task",execute_server);
  ros::ServiceServer nav_flag=n.advertiseService("/nav_flag",nav_flag_server);
  ros::Rate r(10);
  Json::Reader reader;
  Json::Value root;

  while (ros::ok())
  {
    //parameter definition
    //marker definition
    if ((nav_flag==1)&&(exe_type==1)){
      nav_id_vec.clear();
      string map_id_path,task_json;
      stringstream ss,os;
      ss<<nav_map_id;
      ss>>map_id_path;
      os<<exe_task_id;
      os>>exe_task_id;

      task_json="/var/www/task_manager"+map_id_path+"/"+task_id+".json";
      ifstream is;
      is.open(task_id.c_str()); 
      if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of json path of task manager ERROR.");
      if (root["map_id"].asInt()!=exe_map_id){ROS_ERROR("In the task:%d, exe_map_id:%d and task_map_id:%d not matched",exe_task_id,exe_map_id,root["map_id"].asInt());}
      
      int num=root["nav_id"].size();
      ROS_INFO("size of nav_id in task is:%d",num);
      for(int i=0;i<num;i++){
        nav_id_vec.push_back(root["nav_id"][i].asInt());
      }
      //vector needs to write here.
      geometry_msgs::Point p;
      p=draw_nav_pose(nav_id_vec);
      nav_flag=2;
    }
      while(nav_flag==2)
      {


      }

  }
}
