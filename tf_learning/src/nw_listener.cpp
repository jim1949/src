/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: nav and wall positions (vector) service called from android.
functions:
1.get the positions, and store. give response.
2.transform the nav positions to map frame, and store in json file or SQL file.
3.draw wall_positions

//
nav_pose_set.srv:
basic_msgs/nav_pose nav_pose(

# navigation point in a map
string birthtime
float64 gridX
float64 gridY
float64 angle
int32 id
string name
int32 mapid
string mapname
int32 type
geometry_msgs/Pose worldposition
)

---
float64[] data
string errormsg
bool  successed

*/
#include <ros/ros.h>
#include <basic_msgs/nav_pose_set.h>
#include <basic_msgs/wall_pose_set.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <tf_learning/pose_tf.h>
#include <visualization_msgs/Marker.h>

#pragma comment(lib, "json_mtd.lib")
 
#include <cstdio>
#define pi 3.1415926
using namespace std; 
bool flag=false;

geometry_msgs::Pose nav_pose;
bool nav_flag=false;
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
Json::Value transfer_nav_json(basic_msgs::nav_pose_set::Request &req){
    Json::Value root;
    int map_id=req.nav_pose.mapid;
    string map_name=req.nav_pose.mapname;
    int nav_id=req.nav_pose.id;
    string nav_name=req.nav_pose.name;

    const string frame1("picture_frame");
    const string frame2("map");

    nav_pose=transform_point(req.nav_pose.x,req.nav_pose.y,(360-req.nav_pose.angle)*pi/180.0,frame1,frame2);
    nav_flag=true;
    int type=req.nav_pose.type;
    root["type"]=type;
    root["map_id"]=map_id;
    root["map_name"]=map_name;
    root["nav_id"]=nav_id;
    root["nav_name"]=nav_name;
    root["nav_pose"]["position"]["x"]=nav_pose.position.x;
    root["nav_pose"]["position"]["y"]=nav_pose.position.y;
    root["nav_pose"]["position"]["z"]=nav_pose.position.z;

    root["nav_pose"]["orientation"]["x"]=nav_pose.orientation.x;
    root["nav_pose"]["orientation"]["y"]=nav_pose.orientation.y;
    root["nav_pose"]["orientation"]["z"]=nav_pose.orientation.z;
    root["nav_pose"]["orientation"]["w"]=nav_pose.orientation.w;
    ROS_INFO("map_id:%d",map_id);
    return root;

}
Json::Value transfer_wall_json(basic_msgs::wall_pose_set::Request &req){
    // still need to edit...
    Json::Value root;
    Json::Value poses;
    Json::Value pose;
    int map_id=req.wall_pose.mapid;
    string map_name=req.wall_pose.mapname;
    int wall_id=req.wall_pose.id;
    string wall_name=req.wall_pose.name;
    int type=req.wall_pose.type;
    int num=req.wall_pose.worldposition.size();
    ROS_INFO("size of wall pose is:%d",num);
    for (int i=0;i<num;i++){
        pose["x"]=req.wall_pose.worldposition[i].position.x;
        pose["y"]=req.wall_pose.worldposition[i].position.y;
        pose["z"]=req.wall_pose.worldposition[i].position.z;
        poses.append(pose);
    }

    
    root["type"]=type;
    root["map_id"]=map_id;
    root["map_name"]=map_name;
    root["wall_id"]=wall_id;
    root["wall_name"]=wall_name;
    root["wall_poses"]=poses;
    return root;

}

void delete_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.nav_pose.id);
    sprintf(mapfile_id,"%d",req.nav_pose.mapid);

    string folder_build_command="rm "+path+mapfile_id+"/nav_pose/"+file_id+".json";
    system(folder_build_command.c_str());
}

void add_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.nav_pose.id);
    sprintf(mapfile_id,"%d",req.nav_pose.mapid);
    ROS_INFO("mapfile_id:%d",req.nav_pose.mapid);
    string json_path=path+mapfile_id+"/nav_pose/"+file_id+".json";
    string folder_build_command="mkdir -p "+path+"/"+mapfile_id+"/nav_pose/";
    ROS_INFO("%s",folder_build_command.c_str());
    system(folder_build_command.c_str());
    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    ofs.close();
}

void update_nav_json(basic_msgs::nav_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.nav_pose.id);
    sprintf(mapfile_id,"%d",req.nav_pose.mapid);

    // There is a bug here, when we don't have the file, and update it, we will create a new file. But personally, I don't think this will be a problem.
    string json_path=path+mapfile_id+"/nav_pose/"+file_id+".json";
    string folder_build_command="rm "+json_path;
    system(folder_build_command.c_str());

    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    ofs.close();    

}

void delete_wall_json(basic_msgs::wall_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.wall_pose.id);
    sprintf(mapfile_id,"%d",req.wall_pose.mapid);

    string folder_build_command="rm "+path+mapfile_id+"/wall_pose/"+file_id+".json";
	
    system(folder_build_command.c_str());
}
void add_wall_json(basic_msgs::wall_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.wall_pose.id);
    ROS_INFO("mapid:%d",req.wall_pose.mapid);
    ROS_INFO("mapid:%d",root["map_id"].asInt());
    sprintf(mapfile_id,"%d",req.wall_pose.mapid);

    string json_path=path+mapfile_id+"/wall_pose/"+file_id+".json";
    ROS_INFO("wall_pose json_path:%s",json_path.c_str());
    string folder_build_command="mkdir -p "+path+mapfile_id+"/wall_pose/";
    system(folder_build_command.c_str());
    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    ofs.close();
}
void update_wall_json(basic_msgs::wall_pose_set::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/nav_manager/";
    char file_id[4],mapfile_id[4];
    sprintf(file_id,"%d",req.wall_pose.id);
    sprintf(mapfile_id,"%d",req.wall_pose.mapid);

    // There is a bug here, when we don't have the file, and update it, we will create a new file. But personally, I don't think this will be a problem.
    string json_path=path+mapfile_id+"/wall_pose/"+file_id+".json";
    string folder_build_command="rm "+json_path;
    system(folder_build_command.c_str());

    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    ofs.close();   
}

bool nav_server(basic_msgs::nav_pose_set::Request &req, basic_msgs::nav_pose_set::Response &res){
    ROS_INFO("I got the nav_pose message!");
    ROS_INFO("name:%s",req.nav_pose.name.c_str());
    Json::Value root;
    char s[200];

    root=transfer_nav_json(req);
    // 1:delete,2:add,3:update.
    ROS_INFO("type:%d",root["type"].asInt());
    if (root["type"].asInt()==1)
    {
        delete_nav_json(req,root);
        sprintf(s,"got the delete message! pose.x: %f",req.nav_pose.x);
        
    }
    else if(root["type"].asInt()==2){
        add_nav_json(req,root);
        sprintf(s,"got the add message! pose.x: %f",req.nav_pose.x);
    }
    else if(root["type"].asInt()==3){
        update_nav_json(req,root);
        sprintf(s,"got the update message! pose.x: %f",req.nav_pose.x);
    }
    else{
        sprintf(s,"wrong type for type=0");
    }

    // ----response
     
    res.errormsg=s;
    return true;

}


bool wall_server(basic_msgs::wall_pose_set::Request &req,basic_msgs::wall_pose_set::Response &res){
    ROS_INFO("I got the wall_pose message!");
    ROS_INFO("name:%s",req.wall_pose.name.c_str());
    Json::Value root;
    char s[200];

    root=transfer_wall_json(req);
    // 1:delete,2:add,3:update.
    if (root["type"]==1)
    {
        delete_wall_json(req,root);
        sprintf(s,"got the delete message! ");
        
    }
    else if(root["type"]==2){
        add_wall_json(req,root);
        sprintf(s,"got the add message! ");
    }
    else if(root["type"]==3){
        update_wall_json(req,root);
        sprintf(s,"got the update message!");
    }
    else{
        sprintf(s,"wrong type for type=0");
    }

    // ----response
     
    res.errormsg=s;
    return true;
}

int main(int argc,char **argv){
ros::init(argc, argv, "nav_and_wall_positions_node");
ros::NodeHandle n;
ros::ServiceServer nav_service=n.advertiseService("/nav_pose_set",nav_server);
ros::ServiceServer wall_service=n.advertiseService("/wall_pose_set",wall_server);
ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
ros::Rate r(1);

// Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::ARROW;

  while (ros::ok())
  { 
    ros::spinOnce();
    if (nav_flag &&ros::ok()){
    visualization_msgs::Marker marker;

      // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";

    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.pose.position.x=nav_pose.position.x;
    marker.pose.position.y=nav_pose.position.y;
    marker.pose.position.z=nav_pose.position.z;
    marker.pose.orientation.x=nav_pose.orientation.x;
    marker.pose.orientation.y=nav_pose.orientation.y;
    marker.pose.orientation.z=nav_pose.orientation.z;
    marker.pose.orientation.w=nav_pose.orientation.w;
    ROS_INFO("nav_pose got:%f %f %f %f \n %f %f %f",nav_pose.position.x,nav_pose.position.y,nav_pose.position.z,nav_pose.orientation.x,nav_pose.orientation.y,nav_pose.orientation.z,nav_pose.orientation.w);
    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    // while (marker_pub.getNumSubscribers() < 1)
    // {
    //   if (!ros::ok())
    //   {
    //     return 0;
    //   }
    //   ROS_WARN_ONCE("Please create a subscriber to the marker");
    //   sleep(1);
    // }
    marker_pub.publish(marker);

    // Cycle between different shapes
    // switch (shape)
    // {
    // case visualization_msgs::Marker::CUBE:
    //   shape = visualization_msgs::Marker::SPHERE;
    //   break;
    // case visualization_msgs::Marker::SPHERE:
    //   shape = visualization_msgs::Marker::ARROW;
    //   break;
    // case visualization_msgs::Marker::ARROW:
    //   shape = visualization_msgs::Marker::CYLINDER;
    //   break;
    // case visualization_msgs::Marker::CYLINDER:
    //   shape = visualization_msgs::Marker::CUBE;
    //   break;
    // }
}   
    r.sleep();
  
  }
}
// while(ros::ok()){

//     visualization_msgs::Marker marker;
//       // Set the frame ID and timestamp.  See the TF tutorials for information on these.
//     marker.header.frame_id = "map";
//     marker.header.stamp = ros::Time::now();

//     // Set the namespace and id for this marker.  This serves to create a unique ID
//     // Any marker sent with the same namespace and id will overwrite the old one
//     marker.ns = "basic_shapes";
//     marker.id = 0;

//     // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
//     marker.type = visualization_msgs::Marker::ARROW;;

//     // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
//     marker.action = visualization_msgs::Marker::ADD;

//     // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
//     marker.pose=nav_pose;

//     // Set the scale of the marker -- 1x1x1 here means 1m on a side
//     marker.scale.x = 1.0;
//     marker.scale.y = 1.0;
//     marker.scale.z = 1.0;

//     // Set the color -- be sure to set alpha to something non-zero!
//     marker.color.r = 0.0f;
//     marker.color.g = 1.0f;
//     marker.color.b = 0.0f;
//     marker.color.a = 1.0;

//     marker.lifetime = ros::Duration();
//     marker_pub.publish(marker);
    
// r.sleep();

// ros::spinOnce();
// }

// return 0;
// }
