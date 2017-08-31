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
1.draw the nave poses in the task on the map.
2.execute the task.

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



bool execute_server(gpsbot_navigation::execute_nav_task::Request& req,gpsbot_navigation::execute_nav_task::Response& res){
ROS_INFO("execute");
return true;
}

bool nav_flag_server(gpsbot_navigation::nav_flag::Request& req,gpsbot_navigation::nav_flag::Response& res){

return true;
}

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

  int map_id=3;
  string map_id_path;
  string path[4];
  stringstream ss;
  ss<<map_id;
  ss>>map_id_path;
  ss.str("");
  path[0]="/var/www/nav_manager/"+map_id_path+"/nav_pose/3.json";//
  path[1]="/var/www/nav_manager/"+map_id_path+"/nav_pose/4.json";//
  path[2]="/var/www/nav_manager/"+map_id_path+"/nav_pose/5.json";//
  path[3]="/var/www/nav_manager/"+map_id_path+"/nav_pose/6.json";//

  while (ros::ok())
  {

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
    for (uint32_t i = 0; i < 4; i++)
    {
      // ROS_INFO("path:%s",path[i].c_str());
      ifstream is;
      is.open(path[i].c_str());
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

    r.sleep();


  }
}
