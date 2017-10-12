#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <operation/edit_wall_set.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>

using namespace std;
using namespace cv;


bool server(operation::edit_wall_set::Request &req, operation::edit_wall_set::Response &res  ){
    int map_id=req.edit_wall.mapid;
    string map_id_path;
    int wall_id;
    int wall_poses_num;
    string wall_id_path;
    Json::Reader reader;
    Json::Value root;
    
    int wall_num=req.edit_wall.wall_id.size();
    if (wall_num==0){ROS_ERROR("No wall_id.");}
    string path;
    stringstream ss;
    ss<<map_id;
    ss>>map_id_path;
    ss.str("");
    path="/var/www/maps/"+map_id_path+"/"+map_id_path+"_bk.pgm";
   
    // read the map file.
    Mat img = imread(path.c_str(),0); 

    // read the json file. 

    for(int i=0;i<wall_num;i++){
        wall_id=req.edit_wall.wall_id[i];
	    stringstream os;
        os<<wall_id;
        os>>wall_id_path;
        os.str();
        path="/var/www/nav_manager/"+map_id_path+"/wall_pose/"+wall_id_path+".json";
	    ROS_INFO("%s",path.c_str());
	    ifstream is;
        is.open(path.c_str());
        if (!reader.parse(is, root)) {ROS_ERROR("Parse the root of wall_pose in draw_wall ERROR.");} 
        wall_poses_num=root["wall_poses"].size();// 得到"wall_poses"的数组个数
        if (wall_poses_num==0) ROS_ERROR("No wall_poses input in wall_id %d",wall_id);
        Point **ptr=new Point*[1];
        ptr[0]=new Point[wall_poses_num];
        ROS_INFO("wall_num:%d,wall_poses_num:%d",wall_num,wall_poses_num);
        for(int j = 0; j < wall_poses_num; j++)  // 遍历数组  
        {
	    ROS_INFO("x:%f,y:%f",root["wall_poses"][j]["x"].asDouble(),root["wall_poses"][j]["y"].asDouble());
        ptr[0][j]=Point(root["wall_poses"][j]["x"].asDouble(),root["wall_poses"][j]["y"].asDouble());
        } 
        is.close();
        const Point* ppt[1]={ptr[0]};
        int npt[]={wall_poses_num}; 
    
        if(wall_poses_num==2){
         ROS_INFO("draw line.");
            line(img,ptr[0][0],ptr[0][1],Scalar(0), 2);
        }
        else if(wall_poses_num>2){ 
	    ROS_INFO("draw poly.");
            fillPoly(img,ppt,npt,1,Scalar(0));
            }

        path="/var/www/maps/"+map_id_path+"/"+map_id_path+".pgm";
        imwrite(path.c_str(),img);
        
    }


    res.successed=1;
    return true;

}
int main(int argc, char** argv)
{
      //  signal(SIGCHLD,SIG_IGN);//avoid zombie process 
    ros::init(argc, argv, "draw_node");  
    ros::NodeHandle n;
    ros::ServiceServer service = n.advertiseService("/edit_wall_set", server); 
    ros::spin();

}
