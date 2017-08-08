#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#define pi 3.1415926


geometry_msgs::Pose pose_listen(tf::StampedTransform &transform_listener){
    geometry_msgs::Pose pose;
    pose.position.x=transform_listener.getOrigin().x()*20;
    pose.position.y=transform_listener.getOrigin().y()*20;
    pose.position.z=transform_listener.getOrigin().z()*20;
    pose.orientation.x=transform_listener.getRotation().getX();
    pose.orientation.y=transform_listener.getRotation().getY();
    pose.orientation.z=transform_listener.getRotation().getZ();
    pose.orientation.w=transform_listener.getRotation().getW();
    
    return pose;
}

geometry_msgs::PointStamped transformPoint(const tf::TransformListener& listener){
// we'll create a point in the base laser frame that transform to the picture frame.
    geometry_msgs::PointStamped scan_point[3];
    geometry_msgs::PointStamped base_point[3];
    for(int i=1;i<3;i++){
     scan_point[i].header.stamp=ros::Time();
     scan_point[i].header.frame_id="base_footprint";  
     scan_point[i].point.x=5*cos(i*pi);
     scan_point[i].point.y=5*sin(i*pi);
     scan_point[i].point.z=0;

     try {
       
       listener.transformPoint("picture_frame",scan_point[i],base_point[i]);
      ROS_INFO("base_laser: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",scan_point[i].point.x, scan_point[i].point.y, scan_point[i].point.z,base_point[i].point.x, base_point[i].point.y, base_point[i].point.z, base_point[i].header.stamp.toSec());
     }catch(tf::TransformException& ex){ ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_link\": %s", ex.what());}

    }

}

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");

  ros::NodeHandle node;

  ros::Publisher world_pose = node.advertise<geometry_msgs::Pose>("/picture_pose", 2);
  ROS_INFO("start listener");

  tf::TransformListener listener;

  ros::Rate rate(20.0);
  while (node.ok()){
    tf::StampedTransform transform_listener;

    try {
        listener.waitForTransform("/picture_frame", "/base_footprint", ros::Time(0), ros::Duration(10.0) );
        listener.lookupTransform("/picture_frame", "/base_footprint", ros::Time(0), transform_listener);
    } catch (tf::TransformException ex) {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
        continue;
}

    geometry_msgs::Pose pose;
    pose=pose_listen(transform_listener);
// publish the pose of the robot in the map.
    world_pose.publish(pose);
  
    // tf::TransformListener listener(ros::Duration(10));
    // ros::Timer timer=node.createTimer(ros::Duration(1.0),boost::bind(&transformPoint,boost::ref(listener)));
  
    // geometry_msgs::PoseStamped scan_point[3];
    // for(int i=1;i<3;i++){
    //  scan_point[i].header.stamp=ros::Time();
    //  scan_point[i].header.frame_id="base_footprint";  
    //  scan_point[i].point.x=i;
    //  scan_point[i].point.y=0;
    //  scan_point[i].point.z=0;

    // }
    // tf::TransformListener listener;
    // geometry_msgs::PointStamped scan_point[3];
    // geometry_msgs::PointStamped base_point[3];
    
    // for(int i=1;i<3;i++)
    // {
    //  scan_point[i].header.stamp=ros::Time();
    //  scan_point[i].header.frame_id="base_footprint";  
    //  scan_point[i].point.x=5*cos(i*pi);
    //  scan_point[i].point.y=5*sin(i*pi);
    //  scan_point[i].point.z=0;
    //  ROS_INFO("%lf,%lf,%lf",scan_point[i].point.x,scan_point[i].point.y,scan_point[i].point.z);
    //  try {
       
    //    listener.transformPoint("picture_frame",scan_point[i],base_point[i]);
    //   ROS_INFO("base_laser: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",scan_point[i].point.x, scan_point[i].point.y, scan_point[i].point.z,base_point[i].point.x, base_point[i].point.y, base_point[i].point.z, base_point[i].header.stamp.toSec());
    //  }catch(tf::TransformException& ex){ ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_link\": %s", ex.what());}

    // }
    ros::spinOnce();
    rate.sleep();
    
  }
  return 0;
};