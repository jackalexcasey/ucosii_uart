
  
        NAME OS_CPU_A
                
        EXTERN ?b0
        EXTERN ?b1  
        EXTERN ?b2        
        EXTERN ?b3
        EXTERN ?b4
        EXTERN ?b5
        EXTERN ?b6
        EXTERN ?b7
        EXTERN ?b8
        EXTERN ?b9
        EXTERN ?b10
        EXTERN ?b11
        EXTERN ?b12
        EXTERN ?b13
        EXTERN ?b14       
        EXTERN ?b15    
        
        EXTERN OSTaskSwHook
        EXTERN OSRunning
        EXTERN OSTCBHighRdy
        EXTERN OSTCBCur
        EXTERN OSPrioCur
        EXTERN OSPrioHighRdy
        EXTERN OSIntEnter
        EXTERN OSIntExit
        EXTERN OSTimeTick
        
        EXTERN TLI_IRQHandler //add irq
        EXTERN UART2_RX_IRQHandler
        EXTERN UART2_TX_IRQHandler
        EXTERN ADC1_IRQHandler
        
        
        PUBLIC OSStartHighRdy
        PUBLIC OSCtxSw
        PUBLIC OSIntCtxSw
      
SAVE_REGS  MACRO 
        pushw y
        pushw x        
        push  a
        push  cc        
        
        push s:?b0
        push s:?b1

        push s:?b2
        push s:?b3

        push s:?b4
        push s:?b5
        
        push s:?b6
        push s:?b7       
        
        push s:?b8
        push s:?b9

        push s:?b10
        push s:?b11
        
        push s:?b12
        push s:?b13

        push s:?b14
        push s:?b15 
        
        ENDM
        
INT_SAVE_REGS  MACRO        
        push s:?b0
        push s:?b1

        push s:?b2
        push s:?b3

        push s:?b4
        push s:?b5
        
        push s:?b6
        push s:?b7       
        
        push s:?b8
        push s:?b9

        push s:?b10
        push s:?b11
        
        push s:?b12
        push s:?b13

        push s:?b14
        push s:?b15 
        
        ENDM        
        
        
RESTORE_REGS  MACRO     
        pop s:?b15
        pop s:?b14

        pop s:?b13
        pop s:?b12

        pop s:?b11
        pop s:?b10
        
        pop s:?b9
        pop s:?b8      
       
        pop s:?b7
        pop s:?b6

        pop s:?b5
        pop s:?b4
        
        pop s:?b3
        pop s:?b2

        pop s:?b1
        pop s:?b0
                
        pop  cc 
        pop  a
        popw x
        popw y
        
        RETF
        ENDM  
        
INT_RESTORE_REGS  MACRO     
        pop s:?b15
        pop s:?b14

        pop s:?b13
        pop s:?b12

        pop s:?b11
        pop s:?b10
        
        pop s:?b9
        pop s:?b8      
       
        pop s:?b7
        pop s:?b6

        pop s:?b5
        pop s:?b4
        
        pop s:?b3
        pop s:?b2

        pop s:?b1
        pop s:?b0
                       
        IRET
        
        ENDM  

SAVE_OLD_STK_PTR MACRO                        
        LDW    Y,L:OSTCBCur                         //Y=OSTCBCur
        LDW    X,SP                                 //X=SP
        LDW    (Y),X                                //OSTCBCur->OSTCBStkPtr=SP       
        ENDM
       
LOAD_NEW_STK_PTR MACRO
         LDW     Y,   L:OSTCBHighRdy                 //Y=OSTCBHighRdy     
         LDW     Y,(Y)                               //Y= OSTCBHighRdy->OSTCBStkPtr
         LDW     SP, Y         
         ENDM

        SECTION `.far_func.text`:CODE:ROOT(0)
OSStartHighRdy:
         CALLF	OSTaskSwHook	                   //OSTaskSwHook()
	 MOV	        L:OSRunning,#1	           //OSRunning=1
       
         LOAD_NEW_STK_PTR
         RESTORE_REGS      
        
        
   
        SECTION `.far_func.text`:CODE:ROOT(0)
OSCtxSw: 
	SAVE_REGS
        SAVE_OLD_STK_PTR

	CALLF	OSTaskSwHook		            //OSTaskSwHook()
        
        MOV     L:OSPrioCur,L:OSPrioHighRdy         //OSPrioCur=OSPrioHighRdy
        LDW       X, L:OSTCBHighRdy 
        LDW       L:OSTCBCur, X                     //OSTCBCur=OSTCBHighRdy
        
        
        LOAD_NEW_STK_PTR
        RESTORE_REGS 
 
 
        SECTION `.far_func.text`:CODE:ROOT(0)
OSIntCtxSw:
	CALLF	OSTaskSwHook			   //OSTaskSwHook()
        
        MOV     L:OSPrioCur,L:OSPrioHighRdy        //OSPrioCur=OSPrioHighRdy
        LDW       X, L:OSTCBHighRdy 
        LDW       L:OSTCBCur, X                   //OSTCBCur=OSTCBHighRdy
        
        LOAD_NEW_STK_PTR
        RESTORE_REGS 
 
        

        
        PUBLIC _interrupt_25
        SECTION `.far_func.text`:CODE:NOROOT(0)
_interrupt_25:    
         SIM
        
         INT_SAVE_REGS
         
         SAVE_OLD_STK_PTR
        
         BRES       0x5342, #0
          
         CALLF     OSIntEnter
         CALLF     OSTimeTick
         CALLF     OSIntExit
        
         INT_RESTORE_REGS
         
        PUBLIC _interrupt_2
        SECTION `.far_func.text`:CODE:NOROOT(0)
_interrupt_2:    
         SIM
        
         INT_SAVE_REGS
         
         SAVE_OLD_STK_PTR
        
        // BRES       0x5342, #0
          
         CALLF     OSIntEnter
         CALLF     TLI_IRQHandler
         CALLF     OSIntExit
        
         INT_RESTORE_REGS        
         
        PUBLIC _interrupt_22
        SECTION `.far_func.text`:CODE:NOROOT(0)
_interrupt_22:    
         SIM
        
         INT_SAVE_REGS
         
         SAVE_OLD_STK_PTR
        
        // BRES       0x5342, #0
          
         CALLF     OSIntEnter
         CALLF     UART2_TX_IRQHandler
         CALLF     OSIntExit
        
         INT_RESTORE_REGS  
         
         PUBLIC _interrupt_23
        SECTION `.far_func.text`:CODE:NOROOT(0)
_interrupt_23:    
         SIM
        
         INT_SAVE_REGS
         
         SAVE_OLD_STK_PTR
        
        // BRES       0x5342, #0
          
         CALLF     OSIntEnter
         CALLF     UART2_RX_IRQHandler
         CALLF     OSIntExit
        
         INT_RESTORE_REGS
         
        PUBLIC _interrupt_24
        SECTION `.far_func.text`:CODE:NOROOT(0)
_interrupt_24:    
         SIM
        
         INT_SAVE_REGS
         
         SAVE_OLD_STK_PTR
        
        // BRES       0x5342, #0
          
         CALLF     OSIntEnter
         CALLF     ADC1_IRQHandler
         CALLF     OSIntExit
        
         INT_RESTORE_REGS
         
         END
