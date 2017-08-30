/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: edit nav poses in the task (vector) service called from android.
functions:
get the positions, and store task in the map. give response.

*/
#include <ros/ros.h>
#include <gpsbot_navigation/edit_nav_task.h>
#include <stdio.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <cstdlib>



#pragma comment(lib, "json_mtd.lib")
 
#include <cstdio>

using namespace std; 

Json::Value transfer_json(gpsbot_navigation::edit_nav_task::Request &req){
    Json::Value root;
    Json::Value nav_poses;
    string map_name=req.map_name;
    int num=req.nav_id.size();
    root["map_id"]=req.map_id;
    root["map_name"]=map_name;
    root["task_id"]=req.task_id;
    root["type"]=req.type;
    root["num"]=num;

    for(int i=0;i<num;i++){
        nav_poses.append(req.nav_id[i]);
    }
    root.append(nav_poses);
    return root;
}
void delete_json(gpsbot_navigation::edit_nav_task::Request &req,Json::Value root){

    string path="/var/www/task_manager/";
    char task_id[4],mapfile_id[4];
    sprintf(task_id,"%d",req.task_id);
    sprintf(mapfile_id,"%d",req.map_id);
    string json_path=path+mapfile_id+"/"+task_id+".json";
    string folder_build_command="rm "+json_path;
    ROS_INFO("%s",folder_build_command.c_str());
    system(folder_build_command.c_str());
}

void add_json(gpsbot_navigation::edit_nav_task::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/task_manager/";
    char task_id[4],mapfile_id[4];
    sprintf(task_id,"%d",req.task_id);
    sprintf(mapfile_id,"%d",req.map_id);
    string json_path=path+mapfile_id+"/"+task_id+".json";
    string folder_build_command="mkdir -p "+path+"/"+mapfile_id;
    ROS_INFO("%s",folder_build_command.c_str());
    system(folder_build_command.c_str());
    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    ofs.close();
}

void update_json(gpsbot_navigation::edit_nav_task::Request &req,Json::Value root){
    ofstream ofs; 
    string path="/var/www/task_manager/";
    char task_id[4],mapfile_id[4];
    sprintf(task_id,"%d",req.task_id);
    sprintf(mapfile_id,"%d",req.map_id);
    string json_path=path+mapfile_id+"/"+task_id+".json";
    string folder_build_command="rm "+json_path;
    // There is a bug here, when we don't have the file, and update it, we will create a new file. But personally, I don't think this will be a problem.
    ROS_INFO("%s",folder_build_command.c_str());
    system(folder_build_command.c_str());
    ofs.open(json_path.c_str());
    ofs<<root.toStyledString();
    
    ofs.close();
}


bool edit_server(gpsbot_navigation::edit_nav_task::Request &req,gpsbot_navigation::edit_nav_task::Response &res){
    ROS_INFO("Edit the nav task message!");
    ROS_INFO("map name:%s,map_id:%d,nav_task id:%d,type:%d",req.map_name.c_str(),req.map_id,req.task_id,req.type);
    Json::Value root;
    

    root=transfer_json(req);
    // 1:delete,2:add,3:update.
    if (root["type"]==1)
    {
        delete_json(req,root);
        sprintf(s,"got the delete message! ");
        
    }
    else if(root["type"]==2){
        add_json(req,root);
        sprintf(s,"got the add message! ");
    }
    else if(root["type"]==3){
        update_json(req,root);
        sprintf(s,"got the update message!");
    }
    else{
        sprintf(s,"wrong type ");
    }

    // ----response
    
    res.errormsg="successed response.";
    res.successed=1;
    return true;
}

int main(int argc,char **argv){
ros::init(argc, argv, "edit_navigation_task");
ros::NodeHandle n;
ros::ServiceServer nav_service=n.advertiseService("/edit_nav_task",edit_server);

ros::spin();
return 0;
}