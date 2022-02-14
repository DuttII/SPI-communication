/* The following program receives voltage levels communicated by SAMv71 through SPI protcol and displays them after recombination of msb and lsb
*/


#include <SPI.h>
uint8_t v[4];//storage variable to read lsb and msb
byte indx;
uint8_t c[2], b[2];
volatile boolean process;

void setup (void) {
   Serial.begin (115200);
   SPI.beginTransaction(SPISettings(1000000, MSBFIRST , SPI_MODE0));//MSB first : makes MSB of Mastrer data to be serially copied to MSB of slave rescive register ; LSB first : makes LSB of master data to be serially copied to MSB of slave recieve register.
    pinMode(MOSI, INPUT); //master to slave
   SPCR |= _BV(SPE); // turn on SPI in slave mode
 
   
   
   SPI.attachInterrupt(); // turn on interrupt
}
ISR (SPI_STC_vect) // SPI interrupt routine
{
  /*read 8 bit hex values*/
  for(int i=0 ;i<4  ; i++)// loop iterates 4 times to store consecutively lsb anf msb of v1 and lsb and msb of v2
  {
   
   v[i] = SPI.transfer(i);
   
   }
}



void loop (void) {
     /*reasolve v1*/
     uint8_t low1 = v[0];// stores lsb of v1
     uint8_t high1 = v[1];//stores msb of v1
     uint16_t myADCval1 = v[1];
     myADCval1 = ((myADCval1 << 8) | low1);//recombines msb and lsb to display full voltage of v1
     Serial.print("V1=");
     Serial.println(myADCval1); 
     /*resolve v2*/ 
   uint8_t low2 = v[2]; //stores lsb of v2
     uint8_t high2 = v[3]; //stores msb of v2
     uint16_t myADCval2 = v[3];
     myADCval2 = ((myADCval2 << 8) | low2);//recombines msb and lsb to display full voltage of v2
      Serial.print("V2=");
   Serial.println(myADCval2);  
    
   
}

