/**
# @Date    : 2017-07-27 14:42:16
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1
*/
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "basic_msgs/gridpose.h"
#include "geometry_msgs/Pose.h"
#include "basic_msgs/nav_pose.h"
#include <sstream>
#include <time.h>
#include "math.h"

//This is the basic template which demonstrates simple sending of messages over the ROS system.
std_msgs::Int32 test;
basic_msgs::nav_pose nav_pose;


void posecallback(const geometry_msgs::Pose::ConstPtr &msg)

{
static float i=0;
i=i+0.5;

nav_pose.name="pose1";
nav_pose.mapid=1;
time_t timep;
time(&timep);
nav_pose.birthtime=asctime(gmtime(&timep));
nav_pose.gridX=100;
nav_pose.gridY=100;
nav_pose.angle=0;
nav_pose.type=0;
nav_pose.worldposition.orientation.x=0;
nav_pose.worldposition.orientation.y=0;
nav_pose.worldposition.orientation.z=0;
nav_pose.worldposition.orientation.w=1;
nav_pose.worldposition.position.x=msg->position.x+30*cos(i/10.0);
nav_pose.worldposition.position.y=msg->position.y+30*sin(i/10.0);
nav_pose.worldposition.position.z=msg->position.z;
ROS_INFO("I heard x : [%f]", nav_pose.worldposition.position.x);
ROS_INFO("I heard y : [%f]", nav_pose.worldposition.position.x);
ROS_INFO("I heard z : [%f]", nav_pose.worldposition.position.z);

// nav_pose.gridX=msg->position.x;

}

void posecallback2(const std_msgs::Int32::ConstPtr &msg)

{

ros::Duration(1).sleep();
ROS_INFO("I heard i : [%d]", msg->data);
test.data=msg->data;
// ROS_INFO("I heard y : [%f]", msg->y);
// ROS_INFO("I heard angle : [%f]", msg->angle);


// nav_pose.gridX=msg->position.x;
// return nav_pose;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("chatter", 2);
  ros::Publisher pose_pub = n.advertise<basic_msgs::nav_pose>("nav_position", 2);
  ros::Subscriber sub=n.subscribe("robot_pose",2,posecallback);
  ros::Subscriber sub2=n.subscribe("chatter",2,posecallback2);
  ros::Rate loop_rate(2);

  int count = 0;
  int i=1;
  while (ros::ok())
  {

    std_msgs::Int32 msg;
    std::stringstream ss;
    
    msg.data = i++;
    ROS_INFO("%d", test.data);

    chatter_pub.publish(msg);
    pose_pub.publish(nav_pose);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}