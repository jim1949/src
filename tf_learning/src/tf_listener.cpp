
// publish the pose of lasers in the picture frame.
#include <ros/ros.h>

#include <geometry_msgs/PointStamped.h>

#include <tf/transform_listener.h>

#include <sensor_msgs/LaserScan.h>

#include <basic_msgs/laser_points.h>

#include <basic_msgs/points.h>

#include <vector>

#include <iostream>

#define pi 3.1415926

using namespace std;
//  ros::Publisher scan_pub=n.advertise<basic_msgs::laser_points>("laser_points",1000);
bool flag =false;
class Scan_info{
public:
    sensor_msgs::LaserScan ranges;
    geometry_msgs::PointStamped laser_point;
    basic_msgs::laser_points laser_point_vector;
    basic_msgs::laser_points laser_point_picture_vector;
    Scan_info();
    ~Scan_info();
    void scancallback(const sensor_msgs::LaserScanConstPtr& msg);

};

Scan_info::Scan_info(){
    ROS_INFO("Constructor started");
}
Scan_info::~Scan_info(){
        ROS_INFO("Deconstructor started");

}

void Scan_info::scancallback(const sensor_msgs::LaserScanConstPtr& msg){
        const int num=msg->ranges.size();
        basic_msgs::points test_msg;
        Scan_info::laser_point_vector.data.clear();
        // ROS_INFO(" laser num: %d\n", num);
        for (int i=0;i<num;i=i+1){
            float range;
        if (msg->ranges[i]>8.0){range=0.0;}
        else {range=msg->ranges[i];}
        // printf("ranges:[%d]%f, ",i,msg->ranges[i]);
        float sigma=i*pi/num;
        test_msg.x=range*cos(sigma-pi/2);
       
        test_msg.y=range*sin(sigma-pi/2);
        
        Scan_info::laser_point_vector.data.push_back(test_msg);

        }


     } 


void transformPoint(const tf::TransformListener& listener, geometry_msgs::PointStamped& laser_point,basic_msgs::laser_points& laser_point_vector, basic_msgs::laser_points& laser_point_picture_vector,int i){

    //we'll create a point in the base_laser frame that we'd like to transform to the base_footprint frame

    laser_point_picture_vector.data.clear();
    // ROS_INFO("---i is :%d ---",i);
    laser_point.header.frame_id = "base_link";
    int num=laser_point_vector.data.size();
    
    
    for (int i=0;i<num;i++){
    //  cout<<laser_point_vector.data[i]<<endl;

    
    

    //we'll just use the most recent transform available for our simple example

    laser_point.header.stamp = ros::Time();

    

    //just an arbitrary point in space
    // ROS_INFO("laser point.x:%f",laser_point.point.x);
    laser_point.point.x = laser_point_vector.data[i].x;

    laser_point.point.y = laser_point_vector.data[i].y;

    // laser_point.point.z = 0.0;

    

    try{

    geometry_msgs::PointStamped base_point;
    basic_msgs::points point;
    listener.waitForTransform("picture_frame", "base_link", ros::Time(), ros::Duration(10.0) );
    listener.transformPoint("picture_frame", laser_point, base_point);
    point.x=base_point.point.x*20;
    point.y=base_point.point.y*20;
   
    laser_point_picture_vector.data.push_back(point);
    // ROS_INFO(" base_footprint: (%.2f, %.2f. %.2f) -----> picture_frame: (%.2f, %.2f, %.2f) at time %.2f",

    //     laser_point.point.x, laser_point.point.y, laser_point.point.z,

    //     base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());

    flag=true;
    }

    catch(tf::TransformException& ex){

    ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_footprint\": %s", ex.what());
    flag=false;
   

    }
    
    }

}

   

int main(int argc, char** argv){

    ros::init(argc, argv, "robot_tf_listener");

    ros::NodeHandle n;
    Scan_info *scaninfo=new Scan_info();
    int i=1;
    tf::TransformListener listener(ros::Duration(10));
    // geometry_msgs::PointStamped laser_point;
    ros::Subscriber scan_sub=n.subscribe("scan",100,&Scan_info::scancallback,scaninfo);
    ros::Publisher scan_pub=n.advertise<basic_msgs::laser_points>("laser_points",1000);
    ros::Publisher laser_pub=n.advertise<basic_msgs::laser_points>("laser_pose_picture",1000);
   
    

    //we'll transform a point once every second

    ros::Timer timer1 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(scaninfo->laser_point),boost::ref(scaninfo->laser_point_vector),boost::ref(scaninfo->laser_point_picture_vector),i));
    // ros::Timer timer2 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));
    // ros::Timer timer3 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));
    // ros::Timer timer4 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));

    ros::Rate r(1);
   while (ros::ok()){
       if(flag=true){

    scan_pub.publish(scaninfo->laser_point_vector);
    laser_pub.publish(scaninfo->laser_point_picture_vector);

       }

    ros::spinOnce();
    r.sleep();

}    

}

