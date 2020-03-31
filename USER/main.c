#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 


/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//协议初始化
void Gizwits_Init(void)
{
	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}

//数据采集
void userHandle(void)
{

    //判断当前LED1开关量
	   if(LED1==0)
	    currentDataPoint.valueLEDonoff = 1;
      else 
	    currentDataPoint.valueLEDonoff = 0;
	
}

//主函数
 int main(void)
 {		
    int key;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	    //串口初始化为115200
 	LED_Init();			    //LED端口初始化
	KEY_Init();             //按键初始化  
    Gizwits_Init();         //协议初始化
	printf("--------机智云-协议移植LED测试实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");
   	while(1)
	{
	   userHandle();//用户采集
         
       gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}
		delay_ms(200);
		LED0=!LED0;
	}	 

} 
 
