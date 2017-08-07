#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <nav_msgs/MapMetaData.h>
#define pi 3.1415926

int map_height;
float origin_x,origin_y,origin_z,q_x,q_y,q_z,q_w,resolution;
bool flag=false;

void chatterCallback(const nav_msgs::MapMetaData &msg){
  ROS_INFO("I heared");
  flag=true;
map_height=msg.height;
origin_x=msg.origin.position.x;
origin_y=msg.origin.position.y;
origin_z=msg.origin.position.z;
q_x=msg.origin.orientation.x;
q_y=msg.origin.orientation.y;
q_z=msg.origin.orientation.z;
q_w=msg.origin.orientation.w;
resolution=msg.resolution;
ROS_INFO("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d",origin_x,origin_y,origin_z,q_x,q_y,q_z,q_w,map_height);

}



int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;

  ros::Subscriber sub = node.subscribe("map_metadata", 1000, chatterCallback);
  tf::TransformBroadcaster br;
  


 
  ros::Rate rate(20.0);
  while (node.ok()){
    if(flag){
    tf::Transform transform;
    tf::Transform transform2;
    // Here we create a new transform, from the parent map to the new child world. 
    // The carrot1 frame is 2 meters offset to the left from the turtle1 frame. 
    transform.setOrigin( tf::Vector3(origin_x,  origin_y, origin_z) );
    transform.setRotation( tf::Quaternion(q_x, q_y, q_z, q_w) );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "world"));
    
    tf::Quaternion q;
    q.setRPY(3.1415926, 0, 0);
    transform2.setRotation(q);
    transform2.setOrigin( tf::Vector3(0,  map_height*resolution, 0.0) );
    br.sendTransform(tf::StampedTransform(transform2, ros::Time::now(), "world", "picture_frame"));
    }
    ros::spinOnce();
    
  }
  return 0;
};