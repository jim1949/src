#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    printf("0");
    Mat img = imread("/home/jimmy/api_ws/src/0/map.jpeg",0);  
    Json::Reader reader;
    Json::Value root;
    ifstream is;
    int map_id;
    is.open("/home/jimmy/api_ws/src/0/wall_pose/1.json");
    int wall_num=0;
  
    Point **ptr=new Point*[1];
    printf("1");
    if (reader.parse(is, root))  
    {   
        wall_num=root["wall_poses"].size();// 得到"wall_poses"的数组个数
        map_id=root["map_id"].asInt();  
        if (wall_num==0) printf("No wall_poses input");
        ptr[0]=new Point[wall_num];
        
        for(int i = 0; i < wall_num; ++i)  // 遍历数组  
        {  
            ptr[0][i]=Point(root["wall_poses"][i]["x"].asDouble(),root["wall_poses"][i]["y"].asDouble());
 
        }  
    }  
    is.close();
    const Point* ppt[1]={ptr[0]};
    int npt[]={wall_num}; 
    // CvScalar color=(0,0,255);
   
    if(wall_num==2){
        line(img,ptr[0][0],ptr[0][1],Scalar(0), 2);
    }
    else if(wall_num>2){ 
        fillPoly(img,ppt,npt,1,Scalar(0));
        }
    // cvFillPoly(img,ptr,wall_num,1,Scalar(0, 0, 255));
    // Point p1(4, 4);
    // Point p2(100, 50);
    // circle(img,p1,2,Scalar(0,0,255));
    // line(img, p1, p2, Scalar(33, 33, 133), 2);

    // //画第二条线
    // line(img, Point(30, 30), Point(75, 300), Scalar(89, 90, 90), 3);
    string path;
    string map_id_;
    stringstream ss;
    ss<<map_id;
    ss>>map_id_;
    path="/var/www/maps/"+map_id_+"/"+map_id_+"_edit.jpg";
    imwrite(path.c_str(),img);
    imshow("haha", img);

    waitKey();
//     // waitKey(0);
//   if(image.empty()){
//    printf("open error\n");
//    }
//   sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

//   ros::Rate loop_rate(5);
//   while (nh.ok()) {
//     pub.publish(msg);
//     ros::spinOnce();
//     loop_rate.sleep();
//   }
return 0;
}