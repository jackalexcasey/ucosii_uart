#include "includes.h"

INT8U FirstCharFlag=0x01;  //注意：此为串口发送第一个字符的标志，必须定义。




COMM_RING_BUF  CommBuf;


void  CommBufInit (void) 
{
    COMM_RING_BUF *pbuf;

    pbuf                  = &CommBuf;                     /* Initialize the ring buffer for COMM1     */
    pbuf->RingBufRxCtr    = 0;
    pbuf->RingBufRxInPtr  = &pbuf->RingBufRx[0];
    pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
    pbuf->RingBufRxSem    = OSSemCreate(0);
    pbuf->RingBufTxCtr    = 0;
    pbuf->RingBufTxInPtr  = &pbuf->RingBufTx[0];
    pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
    pbuf->RingBufTxSem    = OSSemCreate(COMM_TX_BUF_SIZE);
	   
}



INT8U  CommPutChar (INT8U c, INT16U to)//发送到TX ringbuf
{
  #if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT8U          oserr;
    COMM_RING_BUF  *pbuf;
	extern INT8U FirstCharFlag;
    pbuf=&CommBuf;
    OSSemPend(pbuf->RingBufTxSem, to, &oserr);             /* Wait for space in Tx buffer              */
    if (oserr == OS_TIMEOUT) {
        return (COMM_TX_TIMEOUT);                          /* Timed out, return error code             */
    }
    OS_ENTER_CRITICAL();
    pbuf->RingBufTxCtr++;                                  /* No, increment character count            */
    *pbuf->RingBufTxInPtr++ = c;                           /* Put character into buffer                */
    if (pbuf->RingBufTxInPtr == &pbuf->RingBufTx[COMM_TX_BUF_SIZE]) {     /* Wrap IN pointer           */
        pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
    }
    if (pbuf->RingBufTxCtr == 1) {                         /* See if this is the first character       */
#if 1
      if(FirstCharFlag==0x01)
      
       //TI=1;                                               /* Yes, Enable Tx interrupts                */
      UART2_ITConfig(UART2_IT_TXE, ENABLE);
#endif
    }
    //UART2_ITConfig(UART2_IT_TXE, ENABLE);
    
    OS_EXIT_CRITICAL();
    return (COMM_NO_ERR);
}

void  CommPutRxChar (INT8U c) 
{
    COMM_RING_BUF *pbuf;
	pbuf=&CommBuf;

    if (pbuf->RingBufRxCtr < COMM_RX_BUF_SIZE) {           /* See if buffer is full                    */
        pbuf->RingBufRxCtr++;                              /* No, increment character count            */
        *pbuf->RingBufRxInPtr++ = c;                       /* Put character into buffer                */
        if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE]) { /* Wrap IN pointer           */
            pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
        }
        OSSemPost(pbuf->RingBufRxSem);                     /* Indicate that character was received     */
    }
    //else{pbuf->RingBufRxCtr = 0;}
}

INT8U  CommGetChar (INT16U to, INT8U *err) 
{
  #if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT8U          c;
    static INT8U          oserr;
    COMM_RING_BUF *pbuf;
    pbuf=&CommBuf;
    OSSemPend(pbuf->RingBufRxSem, to, &oserr);             /* Wait for character to arrive             */
    if (oserr == OS_TIMEOUT) {                             /* See if characters received within timeout*/
        *err = COMM_RX_TIMEOUT;                            /* No, return error code                    */
        return (NUL);
    } else {
        OS_ENTER_CRITICAL();
        //if(pbuf->RingBufRxCtr >0)
        {
        pbuf->RingBufRxCtr--; 
        }/* Yes, decrement character count           */
#if 0
        if(pbuf->RingBufRxCtr>COMM_RX_BUF_SIZE)
        {pbuf->RingBufRxCtr = 0;
        }
#endif        
        c = *pbuf->RingBufRxOutPtr++;                      /* Get character from buffer                */
        if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE]) {     /* Wrap OUT pointer     */
            pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
        }
        OS_EXIT_CRITICAL();
        *err = COMM_NO_ERR;
        return (c);
    }
}

INT8U  CommGetTxChar (INT8U *err) //发送中断函数，从tx ringbuffer取数据
{ 
    INT8U          c;
    COMM_RING_BUF *pbuf;
    pbuf=&CommBuf;

    if (pbuf->RingBufTxCtr > 0) {                          /* See if buffer is empty                   */
        pbuf->RingBufTxCtr--;                              /* No, decrement character count            */
        c = *pbuf->RingBufTxOutPtr++;                      /* Get character from buffer                */
        if (pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[COMM_TX_BUF_SIZE]) {     /* Wrap OUT pointer     */
            pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
        }
        OSSemPost(pbuf->RingBufTxSem);                     /* Indicate that character will be sent     */
        *err = COMM_NO_ERR;
        return (c);                                        /* Characters are still available           */
    } else {
        *err = COMM_TX_EMPTY;
        return (NUL);                                      /* Buffer is empty                          */
    }
}


void UART_Print(INT8U *pData)
{
  INT8U i=0;
  //__disable_interrupt();
  while(*(pData+i) != '\0')
  {
    CommPutChar(*(pData+i), 0);
    i++;
   // OSTimeDly(2);
  }
  //__enable_interrupt();
  UART2_ITConfig(UART2_IT_TXE, ENABLE);//填充完毕发送buf，打开发送中断 cj
  asm("nop");
  
}

void UART_Scan(INT8U *pIn)
{
  INT8U  err;
  INT8U ch = 0;
 
  INT8U i=0;
  
  do
  {
   ch  = CommGetChar(0, &err);
   if(ch != '\0')//除去收到的'\0'
   {
   *(pIn +i)= ch;
   i++;
   }
  }
  while(ch !='\n');
  i-=2;
  *(pIn +i)= '\0';
  i++;
  *(pIn +i)= '\0';
}
