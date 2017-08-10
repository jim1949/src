// as a operation server to monitor everything happened on the robot.
#include "ros/ros.h"  
#include "operation/mapping_status.h" 
#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string>
#include <iostream>
  
int parameter=-1;
pthread_t id;
int i,ret;
std::string mapsaver_bringup_path,mapsaver_stop_path;

struct Param{
int type;

};

void *thread(void *ptr)
{
Param *param = (Param *)ptr;
ROS_INFO("type: %d",param->type);
// printf("%s",mapsaver_bringup_path.c_str());

std::string sh="sh ";
std::string bringup=sh+mapsaver_bringup_path;
std::string stop=sh+mapsaver_stop_path;
std::cout<<stop;

if (param->type==-1) {
  ROS_INFO("-1");

} 
if (param->type==0) {
 ROS_INFO("0");

  system(bringup.c_str());
   }

else if(param->type==1) {
  ROS_INFO("1");

}
else if (param->type==2){
  system(stop.c_str());


}
else {

}

}


bool server(operation::mapping_status::Request &req, operation::mapping_status::Response &res)
{
Param *param=new Param();
param->type=req.data;
ret=pthread_create(&id,NULL,thread,param);
if(ret!=0){
printf ("Create pthread error!\n");
exit (1);
}

if (param->type==-1) {
ROS_INFO("Not started mapping yet.");
} 
if (req.data==0) {
 ROS_INFO("0");
  res.successed=true;
  res.errormsg="start mapping";
   }

else if(req.data==1) {
  ROS_INFO("1");
  res.successed=true;
  res.errormsg="pause mapping";
}
else if (req.data==2){

res.successed=true;
res.errormsg="stop mapping";

}
else {
  
  res.successed=false;
  res.errormsg="wrong call service,no such service.";
}

sleep(1);




return true;

}

int main(int argc, char **argv)  
{ 
  //  signal(SIGCHLD,SIG_IGN);//avoid zombie process 
   ros::init(argc, argv, "mapping_status_node");  
   ros::NodeHandle n;
   
   ros::ServiceServer service = n.advertiseService("/mapping_status", server); 
   ros::param::param<std::string>("mapsaver_bringup_path",mapsaver_bringup_path,"/home/jimmy/jimmy_catkin_ws/src/operation/src/mapsaver_bringup.sh");
   ros::param::param<std::string>("mapsaver_stop_path",mapsaver_stop_path,"/home/jimmy/jimmy_catkin_ws/src/operation/src/mapsaver_stop.sh");
     
   ROS_INFO("Provide service which can be called to tell the server to start mapping, pause, or stop.");  
   printf("now we are in parent progress");
   pthread_join(id,NULL);
   ros::spin();   
   ROS_INFO("Whole process is finished.");
 
  return 0;  
}  