#ifndef _I2C_CONTROL_H
#define _I2C_CONTROL_H

#include "stm32f0xx.h"
#ifdef __cplusplus
extern "C" {
#endif

/*************�궨�������*************/
//#define I2C1D//Ӳ��I2C1
//#define I2C1R//Ӳ��I2C1��ӳ��
#define I2C2D//Ӳ��I2C2
//#define I2CS//���ģ��I2C
/*************************************/

#ifdef I2C1D//������Ӳ��I2C1
	#define I2Cx             I2C1
	#define I2Cx_Clock       RCC_APB1Periph_I2C1//I2Cʱ��
	#define I2Cx_SCL_PIN     GPIO_Pin_6//I2C SCL GPIO����
	#define I2Cx_SDA_PIN     GPIO_Pin_7//I2C SDA GPIO����
#endif

#ifdef I2C1R//������Ӳ��I2C1��ӳ��
	#define I2Cx             I2C1
	#define I2Cx_Clock       RCC_APB1Periph_I2C1//I2Cʱ��
	#define I2Cx_SCL_PIN     GPIO_Pin_8//I2C SCL GPIO����
	#define I2Cx_SDA_PIN     GPIO_Pin_9//I2C SDA GPIO����
#endif

#ifdef I2C2D//������Ӳ��I2C2
	#define I2Cx             I2C2
	#define I2Cx_Clock       RCC_APB1Periph_I2C2//I2Cʱ��
	#define I2Cx_SCL_PIN     GPIO_Pin_10//I2C SCL GPIO����
	#define I2Cx_SDA_PIN     GPIO_Pin_11//I2C SDA GPIO����
#endif


	//���ú궨��
#define I2C_Speed        0x10950C27//����ģʽ400k
#define I2Cx_MCU_Addr_1  0X00//��Ƭ�������7λI2C��ַ
#define I2Cx_GPIO_Clock  RCC_AHBPeriph_GPIOB//I2C GPIOʱ��
#define I2Cx_SCL_PORT    GPIOB//I2C SCL GPIO�˿�
#define I2Cx_SDA_PORT    GPIOB//I2C SDA GPIO�˿�
//��������	
uint8_t	I2C_Write_1Byte(uint8_t DeviceAddr,uint8_t WriteAddr,uint8_t Data);//Ӳ��I2Cд1�ֽ�
uint8_t	I2C_Read_1Byte(uint8_t DeviceAddr,uint8_t ReadAddr);//Ӳ��I2C��1�ֽ�
uint8_t	I2C_Write_nByte(uint8_t DeviceAddr,uint16_t WriteAddr,uint8_t *DataArray,uint8_t n);//Ӳ��I2Cдn�ֽ�
uint8_t	I2C_Read_nByte(uint8_t DeviceAddr,uint16_t ReadAddr,uint8_t *DataArray,uint8_t n);//Ӳ��I2C��n�ֽ�
void	I2C_Config(void);//I2C��ʼ��
void	I2C_ERROR(uint8_t errNum);//I2C��������
#ifdef __cplusplus
}
#endif

#endif
