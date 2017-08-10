

#include <ros/ros.h>

#include <geometry_msgs/PointStamped.h>

#include <tf/transform_listener.h>

#include <basic_msgs/nav_pose.h>

#include <basic_msgs/wall_pose.h>

#include <basic_msgs/laser_points.h>

#include <basic_msgs/points.h>

#include <vector>

#include <iostream>

#define pi 3.1415926

using namespace std;
//  ros::Publisher MapSet_pub=n.advertise<basic_msgs::laser_points>("laser_points",1000);
bool flag =false;
class Map_set{
public:
    basic_msgs::nav_pose nav_positions;
    basic_msgs::wall_pose wall_positions;
    Map_set();
    ~Map_set();
    void MapSet_navpose_callback(const basic_msgs::nav_poseConstPtr& msg);
    void MapSet_wallpose_callback(const basic_msgs::wall_poseConstPtr& msg);
};

Map_set::Map_set(){
    ROS_INFO("Constructor started");
}
Map_set::~Map_set(){
        ROS_INFO("Deconstructor started");

}

void Map_set::MapSet_navpose_callback(const basic_msgs::nav_poseConstPtr& msg){
        int num=sizeof(msg->worldposition);
        basic_msgs::points test_msg;
        // Map_set::laser_point_vector.data.clear();
        // printf(" ranges: \n");
        // for (int i=0;i<num;i++){
        // printf("%f, ",msg->ranges[i]);
        
        // test_msg.x=msg->ranges[i]*cos(pi*i/num);
        // test_msg.y=msg->ranges[i]*sin(pi*i/num);
        // Map_set::laser_point_vector.data.push_back(test_msg);
        // Map_set::laser_point_vector.push_back(test_msg);
        // v.push_back(float(test_msg.x));
        

        }

void Map_set::MapSet_wallpose_callback(const basic_msgs::wall_poseConstPtr& msg){
        int num=sizeof(msg->worldposition);
        basic_msgs::points test_msg;
        // Map_set::laser_point_vector.data.clear();
        // // printf(" ranges: \n");
        // for (int i=0;i<num;i++){
        // // printf("%f, ",msg->ranges[i]);
        
        // test_msg.x=msg->ranges[i]*cos(pi*i/num);
        // test_msg.y=msg->ranges[i]*sin(pi*i/num);
        // Map_set::laser_point_vector.data.push_back(test_msg);
        // // Map_set::laser_point_vector.push_back(test_msg);
        // // v.push_back(float(test_msg.x));
        

        // }


     } 

/*
void transformPoint(const tf::TransformListener& listener, geometry_msgs::PointStamped& laser_point,basic_msgs::laser_points& laser_point_vector, basic_msgs::laser_points& laser_point_picture_vector,int i){

    //we'll create a point in the base_laser frame that we'd like to transform to the base_footprint frame

    laser_point_picture_vector.data.clear();
    // ROS_INFO("---i is :%d ---",i);
    laser_point.header.frame_id = "base_link";
    int num=laser_point_vector.data.size();
    
    ROS_INFO("Num:%d",num);
    for (int i=0;i<num;i++){
     cout<<laser_point_vector.data[i]<<endl;

    
    

    //we'll just use the most recent transform available for our simple example

    laser_point.header.stamp = ros::Time();

    

    //just an arbitrary point in space
    ROS_INFO("laser point.x:%f",laser_point.point.x);
    laser_point.point.x = laser_point_vector.data[i].x;

    laser_point.point.y = laser_point_vector.data[i].y;

    // laser_point.point.z = 0.0;

    

    try{

    geometry_msgs::PointStamped base_point;
    basic_msgs::points point;
    listener.transformPoint("picture_frame", laser_point, base_point);
    point.x=base_point.point.x;
    point.y=base_point.point.y;
   
    laser_point_picture_vector.data.push_back(point);
    ROS_INFO(" base_footprint: (%.2f, %.2f. %.2f) -----> picture_frame: (%.2f, %.2f, %.2f) at time %.2f",

        laser_point.point.x, laser_point.point.y, laser_point.point.z,

        base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());

    flag=true;
    }

    catch(tf::TransformException& ex){

    ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_footprint\": %s", ex.what());
    flag=false;
   

    }
    
    }

}
*/
   

int main(int argc, char** argv){

    ros::init(argc, argv, "robot_tf_listener");

    ros::NodeHandle n;
    Map_set *mapset=new Map_set();
    int i=1;
    // tf::TransformListener listener(ros::Duration(10));

    // ros::Subscriber MapSet_wallpose_sub=n.subscribe("wall_pose",100,&Map_set::MapSet_wallpose_callback,mapset);
    // ros::Subscriber MapSet_navpose_sub=n.subscribe("nav_pose",100,&Map_set::MapSet_navpose_callback,mapset);
    // ros::Publisher MapSet_pub=n.advertise<basic_msgs::laser_points>("laser_points",1000);
    // ros::Publisher laser_pub=n.advertise<basic_msgs::laser_points>("laser_pose_picture",1000);
   
    

    //we'll transform a point once every second

    // ros::Timer timer1 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(mapset->laser_point),boost::ref(mapset->laser_point_vector),boost::ref(mapset->laser_point_picture_vector),i));

    ros::Rate r(1);
   while (ros::ok()){
       if(flag=true){
    ROS_INFO("before");

    ROS_INFO("after");
    // for(int i=1;i<2;i++){
    //     ROS_INFO("i: %d, %lf, %lf",i,mapset->laser_point_vector.data[i].x,mapset->laser_point_vector.data[i].y);
    // }
    // MapSet_pub.publish(mapset->laser_point_vector);
    // laser_pub.publish(mapset->laser_point_picture_vector);

       }

    ros::spinOnce();
    r.sleep();

}    

}

