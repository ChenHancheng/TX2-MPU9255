#include "MPU9255.h"

MPU9255::MPU9255()
{
    kI2CBus = 1;
    error = 0;
}
MPU9255::~MPU9255()
{
    closeMPU9255();
}

int MPU9255::openMPU9255()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer, "/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
        // Could not open the file
        error = errno ;
        return -5 ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kMPU9255I2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        return -2 ;
    }
    return 1 ;
}

void MPU9255::closeMPU9255()
{
	if(kI2CFileDescriptor > 0)
	{
		close(kI2CFileDescriptor);
		kI2CFileDescriptor = -1;
	}
}

unsigned char MPU9255::readMPU9255(unsigned char slaveAddress, unsigned char readRegister)
{
	int toReturn;
	toReturn = i2c_smbus_write_byte(slaveAddress, readRegister);
	if(toReturn < 0)
	{
		error = errno;
		toReturn = -1;
	}	  
	toReturn = i2c_smbus_write_byte(kI2CFileDescriptor, readRegister);
	if(toReturn < 0)
	{
		error = errno;
		toReturn = -1;
	}
	toReturn = i2c_smbus_read_byte(kI2CFileDescriptor);
	if(toReturn < 0)
	{
		error = errno;
		toReturn = -1;
	}
	return toReturn;
}

unsigned char MPU9255::writeMPU9255(unsigned char slaveAddress, unsigned char writeRegister, unsigned char writeValue)
{
	int toReturn;
	toReturn = i2c_smbus_write_byte(slaveAddress, writeRegister);
	if(toReturn < 0)
	{
		error = errno;
		toReturn = -1;
	}
	toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
	usleep(10000);
	if(toReturn < 0) 
	{
		error = errno;
		toReturn = -1;
	}
	return toReturn;
}

int MPU9255::init()
{
    int toReturn;
	toReturn = openMPU9255();
	if (toReturn < 0) return toReturn;
	toReturn = writeMPU9255(ACCEL_ADDRESS,PWR_MGMT_1, 0x00);
	if (toReturn < 0) return -2;
	writeMPU9255(ACCEL_ADDRESS,SMPLRT_DIV, 0x07);
	writeMPU9255(ACCEL_ADDRESS,CONFIG, 0x06);
	writeMPU9255(ACCEL_ADDRESS,GYRO_CONFIG, 0x18);
	writeMPU9255(ACCEL_ADDRESS,ACCEL_CONFIG, 0x01);
	return 1;
}
MPU9255Data MPU9255::getAccel()
{
	unsigned char temp[2];
	MPU9255Data accelValue;
	temp[0] = readMPU9255(ACCEL_ADDRESS, ACCEL_XOUT_L);
	temp[1] = readMPU9255(ACCEL_ADDRESS, ACCEL_XOUT_H);
	accelValue.X = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(ACCEL_ADDRESS, ACCEL_YOUT_L);
	temp[1] = readMPU9255(ACCEL_ADDRESS, ACCEL_YOUT_H);
	accelValue.Y = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(ACCEL_ADDRESS, ACCEL_ZOUT_L);
	temp[1] = readMPU9255(ACCEL_ADDRESS, ACCEL_ZOUT_H);
	accelValue.Z = (temp[1] << 8) | temp[0];

	return accelValue;
}

MPU9255Data MPU9255::getGyro()
{
	unsigned char temp[2];
	MPU9255Data gyroValue;
	temp[0] = readMPU9255(GYRO_ADDRESS, GYRO_XOUT_L);
	temp[1] = readMPU9255(GYRO_ADDRESS, GYRO_XOUT_H);
	gyroValue.X = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(GYRO_ADDRESS, GYRO_YOUT_L);
	temp[1] = readMPU9255(GYRO_ADDRESS, GYRO_YOUT_H);
	gyroValue.Y = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(GYRO_ADDRESS, GYRO_ZOUT_L);
	temp[1] = readMPU9255(GYRO_ADDRESS, GYRO_ZOUT_H);
	gyroValue.Z = (temp[1] << 8) | temp[0];

	return gyroValue;
}

MPU9255Data MPU9255::getMag()
{
	unsigned char temp[2];
	MPU9255Data magValue;
	temp[0] = readMPU9255(MAG_ADDRESS, MAG_XOUT_L);
	temp[1] = readMPU9255(MAG_ADDRESS, MAG_XOUT_H);
	magValue.X = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(MAG_ADDRESS, MAG_YOUT_L);
	temp[1] = readMPU9255(MAG_ADDRESS, MAG_YOUT_H);
	magValue.Y = (temp[1] << 8) | temp[0];

	temp[0] = readMPU9255(MAG_ADDRESS, MAG_ZOUT_L);
	temp[1] = readMPU9255(MAG_ADDRESS, MAG_ZOUT_H);
	magValue.Z = (temp[1] << 8) | temp[0];

	return magValue;
}
