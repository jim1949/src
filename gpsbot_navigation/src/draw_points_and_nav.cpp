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
#include <basic_msgs/gridpose.h>
#include <geometry_msgs/Twist.h>  
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

#include <geometry_msgs/Pose.h>


#include "geometry_msgs/Quaternion.h"
#include <tf/transform_listener.h>
#include <signal.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#pragma comment(lib, "json_mtd.lib")

using namespace std;


//0：not ready,1:start,2:pause;3:stop;
//global parameter from navigation


int nav_flag_=0;
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
vector<geometry_msgs::Pose> p_vec_;

basic_msgs::gridpose grid_pose;
geometry_msgs::Pose last_pose;

ros::Publisher cmdVelPub;

//everything is ready flag.
// int flag=false;

//covariance
double covariance_[36]={0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.06853891945200942};

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
  grid_pose=req.gridpose;
  ROS_INFO("nav_flag:%d",nav_flag_);
  if (nav_flag_==1){
    ROS_INFO("Localized.");
    ROS_INFO("grid_pose: %f,%f,%f",grid_pose.x,grid_pose.y,grid_pose.angle);
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
  ROS_INFO("execute the service.map_Id:%d, map_name:%s, task_id:%d. type:%d",req.map_id,req.map_name.c_str(),req.task_id,req.type);

  string path;
  ROS_INFO("nav_flag:%d",nav_flag_);
  if (nav_flag_==2){
    
    exe_map_id=req.map_id;
    exe_map_name=req.map_name;
    exe_task_id=req.task_id;
    exe_rate=req.rate;
    exe_type=req.type;
    ROS_INFO("nav_flag is 1,localized already,start to navigate for task %d",exe_task_id);
    if (exe_map_id!=nav_map_id){ROS_ERROR("exe_map_id:%d and nav_map_id:%d not matched.",exe_map_id,nav_map_id);}
    else {ROS_INFO("exe_map_id:%d and nav_map_id:%d  matched.",exe_map_id,nav_map_id);}
    if (exe_map_name!=nav_map_name){ROS_ERROR("exe_map_name:%s and nav_map_name:%s not matched.",exe_map_name.c_str(),nav_map_name.c_str());}
    else {ROS_INFO("exe_map_name:%s and nav_map_name:%s matched.",exe_map_name.c_str(),nav_map_name.c_str());}
    res.successed=1;
  
  }
  
  return true;
}


vector<geometry_msgs::Pose> draw_nav_pose(vector<int> nav_id_vec,ros::Publisher& marker_pub){
        // flag=false;
        ROS_INFO("start to draw.");
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
          path="/var/www/nav_manager/"+map_id_path+"/nav_pose/"+nav_id_path+".json";
          ROS_INFO("go to nav_id:%d",nav_id_vec[i]);
          ifstream is;
          is.open(path.c_str()); 
          if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of json path ERROR.");
          ROS_INFO("nav_manager json:%s",path.c_str());
          } 

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
        ROS_INFO("draw finished");
        return p_vec_;
    
}

bool callback(const geometry_msgs::PoseWithCovarianceStamped& msg){
  //update initial pose;
  // initial_pose=msg;
  ROS_INFO("got the initial_pose!!!");
  return true;
}

void doneCb(const actionlib::SimpleClientGoalState& state,const move_base_msgs::MoveBaseResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());

}
 
// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}
 
// Called every time feedback is recieved for the goal
void feedbackCb(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback)
{ 
  static  int i=0;
  ROS_INFO("Got Feedback of length %d",i++);
}

void pose_callback(const geometry_msgs::Pose& msg){

last_pose=msg;

}


geometry_msgs::PoseWithCovarianceStamped transformInitalpose(){
    geometry_msgs::PoseWithCovarianceStamped initial_pose;
    tf::Quaternion quat;
    bool flag=false;
	static int sq=1;
    //euler to quaternion.
    double t1 = 0;
    double t2 = 0;
    double t3 = grid_pose.angle;
	ROS_INFO("Orientation:last pose orientation:.x:%f,y:%f,z:%f,w:%f",last_pose.orientation.x,last_pose.orientation.y,last_pose.orientation.z,last_pose.orientation.w);
    tf::quaternionMsgToTF(last_pose.orientation,quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    ROS_INFO("roll:%f,pitch:%f,yaw:%f",roll,pitch,yaw);
    if (pitch>=0) ROS_INFO("Got a positive number for pitch.");
    else ROS_INFO("Got a negative number for pitch.");

    yaw=yaw-t3;
	  ROS_INFO("t3:%f, yaw:%f",t3,yaw);
    // the tf::Quaternion has a method to acess roll pitch and yaw

    tf::TransformListener listener;
    geometry_msgs::PoseStamped gridpose_picture;
    geometry_msgs::PoseStamped gridpose;


    gridpose_picture.header.frame_id="/picture_frame";
    gridpose_picture.header.stamp=ros::Time();
    gridpose_picture.pose.position.x=(grid_pose.x)/20.0;
    gridpose_picture.pose.position.y=(grid_pose.y)/20.0;

    gridpose_picture.pose.orientation.w = -sin(t1 / 2.0) * sin(t2 / 2.0) * sin(yaw / 2.0) + cos(t1 / 2.0) * cos(t2 / 2.0) * cos(yaw / 2.0);
    gridpose_picture.pose.orientation.x = sin(t1 / 2.0) * cos(t2 / 2.0) * cos(yaw / 2.0) + sin(t2 / 2.0) * sin(yaw / 2.0) * cos(t1 / 2.0);
    gridpose_picture.pose.orientation.y = -sin(t1 / 2.0) * sin(yaw / 2.0) * cos(t2 / 2.0) + sin(t2 / 2.0) * cos(t1 / 2.0) * cos(yaw / 2.0);
    gridpose_picture.pose.orientation.z = sin(t1 / 2.0) * sin(t2 / 2.0) * cos(yaw / 2.0) + sin(yaw / 2.0) * cos(t2 / 2.0) * cos(t2 / 2.0);

    
    while(flag==false){
    try{
      listener.waitForTransform("picture_frame", "map", ros::Time(), ros::Duration(10.0) );
      listener.transformPose("/map", gridpose_picture, gridpose);
      ROS_INFO("transform from a point from picture_frame to map without any error ");
      flag=true;
    }
    catch(tf::TransformException& ex){
      flag=false;
      ROS_ERROR("Received an exception trying to transform a point from \"picture_frame\" to \"map\": %s", ex.what());
    }
    }
    initial_pose.pose.pose=gridpose.pose;
    initial_pose.header=gridpose.header;
	initial_pose.header.seq=sq++;
    for (int i=0;i<36;i++){
    initial_pose.pose.covariance[i]=covariance_[i];
    }
    // initial_pose.pose.covariance
    return initial_pose;
}


bool nav_start(vector<geometry_msgs::Pose> p_vec_,MoveBaseClient& client,int nav_rate,ros::NodeHandle& n){

  //arg in navigation.
  int n_locations=p_vec_.size();
  int n_goals=0;
  int n_successes=0;
  
  int start_time=ros::Time::now().toSec();
  float running_time=0;
  
  bool finished_within_time,getinitialpose_within_time;
  move_base_msgs::MoveBaseGoal goal;

  // //0.make sure we have initial pose,no time limited
  // ROS_INFO("waiting for the initialpose message");
  // ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("initialpose");

  // ros::Subscriber sub=n.subscribe("initialpose",1,callback);

  //循环条件：nav_flag=2,<循环rate,not shutdown.
 for(int n_rate=0;(nav_flag_==2)&&(n_rate<nav_rate);n_rate++){
   ROS_INFO("rate:%d",n_rate);
  for(int i=0;(i<n_locations)&&(ros::ok());i++)
  {
      //1.if done, restart from beginning.
      if (i>=n_locations) 
      i=0;
      
      //2.calculate the distance.

      //3.send goal to move_base.
      goal.target_pose.pose=p_vec_[i];
      goal.target_pose.header.frame_id="map";
      goal.target_pose.header.stamp=ros::Time::now();
      
      //  client.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

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

      //6.shutdown. cmd_vel publish.
     
      signal(SIGINT, shutdown);
  }
  }
  exe_type=2;//execution is finished.
  ROS_INFO("finish this task.");
  return true;
}

//task_id->task->pose_id
int main( int argc, char** argv )
{
  ros::init(argc, argv, "points_and_lines",ros::init_options::NoSigintHandler);
  ros::NodeHandle n;
  ROS_INFO("start draw points and navigation");
  // nav_info nav;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);//
  ros::Publisher initialpose_pub=n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",1);
  ros::Subscriber pose_sub=n.subscribe("robot_pose",1,pose_callback);
  ros::ServiceServer execute_task=n.advertiseService("/execute_nav_task",execute_server);
  ros::ServiceServer nav_flag=n.advertiseService("/nav_flag",nav_flag_server);
  MoveBaseClient client("move_base",true);//true: don't need ros::spin().
  
  ros::Rate r(10);
  Json::Reader reader;
  Json::Value root;
  //initial pose;
  geometry_msgs::PoseWithCovarianceStamped initial_pose;

  //read the initialpose from yaml.


  //---

  ros::Duration(3).sleep();

  while (ros::ok())
  { ros::spinOnce();
    //parameter definition
    //marker definition
   
     if (nav_flag_ == 1){
       ROS_INFO("Got the initial pose!");
       //publish the initial pose.

       initial_pose=transformInitalpose();
       initialpose_pub.publish(initial_pose);

       ROS_INFO("nav_flag:%d,exe_type:%d",nav_flag_,exe_type);
       //---
       nav_flag_=2;
    }
   // nav_flag_=2;
    while ((nav_flag_== 2)&&(exe_type == 1)){
      // ROS_INFO("Start navigation.");
      nav_id_vec.clear();
      string map_id_path,task_json,task_id_path;
      stringstream ss,os;
      ss<<nav_map_id;
      ss>>map_id_path;
      os<<exe_task_id;
      os>>task_id_path;

      task_json="/var/www/task_manager/"+map_id_path+"/"+task_id_path+".json";
      ifstream is;
      // ROS_INFO("task path:%s",task_json.c_str());
      is.open(task_json.c_str()); 

      if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of json path of task manager ERROR.");}
      if (root["map_id"].asInt()!=exe_map_id){ROS_ERROR("In the task:%d, exe_map_id:%d and task_map_id:%d not matched",exe_task_id,exe_map_id,root["map_id"].asInt());}

      
      int num=root["nav_id"].size();
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

      ros::spinOnce();
    }

  }

}
