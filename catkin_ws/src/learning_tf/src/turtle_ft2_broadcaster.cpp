#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
// the below is a standard geometry stuff
#include <geometry_msgs/TransformStamped.h>
#include <turtlesim/Pose.h>

using namespace ros;

std::string turtlename;

void poseCallback(const turtlesim::PoseConstPtr &msg)
{
    //Here we create a StaticTransformBroadcaster object that we'll use later to send transformations over the wire.
    static tf2_ros::TransformBroadcaster br; // broadcaster handle
    geometry_msgs::TransformStamped transformStamped;

    //Here we create a TransformStamped object which will be the message we will 
    // send over once populated. Before stuffing the actual transform values we need to give it the appropriate metadata.
    // broadcaster header
    transformStamped.header.stamp = Time::now();
    transformStamped.header.frame_id = "world";
    transformStamped.child_frame_id = turtlename;
    // translation data
    transformStamped.transform.translation.x = msg->x;
    transformStamped.transform.translation.y = msg->y;
    transformStamped.transform.translation.z = 0.0;
    // rotation data
    tf2::Quaternion q;          // need to convert theta(in geometry message) to quaternions (used in tf)
    q.setRPY(0, 0, msg->theta); // for turtle sim rot is along z axis
    transformStamped.transform.rotation.x = q.x();
    transformStamped.transform.rotation.y = q.y();
    transformStamped.transform.rotation.z = q.z();
    transformStamped.transform.rotation.w = q.w();

    br.sendTransform(transformStamped);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_tf_broadcaster");

    ros::NodeHandle private_node("~"); // forgot about ~

    if (!private_node.hasParam("turtle"))
    {
        if (argc != 2)
        {
            ROS_ERROR("need turtle name as argument");
            return -1;
        };
        turtlename = argv[1];
    }
    else
    {
        private_node.getParam("turtle", turtlename);
    }

  NodeHandle node;
  Subscriber sub = node.subscribe(turtlename+"/pose", 10, &poseCallback);

  spin();
  return 0;
}