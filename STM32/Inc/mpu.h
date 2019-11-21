

#ifndef __MPU_H
#define __MPU_H
#include "stm32f4xx_hal.h"


I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart2;

#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75
#define PI 3.14


int16_t Accel_X_RAW;
int16_t Accel_Y_RAW;
int16_t Accel_Z_RAW;

int16_t Gyro_X_RAW;
int16_t Gyro_Y_RAW;
int16_t Gyro_Z_RAW;

float Ax, Ay, Az, Gx, Gy, Gz;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;


void MPU6050_Init (void);
void reverse(uint8_t *str, int len);
int intToStr(int x, uint8_t str[], int d);
void ftoa(float n, uint8_t  *res, int afterpoint);
void MPU6050_Read_Accel (void);
void MPU6050_Read_Gyro (void);


#endif
