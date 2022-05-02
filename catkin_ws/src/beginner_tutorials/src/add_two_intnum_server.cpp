#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"

using namespace ros;

bool add(beginner_tutorials::AddTwoInts::Request &req,
         beginner_tutorials::AddTwoInts::Response &res){

        res.sum = req.a + req.b;
        ROS_INFO("ADDING COMPLETED requestrequest: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
        ROS_INFO("sending response : [%ld]",(long int )res.sum);

    return true;
}

int main(int argc,char **argv){

    init(argc,argv,"add_two_integernumber_SERVER");
    NodeHandle nh;
    ServiceServer  add_two_server=nh.advertiseService("add_two_intnums",add);
    ROS_INFO("Ready to add two ints.");
    spin();

    
}