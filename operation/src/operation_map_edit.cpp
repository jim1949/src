// as a operation server to monitor everything happened on the robot.
/*
should be the monitor of navigation status:
1.need set maploader_bringup_path,maploader_stop_path.
2.service(navigation_status):
  0)start navigation
      maploader_bringup.sh(start map_server launch file. tf_learning :start publish laser and robot pose to\
      the map.)
  1)pause navigation(stop tf_learning.)
  2)stop navigation(stop map_server.transfer the map folder to another folder with unique id.)




*/
#include "ros/ros.h"  
#include "operation/navigation_status.h" 
#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string>
#include <iostream>
  
int parameter=-1;
pthread_t id;
int i,ret;
std::string maploader_bringup_path,maploader_pause_path,maploader_stop_path;

struct Param{
int type;
std::string map_path;
std::string map_name;
std::string map_id;

};

void *thread(void *ptr)
{
Param *param = (Param *)ptr;
ROS_INFO("type: %d",param->type);
// printf("%s",maploader_bringup_path.c_str());

std::string sh="sh ";
std::string bringup=sh+maploader_bringup_path;
std::string pause=sh+maploader_pause_path;
std::string stop=sh+maploader_stop_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::cout<<stop;

if (param->type==-1) {
  ROS_INFO("-1");
  ROS_INFO("wrong input for navigation.");

} 
if (param->type==3) {
 ROS_INFO("start to navigation ...");

  system(bringup.c_str());
   }

else if(param->type==4) {
  ROS_INFO("pause the map ...");
  system(pause.c_str());
}
else if (param->type==5){
  system(stop.c_str());
  


}
else {

}

}


bool server(operation::navigation_status::Request &req, operation::navigation_status::Response &res)
{
    Param *param=new Param();
    param->type=req.data;
    param->map_name=req.map_name;
    param->map_id=req.map_id;
    param->map_path=req.map_path;
    ret=pthread_create(&id,NULL,thread,param);
    if(ret!=0){
    printf ("Create pthread error!\n");
    exit (1);
    }

    if (param->type==-1) {
    ROS_INFO("Not started navigation yet.");
    } 
    if (req.data==3) {
    ROS_INFO("0");
    res.successed=true;
    res.errormsg="start navigation";
    }

    else if(req.data==4) {
    ROS_INFO("1");
    res.successed=true;
    res.errormsg="pause navigation";
    }
    else if (req.data==5){

    res.successed=true;
    res.errormsg="stop navigation";

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
   ros::init(argc, argv, "operation_nav");  
   ros::NodeHandle n;
   
   ros::ServiceServer service = n.advertiseService("/navigation_status", server); 
   ros::param::param<std::string>("maploader_bringup_path",maploader_bringup_path,"/home/relaybot/api_ws/src/operation/src/nav_sh/maploader_bringup.sh");
   ros::param::param<std::string>("maploader_pause_path",maploader_pause_path,"/home/relaybot/api_ws/src/operation/src/nav_sh/maploader_pause.sh");
   ros::param::param<std::string>("maploader_stop_path",maploader_stop_path,"/home/relaybot/api_ws/src/operation/src/nav_sh/maploader_stop.sh");
     
   ROS_INFO("Provide service which can be called to tell the server to start navigation, pause, or stop.");  
   printf("now we are in parent progress");
   pthread_join(id,NULL);
   ros::spin();   
   ROS_INFO("Whole process is finished.");
 
  return 0;  
}  
