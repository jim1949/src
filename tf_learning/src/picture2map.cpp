
/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: pose from the picture frame(frame 1), target frame(frame 2).
output:pose in the target frame.

*/
#include <ros/ros.h>
#include <basic_msgs/nav_pose_set.h>
#include <basic_msgs/wall_pose_set.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <cstdio>
using namespace std; 



//frame1: picture_frame
//frame2: map
geometry_msgs::Pose transform_point(geometry_msgs::Pose & pose_in_frame1,const string & frame1,const string & frame2){
    
    tf::TransformListener listener;
    geometry_msgs::PoseStamped PoseStamped_in_frame1,PoseStamped_in_frame2;
    geometry_msgs::Pose pose_in_frame2;
    if (frame1=="picture_frame"){
        cout<<"it's picture_frame"<<endl;
        pose_in_frame1.position.x=pose_in_frame1.position.x/20;
        pose_in_frame1.position.y=pose_in_frame1.position.y/20;
        pose_in_frame1.position.z=pose_in_frame1.position.z/20;
    }
    else{
        cout<<"not picture frame."<<endl;
    }

    PoseStamped_in_frame1.header.frame_id="frame1";
    PoseStamped_in_frame1.header.stamp=ros::Time();

    bool flag=false;
    
    while(flag==false){
    try{
    listener.waitForTransform(frame2, frame1, ros::Time(), ros::Duration(10.0) );
    listener.transformPose(frame2, PoseStamped_in_frame1,PoseStamped_in_frame2);
    ROS_INFO("transform from a point from %s to %s without any error ",frame1.c_str(),frame2.c_str());
    flag=true;
    }
    catch(tf::TransformException& ex){
    flag=false;
    ROS_ERROR("Received an exception trying to transform a point from %s to %s: %s",frame1.c_str(),frame2.c_str(), ex.what());
    }
    }

    pose_in_frame2.position=pose_in_frame2.position;
    pose_in_frame2.orientation=pose_in_frame2.orientation;
    // cout<<pose_in_frame2.position.x<<pose_in_frame2.position.y<<pose_in_frame2.position.z<<pose_in_frame2.orientation.x<<pose_in_frame2.orientation.y<<pose_in_frame2.z<<endl;
    return pose_in_frame2;

}

int main(int argc,char **argv){
ros::init(argc, argv, "pose_transform_node");
ros::NodeHandle n;
// ros::ServiceServer nav_service=n.advertiseService("/nav_pose_set",nav_server);
geometry_msgs::Pose pose_in_frame1;


const string frame1("/picture_frame");
const string frame2("/map");
transform_point(pose_in_frame1,frame1,frame2);
ros::spin();
return 0;
}