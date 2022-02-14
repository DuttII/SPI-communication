/* The following program receives voltage levels communicated by arduino through SPI protcol and displays them after recombination of msb and lsb
*/
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>
#include<stdio.h>


unsigned int  v1 , v2; //these are the voltage levels that correspond to afec output
uint8_t rxData[4];// since arduino transmits 8-bit data our receive variables have been configured as such
uint8_t low1,low2,high1,high2 ;// these variables perform the function of storing lsb and msb of voltage levels transmitted by arduino 
uint16_t myADCval1,myADCval2;// the final voltage reading that combines lsb and msb is stored here
volatile bool transferStatus=false;// variable in the write read Interrupt subroutine to signify communication
 

/* This function will be called by SPI PLIB when transfer is completed */
void SPI0_Callback(uintptr_t context )
{
    transferStatus = true;
}
 
int main ( void )
{
   
    /* Initialize all modules */
     SYS_Initialize ( NULL );
     SYSTICK_TimerStart();
     SPI0_CallbackRegister(SPI0_Callback, 0);
     while(1)
    {
        SYSTICK_DelayMs(1000);
      
          for(int i=0 ; i<4 ; i++)// the loop is iterated 4 times to recieve 2 lsbs and 2 msbs
          {   /*SPI Read*/
              SPI0_Read( &rxData[i], sizeof(rxData[i]));// calls the function SPIO read to read the data appearing in SPI registers
               printf("rxData[%d] = %d\n\r",i+1,rxData[i]);
          }    /*Resolve V1*/
               high1 = rxData[1];
                low1 = rxData[0];
                myADCval1 = high1;// recombination logic. It takes in msb and lsb separately, left shifts msb to appead at first 8 bits and then directly adds the other 8 bits of lsb
                myADCval1 = (myADCval1<<8) | low1;
                printf("v1 = %d\r\n", myADCval1); //final value of V1
                /* resolve V2*/
                high2 = rxData[3];
                low2 = rxData[2];
                myADCval2 = high2;
                myADCval2 = (myADCval1<<8) | low2;
                printf("v2 = %d\r\n", myADCval2); //final value of V2
           }
       
        if(transferStatus == true)// communication check 
        {
            transferStatus = false;

            
            printf("message Recieved\n\r");
            LED_Clear(); //turn on built in LED if message is sent
           
           
           
        }

    }
