/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

*/
#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <tf_learning/pose_tf.h>
#include <geometry_msgs/Pose.h>
#include <json/json.h>

#pragma comment(lib, "json_mtd.lib")
#define Pi 3.1415926


using namespace std;

int main(int argc,char **argv){
    ros::init(argc, argv, "robot_path_node");
    ros::NodeHandle n; 
    string map_path,map_id,map_name,path,tmp,robot_path,robot_path_map;
    //===============get the map name, id , path.======================================//
    if (argc<3){
        ROS_INFO("No map path and name input. Will use the default map path:%s.jpg",argv[1]);
        map_id="now_id";
        map_name="now";
        tmp=argv[1];
        path=tmp+".jpg";
    }
    else {

        map_path=argv[1];
        map_id=argv[2];
        map_name=argv[3];
        ROS_INFO("Receive the map_path:%s map_id:%s map_name:%s",map_path.c_str(),map_id.c_str(),map_name.c_str());
        path=map_path+"/"+map_id+"/"+map_id+".jpg";
        ROS_INFO("path is:%s",path.c_str());
    }
    robot_path=map_path+"/"+map_id+"/robot_path.txt";
    robot_path_map=map_path+"/"+map_id+"/robot_path_map.txt";
        //==============got the robot_path txt in picture frame.=============================//
        fstream myfile(robot_path.c_str(), ios_base::in);
        ofstream ofile;
        ofile.open(robot_path_map.c_str());
        float ft_line;
        int num;
        int count = 0;
        Json::Value root;
        Json::Value poses;
        Json::Value pose;

            //==============transfer the robot_path txt in /picture_frame to /map.=============================//
        if(myfile.is_open())
        {
            cout << " txt opened" << endl;
            myfile >> ft_line;
            num=ft_line;
            cout<<"num:"<<num<<endl;
            for(int i=0;i<num & ros::ok();i++)
            {   printf("\nrow %d: ",i);
                geometry_msgs::Pose pose_in_picture_frame1,pose_in_picture_frame2;
                for(int j=0;j<4;j++){

                    myfile>>ft_line;
                    if (j==0){
                        //vector x
                        pose_in_picture_frame1.position.x=ft_line;
                    }
                    else if (j==1){
                        //vector y
                        pose_in_picture_frame1.position.y=ft_line;
                    }
                    else if (j==2){
                        //vector w
                        pose_in_picture_frame1.orientation.w=ft_line;
                    }
                    else if (j==3){
                        //vector z
                        pose_in_picture_frame1.orientation.z=ft_line;
                    }
                    printf(" column %d: %f",j,ft_line);
                }
                pose_in_picture_frame1.position.z=0.0;
                pose_in_picture_frame1.orientation.x=0.0;
                pose_in_picture_frame1.orientation.y=0.0;

                //===========================transform point.======================//
                const string frame1("picture_frame");
                const string frame2("map");
                pose_in_picture_frame2= transform_point(pose_in_picture_frame1,frame1,frame2);//from frame1 to frame2
                
                pose["position"]["x"]=pose_in_picture_frame2.position.x;
                pose["position"]["y"]=pose_in_picture_frame2.position.y;
                pose["position"]["z"]=pose_in_picture_frame2.position.z;
                pose["orientation"]["x"]=pose_in_picture_frame2.orientation.x;
                pose["orientation"]["y"]=pose_in_picture_frame2.orientation.y;
                pose["orientation"]["z"]=pose_in_picture_frame2.orientation.z;
                pose["orientation"]["w"]=pose_in_picture_frame2.orientation.w;
                poses.append(pose);
                //x,y,w,z
            }

            root["map_id"]=map_id;
            root["map_name"]=map_name;
            root["path"]=poses;

            cout<<endl;		
        }
        ofile<<root.toStyledString();
        cout<<root.toStyledString();
        myfile.close();
        ofile.close();
    

    return 0;
}
