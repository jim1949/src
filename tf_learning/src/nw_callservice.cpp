#include "ros/ros.h"  
#include "basic_msgs/nav_pose_set.h"  
#include "basic_msgs/wall_pose_set.h"
#include <tf_learning/pose_tf.h>
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "navposeSet_client");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<basic_msgs::nav_pose_set>("nav_pose_set");
  ros::ServiceClient client1 = n.serviceClient<basic_msgs::wall_pose_set>("wall_pose_set");  
  basic_msgs::nav_pose_set srv;
  basic_msgs::wall_pose_set srv1;
    
  srv.request.nav_pose.name=argv[1];
  srv.request.nav_pose.id=atoi(argv[2]);
  srv.request.nav_pose.mapid=atoi(argv[3]);
  srv.request.nav_pose.type=atoi(argv[4]);
  srv.request.nav_pose.x=5;
  srv.request.nav_pose.y=0;
  srv.request.nav_pose.angle=0;

  // srv1.request.wall_pose.name=argv[5];
  // srv1.request.wall_pose.id=atoi(argv[2]);
  // srv1.request.wall_pose.mapid=atoi(argv[3]);
  // srv1.request.wall_pose.type=atoi(argv[6]);

  // srv1.request.wall_pose.worldposition[0].position.x=1.0;
  // srv1.request.wall_pose.worldposition[1].position.x=5.0;
   
  if (client.call(srv))  
  {  
//     ROS_INFO("errormsg: %s", srv.response.errormsg);  
    ROS_INFO("successed:%d  %s",(int)srv.response.successed, srv.response.errormsg.c_str());
    
  }  
  else  
  {  
    ROS_ERROR("Failed to call service /nav_pose_set");  
    return 1;  
  }  
  
//     if (client.call(srv1))  
//   {  
// //     ROS_INFO("errormsg: %s", srv.response.errormsg);  
//     ROS_INFO("wall pose successed:%d  %s",(int)srv1.response.successed, srv1.response.errormsg.c_str());
    
//   }  
//   else  
//   {  
//     ROS_ERROR("Failed to call service /wall_pose_set");  
//     return 1;  
//   }  
  

  return 0;  
}  
