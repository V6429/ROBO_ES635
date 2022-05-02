#include "ros/ros.h"
#include "std_msgs/String.h"


using namespace ros;

void listencallbackfunction(const std_msgs::String::ConstPtr& msg);

int main(int argc,char **argv){

    init(argc,argv,"NODENAME_listener");
    NodeHandle sub_nh;
    Subscriber listener_subscriber =sub_nh.subscribe("TOPICNAME_pewpew",1000,listencallbackfunction);

    spin();

    return 0;
}

void listencallbackfunction(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}