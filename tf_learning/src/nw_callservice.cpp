#include "ros/ros.h"  
#include "basic_msgs/nav_pose_set.h"  
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "navposeSet_client");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<basic_msgs::nav_pose_set>("nav_pose_set");  
  basic_msgs::nav_pose_set srv;
    
  srv.request.nav_pose.name=argv[1];
  srv.request.nav_pose.id=atoi(argv[2]);
  srv.request.nav_pose.mapid=atoi(argv[3]);
  srv.request.nav_pose.type=atoi(argv[4]);
   
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