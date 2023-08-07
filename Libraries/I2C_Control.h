#ifndef _I2C_CONTROL_H
#define _I2C_CONTROL_H

#include "stm32f0xx.h"
#ifdef __cplusplus
extern "C" {
#endif

/*************宏定义操作区*************/
//#define I2C1D//硬件I2C1
//#define I2C1R//硬件I2C1重映射
#define I2C2D//硬件I2C2
//#define I2CS//软件模拟I2C
/*************************************/

#ifdef I2C1D//若定义硬件I2C1
	#define I2Cx             I2C1
	#define I2Cx_Clock       RCC_APB1Periph_I2C1//I2C时钟
	#define I2Cx_SCL_PIN     GPIO_Pin_6//I2C SCL GPIO引脚
	#define I2Cx_SDA_PIN     GPIO_Pin_7//I2C SDA GPIO引脚
#endif

#ifdef I2C1R//若定义硬件I2C1重映射
	#define I2Cx             I2C1
	#define I2Cx_Clock       RCC_APB1Periph_I2C1//I2C时钟
	#define I2Cx_SCL_PIN     GPIO_Pin_8//I2C SCL GPIO引脚
	#define I2Cx_SDA_PIN     GPIO_Pin_9//I2C SDA GPIO引脚
#endif

#ifdef I2C2D//若定义硬件I2C2
	#define I2Cx             I2C2
	#define I2Cx_Clock       RCC_APB1Periph_I2C2//I2C时钟
	#define I2Cx_SCL_PIN     GPIO_Pin_10//I2C SCL GPIO引脚
	#define I2Cx_SDA_PIN     GPIO_Pin_11//I2C SDA GPIO引脚
#endif


	//配置宏定义
#define I2C_Speed        0x10950C27//快速模式400k
#define I2Cx_MCU_Addr_1  0X00//单片机本身的7位I2C地址
#define I2Cx_GPIO_Clock  RCC_AHBPeriph_GPIOB//I2C GPIO时钟
#define I2Cx_SCL_PORT    GPIOB//I2C SCL GPIO端口
#define I2Cx_SDA_PORT    GPIOB//I2C SDA GPIO端口
//操作函数	
uint8_t	I2C_Write_1Byte(uint8_t DeviceAddr,uint8_t WriteAddr,uint8_t Data);//硬件I2C写1字节
uint8_t	I2C_Read_1Byte(uint8_t DeviceAddr,uint8_t ReadAddr);//硬件I2C读1字节
uint8_t	I2C_Write_nByte(uint8_t DeviceAddr,uint16_t WriteAddr,uint8_t *DataArray,uint8_t n);//硬件I2C写n字节
uint8_t	I2C_Read_nByte(uint8_t DeviceAddr,uint16_t ReadAddr,uint8_t *DataArray,uint8_t n);//硬件I2C读n字节
void	I2C_Config(void);//I2C初始化
void	I2C_ERROR(uint8_t errNum);//I2C错误处理函数
#ifdef __cplusplus
}
#endif

#endif
