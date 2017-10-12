/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: 
1.map positions: path, name.
2.some initial parameters of the robot. Which will be set in the future.(Could be set as a global parameter.)
functions:
1.get the positions, and store. give response.
2.transform the nav positions to map frame, and store in json file or SQL file.
3.draw wall_positions

nav_pose_set.srv:


*/
#include <ros/ros.h>
#include <basic_msgs/nav_pose_set.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>



#pragma comment(lib, "json_mtd.lib")
 
#include <cstdio>
using namespace std; 
bool flag=false;
// geometry_msgs::Pose transform_point(geometry_msgs::Pose &nav_pose_picture){
    
//     tf::TransformListener listener;
//     geometry_msgs::PointStamped navpose_picture;
//     geometry_msgs::PointStamped navpose;
//     geometry_msgs::Pose nav_pose;
//     navpose_picture.header.frame_id="/picture_frame";
//     navpose_picture.header.stamp=ros::Time();
//     navpose_picture.point.x=(nav_pose_picture.position.x)/20;
//     navpose_picture.point.y=(nav_pose_picture.position.y)/20;
//     navpose_picture.point.z=(nav_pose_picture.position.z)/20;

//     flag=false;
//     while(flag==false){
//     try{
//     listener.waitForTransform("picture_frame", "map", ros::Time(), ros::Duration(10.0) );
//     listener.transformPoint("/map", navpose_picture, navpose);
//     ROS_INFO("transform from a point from picture_frame to map without any error ");
//     flag=true;
//     }
//     catch(tf::TransformException& ex){
//     flag=false;
//     ROS_ERROR("Received an exception trying to transform a point from \"picture_frame\" to \"map\": %s", ex.what());
//     }
//     }
//     nav_pose.position=navpose.point;
//     nav_pose.orientation=nav_pose_picture.orientation;

//     return nav_pose;


// }
// Json::Value transfer_nav_json(basic_msgs::nav_pose_set::Request &req){
//     Json::Value root;
//     int map_id=req.nav_pose.mapid;
//     string map_name=req.nav_pose.mapname;
//     int nav_id=req.nav_pose.id;
//     string nav_name=req.nav_pose.name;
//     geometry_msgs::Pose nav_pose_picture=req.nav_pose.worldposition;

//     geometry_msgs::Pose nav_pose=transform_point(nav_pose_picture);
    
//     int type=req.nav_pose.type;
//     root["type"]=type;
//     root["map_id"]=map_id;
//     root["map_name"]=map_name;
//     root["nav_id"]=nav_id;
//     root["nav_name"]=nav_name;
//     root["nav_pose"]["position"]["x"]=nav_pose.position.x;
//     root["nav_pose"]["position"]["y"]=nav_pose.position.y;
//     root["nav_pose"]["position"]["z"]=nav_pose.position.z;

//     root["nav_pose"]["orientation"]["x"]=nav_pose.orientation.x;
//     root["nav_pose"]["orientation"]["y"]=nav_pose.orientation.y;
//     root["nav_pose"]["orientation"]["z"]=nav_pose.orientation.z;
//     root["nav_pose"]["orientation"]["w"]=nav_pose.orientation.w;
//     ROS_INFO("map_id:%d",map_id);
//     return root;

// }

// void delete_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
//     ofstream ofs; 
//     string path="/var/www/nav_manager/";
//     char file_id[4],mapfile_id[4];
//     sprintf(file_id,"%d",req.nav_pose.id);
//     sprintf(mapfile_id,"%d",req.nav_pose.mapid);

//     string folder_build_command="rm "+path+mapfile_id+"/nav_pose/"+file_id+".json";
//     system(folder_build_command.c_str());
// }

// void add_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
//     ofstream ofs; 
//     string path="/var/www/nav_manager/";
//     char file_id[4],mapfile_id[4];
//     sprintf(file_id,"%d",req.nav_pose.id);
//     sprintf(mapfile_id,"%d",req.nav_pose.mapid);
//     ROS_INFO("mapfile_id:%d",req.nav_pose.mapid);
//     string json_path=path+mapfile_id+"/nav_pose/"+file_id+".json";
//     string folder_build_command="mkdir -p "+path+"/"+mapfile_id+"/nav_pose/";
//     ROS_INFO("%s",folder_build_command.c_str());
//     system(folder_build_command.c_str());
//     ofs.open(json_path.c_str());
//     ofs<<root.toStyledString();
//     ofs.close();
// }

// void update_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
//     ofstream ofs; 
//     string path="/var/www/nav_manager/";
//     char file_id[4],mapfile_id[4];
//     sprintf(file_id,"%d",req.nav_pose.id);
//     sprintf(mapfile_id,"%d",req.nav_pose.mapid);

//     // There is a bug here, when we don't have the file, and update it, we will create a new file. But personally, I don't think this will be a problem.
//     string json_path=path+mapfile_id+"/nav_pose/"+file_id+".json";
//     string folder_build_command="rm "+json_path;
//     system(folder_build_command.c_str());

//     ofs.open(json_path.c_str());
//     ofs<<root.toStyledString();
//     ofs.close();    

// }

// bool nav_server(basic_msgs::nav_pose_set::Request &req, basic_msgs::nav_pose_set::Response &res){
//     ROS_INFO("I got the nav_pose message!");
//     ROS_INFO("name:%s",req.nav_pose.name.c_str());
//     Json::Value root;
//     char s[200];

//     root=transfer_nav_json(req);
//     // 1:delete,2:add,3:update.
//     ROS_INFO("type:%d",root["type"].asInt());
//     if (root["type"]==1)
//     {
//         delete_nav_json(req,root);
//         sprintf(s,"got the delete message! pose.x: %f",req.nav_pose.worldposition.position.x);
        
//     }
//     else if(root["type"]==2){
//         add_nav_json(req,root);
//         sprintf(s,"got the add message! pose.x: %f",req.nav_pose.worldposition.position.x);
//     }
//     else if(root["type"]==3){
//         update_nav_json(req,root);
//         sprintf(s,"got the update message! pose.x: %f",req.nav_pose.worldposition.position.x);
//     }
//     else{
//         sprintf(s,"wrong type for type=0");
//     }

//     // ----response
     
//     res.errormsg=s;
//     return true;

// }

int main(int argc,char **argv){
    ros::init(argc, argv, "ppl_nw_listener_node");
    ros::NodeHandle n;  
    string map_path,map_id,map_name;
    ROS_INFO("The map path arg in ppl:%d",argc);
    if (argc==2){
        map_path=argv[1];
        map_id=argv[2];
        map_name=argv[3];
        ROS_INFO("map_path:%s,map_id:%s,map_name:%s",map_path.c_str(),map_id.c_str(),map_name.c_str());

        //read the map.


        //process.

    }
    ros::spin();
    return 0;
}