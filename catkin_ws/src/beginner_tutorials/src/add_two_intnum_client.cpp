#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"
#include <cstdlib>

using namespace ros;

int main(int argc, char **argv)
{

    init(argc, argv, "add_two_intnumber_client");
    if (argc != 3)
    {
        ROS_INFO("usage: add_two_ints_client X Y");
        return 1;
    }

    NodeHandle nh;
    ServiceClient clntserver = nh.serviceClient<beginner_tutorials::AddTwoInts>("add_two_intnums");

    beginner_tutorials::AddTwoInts srv;
    srv.request.a = atoi(argv[1]);
    srv.request.b = atoi(argv[2]);

    if (clntserver.call(srv))
    {
        ROS_INFO("Sum: %ld", (long int)srv.response.sum);
    }
    else
    {
        ROS_ERROR("Failed to call service add_two_ints");
        return 1;
    }

    return 0;
}