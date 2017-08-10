#include "ros/ros.h"  
#include "operation/mapping_status.h"  
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "mapping_status_client");  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<operation::mapping_status>("/mapping_status");  
  operation::mapping_status srv;
    
  srv.request.data = atoll(argv[1]);  
   
  if (client.call(srv))  
  {  
//     ROS_INFO("errormsg: %s", srv.response.errormsg);  
    ROS_INFO("successed:%d",(int)srv.response.successed);
  }  
  else  
  {  
    ROS_ERROR("Failed to call service /mapping_status");  
    return 1;  
  }  
  
  return 0;  
}  