#ifndef _MPU9255_H_
#define _MPU9255_H_

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// 定义MPU9250内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08
//电源管理，典型值：0x00(正常启用)
#define	PWR_MGMT_1		0x6B	
//IIC地址寄存器(默认数值0x68，只读)
#define WHO_AM_I                0x75	


//****************************

#define	GYRO_ADDRESS    0xD0	 //Address of Gyroscope
#define MAG_ADDRESS     0x18     //Address of Magnitude
#define ACCEL_ADDRESS   0xD0     //Address of Accelrity

#define kMPU9255I2CAddress 0x68

struct MPU9255Data
{
	short int X;
	short int Y;
	short int Z;
};

class MPU9255
{
public:
    unsigned char kI2CBus ;         // I2C bus of the Lidar-Lite
    int kI2CFileDescriptor ;        // File Descriptor to the Lidar-Lite
    int error ;
    MPU9255();
    ~MPU9255();
    int init();
    int openMPU9255();
    void closeMPU9255();
    unsigned char readMPU9255(unsigned char slaveAddress, unsigned char readRegister);
    unsigned char writeMPU9255(unsigned char slaveAddress, unsigned char writeRegister, unsigned char writeValue);
    MPU9255Data getAccel();
    MPU9255Data getGyro();
    MPU9255Data getMag();	
};

#endif