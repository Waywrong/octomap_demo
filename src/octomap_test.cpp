#include <ros/ros.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>
#include "pcl_ros/transforms.h"
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>
#include <octomap/octomap.h>
#include <octomap_msgs/Octomap.h>
#include <octomap_msgs/conversions.h>
#include <octomap/ColorOcTree.h>

#define OCTO_COLOR 
#define OCTO_RESOLUTION 0.1

ros::Publisher Octomap_pub;

void kinectCallbacks( const sensor_msgs::PointCloud2ConstPtr& cloud2_msg ) 
{
  pcl::PCLPointCloud2 cloud2;
  pcl_conversions::toPCL(*cloud2_msg, cloud2);
#ifdef OCTO_COLOR
  pcl::PointCloud<pcl::PointXYZRGBA>* cloud_local(new pcl::PointCloud<pcl::PointXYZRGBA>);
  pcl::fromPCLPointCloud2(cloud2,*cloud_local);
  octomap::ColorOcTree tree(OCTO_RESOLUTION);
  for (auto p:cloud_local->points)
  {
    tree.updateNode( octomap::point3d(p.x, p.y, p.z), true);
    tree.integrateNodeColor( p.x, p.y, p.z, p.r, p.g, p.b );
  }
#else//no color
  pcl::PointCloud<pcl::PointXYZ>* cloud_local(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromPCLPointCloud2(cloud2,*cloud_local);
  octomap::OcTree tree(OCTO_RESOLUTION);
  for (auto p:cloud_local->points)
    tree.updateNode( octomap::point3d(p.x, p.y, p.z), true);
#endif

  tree.updateInnerOccupancy();
  octomap_msgs::Octomap msg_octomap;
  octomap_msgs::binaryMapToMsg(tree, msg_octomap);
  msg_octomap.binary = 1;
  msg_octomap.id = 1;
  msg_octomap.resolution = OCTO_RESOLUTION;
  msg_octomap.header.frame_id = "camera_depth_optical_frame";
  msg_octomap.header.stamp = ros::Time::now();
  Octomap_pub.publish(msg_octomap);

  delete cloud_local;
}

int main(int argc, char **argv)
{
  // Initialize node
  ros::init(argc, argv, "octomap_test");
  ros::NodeHandle nh;

  // Subscripe topic
  ros::Subscriber kinect_sub = 
      nh.subscribe<sensor_msgs::PointCloud2>("/camera/depth/points", 1, kinectCallbacks);

  // Advertise topic
  Octomap_pub = 
      nh.advertise<octomap_msgs::Octomap>("octomap_3d",1);

  ros::Rate r(10);
  while(ros::ok())
  {
    
    r.sleep();
    ros::spinOnce();
  }
}