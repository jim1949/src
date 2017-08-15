#include "ros/ros.h"  
#include "basic_msgs/nav_pose_set.h"  
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "navposeSet_client");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<basic_msgs::nav_pose_set>("/nav_pose_set");  
  basic_msgs::nav_pose_set srv;
    
  srv.request.nav_pose.name="haha";
  srv.request.nav_pose.id=10;
   
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
  
  return 0;  
}  