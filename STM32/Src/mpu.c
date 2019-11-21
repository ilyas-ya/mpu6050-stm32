/*
 * mpu.c
 *
 *  Created on: 12 nov. 2019
 *      Author: Ilyas
 */


#include "mpu.h"
#include "stm32f4xx_hal.h"
#include <math.h>
                               //INITIALIZE MPU6050
void MPU6050_Init (void)
{
	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I

    	HAL_I2C_Mem_Read (&hi2c3, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 °/s
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}



                                         // CONVERTING FLOAT TO STRING


void reverse(uint8_t *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, uint8_t  str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}
void ftoa(float n, uint8_t  *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}







                     //READING FROM ACCELEROMETER AND GYROSCOPE


void MPU6050_Read_Accel (void)
	{
		uint8_t Rec_Data[6];

		// Read 6 BYTES of data starting from ACCEL_XOUT_H register

		HAL_I2C_Mem_Read (&hi2c3, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

		Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
		Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
		Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

		/*** convert the RAW values into acceleration in 'g'
		     we have to divide according to the Full scale value set in FS_SEL
		     I have configured FS_SEL = 0. So I am dividing by 16384.0
		     for more details check ACCEL_CONFIG Register              ****/

		Ax = Accel_X_RAW/16384.0;
		Ay = Accel_Y_RAW/16384.0;
		Az = Accel_Z_RAW/16384.0;
	}


	void MPU6050_Read_Gyro (void)
		{
			uint8_t Rec_Data[6];

			// Read 6 BYTES of data starting from GYRO_XOUT_H register

			HAL_I2C_Mem_Read (&hi2c3, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);

			Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
			Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
			Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

			/*** convert the RAW values into dps (°/s)
			     we have to divide according to the Full scale value set in FS_SEL
			     I have configured FS_SEL = 0. So I am dividing by 131.0
			     for more details check GYRO_CONFIG Register              ****/

			Gx = Gyro_X_RAW/131.0;
			Gy = Gyro_Y_RAW/131.0;
			Gz = Gyro_Z_RAW/131.0;
		}
