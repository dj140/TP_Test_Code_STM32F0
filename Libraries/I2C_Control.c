#include "I2C_Control.h"

#define  I2C_Wait_Short 0x1000//硬件I2C短等待时间
#define  I2C_Wait_Long  0xA000//硬件I2C长等待时间

uint32_t I2C_Wait = 0;//硬件I2C等待变量
uint8_t I2C_ERROR_Count = 0;//I2C错误计数变量

void I2C_Config(void)//I2C初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;//创建GPIO初始化结构体
	
	#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//若定义硬件I2C
		I2C_InitTypeDef  I2C_InitStructure;//创建I2C初始化结构体
		RCC_APB1PeriphClockCmd( I2Cx_Clock,ENABLE );//使能与I2C有关的时钟
		RCC_APB2PeriphClockCmd( I2Cx_GPIO_Clock,ENABLE );//使能与I2C GPIO有关的时钟
		
		#ifdef I2C1R//若定义硬件I2C1重映射
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE );//使能重映射时钟,注意之后不得取消使能
			GPIO_PinRemapConfig( GPIO_Remap_I2C1,ENABLE );//使能重映射
		#endif
		
		GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;//SCL引脚
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//开漏输出
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出

		GPIO_Init( I2Cx_SCL_PORT,&GPIO_InitStructure );//GPIO初始化
		
		GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;//SDA引脚
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//开漏输出
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
		GPIO_Init( I2Cx_SDA_PORT,&GPIO_InitStructure );//GPIO初始化
		
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//配置为普通I2C模式
//		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//高电平数据稳定,低电平数据变化SCL时钟线的占空比
		I2C_InitStructure.I2C_OwnAddress1 =I2Cx_MCU_Addr_1;//单片机本身的7位或10位I2C地址
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//使能应答
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//7位或10位寻址模式
		I2C_InitStructure.I2C_Timing  = I2C_Speed;//通信速率
		I2C_Init(I2Cx, &I2C_InitStructure);//I2C初始化
		I2C_Cmd(I2Cx, ENABLE);//使能I2C
	#endif
	

}

#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//若定义硬件I2C
uint8_t I2C_Write_1Byte(uint8_t DeviceAddr,uint8_t WriteAddr,uint8_t Data)//硬件I2C写1字节
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
    I2C_SendData(I2Cx, WriteAddr);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
    I2C_SendData(I2Cx, Data);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
	
  	return 1;

}

uint8_t I2C_Read_1Byte(uint8_t DeviceAddr,uint8_t ReadAddr)//硬件I2C读1字节
{
   uint8_t ret;

    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
    I2C_SendData(I2Cx, ReadAddr);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);
    I2C_TransferHandling(I2Cx,DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
    ret = I2C_ReceiveData(I2Cx);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
    return ret;

}

uint8_t I2C_Write_nByte(uint8_t DeviceAddr,uint16_t WriteAddr,uint8_t *DataArray,uint8_t n)//硬件I2C写n字节
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		I2C_SendData( I2Cx,WriteAddr >> 8);//写8位寄存器地址
		I2C_SendData( I2Cx,WriteAddr );//写8位寄存器地址
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		for(; n>0; n--)
		{
			I2C_SendData( I2Cx,*DataArray );//写8位数据
			while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
			DataArray++;
		}
//	I2C_GenerateSTOP( I2Cx,ENABLE );//发送结束符
	return 1;
}

uint8_t I2C_Read_nByte(uint8_t DeviceAddr,uint16_t ReadAddr,uint8_t *DataArray,uint8_t n)//硬件I2C读n字节
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		I2C_SendData( I2Cx,ReadAddr >> 8 );//写8位寄存器地址
		I2C_SendData( I2Cx,ReadAddr );//写8位寄存器地址
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);
    I2C_TransferHandling(I2Cx,DeviceAddr << 1, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
	
	for(; n>0; n--)
	{
		if(n == 1)
		{
			I2C_AcknowledgeConfig( I2Cx,DISABLE );//不使能应答
			I2C_GenerateSTOP( I2Cx,ENABLE );//发送结束符
		}
//		I2C_Wait = I2C_Wait_Long;//等待时间复位
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

		
		*DataArray = I2C_ReceiveData(I2Cx);
		DataArray++;
	}
//	I2C_AcknowledgeConfig( I2Cx,ENABLE );//为之后的通讯使能应答
    return 1;
}
#endif


//void I2C_ERROR(uint8_t errNum)//I2C错误处理函数
//{
////	printf("I2C_err%d\n", errNum);//串口打印错误信息
//	//PCout(13) = 0;//点亮板载LED
//	#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//若定义硬件I2C
//	I2C_ERROR_Count++;
//	
//	if(((uint8_t)(errNum / 10) % 2) == 0){
//		I2C_GenerateSTOP( I2Cx,ENABLE );//若来自硬件I2C错误则发送硬件I2C结束符
//	}
//	
//	if(I2C_ERROR_Count >= 8){//多次错误则重启I2C总线
//		I2C_ERROR_Count = 0;
//		I2C_DeInit(I2Cx);
//		I2C_Config();
////		printf("I2C_RESTART\n");//串口打印I2C重启信息
//	}
//	#endif	
//}
