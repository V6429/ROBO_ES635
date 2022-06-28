#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
from std_msgs.msg import Float32
import serial
import sys

ser = serial.Serial(port=rospy.get_param('devtty', '/dev/ttyUSB0'),
                    baudrate = 9600,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=1.5
                )


def valuetobytes(value):
    value=str(value)
    return(bytes(value.encode('utf-8')))


def callback(speed):
    rospy.loginfo("Writing speed:" + ( str(speed.data)+ " "))
    ser.write(valuetobytes(str(speed.data)+ " "))


def pub_motorodoms():
    rospy.init_node('MOTOR_UART_INTERFACE', anonymous=True)

    while True:
        try :
            # so that the arduino resets and connections are open
            ser.readline()
            ser.readline()
            ser.write(valuetobytes(0))
            ser.write(valuetobytes(0))
            break
        except :
            rospy.loginfo("DID NOT GET TTY ACCESS",sys.version)
    
    rospy.loginfo("TTL DEVICE OK  NODE:"+str(rospy.get_name()))
    pub = rospy.Publisher('xwheel', Int16, queue_size=10)
    rospy.Subscriber("motor_cmd", Float32, callback)

    rate = rospy.Rate(10) # 10hz


    # while True:
        # try :

    # so that the arduino resets and connections are open
    ser.readline()
    ser.readline()
    ser.write(valuetobytes(0))
    ser.write(valuetobytes(0))

        #     break
        # except :
        #     rospy.loginfo("DID NOT GET TTY ACCESS")


    rospy.loginfo("WORKING")
    encodercount=0

    while not rospy.is_shutdown():
        # get encoder count
        try:
            ser.write(valuetobytes(b'e'))
            encodercount=int(ser.readline().decode('utf-8'))
        except:
            rospy.loginfo("FAILED TO GET SERIAL encoder" +str(rospy.get_name()))
        # rospy.loginfo(hello_str)
        pub.publish(encodercount)
        rate.sleep()



if __name__ == '__main__':

    try:
        pub_motorodoms()
    except rospy.ROSInterruptException:
        pass