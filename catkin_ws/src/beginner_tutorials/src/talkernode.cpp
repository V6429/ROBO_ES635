
// including ros libraries
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace ros; // i dont waant to write " ros::"   multiple times

//the main body 

int main(int argc,char **argv){


    //First initialize ROS--> this handles all command line stuff
    // GIVE the node a NAME
    init(argc,argv,"THENODENAMEIS_talker");

    // we need an handler to manage resources of node(process)
    NodeHandle  talkerhandle;
    
    //tell the ROS master  that we are PUBLISHING
    // specify message & topicname
    // specify a buffer(q size)--> if buffer gets filled : old messages gets thrown away before reading
    Publisher  talker_IPublishStuff= talkerhandle.advertise<std_msgs::String>("TOPICNAME_pewpew",1000);

    // NodeHandle::advertise() returns a ros::Publisher object, which serves two purposes:
    // 1) it contains a publish() method that lets you publish messages onto the topic it was created with, and 
    // 2) when it goes out of scope, it will automatically unadvertise.

    Rate loop_rate(10);

    // A ros::Rate object allows you to specify a frequency that you 
    // would like to loop at. It will keep track of how long it has been since the last call 
    // to Rate::sleep(), and sleep for the correct amount of time.


    int msgsendcount=0;


    /// you can use ctrl+c if you use the ROS::ok() function
    while(ok()){
        std_msgs::String   string_msg;
        std::stringstream   ss; // for ease 

        ss<<"AM I WORKING ? :"<<msgsendcount;

        string_msg.data=ss.str();// loding to data variable(if multiple u may have to like this multiple times)
                                // see:   rosmsg show std_msgs/String

        // PUBLISH OBJECT WE GOT AFTER ADVERTISING WITH ROSMASTER
        // IS NOW PUBLISHING THE MESSAGE
        talker_IPublishStuff.publish(string_msg);


        ROS_INFO("%s", string_msg.data.c_str());
        // print the same content to rosout screen , similar to printf
    
        spinOnce(); // used to invoke node when working as subscriber
        // no use here still keep it for  good measure

        loop_rate.sleep();

        msgsendcount++;
    
    }


}