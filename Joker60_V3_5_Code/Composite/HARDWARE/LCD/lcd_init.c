#include "lcd_init.h"
#include "main.h"
// #include "delay.h"
#include "spi.h"

void delay(int t)
{
	while(t--);
}

/*  ����ʹ�õĲ��� */
SPI_HandleTypeDef LCD_SPI;


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;
	LCD_CS_Clr();
	/*
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
	*/
	while(HAL_SPI_GetState(&LCD_SPI) != HAL_SPI_STATE_READY);//?��?��????����????
	HAL_SPI_Transmit(&LCD_SPI, &dat, 1, 10000);

  LCD_CS_Set();	
	
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set();//д����
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_DC_Set();//д����
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	//LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����CMD
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}

/*
* @brief ��ʼ��led��ʾ
* @param SPI_LCD ʹ�õ�Ӳ��spi��
*/

void LCD_Init(SPI_HandleTypeDef SPI_LCD)
{
	//SPI1_Init();
	//LCD_GPIO_Init();//��ʼ��GPIO
	//MX_SPI1_Init();
	LCD_SPI = SPI_LCD;
	
	LCD_RES_Clr();//��λ
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//�򿪱���
  	HAL_Delay(100);
	
	//return;
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11);                                             //�޴�ָ�����������ʼ��оƬ������ʾ
	HAL_Delay(120); 
	LCD_WR_REG(0x11);                                             //! ִ������űȽ��ȶ��������м�������������
	HAL_Delay(120); 

	LCD_WR_REG(0x36);                                             //�����ڴ�ɨ�跽��0X00����ɨ�裬�������£��������ң�RGB��ʽ
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);
 
	LCD_WR_REG(0x3A);                                             //���ݸ�ʽ��65Kɫ,565
	LCD_WR_DATA8(0x05);
 
	LCD_WR_REG(0xB2);                                             //֡Ƶ����
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 
 
  LCD_WR_REG(0xC6);                                               //����ģʽ�µ�֡���ʿ���
//  LCD_WR_DATA8(0x01);                                            //����ģʽ�£���Ļˢ���� 111Hz
  LCD_WR_DATA8(0x1F);                                            //����ģʽ�£���Ļˢ���� 39Hz
  
	LCD_WR_REG(0xB7);                                             //GATE ����
	LCD_WR_DATA8(0x35);  
 
	LCD_WR_REG(0xBB);                                             //VCOM���� 
	LCD_WR_DATA8(0x19);
 
	LCD_WR_REG(0xC0);                                             //LCM����,Ĭ��0x2c
	LCD_WR_DATA8(0x2C);
 
	LCD_WR_REG(0xC2);                                             //VDV&VRH SET ,Ĭ��0x01
	LCD_WR_DATA8(0x01);
 
	LCD_WR_REG(0xC3);                                             //VRHS SET��Ĭ��0x0b
	LCD_WR_DATA8(0x12);                                           //�˴�����ʵ���������
 
	LCD_WR_REG(0xC4);                                             //VDV SET��Ĭ��0x20
	LCD_WR_DATA8(0x20);  
 
	LCD_WR_REG(0xC6);                                             // FR SET, Ĭ��0x0F
	LCD_WR_DATA8(0x0F);    
 
	LCD_WR_REG(0xD0);                                             //��Դ����1
	LCD_WR_DATA8(0xA4);                                           //�ò�������  
	LCD_WR_DATA8(0xA1);                                           //�˴�����ʵ������޸�
 
	LCD_WR_REG(0xE0);                                             //������GAMMA���� 
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);
 
	LCD_WR_REG(0xE1);                                              //������GAMMA����
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);
 
	LCD_WR_REG(0x21);                                             //���Կ���Ĭ����0X20������ģʽ
  
	LCD_WR_REG(0x29);                                             //��ʾ�����ȴ�MCU������
} 








