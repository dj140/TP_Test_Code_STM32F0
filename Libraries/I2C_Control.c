#include "I2C_Control.h"

#define  I2C_Wait_Short 0x1000//Ӳ��I2C�̵ȴ�ʱ��
#define  I2C_Wait_Long  0xA000//Ӳ��I2C���ȴ�ʱ��

uint32_t I2C_Wait = 0;//Ӳ��I2C�ȴ�����
uint8_t I2C_ERROR_Count = 0;//I2C�����������

void I2C_Config(void)//I2C��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ��
	
	#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//������Ӳ��I2C
		I2C_InitTypeDef  I2C_InitStructure;//����I2C��ʼ���ṹ��
		RCC_APB1PeriphClockCmd( I2Cx_Clock,ENABLE );//ʹ����I2C�йص�ʱ��
		RCC_APB2PeriphClockCmd( I2Cx_GPIO_Clock,ENABLE );//ʹ����I2C GPIO�йص�ʱ��
		
		#ifdef I2C1R//������Ӳ��I2C1��ӳ��
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE );//ʹ����ӳ��ʱ��,ע��֮�󲻵�ȡ��ʹ��
			GPIO_PinRemapConfig( GPIO_Remap_I2C1,ENABLE );//ʹ����ӳ��
		#endif
		
		GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;//SCL����
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//��©���
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���

		GPIO_Init( I2Cx_SCL_PORT,&GPIO_InitStructure );//GPIO��ʼ��
		
		GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;//SDA����
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//��©���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
		GPIO_Init( I2Cx_SDA_PORT,&GPIO_InitStructure );//GPIO��ʼ��
		
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//����Ϊ��ͨI2Cģʽ
//		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//�ߵ�ƽ�����ȶ�,�͵�ƽ���ݱ仯SCLʱ���ߵ�ռ�ձ�
		I2C_InitStructure.I2C_OwnAddress1 =I2Cx_MCU_Addr_1;//��Ƭ�������7λ��10λI2C��ַ
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//ʹ��Ӧ��
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//7λ��10λѰַģʽ
		I2C_InitStructure.I2C_Timing  = I2C_Speed;//ͨ������
		I2C_Init(I2Cx, &I2C_InitStructure);//I2C��ʼ��
		I2C_Cmd(I2Cx, ENABLE);//ʹ��I2C
	#endif
	

}

#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//������Ӳ��I2C
uint8_t I2C_Write_1Byte(uint8_t DeviceAddr,uint8_t WriteAddr,uint8_t Data)//Ӳ��I2Cд1�ֽ�
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

uint8_t I2C_Read_1Byte(uint8_t DeviceAddr,uint8_t ReadAddr)//Ӳ��I2C��1�ֽ�
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

uint8_t I2C_Write_nByte(uint8_t DeviceAddr,uint16_t WriteAddr,uint8_t *DataArray,uint8_t n)//Ӳ��I2Cдn�ֽ�
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		I2C_SendData( I2Cx,WriteAddr >> 8);//д8λ�Ĵ�����ַ
		I2C_SendData( I2Cx,WriteAddr );//д8λ�Ĵ�����ַ
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		for(; n>0; n--)
		{
			I2C_SendData( I2Cx,*DataArray );//д8λ����
			while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
			DataArray++;
		}
//	I2C_GenerateSTOP( I2Cx,ENABLE );//���ͽ�����
	return 1;
}

uint8_t I2C_Read_nByte(uint8_t DeviceAddr,uint16_t ReadAddr,uint8_t *DataArray,uint8_t n)//Ӳ��I2C��n�ֽ�
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
    I2C_TransferHandling(I2Cx, DeviceAddr << 1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
		I2C_SendData( I2Cx,ReadAddr >> 8 );//д8λ�Ĵ�����ַ
		I2C_SendData( I2Cx,ReadAddr );//д8λ�Ĵ�����ַ
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);
    I2C_TransferHandling(I2Cx,DeviceAddr << 1, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
	
	for(; n>0; n--)
	{
		if(n == 1)
		{
			I2C_AcknowledgeConfig( I2Cx,DISABLE );//��ʹ��Ӧ��
			I2C_GenerateSTOP( I2Cx,ENABLE );//���ͽ�����
		}
//		I2C_Wait = I2C_Wait_Long;//�ȴ�ʱ�临λ
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

		
		*DataArray = I2C_ReceiveData(I2Cx);
		DataArray++;
	}
//	I2C_AcknowledgeConfig( I2Cx,ENABLE );//Ϊ֮���ͨѶʹ��Ӧ��
    return 1;
}
#endif


//void I2C_ERROR(uint8_t errNum)//I2C��������
//{
////	printf("I2C_err%d\n", errNum);//���ڴ�ӡ������Ϣ
//	//PCout(13) = 0;//��������LED
//	#if (defined I2C1D)||(defined I2C1R)||(defined I2C2D)//������Ӳ��I2C
//	I2C_ERROR_Count++;
//	
//	if(((uint8_t)(errNum / 10) % 2) == 0){
//		I2C_GenerateSTOP( I2Cx,ENABLE );//������Ӳ��I2C��������Ӳ��I2C������
//	}
//	
//	if(I2C_ERROR_Count >= 8){//��δ���������I2C����
//		I2C_ERROR_Count = 0;
//		I2C_DeInit(I2Cx);
//		I2C_Config();
////		printf("I2C_RESTART\n");//���ڴ�ӡI2C������Ϣ
//	}
//	#endif	
//}
