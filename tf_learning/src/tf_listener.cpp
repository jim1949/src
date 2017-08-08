

#include <ros/ros.h>

#include <geometry_msgs/PointStamped.h>

#include <tf/transform_listener.h>

#include <sensor_msgs/LaserScan.h>

class Scan_info{
public:
    sensor_msgs::LaserScan ranges;
    geometry_msgs::PointStamped laser_point;

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
        int num=sizeof(msg->ranges);
        // printf(" ranges: \n");
        // for (int i=0;i<num;i++){
        // printf("%f, ",msg->ranges[i]);

        // }
    //    ROS_INFO("number of laser scans:%ld\n",sizeof(msg->ranges));
       static float n=0.5;
       n=n+0.5;
       Scan_info::laser_point.point.x=n;

     } 


void transformPoint(const tf::TransformListener& listener, geometry_msgs::PointStamped& laser_point,int i){

    //we'll create a point in the base_laser frame that we'd like to transform to the base_footprint frame

    
    // ROS_INFO("---i is :%d ---",i);
    laser_point.header.frame_id = "base_link";

    

    //we'll just use the most recent transform available for our simple example

    laser_point.header.stamp = ros::Time();

    

    //just an arbitrary point in space
    ROS_INFO("laser point.x:%f",laser_point.point.x);
    // laser_point.point.x = 0.1;

    // laser_point.point.y = 0.2;

    // laser_point.point.z = 0.0;

    

    // try{

    // geometry_msgs::PointStamped base_point;

    // listener.transformPoint("picture_frame", laser_point, base_point);

    

    // ROS_INFO(" base_footprint: (%.2f, %.2f. %.2f) -----> picture_frame: (%.2f, %.2f, %.2f) at time %.2f",

    //     laser_point.point.x, laser_point.point.y, laser_point.point.z,

    //     base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());

    // }

    // catch(tf::TransformException& ex){

    // ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_footprint\": %s", ex.what());

   

    // }

}

   

int main(int argc, char** argv){

    ros::init(argc, argv, "robot_tf_listener");

    ros::NodeHandle n;
    Scan_info *scaninfo=new Scan_info();
    int i=1;
    tf::TransformListener listener(ros::Duration(10));
    // geometry_msgs::PointStamped laser_point;
    ros::Subscriber scan_sub=n.subscribe("base_scan",100,&Scan_info::scancallback,scaninfo);
    

   
    

    //we'll transform a point once every second

    ros::Timer timer1 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(scaninfo->laser_point),i));
    // ros::Timer timer2 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));
    // ros::Timer timer3 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));
    // ros::Timer timer4 = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener),boost::ref(laser_point)));

    

    ros::spin();

    

}

