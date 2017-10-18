// as a operation server to monitor everything happened on the robot.
/*
should be the monitor of mapping status:
1.need set mapsaver_bringup_path,mapsaver_pause_path,mapsaver_cancel_path,mapsaver_save_path.
2.service(mapping_status):
  0)start mapping
      mapsaver_bringup.sh(start map_server launch file. tf_learning :start publish laser and robot pose to\
      the map.)
  1)pause mapping(stop tf_learning.)
  2)save mapping(stop map_server.transfer the map folder to another folder with unique id.)
  3)cancel mapping. kill all nodes in the mapsaver.

*/
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
std::string mapsaver_bringup_path,mapsaver_pause_path,mapsaver_save_path,mapsaver_cancel_path;
std::string editmap_bringup_path,editmap_stop_path,task_bringup_path,task_stop_path,edit_task_bringup_path,edit_task_stop_path;
std::string ppl_task_bringup_path,ppl_task_stop_path;

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

std::string sh="sh ";
std::string bringup=sh+mapsaver_bringup_path;
std::string pause=sh+mapsaver_pause_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::string save=sh+mapsaver_save_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::string cancel=sh+mapsaver_cancel_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;

std::string edit_map_start=sh+editmap_bringup_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::string edit_map_stop=sh+editmap_stop_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;

std::string task_start=sh+task_bringup_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::string task_stop=sh+task_stop_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;

std::string edittask_bringup=sh+edit_task_bringup_path;
std::string edittask_stop=sh+edit_task_stop_path;
std::string ppl_task_bringup=sh+ppl_task_bringup_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;
std::string ppl_task_stop=sh+ppl_task_stop_path+" "+param->map_path+" "+param->map_id+" "+param->map_name;

if (param->type==-1) {
  ROS_INFO("-1");
  ROS_INFO("wrong input for build the map.");

} 
if (param->type==0) {
 ROS_INFO("start to build the map ...");

  system(bringup.c_str());
   }

else if(param->type==1) {
  ROS_INFO("pause the map ...");
  system(pause.c_str());
}
else if (param->type==2){
  ROS_INFO("save the map ...");
  system(save.c_str()); 

}
else if (param->type==3){
  ROS_INFO("cancel the map ...");
  system(cancel.c_str()); 
}
else if (param->type==4){
  ROS_INFO("edit the map ...");
  system(edit_map_start.c_str()); 
}
else if (param->type==5){
  ROS_INFO("stop editting the map ...");
  system(edit_map_stop.c_str()); 
}
else if (param->type==6){
  ROS_INFO("start task ...");
  system(task_start.c_str()); 
}
else if (param->type==7){
  ROS_INFO("stop task ...");
  system(task_stop.c_str()); 
}
else if (param->type==8){
  ROS_INFO("start edit task ...");
  system(edittask_bringup.c_str()); 
}
else if (param->type==9){
  ROS_INFO("stop edit task ...");
  system(edittask_stop.c_str()); 
}
else if (param->type==10){
  ROS_INFO("start automatic path plan task ...");
  system(ppl_task_bringup.c_str()); 
}
else if (param->type==11){
  ROS_INFO("stop path plan task ...");
  system(ppl_task_stop.c_str()); 
}


}


bool server(operation::mapping_status::Request &req, operation::mapping_status::Response &res)
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
ROS_INFO("Not started mapping yet.");
} 
switch(req.data){
  case 0: 
    ROS_INFO("0");
    res.successed=true;
    res.errormsg="start mapping";
    break;

  case 1:
    ROS_INFO("1");
    res.successed=true;
    res.errormsg="pause mapping";
    break;
  case 2:
    res.successed=true;
    res.errormsg="save mapping";
    break;

  case 3:
    res.successed=true;
    res.errormsg="cancel mapping";
    break;

  case 4:

    res.successed=true;
    res.errormsg="editmap start.";
    break;

  case 5:
    res.successed=true;
    res.errormsg="editmap stop";
    break;
  case 6:
    res.successed=true;
    res.errormsg="task start";
    break;

  case 7:
    res.successed=true;
    res.errormsg="task stop";
    break;
  
  case 8:
    res.successed=true;
    res.errormsg="edit task start";
    break;
  
  case 9:
    res.successed=true;
    res.errormsg="edit task stop";
    break;

  case 10:
    res.successed=true;
    res.errormsg="add automatic path plan task.";
    break;

  case 11:
    res.successed=true;
    res.errormsg="stop creating the path plan task.";
    break;
  
  default:    
    res.successed=false;
    res.errormsg="wrong call service,no such service.";
    break;
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
   ros::param::param<std::string>("mapsaver_bringup_path",mapsaver_bringup_path,"/home/relaybot/api_ws/src/operation/src/mapping/mapsaver_bringup.sh");
   ros::param::param<std::string>("mapsaver_pause_path",mapsaver_pause_path,"/home/relaybot/api_ws/src/operation/src/mapping/mapsaver_pause.sh");
   ros::param::param<std::string>("mapsaver_save_path",mapsaver_save_path,"/home/relaybot/api_ws/src/operation/src/mapping/mapsaver_save.sh");
   ros::param::param<std::string>("mapsaver_cancel_path",mapsaver_cancel_path,"/home/relaybot/api_ws/src/operation/src/mapping/mapsaver_cancel.sh");

  //edit_map
   ros::param::param<std::string>("editmap_bringup_path",editmap_bringup_path,"/home/relaybot/api_ws/src/operation/src/edit_map/editmap_bringup.sh");
   ros::param::param<std::string>("editmap_stop_path",editmap_stop_path,"/home/relaybot/api_ws/src/operation/src/edit_map/editmap_stop.sh");

   //edit task
   ros::param::param<std::string>("edit_task_bringup_path",edit_task_bringup_path,"/home/relaybot/api_ws/src/operation/src/nw_task/edit_task_bringup.sh");
   ros::param::param<std::string>("edit_task_stop_path",edit_task_stop_path,"/home/relaybot/api_ws/src/operation/src/nw_task/edit_task_stop.sh");
   ros::param::param<std::string>("task_bringup_path",task_bringup_path,"/home/relaybot/api_ws/src/operation/src/nw_task/task_bringup.sh");
   ros::param::param<std::string>("task_stop_path",task_stop_path,"/home/relaybot/api_ws/src/operation/src/nw_task/task_stop.sh");

   //ppl task
   ros::param::param<std::string>("ppl_task_bringup_path",ppl_task_bringup_path,"/home/relaybot/api_ws/src/operation/src/ppl/ppl_task_bringup.sh");
   ros::param::param<std::string>("ppl_task_stop_path",ppl_task_stop_path,"/home/relaybot/api_ws/src/operation/src/ppl/ppl_task_stop.sh");

   ROS_INFO("Provide service which can be called to tell the server to start mapping, pause,  save or cancel.");  
   ROS_INFO("now we are in parent progress");
   pthread_join(id,NULL);
   ros::spin();   
   ROS_INFO("Whole process is finished.");
 
  return 0;  
}  
