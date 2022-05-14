// since ros
#include <ros/ros.h>
// since tf listener
#include <tf2_ros/transform_listener.h>
// since transform are in the following form
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>

// to spaen another turtle
#include <turtlesim/Spawn.h>

using namespace ros;

int main(int argc, char **argv)
{
    init(argc, argv, "tf2_listener_node");
    NodeHandle node;
    // cool stuff : you can call a ros service from a node

    // check whether service is available or not
    service::waitForService("spawn");
    // need a handling object
    ros::ServiceClient spawner = node.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn turtle;
    // specific to turtle node
    turtle.request.x = 4;
    turtle.request.y = 2;
    turtle.request.theta = 0;
    turtle.request.name = "turtle2";
    spawner.call(turtle);

    // register a publisher with rosmaster
    ros::Publisher turtle_vel = node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);

    // Here, we create a TransformListener object. Once the listener is created, it starts receiving tf2 transformations over the wire, and buffers them for up to 10 seconds.
    //  The TransformListener object should be scoped to persist otherwise its cache will be unable to fill and almost every query will fail. A common method is to make the TransformListener object a member variable of a clas

    tf2_ros::Buffer tfBuffer; /// very important buffer
    tf2_ros::TransformListener tfListener(tfBuffer);
    Rate rate(10.0);

    /// looking up transformation if possible
    while (node.ok())
    {
        geometry_msgs::TransformStamped transformStamped;
        try
        {// targetframe is turtle2   sourceframe is turtle1  ,use recent one
            transformStamped = tfBuffer.lookupTransform("turtle2", "turtle1", ros::Time(0));
        }
        catch (tf2::TransformException &ex)
        {
            ROS_WARN("%s", ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }

        // we query the listener for a specific transformation. Let's take a look at the four arguments (full details of lookupTransform(
        // We want the transform to this frame (target frame) ...
        //  ... from this frame (source frame).
        //  The time at which we want to transform. Providing ros::Time(0) will just get us the latest available transform.
        //  Duration before timeout. (optional, default=ros::Duration(0.0))

        geometry_msgs::Twist vel_msg;
        vel_msg.angular.z = 4.0 * atan2(transformStamped.transform.translation.y, transformStamped.transform.translation.x);
        vel_msg.linear.x = 0.5 * sqrt(pow(transformStamped.transform.translation.x, 2) + pow(transformStamped.transform.translation.y, 2));
        turtle_vel.publish(vel_msg);
        rate.sleep();
    }
    return 0;
}
