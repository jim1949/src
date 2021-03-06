
/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: pose from the picture frame(frame 1), target frame(frame 2).
output:pose in the target frame.

*/
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <tf_learning/pose_tf.h>
using namespace std; 



//frame1: picture_frame
//frame2: map

geometry_msgs::Pose transform_point(geometry_msgs::Pose & pose_in_frame1,const string & frame1,const string & frame2){
    ROS_INFO("start transform.");
    tf::TransformListener listener;
    geometry_msgs::PoseStamped PoseStamped_in_frame1,PoseStamped_in_frame2;
    geometry_msgs::Pose pose_in_frame2;
    string pic_frame("picture_frame");
    ROS_INFO("frame1:%s",frame1.c_str());

    cout<< " "<< pose_in_frame1.position.x<< " "<< pose_in_frame1.position.y<< " "<< pose_in_frame1.position.z<< " "<< pose_in_frame1.orientation.x<< " "<< pose_in_frame1.orientation.y<< " "<< pose_in_frame1.orientation.z<< " "<< pose_in_frame1.orientation.w<< " "<< endl;
    if (frame1==pic_frame){
        ROS_INFO("frame1 is picture_frame");
        PoseStamped_in_frame1.pose.position.x=pose_in_frame1.position.x/20;
        PoseStamped_in_frame1.pose.position.y=pose_in_frame1.position.y/20;
        PoseStamped_in_frame1.pose.position.z=pose_in_frame1.position.z/20;
    }
    else{
        ROS_INFO("frame1 not picture frame.");
    }
	PoseStamped_in_frame1.pose.orientation=pose_in_frame1.orientation;
    PoseStamped_in_frame1.header.frame_id=frame1;
    PoseStamped_in_frame1.header.stamp=ros::Time();
    //PoseStamped_in_frame1.pose=pose_in_frame1;
    cout<< " "<< PoseStamped_in_frame1.pose.position.x<< " "<< PoseStamped_in_frame1.pose.position.y<< " "<< PoseStamped_in_frame1.pose.position.z<< " "<< PoseStamped_in_frame1.pose.orientation.x<< " "<< PoseStamped_in_frame1.pose.orientation.y<< " "<< PoseStamped_in_frame1.pose.orientation.z<< " "<< PoseStamped_in_frame1.pose.orientation.w<< " "<< endl;

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
    exit(0);
    }
    }

    if (frame2==pic_frame){
        ROS_INFO("frame2 is picture_frame");
        PoseStamped_in_frame2.pose.position.x=PoseStamped_in_frame2.pose.position.x*20.0;
        PoseStamped_in_frame1.pose.position.y=PoseStamped_in_frame2.pose.position.y*20.0;
        PoseStamped_in_frame1.pose.position.z=PoseStamped_in_frame2.pose.position.z*20.0;
    }
    else{
        ROS_INFO("frame2 not picture frame.");
    }


    pose_in_frame2=PoseStamped_in_frame2.pose;

    cout<< " "<< pose_in_frame2.position.x<< " "<< pose_in_frame2.position.y<< " "<< pose_in_frame2.position.z<< " "<< pose_in_frame2.orientation.x<< " "<< pose_in_frame2.orientation.y<< " "<< pose_in_frame2.orientation.z<< " "<< pose_in_frame2.orientation.w<< " "<< endl;
    return pose_in_frame2;

}

geometry_msgs::Pose transform_point(const float x_frame1,const float y_frame1,const float angle_frame1,const string & frame1,const string & frame2){
    ROS_INFO("start transform.");
    tf::TransformListener listener;
    geometry_msgs::PoseStamped PoseStamped_in_frame1,PoseStamped_in_frame2;
    geometry_msgs::Pose pose_in_frame2;
    if (frame1=="picture_frame"){
        ROS_INFO("it's picture_frame");
        PoseStamped_in_frame1.pose.position.x=x_frame1/20;
        PoseStamped_in_frame1.pose.position.y=y_frame1/20;
    }
    else{
        ROS_INFO("not picture frame.");
    }


    PoseStamped_in_frame1.header.frame_id=frame1;
    PoseStamped_in_frame1.header.stamp=ros::Time();

    tf::Quaternion q;
    q.setRPY(0,0,angle_frame1);
    PoseStamped_in_frame1.pose.orientation.x=q.getX();
    PoseStamped_in_frame1.pose.orientation.y=q.getY();
    PoseStamped_in_frame1.pose.orientation.z=q.getZ();
    PoseStamped_in_frame1.pose.orientation.w=q.getW();
    cout<< " "<< PoseStamped_in_frame1.pose.position.x<< " "<< PoseStamped_in_frame1.pose.position.y<< " "<< PoseStamped_in_frame1.pose.position.z<< " "<< PoseStamped_in_frame1.pose.orientation.x<< " "<< PoseStamped_in_frame1.pose.orientation.y<< " "<< PoseStamped_in_frame1.pose.orientation.z<< " "<< PoseStamped_in_frame1.pose.orientation.w<< " "<< endl;

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
    exit(0);
    }
    }

    pose_in_frame2=PoseStamped_in_frame2.pose;

    cout<< " "<< pose_in_frame2.position.x<< " "<< pose_in_frame2.position.y<< " "<< pose_in_frame2.position.z<< " "<< pose_in_frame2.orientation.x<< " "<< pose_in_frame2.orientation.y<< " "<< pose_in_frame2.orientation.z<< " "<< pose_in_frame2.orientation.w<< " "<< endl;
    return pose_in_frame2;

}
