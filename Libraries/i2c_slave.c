#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_i2c.h"
//#include "misc.h"

#include "i2c_slave.h"
uint8_t flag = 1;

/*******************************************************************/
extern volatile uint8_t ADCBuffer[16];
#define DEV_SLAVE_ADDR 0x48 << 1;
unsigned char i2c1_mode=I2C1_MODE_WAITING;
/*---------IIC1---------------*/
uint8_t  Buffer_Rx_IIC1[40];//????
uint8_t  Rx_Idx_IIC1=0;//????
uint8_t  Flag_RcvOK_IIC1 = 0;// ?????? 
uint8_t  Tx_Idx_IIC1=0;//????
uint8_t Response_Message[40];//????
uint8_t i2c1_ram_adr=0;
uint8_t i2c1_send_adr=0;

uint8_t i2c1_ram[I2C1_RAM_SIZE+1];

/*******************************************************************/
uint8_t get_i2c1_ram(uint8_t adr) {
	//ADC data
//	if ((ADC_ADDR_START <= adr) & (adr < ADC_ADDR_START + ADC_CHANNELS*2)) {
//		return ADCBuffer[adr - ADC_ADDR_START];
//	}
//	else {
//		// Other addresses
//	uint8_t buffer[10];
//	buffer[0] = 0x22;

//		buffer[1] = 0x22;
//		buffer[2] = 0xE6;
//		buffer[3] = 0x22;

		return i2c1_ram[adr];
//	}
}

void set_i2c1_ram(uint8_t adr, uint8_t val) {
	i2c1_ram[adr] = val;
	return;
}


/*******************************************************************/
void I2C1_Slave_init(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    I2C_InitTypeDef     I2C_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    // I2C port
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
 
    // I2C config
    I2C_InitStructure.I2C_Timing               = 0x00731012;
    I2C_InitStructure.I2C_AnalogFilter         = I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter        = 0x00;
    I2C_InitStructure.I2C_Mode                 = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1          = DEV_SLAVE_ADDR;
    I2C_InitStructure.I2C_Ack                  = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress  = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
 
    I2C_StretchClockCmd(I2C1, DISABLE);
 
    NVIC_InitStructure.NVIC_IRQChannel         = I2C1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0; // Low Medium Hight VeryHigh: 0 - 3
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    uint32_t temp = I2C_IT_STOPI |
                    I2C_IT_ADDRI |
                    I2C_IT_RXI   |
                    I2C_IT_TXI;
    I2C_ITConfig(I2C1, temp, ENABLE);

}
/*******************************************************************/

/*******************************************************************/
void I2C1_ClearFlag(void) {
//  // ADDR-Flag clear
//  while((I2C1->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR) {
//    I2C1->SR1;
//    I2C1->SR2;
//  }

//  // STOPF Flag clear
//  while((I2C1->SR1&I2C_SR1_STOPF) == I2C_SR1_STOPF) {
//    I2C1->SR1;
//    I2C1->CR1 |= 0x1;
//  }
}
/*******************************************************************/

/*******************************************************************/
//void I2C1_IRQHandler(void)
//{

//  __IO uint32_t SR1Register =0;
//  __IO uint32_t SR2Register =0;

//  SR1Register = I2C1->ISR;
////  SR2Register = I2C1->SR2;

//    /* I2C1�Ǵӻ�(MSL = 0) */
//  if(SR2Register & I2C_ISR_ADDR)
//  {
//         // I2C1 Address match event occurs
//        I2C_ClearITPendingBit(I2C1, I2C_ISR_ADDR);


//    /* ��������(RXNE = 1: EV2) */
//    if((SR1Register & 0x0040) == 0x0040)
//    {
//        Buffer_Rx_IIC1[Rx_Idx_IIC1++] = I2C1->RXDR;
//        SR1Register = 0;
//        SR2Register = 0;		
//			if(Buffer_Rx_IIC1[1] == 0xCC | Buffer_Rx_IIC1[1] == 0x20)
//			{
//						Response_Message[0] = 0x48;
//						Response_Message[1] = 0xE5;
//			}
//			else if(Buffer_Rx_IIC1[0] == 0x80)
//			{
//						Response_Message[0] = 0x04;
//						Response_Message[1] = 0x08;
//			}
//			else if(Buffer_Rx_IIC1[0] == 0x2C)
//			{
//						Response_Message[0] = 0xAA;
//						Response_Message[1] = 0x55;
//			}
////		  else
////			{
////						Response_Message[0] = 0x01;
////						Response_Message[1] = 0x02;
////			}
//    }
//    /* ��⵽ֹͣ����(STOPF =1: EV4) */
//    if(( SR1Register & 0x0010) == 0x0010)
//    {
//        I2C1->CR1 |= 0x0001;
//        SR1Register = 0;
//        SR2Register = 0;
//        Flag_RcvOK_IIC1 = 1;            
//    }




//    /* ��������(TxE = 1: EV3��EV3-1) */
//    if((SR1Register & 0x0080) == 0x0080)
//    {

////			Response_Message[0] = 0x01;
////						Response_Message[1] = 0x02;
//				Response_Message[2] = 0x03;
//				Response_Message[3] = 0x04;
//				Response_Message[4] = 0x05;
//				Response_Message[5] = 0x06;
//				Response_Message[6] = 0x07;
//				Response_Message[7] = 0x08;
//				Response_Message[8] = 0x09;
//				Response_Message[9] = 0x10;
//				Response_Message[10] = 0x11;
//				Response_Message[11] = 0x12;
//				Response_Message[12] = 0x13;
//				Response_Message[13] = 0x14;
//				Response_Message[14] = 0x15;
//				Response_Message[15] = 0x16;


//        I2C1->TXDR = Response_Message[Tx_Idx_IIC1++]; 
//        SR1Register = 0;
//        SR2Register = 0;
//    }
//    /* ��⵽��Ӧ��(AF =1: EV3-2) */
//    if(( SR1Register & 0x0400) == 0x0400)
//    {
////        I2C1->SR1 &= 0xFDFF;
//        SR1Register = 0;
//        SR2Register = 0;        
//    }       
//  }

//}
static uint8_t  Instruct = 0;
static uint8_t  Offset   = 0;
static uint8_t  MaxBytes = 0;
 
uint8_t *TxBuff;
uint8_t RxBuff[40];

void I2C1_IRQHandler(void)
{
    uint8_t  temp;
    uint32_t status = I2C1->ISR;
 
//    DEBUG_I2C("I2C1->ISR: %08X\n\r", status);
    if(status & I2C_ISR_ADDR)
    {   // I2C1 Address match event occurs
        I2C_ClearITPendingBit(I2C1, I2C_ISR_ADDR);
         Rx_Idx_IIC1=0;
        Tx_Idx_IIC1=0;
        if(status & I2C_ISR_DIR)
        {   // I2C1: slave enters transmitter mode.
            ;
        }
        else
        {   // I2C1: slave enters receiver mode.
            Instruct = 1;
        }
 
        Offset = 0;
    }
    else if(status & I2C_ISR_RXNE)
    {
        I2C1->ISR |= I2C_ISR_TXE;
        Buffer_Rx_IIC1[Rx_Idx_IIC1++] = I2C_ReceiveData(I2C1);
        if(1)
        {   // Host Instruct
           	if(Buffer_Rx_IIC1[0] == 0xAE)
			{
						Response_Message[0] = 0x26;				
			}
			else if(Buffer_Rx_IIC1[0] == 0xAF | Buffer_Rx_IIC1[0] == 0x10)
			{
				for(int i = 0; i < 40; i++)
				{
					
						Response_Message[i] = 0x00;
				}
										if(read_buf[2] == 0x02)		
										{
											flag++;
											if(flag == 0x01)
											{
					  //Finger 0 event info (touch / event type / hover / palm / event id[0~3])
						Response_Message[0] = 0xA2;
																  //Finger 0 xy coordinate (high)  y coordinate (bit 11 ~ bit 8) x coordinate (bit 11 ~ bit 8)
						Response_Message[1] = (read_buf[13] << 4) | (read_buf[11] & 0x0F);
					
					  //Finger 0 x coordinate (bit 7 ~ bit 0)
						Response_Message[2] = read_buf[10] ;
					
					  //Finger 0 y coordinate (bit 7 ~ bit 0)
						Response_Message[3] = read_buf[12] ;

					  //Finger 0 z (strength)
					  Response_Message[4] = read_buf[14] ;
											}
											else
											{
																	  //Finger 0 event info (touch / event type / hover / palm / event id[0~3])
						Response_Message[0] =  0xA1;									
									
					  //Finger 0 xy coordinate (high)  y coordinate (bit 11 ~ bit 8) x coordinate (bit 11 ~ bit 8)
						Response_Message[1] = (read_buf[7] << 4) | (read_buf[5] & 0x0F);
					
					  //Finger 0 x coordinate (bit 7 ~ bit 0)
						Response_Message[2] = read_buf[4] ;
					
					  //Finger 0 y coordinate (bit 7 ~ bit 0)
						Response_Message[3] = read_buf[6] ;

					  //Finger 0 z (strength)
					  Response_Message[4] = read_buf[8] ;
												flag = 0;
											}
										}
										else{
						  //Finger 0 event info (touch / event type / hover / palm / event id[0~3])
						Response_Message[0] = (read_buf[1]<<4 | 0x7F) & 0xA1;									
									
					  //Finger 0 xy coordinate (high)  y coordinate (bit 11 ~ bit 8) x coordinate (bit 11 ~ bit 8)
						Response_Message[1] = (read_buf[7] << 4) | (read_buf[5] & 0x0F);
					
					  //Finger 0 x coordinate (bit 7 ~ bit 0)
						Response_Message[2] = read_buf[4] ;
					
					  //Finger 0 y coordinate (bit 7 ~ bit 0)
						Response_Message[3] = read_buf[6] ;

					  //Finger 0 z (strength)
					  Response_Message[4] = read_buf[8] ;
            	}
					  //Finger 1 event info (touch / event type / hover / palm / event id[0~3])		
            if(read_buf[2] == 0x02)		
						{							
						  Response_Message[8] = 0xA2;
						}
						else
						{
						  Response_Message[8] = 0x22;						
						}
					  //Finger 1 xy coordinate (high)  y coordinate (bit 11 ~ bit 8) x coordinate (bit 11 ~ bit 8)
						Response_Message[9] = (read_buf[13] << 4) | (read_buf[11] & 0x0F);					

					  //Finger 1 x coordinate (bit 7 ~ bit 0)
						Response_Message[10] = read_buf[10] ;
					
					  //Finger 1 y coordinate (bit 7 ~ bit 0)
						Response_Message[11] = read_buf[12] ;

					  //Finger 1 z (strength)
					  Response_Message[12] = read_buf[14] ;
						
				

			}
			else if(Buffer_Rx_IIC1[0] == 0x0F)
			{
//					if(read_buf[2] == 0x02)		
//					{
//	          Response_Message[0] = 0x10;
//					}
//					else
//					{
			      Response_Message[0] = 0x08;

//					}

			}
			else if(Buffer_Rx_IIC1[0] == 0xE1)
			{
						Response_Message[0] = 0x04;
						Response_Message[1] = 0x82;
						Response_Message[2] = 0x07;
						Response_Message[3] = 0x00;
			}
			
			else if(Buffer_Rx_IIC1[0] == 0xF6)
			{
						Response_Message[0] = 0x55;
						Response_Message[1] = 0x56;
						Response_Message[2] = 0x32;
						Response_Message[3] = 0x00;
						Response_Message[4] = 0x00;
						Response_Message[5] = 0x00;
						Response_Message[6] = 0x00;
						Response_Message[7] = 0x00;
						Response_Message[8] = 0x11;
						Response_Message[9] = 0x08;

			}
			else if(Buffer_Rx_IIC1[0] == 0x02)
			{
						Response_Message[0] = 0x54;
						Response_Message[1] = 0x00;
						Response_Message[2] = 0x00;
						Response_Message[3] = 0x00;
			}
			else if(Buffer_Rx_IIC1[0] == 0x0B)
			{
						Response_Message[0] = 0x13;
						Response_Message[1] = 0x0C;
						Response_Message[2] = 0x00;
			}
			
			else if(Buffer_Rx_IIC1[0] == 0xE1)
			{
						Response_Message[0] = 0x04;
						Response_Message[1] = 0x82;
						Response_Message[2] = 0x07;
						Response_Message[2] = 0x00;
			}
			else if(Buffer_Rx_IIC1[0] == 0xE2)
			{
						Response_Message[0] = 0x82;
			}
			else if(Buffer_Rx_IIC1[0] == 0xE3)
			{
						Response_Message[0] = 0x07;
			}
//		  else
//			{
//						Response_Message[0] = 0x01;
//						Response_Message[1] = 0x02;
//			}

    
 
//            Instruct = 0;
        }
 
//        RxBuff[Offset++] = temp;
    }
    else if(status & I2C_ISR_TXIS)
    {
        I2C1->ISR |= I2C_ISR_TXE;

        I2C_SendData(I2C1, Response_Message[Tx_Idx_IIC1++]);

    }
    else if(status & I2C_ISR_STOPF)
    {
        I2C_ClearITPendingBit(I2C1, I2C_ISR_STOPF);

    }
    else
    {
        I2C_ClearITPendingBit(I2C1, 0x00003F38);
//        printf("I2C1 Unknown event occurs!\n\r");
    }
}

/*******************************************************************/
//void I2C1_ER_IRQHandler(void) {
////  if (I2C_GetITStatus(I2C1, I2C_IT_AF)) {
////    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
////  }
//	  __IO uint32_t SR1Register =0;
//  __IO uint32_t SR2Register =0;
//  SR1Register = I2C1->SR1;
//  SR2Register = I2C1->SR2;

//    if(I2C_GetITStatus(I2C1,        I2C_IT_SMBALERT)) {
//    }
//    else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT)) {
//    } 
//    else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR)) {
//    } 
//    else if(I2C_GetITStatus(I2C1, I2C_IT_OVR)) {

//    }
//    else if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {

//        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
//    }
//    else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO)) {

//    }
//    else if(I2C_GetITStatus(I2C1, I2C_IT_BERR)) {

//    }
//        I2C1->CR1 |= 0x0001;
//        SR1Register = 0;
//        SR2Register = 0;  

//}
/*******************************************************************/
//void I2C1_IRQHandler(void)
//{
//   __IO uint32_t I2CFlagStatus = 0x00000000;
//   uint8_t data;
//   
//   I2CFlagStatus = (uint32_t)(I2C1->ISR & (uint16_t)0x0000100FE);
//   if ((I2CFlagStatus & I2C_ISR_ADDR) != 0)
//  	{
//	    if(I2C1->ISR&I2C_ISR_DIR) //tx mode
//	    {
//             Tx_count = 0;
//         	I2C1->ISR |= I2C_ISR_TXE;
//             I2C1->ICR |= I2C_ICR_ADDRCF; 
//	   } 
//		
//	 if((I2C1->ISR&I2C_ISR_DIR)==0) //rx mode
//        {           
//	    Rx_buffer[0] = 0;
//	    Rx_count= 1;
//           I2C1->ICR |= I2C_ICR_ADDRCF; 
//        }
//    }
//   else if ((I2CFlagStatus & I2C_ISR_RXNE) != 0)
//   {
//   	 data = I2C_ReceiveData(I2C1);
//	 if(Rx_count < Rx_MAX){
//            Rx_buffer[Rx_count++] = data ;
//	     if(Rx_count == 2){
//	 	   reg_offset = Rx_buffer[1];
//	     }
//	 }
//   }
//  else if ((I2CFlagStatus & I2C_ISR_TXIS) != 0)
//  {
//		I2C_SendData(I2C1,Tx_buffer[reg_offset+Tx_count]);
//		Tx_count++;
//		if(Tx_count >= Tx_MAX)
//		{
//			Tx_count = 0; //tx ok
//		}
//  }
//  else if ((I2CFlagStatus & I2C_ISR_STOPF) != 0)
//  {
// 		I2C1->ICR |= I2C_ICR_STOPCF;
//		Rx_buffer[0] = Rx_count;
// 		Rx_count = 0;
// 		Tx_count = 0;
//  } 
//  
//}