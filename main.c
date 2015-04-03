#include "includes.h"

#define  OS_TASK_0_STK_SIZE                200
#define  OS_TASK_1_STK_SIZE                200
#define  OS_TASK_2_STK_SIZE                200

#define  OS_TASK_0_PRIO                    1
#define  OS_TASK_1_PRIO                    0
#define  OS_TASK_2_PRIO                    2

OS_STK	Task0Stack[OS_TASK_0_STK_SIZE];	
OS_STK	Task1Stack[OS_TASK_1_STK_SIZE];
OS_STK	Task2Stack[OS_TASK_2_STK_SIZE];


#define ONE_FRAME_EN 1
typedef enum
{  
  UART_AT_STATE_INIT2_REP,
  UART_AT_STATE_ATI,
  UART_AT_STATE_AT,
  UART_AT_STATE_OK
 
}
UART_AT_STATE_TypeDef;
/*
#define LED0_On()    GPIOD->ODR |=    1<<3;
#define LED0_Off()   GPIOD->ODR &=  ~(1<<3);

#define LED1_On()    GPIOD->ODR |=    1<<2;
#define LED1_Off()   GPIOD->ODR &=  ~(1<<2);

#define LED2_On()    GPIOD->ODR |=    1<<0;
#define LED2_Off()   GPIOD->ODR &=  ~(1<<0);
*/
#define TIME_DELAY 999// can not above 1000



void task0( void * pdata )
{
	while(1)
	{
          //printf("task0\r\n");
#if ONE_FRAME_EN
          //UART_Print("task0\r\n");
          //UART_Print("AT\r\n");
#endif
		//__disable_interrupt();
		//LED0_On();
                LEDOn(RED);
		//__enable_interrupt(); 
OSTimeDlyHMSM(0,0,1,920);
		//OSTimeDlyHMSM(0,0,0,TIME_DELAY);
		
#if 1
		//__disable_interrupt();
		//LED0_Off();
                LEDOn(GREEN);
                OSTimeDlyHMSM(0,0,1,920);
		//__enable_interrupt(); 

		//OSTimeDlyHMSM(0,0,1,TIME_DELAY );
#endif
	}
	
}
/*
void task1(void * pdata )
{
  while(1)
  {
    printf("task1 ");
    
    __disable_interrupt();
    __enable_interrupt(); 
    
    static u8 voltageStr[20] = {0};
    static u8 temp_vol = 0;
    if(1)
    {
      temp_vol = (u8)GetADC(ADC_CHANNEL_LIGHT);
      
    }
    else
    {
      temp_vol = (u8)GetADC(ADC_CHANNEL_BATTERY);
    }
    voltageStr[0]= temp_vol/100 +'0';
    voltageStr[1] = '.';
    voltageStr[2]= temp_vol%100/10  +'0';
    voltageStr[3]= temp_vol%10  +'0';
    voltageStr[4]= '\0';
    //if(g_flag_light_adc)
      if(1)
    {
     printf("light: %s V\r\n",voltageStr); 
    }
    else
    {
     printf("battery: %s V\r\n",voltageStr); 
    }
    //Delay(0x5ffff);
   
    OSTimeDlyHMSM(0,0,0,TIME_DELAY );
    //LEDOn(GREEN);
    
#if 0
    __disable_interrupt();
    //LED1_Off();
    LEDOn(LED_OFF);
    __enable_interrupt(); 
    OSTimeDlyHMSM(0,0,0,TIME_DELAY -600);
#endif
  }
  
}
*/

void task1(void * pdata )
{
  
  INT8U  ch;
  INT8U  err;
  static INT8U print_out_buf[60] = {0};
  static INT8U scan_buf[90]={0};
  static UART_AT_STATE_TypeDef g_uart_at_state = UART_AT_STATE_AT;
  //U10开机信号 
  static bool flag_start_u10_sig = true;
   flag_start_u10_sig = true;
   
   GPIO_WriteHigh(GPIOD,GPIO_PIN_2);//高
   OSTimeDlyHMSM(0,0,4,0 );
   GPIO_WriteLow(GPIOD,GPIO_PIN_2);
    
  
  //cszdata=cszdata;
  //UART_Print("AT\r\n");
  
  //UART_Print("AT12345abcdefghijklmnopqrest\r\n");
  
  //g_uart_at_state = UART_AT_STATE_AT;
   g_uart_at_state = UART_AT_STATE_INIT2_REP;
  //注意！在最高优先级任务循环前打开定时器中断，以满足在OSStart()前不产生中断的要求。
  //在系统调用OSInit（）时会自动创建一个优先级最低的系统任务，创建过程中会调用OS_EXIT_CRITICAL（）打开EA。
  //若在InitTimer0()里打开T0中断，则违反了在OSStart()前不产生中断的要求。
  //切记将ET0=1;放在最高优先级任务里，OSStart()将调用OSStartHighRdy()第一个运行最高优先级任务，这样ET0=1总能被第一个执行。
  //ET0=1;      
  
  for (;;)
  {
#if 0
    if(flag_start_u10_sig)//只执行一次 //U10开机信号 
    {
      GPIO_WriteHigh(GPIOD,GPIO_PIN_2);//高
      flag_start_u10_sig = false;
      OSTimeDlyHMSM(0,0,4,0 );
      //  GPIO_WriteLow(GPIOD,GPIO_PIN_2);
      //  Delay(0x3ffff);
      GPIO_WriteLow(GPIOD,GPIO_PIN_2);
    }
#endif
#if ONE_FRAME_EN
    //INT8U scan_buf[20]={0};
    
    //printf("task1 \r\n");
    //UART_Print("AT\r\n");
    //OSTimeDlyHMSM(0,0,0,20);
    // CommPutChar('9', OS_TICKS_PER_SEC);
    //      CommPutChar('\r', OS_TICKS_PER_SEC);
    //      CommPutChar('\n', OS_TICKS_PER_SEC);
    UART_Scan(scan_buf);
#if DEBUG_EN   
    if(scan_buf[0] != '\0')
    {
      sprintf(print_out_buf,"YOU SEND: %s\r\n",scan_buf);
      UART_Print(print_out_buf);
      memset(scan_buf,0,60);
    }
#endif
    
#if 0
    if(strstr((char const *)scan_buf, "abcdef"))
    {
      UART_Print("YOU SEND: abcdef\r\n");
      
    }
#endif
#if 1
    
    switch(g_uart_at_state)
    {
    case UART_AT_STATE_INIT2_REP:
      if(strstr((char const *)scan_buf, "+INIT: 2"))
      {
        UART_Print("AT\r\n");
        g_uart_at_state = UART_AT_STATE_AT;
      }
      break;
    case UART_AT_STATE_AT:
      if(strstr((char const *)scan_buf, "OK"))
      {
        UART_Print("ATI\r\n");
        g_uart_at_state = UART_AT_STATE_ATI;
      }
      
      break;
    case UART_AT_STATE_ATI:
      if(strstr((char const *)scan_buf, "OK"))
      {
        UART_Print("AT+CREG?\r\n");
        g_uart_at_state = UART_AT_STATE_OK;
      }
      
      break;
    case UART_AT_STATE_OK:
      if(strstr((char const *)scan_buf, "+CREG:"))
      {
        UART_Print("AT+qflst=\"*\"\r\n");
        //g_uart_at_state = UART_AT_STATE_AT;
        g_uart_at_state = UART_AT_STATE_INIT2_REP;
      }
      //g_uart_at_state = UART_AT_STATE_AT;
      
      break;
    default:
      break;
      
    }
#endif
    
    //OSTimeDlyHMSM(0,0,0,20);
#else
    ch  = CommGetChar(0, &err);
    if(ch!=NUL)
    {
      CommPutChar(ch, 0);
      
    }
    OSTimeDly(2);
#endif
  }
  
#if 0
  while(1)
  {
#if 0
    printf("task1 ");
    
    if(g_flag__rx_frame_complete)//串口收到消息，字符串
    {
      g_flag__rx_frame_complete=false;
      //pBuf=UART2_GetData(&len);
      Uart_GetStr( (char *)g_uart_rx_content);
      if(strstr((char const *)g_uart_rx_content,"halt") )
      {
        printf("you send the message is :halt \r\n"); 
        asm("halt\n");
      }
    }
    
#endif   
    
    OSTimeDlyHMSM(0,0,0,TIME_DELAY );
    
  }
#endif
}

#if 0
void task2(void * pdata)
{
  static bool  sb_channel_light =false;
  u8 buffer[40] = {0};
  while(1)
  {
    //printf("task2 ");
    Uart_PrintStr("task2 ");
    
    __enable_interrupt(); 

    if(g_flag__rx_frame_complete)//串口收到消息，字符串
    {
      g_flag__rx_frame_complete=false;
      //pBuf=UART2_GetData(&len);
      Uart_GetStr( (char *)g_uart_rx_content);
      if(strstr((char const *)g_uart_rx_content,"light") )
      {
        //printf("you send the message is :light \r\n"); 
        Uart_PrintStr("you send the message is :light \r\n"); 
        sb_channel_light = true;
      }
      else if(strstr((char const *)g_uart_rx_content,"bat"))
      {
       Uart_PrintStr("you send the message is :bat \r\n"); 
        sb_channel_light = false;
      }
      else if(strstr((char const *)g_uart_rx_content,"halt"))
      {
       Uart_PrintStr("you send the message is :halt,now halt the system! \r\n"); 
       asm("halt\n");
       // halt(); /* Program halted */
      }
    }
    
    static u8 voltageStr[20] = {0};
    static u16 temp_vol = 0;//8bit will overflow !! >255
    if(sb_channel_light)
    {
      temp_vol = (u16)GetADC(ADC_CHANNEL_LIGHT);
      
    }
    else
    {
      temp_vol = (u16)GetADC(ADC_CHANNEL_BATTERY);
    }
    voltageStr[0]= temp_vol/100 +'0';
    voltageStr[1] = '.';
    voltageStr[2]= temp_vol%100/10  +'0';
    voltageStr[3]= temp_vol%10  +'0';
    voltageStr[4]= '\0';
    if(sb_channel_light)
    {
     sprintf(buffer,"light: %s V\r\n",voltageStr); 
     Uart_PrintStr(buffer);
     //printf("light: %s V\r\n",voltageStr); 
    }
    else
    {
     //printf("battery: %s V\r\n",voltageStr); 
     sprintf(buffer,"battery: %s V\r\n",voltageStr); 
     Uart_PrintStr(buffer);
    }
    
    g_flag_awu =true;
  
#if 0
    if(g_flag_awu)//已退出睡眠
    {
      g_flag_awu = false;
      LEDOn(GREEN);
      Delay(0x3ffff);
      LEDOn(LED_OFF);
      asm("halt\n");
    }
#endif
    //      current_uart_handler(g_uart_rx_content, len);
    //      break;
    
    OSTimeDlyHMSM(0,0,0,TIME_DELAY-800 );
    //LEDOn(LED_OFF);
#if 0
    __disable_interrupt();
    //LED2_On();
    //LEDOn(LED_OFF);
    __enable_interrupt(); 
    OSTimeDlyHMSM(0,0,0,TIME_DELAY );
    __disable_interrupt();
    //LED2_Off();
    //LEDOn(LED_OFF);
    __enable_interrupt(); 
    OSTimeDlyHMSM(0,0,0,TIME_DELAY);
#endif
  }	
}
#endif


void T4_Init(void)
{ 
       TIM4_ARRPreloadConfig(1);
       TIM4_PrescalerConfig(TIM4_PRESCALER_32,TIM4_PSCRELOADMODE_IMMEDIATE);  //128分频
       TIM4_SetAutoreload((F_CPU/32/OS_TICKS_PER_SEC));
       TIM4_Cmd(1);                   
       TIM4_ITConfig(TIM4_IT_UPDATE,1);    
}

int main()
{
  __disable_interrupt();
  //InitQueue(&uart_rx_cirqueue);
  //InitQueue(&uart_tx_cirqueue);
  
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);     
  //GPIO_Init(GPIOD, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_SLOW);  
  GPIO_INIT();//led
  GPIO_Init(GPIOD, GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);//开启U10的控制引脚
  pir_RI_io_init();//pir中断
  T4_Init();
  //Uart_Init(); 
  
  CommInit();
  /*AWU configuration --------------------------------------------*/
  AWU_Config();
  
  
  OSInit();
  CommBufInit();//创建信号量的操作不能再OSInit();之前，也就是不能在OS_InitEventList();之前
  
  
  OSTaskCreate( task0, (void *)OS_TASK_0_STK_SIZE, &Task0Stack[OS_TASK_0_STK_SIZE-1], OS_TASK_0_PRIO );
  OSTaskCreate( task1, (void *)OS_TASK_1_STK_SIZE, &Task1Stack[OS_TASK_1_STK_SIZE-1], OS_TASK_1_PRIO );
  //OSTaskCreate( task2, (void *)OS_TASK_2_STK_SIZE, &Task2Stack[OS_TASK_2_STK_SIZE-1], OS_TASK_2_PRIO );
  
  OSStart();
  return	0;
}


