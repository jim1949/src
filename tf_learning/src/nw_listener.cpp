/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: nav and wall positions (vector) service called from android.
functions:
1.get the positions, and store. give response.
2.transform the nav positions to map frame, and store in json file or SQL file.
3.draw wall_positions

nav_pose_set.srv:


*/
#include <ros/ros.h>
#include <basic_msgs/nav_pose_set.h>
#include <basic_msgs/wall_pose_set.h>
#include <tf/transform_listener.h>


bool nav_server(basic_msgs::nav_pose_set::Request &req, basic_msgs::wall_pose_set::Response &res){
return 0;

}


bool wall_server(basic_msgs::wall_pose_set::Request &req,basic_msgs::wall_pose_set::Response &res){

  return 0;
}

int main(int argc,char **argv){
ros::init(argc, argv, "nav_and_wall_positions_node");
ros::NodeHandle n;
ros::ServiceServer nav_service=n.advertiseService("nav_pose_set",nav_server);
// ros::ServiceServer wall_service=n.advertiseService("/wall_pose_set",wall_server);
ros::spin();
return 0;
}
