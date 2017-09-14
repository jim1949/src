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

//todo:
1.shutdown and change the goal at any time.

*/

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Pose.h>
#include <gpsbot_navigation/execute_nav_task.h>
#include <gpsbot_navigation/nav_flag.h>
#include <geometry_msgs/Twist.h>  
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <signal.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#pragma comment(lib, "json_mtd.lib")

#include <cstdio>

using namespace std;

//0：not ready,1:start,2:pause;3:stop;
//global parameter from navigation
int nav_flag_=0;
int nav_map_id=3;
string nav_map_name;

//global parameter from execution.
int exe_map_id=3;
string exe_map_name;
int exe_task_id=2;
int exe_rate=100;
// type:1:start,2:pause,3:stop
int exe_type=1;

//global paramter store the navigation array
vector<int> nav_id_vec;
vector<geometry_msgs::Pose> p_vec_;
//initial pose;
geometry_msgs::PoseWithCovarianceStamped initial_pose;


ros::Publisher cmdVelPub;

//everything is ready flag.
// int flag=false;

void shutdown(int sig)
{
  cmdVelPub.publish(geometry_msgs::Twist());//使机器人停止运动
  ROS_INFO("Navigation ended!");
  ros::shutdown();
}


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


bool nav_flag_server(gpsbot_navigation::nav_flag::Request& req,gpsbot_navigation::nav_flag::Response& res){
  ROS_INFO("navigation is ready! Localized already!");
  nav_flag_=req.nav_flag;
  nav_map_id=req.map_id;
  nav_map_name=req.map_name;
  if (nav_flag_==1){
    ROS_INFO("Localized.");
  }
  else if(nav_flag_==2){
    ROS_INFO("Haven't localized.");
  }
  else{
    ROS_INFO("Wrong nav_flag.");

  }
  
  res.successed=1;

  return true;
}

bool execute_server(gpsbot_navigation::execute_nav_task::Request& req,gpsbot_navigation::execute_nav_task::Response& res){
  ROS_INFO("execute the service.");

  string path;

  if (exe_map_id!=nav_map_id){ROS_ERROR("exe_map_id:%d and nav_map_id:%d not matched.",exe_map_id,nav_map_id);}
  if (exe_map_name!=nav_map_name){ROS_ERROR("exe_map_name:%s and nav_map_name:%s not matched.",exe_map_name.c_str(),nav_map_name.c_str());}
  if (nav_flag_==1){
    ROS_INFO("nav_flag is 1,localized already,start to navigate for task %d",exe_task_id);
    exe_map_id=req.map_id;
    exe_map_name=req.map_name;
    exe_task_id=req.task_id;
    exe_rate=req.rate;
    exe_type=req.type;
  
  }
  res.successed=1;
  return true;
}


vector<geometry_msgs::Pose> draw_nav_pose(vector<int> nav_id_vec,ros::Publisher& marker_pub){
        // flag=false;
        ROS_INFO("start to draw");
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
        p_vec_.clear();
        // Create the vertices for the points and lines
        for (uint32_t i = 0; i < nav_id_vec.size(); i++)
        {
          // ROS_INFO("path:%s",path[i].c_str());
          string path,map_id_path,nav_id_path;
          stringstream ss,os;
          ss<<exe_map_id;
          ss>>map_id_path;
          os<<nav_id_vec[i];
          os>>nav_id_path;

          Json::Reader reader;
          Json::Value root;

          // char map_id_path[8],nav_id_path[8];
          // sprintf(map_id_path,"%d",exe_map_id);
          // sprintf(nav_id_path,"%d",nav_id_vec[i]);
          // path="http://192.168.31.130:82/nav_manager/3/nav_pose/4.json"
          path="/var/www/nav_manager/"+map_id_path+"/nav_pose/"+nav_id_path+".json";//**
        //   ROS_INFO("%s",path.c_str());
          ifstream is;
          is.open(path.c_str()); 
          if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of nav_manager json path ERROR.");} 

          geometry_msgs::Pose p;
          p.position.x = root["nav_pose"]["position"]["x"].asDouble();
          p.position.y = root["nav_pose"]["position"]["y"].asDouble();
          p.position.z = root["nav_pose"]["position"]["z"].asDouble();
          p.orientation.w=1;
          p.orientation.x=0;
          p.orientation.y=0;
          p.orientation.z=0;
          // ROS_INFO("x:%f,y:%f,z:%f",p.x,p.y,p.z);

          p_vec_.push_back(p);
          points.points.push_back(p.position);
          line_strip.points.push_back(p.position);

          // The line list needs two points for each line
          line_list.points.push_back(p.position);
          p.position.z += 1.0;
          line_list.points.push_back(p.position);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(line_list);
        // flag=true;
        return p_vec_;
    
}

void callback(const geometry_msgs::PoseWithCovarianceStamped& msg){
  //update initial pose;
  initial_pose=msg;

}

void doneCb(const actionlib::SimpleClientGoalState& state,
            const move_base_msgs::MoveBaseResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());

}
 
// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}
 
// Called every time feedback is received for the goal
void feedbackCb(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback)
{ static  int i=0;
  ROS_INFO("Got Feedback of length %d",i++);
}
 


bool nav_start(vector<geometry_msgs::Pose> p_vec_,MoveBaseClient& client,int nav_rate,ros::NodeHandle& n){

  //arg in navigation.
  int n_locations=p_vec_.size();
  int n_goals=0;
  int n_successes=0;
  int i=0;
  int start_time=ros::Time::now().toSec();
  float running_time=0;
  int n_rate=0;
  bool finished_within_time,getinitialpose_within_time;
  move_base_msgs::MoveBaseGoal goal;

  //0.make sure we have initial pose,no time limited**
//   ROS_INFO("waiting for the initialpose message");
//   getinitialpose_within_time=ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("initialpose");

//   ros::Subscriber sub=n.subscribe("initialpose",1,callback);

  //循环条件：nav_flag=2,<循环rate,not shutdown.
  ROS_INFO("nav_start");
  ROS_INFO("nav_flag_:%d,n_rate:%d,nav_rate:%d",nav_flag_,n_rate,nav_rate);
  while((nav_flag_==2) && (n_rate<nav_rate) &&(ros::ok())){
      ROS_INFO("start to nav.");
  //1.if done, restart from beginning.
  if (i>=n_locations) i=0;
  
  //2.calculate the distance.

  //3.send goal to move_base.
  goal.target_pose.pose=p_vec_[i];
  goal.target_pose.header.frame_id="map";
  goal.target_pose.header.stamp=ros::Time::now();
//   client.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  client.sendGoal(goal);
  //4.wait for result. finished on time? get state. cancel goal.
  finished_within_time=client.waitForResult(ros::Duration(300));
  if (!finished_within_time) {
    ROS_INFO("Timed out to achieve goal %d.",i);
    client.cancelGoal();
  }
  else {
    
    if (client.getState()==actionlib::SimpleClientGoalState::SUCCEEDED){
      ROS_INFO("Goal %d SUCCESS!",i);
      ROS_INFO("Goal pose: x:%f, y:%f, z:%f",p_vec_[i].position.x,p_vec_[i].position.y,p_vec_[i].position.z);
      n_successes+=1;

    }
    else {
      ROS_INFO("Goal %d Failed! with state:%s",i,client.getState().toString().c_str());

    }
  }
  //5.get the running time.
  running_time=ros::Time::now().toSec()-start_time;
  running_time=running_time/60.0;
  i++;

  //6.shutdown. cmd_vel publish.

  signal(SIGINT, shutdown);
  }
    return true;
}

//task_id->task->pose_id
int main( int argc, char** argv )
{
  ros::init(argc, argv, "points_and_lines_test",ros::init_options::NoSigintHandler);
  ros::NodeHandle n;
  // nav_info nav;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);//
  ros::Publisher initialpose_pub=n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",1);
  ros::ServiceServer execute_task=n.advertiseService("/execute_nav_task",execute_server);
  ros::ServiceServer nav_flag=n.advertiseService("/nav_flag",nav_flag_server);
  MoveBaseClient client("move_base",true);//true: don't need ros::spin().
  
  ros::Rate r(10);
  Json::Reader reader;
  Json::Value root;

  //read the initialpose from yaml.


  //---

  ros::Duration(3).sleep();

  while (ros::ok())
  {
    //parameter definition
    //marker definition
    nav_flag_ = 1;
    exe_type = 1;

    if (nav_flag_ == 1){
      ROS_INFO("got the initial pose");
      //write the initialpose back to yaml.


      //---
      nav_flag_=2;
    }
    ROS_INFO("nav_flag:%d,exe_type:%d",nav_flag_,exe_type);
    
    while ((nav_flag_== 2)&&(exe_type == 1)){
      nav_id_vec.clear();
      string map_id_path,task_json,task_id_path;
      stringstream ss,os;
      ss<<3;
      ss>>map_id_path;
      os<<2;
      os>>task_id_path;

      task_json="/var/www/task_manager/"+map_id_path+"/"+task_id_path+".json";
      static int i=0;
      if (i++==0)
        ROS_INFO("%s",task_json.c_str());
      ifstream is;
      is.open(task_json.c_str()); 
    //   ROS_INFO("hah1");
    //   if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of json path of task manager ERROR.");
    //  ROS_INFO("hah2");
    //   if (root["map_id"].asInt()!=exe_map_id){ROS_ERROR("In the task:%d, exe_map_id:%d and task_map_id:%d not matched",exe_task_id,exe_map_id,root["map_id"].asInt());}
    //   else {ROS_INFO("In the task:%d, exe_map_id:%d and task_map_id:%d  matched",exe_task_id,exe_map_id,root["map_id"].asInt());}
      if (reader.parse(is, root, false)) {
      int num=root["nav_id"].size();
    //   ROS_INFO("hah");
      ROS_INFO("size of nav_id in task is:%d",num);
      for(int i=0;i<num;i++){
        nav_id_vec.push_back(root["nav_id"][i].asInt());
      }
      //vector needs to write here.
      vector<geometry_msgs::Pose> p_vec_;
      //draw the navigation points in the rviz.
      p_vec_=draw_nav_pose(nav_id_vec,marker_pub);

      //start navigation.
      nav_start(p_vec_,client,exe_rate,n);
      


    }

  }
}
}