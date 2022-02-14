/* The following program enables arduino to sense the voltages connected to it's analog pins and communicate them to SAMv71 through spi protocol
*/

#include <SPI.h> 
uint8_t v[4];
uint16_t  v1,v2;
volatile uint8_t lowv1, highv1, lowv2, highv2 ;
byte indx;
uint8_t c[2], b[2];
volatile boolean process;

void setup (void) {
   Serial.begin (115200);
   SPI.beginTransaction(SPISettings(1000000, MSBFIRST , SPI_MODE0)); //MSB first : makes MSB of Master data to be serially copied to MSB of slave rescive register ; LSB first : makes LSB of master data to be serially copied to MSB of slave recieve register.
   pinMode(MISO, OUTPUT); // master to Slave. this pin communicates the data serially into SAMv71
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   SPI.attachInterrupt(); // turn on interrupt
} 


ISR (SPI_STC_vect) // SPI interrupt routine
{
  
   SPI.transfer(lowv1);  //The following sequence enables voltage to be transmitted in the order of measurement.Lsb then msb of v1 follwoing which lsb and msb of v2
   SPI.transfer(highv1);
   SPI.transfer(lowv2);
   SPI.transfer(highv2);
}
   
  
void loop (void) {
        //read voltages
       v1 = analogRead(A0); //Ardunio reads the voltage level connected to pin A0
       v2 = analogRead(A1);//Ardunio reads the voltage level connected to pin A1
       v1 = v1* (5000.0 / 1023.0);
       v2 = v2* (5000.0 / 1023.0);
       Serial.print("v1 = ");
       Serial.println(v1);
       Serial.print("v2 = ");
       Serial.println(v2);

     //convert v1 into two 8 bits
     highv1 = (v1 >> 8);//right shift to get msb of v1
     lowv1 = (v1 & 0x00FF);//multiply by 0x11111111(2) to get lsb of v1
     Serial.println(lowv1);
     Serial.println(highv1);
     //convert v2 into two 8 bits
      highv2 = (v2 >> 8);
     lowv2 = (v2 & 0x00FF);//right shift to get msb of v2
     Serial.println(lowv2//multiply by 0x11111111(2) to get lsb of v2
     Serial.println(highv2);
}
     
 