   
#include <reg52.h> 
#include <stdio.h> 
#include <intrins.h> 
#include<string.h> 
#define uchar unsigned char 
#define uint unsigned int 
#define BOOL bit  
sbit DSPORT=P3^7;
sbit SDA = P3 ^ 1;    
sbit SCL = P3 ^ 2;    
sbit KEY1 = P1 ^ 0;   
sbit KEY2 = P1 ^ 1;   
sbit RS=P2^6;							   
sbit RW=P2^5;							   
sbit EN=P2^7; 
sbit LSA=P2^2; 
sbit LSB=P2^3; 
sbit LSC=P2^4; 
   
unsigned char radio_write_data[5] = {0x30, 0x50, 0x70, 0x16, 0x80};   
unsigned long frequency = 101100;                                     
unsigned int pll = 101100;                                            
//unsigned char code str1[]="Temp:";//显示温度
//unsigned char code str2[]="Freq:"; //显示频率
unsigned char data DisplayData[6];//temperature
unsigned char temp_temp[16];
unsigned char temp_freq[16]; 
void DelayUs2x(unsigned char t) 
{    
 while(--t); 
} 
void DelayMs(unsigned char t) 
{    
 while(t--) 
 { 
     DelayUs2x(245); 
	 DelayUs2x(245); 
 } 
} 
void Delayus(unsigned char x)   
{ 
    for (; x > 0; x--) 
        ; 
}  
void iic_start()   
{ 
    SDA = 1; 
    Delayus(4); 
    SCL = 1; 
    Delayus(4); 
    SDA = 0; 
    Delayus(4); 
    SCL = 0; 
    Delayus(4); 
}  
void iic_stop()   
{ 
    SCL = 0; 
    Delayus(4); 
    SDA = 0; 
    Delayus(4); 
    SCL = 1; 
    Delayus(4); 
    SDA = 1; 
    Delayus(4); 
}  
void iic_ack()   
{ 
    SDA = 0; 
    Delayus(4); 
    SCL = 1; 
    Delayus(4); 
    SCL = 0; 
    Delayus(4); 
    SDA = 1; 
    Delayus(4); 
}  
bit iic_testack()   
{ 
    bit ErrorBit; 
    SDA = 1; 
    Delayus(4); 
    SCL = 1; 
    Delayus(4); 
    ErrorBit = SDA; 
    Delayus(4); 
    SCL = 0; 
    return ErrorBit; 
}  
void iic_write8bit(unsigned char input)   
{ 
    unsigned char temp; 
    for (temp = 8; temp > 0; temp--) 
    { 
        SDA = (bit)(input & 0x80); 
        Delayus(4); 
        SCL = 1; 
        Delayus(4); 
        SCL = 0; 
        Delayus(4); 
        input = input << 1; 
    } 
}  
void delays(unsigned char b)   
{ 
    unsigned char i; 
    for (b; b > 0; b--) 
        for (i = 0; i < 250; i++) 
            ; 
}  
void radio_write(void)   
{ 
    unsigned char i; 
    iic_start(); 
    iic_write8bit(0xc0);   
    if (!iic_testack()) 
    { 
        for (i = 0; i < 5; i++) 
        { 
            iic_write8bit(radio_write_data[i]); 
            iic_ack(); 
        } 
    } 
    iic_stop(); 
}  
void search()   
{ 
    pll = (unsigned int)((float)((frequency + 225)) / (float)8.192);   
    radio_write_data[0] = pll / 256;                                   
    radio_write_data[1] = pll % 256;                                   
    radio_write_data[2] = 0x70;                                        
    radio_write_data[3] = 0x16;                                        
    radio_write_data[4] = 0x80;                                        
    radio_write(); 
} 
void lcddelay1ms(unsigned int ms)  
{ 
  unsigned char i; 
	while(ms--) 
	{ 
	  for(i = 0; i< 250; i++) 
	  { 
	   _nop_();	    
	   _nop_(); 
	   _nop_();  
	   _nop_(); 
	  } 
	} 
} 
BOOL lcd_bz() 
{                          
	BOOL result; 
	RS = 0; 
	RW = 1; 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	result = (BOOL)(P0 & 0x80);	   
	EN = 0; 
	return result;  
} 
void wr_com(int cmd)  
{  
	while(lcd_bz()); 
	RS = 0; 
	RW = 0; 
	EN = 0; 
	_nop_(); 
	_nop_();  
	P0 = cmd;   
	_nop_(); 
	_nop_();   
	_nop_(); 
	_nop_(); 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	EN = 0;   
} 
void lcd_wcmd(int cmd) 
	{                            
	while(lcd_bz()); 
	RS = 0; 
	RW = 0; 
	EN = 0; 
	_nop_(); 
	_nop_();  
	P0 = cmd;   
	_nop_(); 
	_nop_();   
	_nop_(); 
	_nop_(); 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	EN = 0;   
	P0 = (cmd&0x0f)<<4;   
	_nop_(); 
	_nop_();   
	_nop_(); 
	_nop_(); 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	EN = 0;   
	} 
void wr_dat(int dat) 
{  
	while(lcd_bz()); 
	RS = 1; 
	RW = 0; 
	EN = 0; 
	P0 = dat ;    
	_nop_(); 
	_nop_();     
	_nop_(); 
	_nop_(); 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	EN = 0;   
	RS = 1; 
	RW = 0; 
	EN = 0; 
		_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	P0 = (dat&0x0f)<<4;    
	_nop_(); 
	_nop_();     
	_nop_(); 
	_nop_(); 
	EN = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	EN = 0;  
} 
void lcd_init() 
{ 
	 wr_com(0x38);                         
	lcddelay1ms(1);   
	wr_com(0x38);                         
	lcddelay1ms(1);   
	wr_com(0x28);       
	lcddelay1ms(1);   
	lcd_wcmd(0x28); 
	lcddelay1ms(1); 
  	lcd_wcmd(0x0c);       
	lcddelay1ms(1); 
   	lcd_wcmd(0x02);       
	lcddelay1ms(1); 
   	lcd_wcmd(0x01);       
	lcddelay1ms(1); 
} 

void lcd_pos(unsigned char pos) 
	{                            
	lcd_wcmd(pos | 0x80); 
	} 

void display1(unsigned char *p)  
{ 
	unsigned char i=0; 
	lcd_pos(0x00); 
	while(p[i]!='\0') 
	{ 
		wr_dat(p[i]); 
		i++; 
	} 
} 

void display(uint frq)   
{ 
    unsigned char i=0; 
    sprintf(temp_freq, "Freq:%5.1f MHz", ((float)frq) / 10); 
    lcd_pos(0x40); 
	//temp=strcat(temp,str2)
    while(temp_freq[i]!='\0') 
	{ 
		wr_dat(temp_freq[i]); 
		i++; 
	} 
} 
void init_play() 
{  
	lcd_init(); 
  lcd_pos(0x00); 
	display1("Temp:"); 
	lcd_pos(0x41); 
	search();  
  radio_write(); 
	display(frequency/100); 
}  
///////////////////获取温度///////////////////////
void Delay(unsigned char y)
{
	unsigned char x;
	for( ; y>0; y--)
	{
		for(x=110; x>0; x--);
	}
}
/*******************************************************************************
* 函 数 名         : Ds18b20Init
* 函数功能		   : 初始化
* 输    入         : 无
* 输    出         : 初始化成功返回1，失败返回0
*******************************************************************************/
unsigned char Ds18b20Init()
{
	unsigned char i;
	DSPORT = 0;			 //将总线拉低480us~960us
	i = 70;	
	while(i--);//延时642us
	DSPORT = 1;			//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	i = 0;
	while(DSPORT)	//等待DS18B20拉低总线
	{
		Delay(1);
		i++;
		if(i>5)//等待>5MS
		{
			return 0;//初始化失败
		}
	
	}
	return 1;//初始化成功
}
/*******************************************************************************
* 函 数 名         : Ds18b20WriteByte
* 函数功能		   : 向18B20写入一个字节
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds18b20WriteByte(unsigned char dat)
{
	unsigned char i, j;

	for(j=0; j<8; j++)
	{
		DSPORT = 0;	     	  //每写入一位数据之前先把总线拉低1us
		i++;
		DSPORT = dat & 0x01;  //然后写入一个数据，从最低位开始
		i=6;
		while(i--); //延时68us，持续时间最少60us
		DSPORT = 1;	//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadByte
* 函数功能		   : 读取一个字节
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/


uchar Ds18b20ReadByte()
{
	unsigned char byte, bi;
	unsigned char i, j;	
	for(j=8; j>0; j--)
	{
		DSPORT = 0;//先将总线拉低1us
		i++;
		DSPORT = 1;//然后释放总线
		i++;
		i++;//延时6us等待数据稳定
		bi = DSPORT;	 //读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;		//读取完之后等待48us再接着读取下一个数
		while(i--);
	}				
	return byte;
}
/*******************************************************************************
* 函 数 名         : Ds18b20ChangTemp
* 函数功能		   : 让18b20开始转换温度
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay(1);
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
	//Delay(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
   
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadTempCom
* 函数功能		   : 发送读取温度命令
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay(1);
	Ds18b20WriteByte(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xbe);	 //发送读取温度命令
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadTemp
* 函数功能		   : 读取温度
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

int Ds18b20ReadTemp()
{
	int temp = 0;
	uchar tmh, tml;
	Ds18b20ChangTemp();			 	//先写入转换命令
	Ds18b20ReadTempCom();			//然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadByte();		//读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte();		//再读高字节
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}
/*******************************************************************************
* 函 数 名         : datapros()
* 函数功能		   : 温度读取处理转换函数
* 输    入         : temp
* 输    出         : 无
*******************************************************************************/

void datapros() 	 
{
	int temp=Ds18b20ReadTemp();
  float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x2d;//0x40; 	  //   -
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		DisplayData[0] = 0x2b;//0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	DisplayData[1]=temp/1000+0x30;//百位数
  DisplayData[2]=temp%1000/100+0x30;//十位数
  DisplayData[4]=temp%100/10+0x30;//个位数
	DisplayData[3]=0x2e;
  DisplayData[5]=temp%10+0x30;//小数位
	//DisplayData[6]=0xe5;
}

void main()   
{ 
    init_play(); 
      
    while (1) 
    { 
		//显示温度
		datapros();
		lcd_pos(0x00);//这个位置要改
		//temp_temp=strcat("Temp:",DisplayData);
		strcpy(temp_temp,strcat(strcpy(temp_temp,"Temp:"),DisplayData));
		display1(temp_temp);
		
		//
        if (!KEY1)   
        { 
            delays(10);//(250);   
            if (!KEY1) 
            { 
                frequency += 100; 
                if (frequency > 108500)   
                    frequency = 75000;//87500;    
                search(); 
                display(frequency / 100);   
            } 
        }  
        else if (!KEY2)   
        { 
            delays(10);//(250);   
            if (!KEY2) 
            { 
                frequency -= 100; 
                if (frequency < 75000)//87500)    
                    frequency = 108500;   
                search(); 
                display(frequency / 100);   
            } 
            display(frequency / 100);   
        } 
		else
		{
			Delay(100);
		}
		
    } 
} 
