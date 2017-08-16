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
  ROS_INFO("arg in second:%d",argc);
  if (srv.request.data==2){
  srv.request.map_path=argv[2];
  srv.request.map_id=argv[3];
  srv.request.map_name=argv[4];
  // ROS_INFO(srv.request.map_path);
  ROS_INFO("map_path: %s,map_id :%s ",srv.request.map_path.c_str(),srv.request.map_id.c_str());
  }
  
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