#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

class pointcloud_to_laserscan {
public:
  pointcloud_to_laserscan(ros::NodeHandle *n)
  {
    nh = n;

    //Set Points Topic Name
    if(nh->hasParam("/pointcloud_to_laserscan/points_topic_name"))
    {
      ROS_INFO("Points Topic Name Changed");
      nh->getParam("/pointcloud_to_laserscan/points_topic_name",points_topic_name);
    }

    ROS_INFO("points_topic_name : %s", points_topic_name.c_str());

    //Set Scan Topic Name
    if(nh->hasParam("/pointcloud_to_laserscan/scan_topic_name"))
    {
      ROS_INFO("Scan Topic Name Changed");
      nh->getParam("/pointcloud_to_laserscan/scan_topic_name",scan_topic_name);
    }

    ROS_INFO("scan_topic_name : %s", scan_topic_name.c_str());

    //Static TF Broadcastor
    static_transformStamped.header.stamp = ros::Time::now();

    static_transformStamped.header.frame_id = "points_frame";
    static_transformStamped.child_frame_id = "scan_frame";

    static_transformStamped.transform.translation.x = 0;
    static_transformStamped.transform.translation.y = 0;
    static_transformStamped.transform.translation.z = 0;

    static_transformStamped.transform.rotation.x = 0;
    static_transformStamped.transform.rotation.y = 0;
    static_transformStamped.transform.rotation.z = 0;
    static_transformStamped.transform.rotation.w = 1;

    //Initial Topics
    points_sub = nh->subscribe(points_topic_name,100,&pointcloud_to_laserscan::points_callback,this);
    tf_timer = nh->createTimer(ros::Duration(1/tf_frequency),&pointcloud_to_laserscan::tf_callback,this);


    ros::spin();
  }

  void tf_callback(const ros::TimerEvent&)
  {
    static_broadcaster.sendTransform(static_transformStamped);
  }

  void points_callback(const sensor_msgs::PointCloud2::ConstPtr& new_msg)
  {
    ROS_INFO("New Points Fetched");
  }

  ~pointcloud_to_laserscan()
  {
    //Clean Param Server
    nh->deleteParam("/pointcloud_to_laserscan/points_topic_name");
    nh->deleteParam("/pointcloud_to_laserscan/scan_topic_name");
  }

private:

  ros::NodeHandle *nh;
  ros::Subscriber points_sub;
  ros::Timer tf_timer;
  tf2_ros::StaticTransformBroadcaster static_broadcaster;
  std::string points_topic_name = "/points";
  std::string scan_topic_name = "/scan";
  uint16_t tf_frequency = 30;
  uint16_t scan_frequency = 30;
  geometry_msgs::TransformStamped static_transformStamped;

};

int main(int argc, char **argv)
{
  //Initial Start Up
  ros::init(argc,argv,"pointcloud_to_laserscan");
  ros::NodeHandle n;

  ROS_INFO("pointcloud_to_laserscan Node Started");

  pointcloud_to_laserscan x = pointcloud_to_laserscan(&n);


  return 0;
}
