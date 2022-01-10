#include <ros/ros.h>
#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <cmath>
#include <iostream>
#include <math.h>

using namespace grid_map;

int main(int argc, char** argv)
{
  // Node parameters
    int length; 
    int size; 
    int resolution;

  // Initialize node and publisher.
  ros::init(argc, argv, "grid_map_simple_demo");
  ros::NodeHandle nh("~");

    nh.param("length", length, 50);
    nh.param("size", size, 50);
    nh.param("resolution", resolution, 1);
  ros::Publisher publisher = nh.advertise<grid_map_msgs::GridMap>("grid_map", 1, true);




  // Create grid map.
  GridMap map({"elevation"});
  map.setFrameId("world_ned");
  map.setGeometry(Length(size, length), resolution);
  ROS_INFO("Created map with size %f x %f m (%i x %i cells).",
    map.getLength().x(), map.getLength().y(),
    map.getSize()(0), map.getSize()(1));

  // Work with grid map in a loop.
  ros::Rate rate(30.0);
  while (nh.ok()) {

    // Add data to grid map.
    ros::Time time = ros::Time::now();
    for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
      Position position;
      map.getPosition(*it, position);
      map.at("elevation", *it) =   5 + std::sin(position.x()/7);
    }

    // Publish grid map.
    map.setTimestamp(time.toNSec());
    grid_map_msgs::GridMap message;
    GridMapRosConverter::toMessage(map, message);
    publisher.publish(message);
    ROS_INFO_THROTTLE(1.0, "Grid map (timestamp %f) published.", message.info.header.stamp.toSec());

    // Wait for next cycle.
    rate.sleep();
  }

  return 0;
}
