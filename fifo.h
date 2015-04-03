#ifndef __COMMRTOS_H__
#define __COMMRTOS_H__

#define  COMM_RX_BUF_SIZE      64                /* Number of characters in Rx ring buffer             */
#define  COMM_TX_BUF_SIZE      64                /* Number of characters in Tx ring buffer             */


#define  NUL                 0x00


typedef struct {
    INT16S     RingBufRxCtr;                /* Number of characters in the Rx ring buffer              */
    OS_EVENT  *RingBufRxSem;                /* Pointer to Rx semaphore                                 */
    INT8U     *RingBufRxInPtr;              /* Pointer to where next character will be inserted        */
    INT8U     *RingBufRxOutPtr;             /* Pointer from where next character will be extracted     */
    INT8U      RingBufRx[COMM_RX_BUF_SIZE]; /* Ring buffer character storage (Rx)                      */
    INT16S     RingBufTxCtr;                /* Number of characters in the Tx ring buffer              */
    OS_EVENT  *RingBufTxSem;                /* Pointer to Tx semaphore                                 */
    INT8U     *RingBufTxInPtr;              /* Pointer to where next character will be inserted        */
    INT8U     *RingBufTxOutPtr;             /* Pointer from where next character will be extracted     */
    INT8U      RingBufTx[COMM_TX_BUF_SIZE]; /* Ring buffer character storage (Tx)                      */
} COMM_RING_BUF;

extern COMM_RING_BUF  CommBuf;
extern INT8U FirstCharFlag   ;                                              /* ERROR CODES                                        */
#define  COMM_NO_ERR            0                /* Function call was successful                       */
#define  COMM_BAD_CH            1                /* Invalid communications port channel                */
#define  COMM_RX_EMPTY          2                /* Rx buffer is empty, no character available         */
#define  COMM_TX_FULL           3                /* Tx buffer is full, could not deposit character     */
#define  COMM_TX_EMPTY          4                /* If the Tx buffer is empty.                         */
#define  COMM_RX_TIMEOUT        5                /* If a timeout occurred while waiting for a character*/
#define  COMM_TX_TIMEOUT        6                /* If a timeout occurred while waiting to send a char.*/


#define  COMM_PARITY_NONE       0                /* Defines for setting parity                         */
#define  COMM_PARITY_ODD        1
#define  COMM_PARITY_EVEN       2



INT8U    CommGetChar(INT16U to, INT8U *err) ;
INT8U    CommGetTxChar(INT8U *err) ;
void     CommBufInit(void) ;
BOOLEAN  CommIsEmpty(INT8U ch);
BOOLEAN  CommIsFull(INT8U ch);
INT8U    CommPutChar(INT8U c, INT16U to) ;
void     CommPutRxChar(INT8U c) ;

void UART_Print(INT8U *pData);//jordan
void UART_Scan(INT8U *pIn);
#endif
