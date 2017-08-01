#include "MPU9255.h"
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"


int main(int argc, char** argv)
{
	MPU9255 *MPU9255A = new MPU9255();
	struct MPU9255Data accelData, gyroData;
	int temp;

	sensor_msgs::Imu imuData;
	ros::init(argc, argv, "mpu9255");
	ros::NodeHandle nh;
	ros::Publisher imuDataPub = nh.advertise<sensor_msgs::Imu>("imuData", 1000);
	//intital the MPU9255
	temp = MPU9255A->init();
	if(temp != 1)
	{
	  ROS_FATAL_STREAM("Fail to init the MPU9255");
	  return -1;
	}
	//set pwr mode of MPU9255
	temp = MPU9255A->readMPU9255(ACCEL_ADDRESS, WHO_AM_I);
	if(temp != 115) 
	{
	  ROS_FATAL_STREAM("Fail to set the MPU9255");
	  return -1;
	}

	ros::Rate loop_rate(10);
	while(ros::ok())
	{
	//m2/s
	accelData = MPU9255A->getAccel();
	imuData.linear_acceleration.x = ((double) accelData.X)/16384*9.8;
	imuData.linear_acceleration.y = ((double) accelData.Y)/16384*9.8;
	imuData.linear_acceleration.z = ((double) accelData.Z)/16384*9.8;
	// degree/s
        gyroData = MPU9255A->getGyro();
	imuData.angular_velocity.x = ((double) gyroData.X)/2000;
	imuData.angular_velocity.y = ((double) gyroData.Y)/2000;
	imuData.angular_velocity.z = ((double) gyroData.Z)/2000;

	imuDataPub.publish(imuData);
	loop_rate.sleep(); 

	}	    
	return(-1);
}
