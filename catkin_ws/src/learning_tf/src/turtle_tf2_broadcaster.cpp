#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
/// To use the TransformBroadcaster, we need to include the tf2_ros/transform_broadcaster.h header file.
#include <tf2_ros/transform_broadcaster.h>
// the below is a standard geometry stuff
#include <geometry_msgs/TransformStamped.h>
#include <turtlesim/Pose.h>

using namespace ros;

std::string turtlename;

void poseCallback(const turtlesim::PoseConstPtr &msg)
{
    //Here we create a StaticTransformBroadcaster object that we'll use later to send transformations over the wire.
    static tf2_ros::TransformBroadcaster br_caster;            // broadcaster handle
    geometry_msgs::TransformStamped transformStamped;

    //Here we create a TransformStamped object which will be the message we will 
    // send over once populated. Before stuffing the actual transform values we need to give it the appropriate metadata.
    // broadcaster header
    transformStamped.header.stamp = Time::now();
    transformStamped.header.frame_id = "world";   // name of the parent frame of the link we're creating, in this case "world
    transformStamped.child_frame_id = turtlename; // name of child link
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

    br_caster.sendTransform(transformStamped);
}

int main(int argc, char **argv)
{
    // initialize the node
    ros::init(argc, argv, "my_tf_broadcaster");
    /// creating a handler
    ros::NodeHandle private_node("~"); // forgot about ~

    // removed code for getting value from parameter server
        if (argc != 2)
        {
            ROS_ERROR("need turtle name as argument");
            return -1;
        };
        turtlename = argv[1];

    NodeHandle node;
    // on receiving a specific topic 
    // the broadcaster broadcasts its transform
    Subscriber sub = node.subscribe(turtlename+"/pose", 10, &poseCallback);//!!! msg is also passed to call back function by ros

    spin();
    return 0;
}