#include "Arduino.h"
#include "I2C_Control.h"
#include "i2c_slave.h"
#include "Wire.h"

#define RESET_Pin PB2
#define TP_INT_Pin PB9
#define TE_IN_Pin PB1
#define PWM_Pin PA0
#define ZT7568_SLAVE_ADDR 0x20

#define Soft_i2C

TwoWire ZT7568(SCL_Pin, SDA_Pin, SOFT_FAST);

uint8_t read_buf[40];
uint8_t cmd_buf[10];

void LED_Toogle()
{
			  #ifdef HW_i2C			
			  I2C_Read_nByte(0x20, 0x8000,read_buf, 40);
	      I2C_Write_nByte(0x20, 0x0300, 0, 0);	
			  #endif
			
        #ifdef Soft_i2C			
				ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
				ZT7568.write(0x80);        		
				ZT7568.write(0x00);        		    		
				ZT7568.endTransmission(); 
				delay_us(50);
			
				ZT7568.requestFrom(ZT7568_SLAVE_ADDR, 40);    
			
				while (ZT7568.available()) 
				{ 
						for(uint8_t i = 0; i < 40; i++)
					{
					  read_buf[i] = ZT7568.read(); 
					}
				}	
				
				ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
				ZT7568.write(0x03);        		
				ZT7568.write(0x00);        		    		
				ZT7568.endTransmission(); 

			  #endif		
				digitalWrite(TP_INT_Pin, LOW);
        delay_us(1000);
			  digitalWrite(TP_INT_Pin, HIGH);

}
void setup() {
//	   uint8_t read_buf[2];
    /*PWM DutyCycle: 0~1000 -> 0~100%*/
    /*PWM Frequency: 10KHz*/
    PWM_Init(PWM_Pin, 1000, 60);
	  pwmWrite(PWM_Pin, 500);
		// put your setup code here, to run once:
		pinMode(RESET_Pin, OUTPUT);
		pinMode(TP_INT_Pin, OUTPUT);
		digitalWrite(TP_INT_Pin, HIGH);

    pinMode(TE_IN_Pin, INPUT_PULLUP);
    attachInterrupt(TE_IN_Pin, LED_Toogle, FALLING);
	
	  digitalWrite(RESET_Pin, HIGH);
		delay(10);
		digitalWrite(RESET_Pin, LOW);
		delay(100);
		digitalWrite(RESET_Pin, HIGH);
		delay(10);
	
		I2C1_Slave_init();
	  #ifdef Soft_i2C
	  ZT7568.begin();
	
		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;
		ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
		ZT7568.write(0x00);        		
		ZT7568.write(0xC0);        		
		ZT7568.write(cmd_buf, 2);        		
		ZT7568.endTransmission(); 
		delay(10);

		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;
		ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
		ZT7568.write(0x02);        		
		ZT7568.write(0xC0);        		
		ZT7568.write(cmd_buf, 2);        		
		ZT7568.endTransmission(); 
		delay(10);
		

		ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
		ZT7568.write(0x04);        		
		ZT7568.write(0xC0);        		
		ZT7568.endTransmission();
		delay(10);
		
		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;
		ZT7568.beginTransmission(ZT7568_SLAVE_ADDR); 		
		ZT7568.write(0x01);        		
		ZT7568.write(0xC0);        		
		ZT7568.write(cmd_buf, 2);        		
		ZT7568.endTransmission(); 
		delay(10);
		#endif
		
		#ifdef HW_i2C
		
		I2C_Config();

		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;	
		I2C_Write_nByte(0x20,0x00C0,cmd_buf,2);
		delay(10);
		
		I2C_Read_nByte(0x20, 0x00CC,read_buf, 2);
		delay(10);			
//		
		I2C_Write_nByte(0x20, 0x04C0, cmd_buf, 0);
		delay(10);						

		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;		
		I2C_Write_nByte(0x20, 0x02C0,cmd_buf, 2);						
		delay(2);
		
		cmd_buf[0] = 0x01;
		cmd_buf[1] = 0x00;		
		I2C_Write_nByte(0x20, 0x01C0,cmd_buf, 2);						
		delay(2);
//		
//		cmd_buf[0] = 0x01;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20, 0x1900,  cmd_buf,2);						
//		delay(1);	
//		
//		cmd_buf[0] = 0x01;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20, 0x1000, cmd_buf, 2);						
//		delay(1);	

//		cmd_buf[0] = 0x01;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20, 0x1500,cmd_buf,  2);						
//		delay(1);	
//		
//		cmd_buf[0] = 0xE3;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20,  0xC000,cmd_buf, 2);						
//		delay(1);	
//		
//		cmd_buf[0] = 0xCB;
//		cmd_buf[1] = 0x01;		
//		I2C_Write_nByte(0x20, 0xC100, cmd_buf, 2);						
//		delay(1);	
//		
//		I2C_Write_nByte(0x20, 0x0600, NULL, 0);
//		delay(1);	
//		
//		cmd_buf[0] = 0x1F;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20, 0xF000, cmd_buf, 2);						
//		delay(1);		

//		cmd_buf[0] = 0x00;
//		cmd_buf[1] = 0x00;		
//		I2C_Write_nByte(0x20, 0x2601,cmd_buf,  2);						
//		delay(1);	
      #endif

}

void loop() {

//    // put your main code here, to run repeatedly:

}


/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    InternalClocks_Init();
    Delay_Init();
//    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
