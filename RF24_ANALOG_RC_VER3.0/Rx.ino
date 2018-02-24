/*
  Copyright (C) 2018 FAHHLA co
*/

/**
   Remote controller for JANGJU®

   This is the Rx version of RC board

   Arduino Mega2560

   4 channel output pwm
   xop, yop, sop, top

   2 binary output
   jck0, jck1

 ***********

     NRF24L01:

    CE ==>> 9
    CS ==>> 10
*/


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//
// Hardware configuration
//

// nRF24L01 radio on (CE & CS)
RF24 radio(9, 10);
int TimeOut = 0;

//
// Topology
//
const uint64_t pipe = 0xE37E6F24E1LL;

uint8_t xinD = 0;
uint8_t yinD = 0;
uint8_t sinD = 0;
uint8_t tinD = 0;

//
// Payload
//
uint8_t DataRx[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t DBS0 = 128;
uint8_t DBS1 = 160;
uint8_t DBS2 = 192;
uint8_t DES0 = 130;
uint8_t DES1 = 162;
uint8_t DES2 = 194;

//
// Setup
//
void setup(void)
{
  Serial.begin(115200);
  delay(500);
  //Serial.println("radio begining");
  //
  // Setup and configure rf radio
  //
  radio.begin();
  //Setup and configure rf radio
  radio.setChannel(64);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  //radio.setRetries(2,15);
  radio.openReadingPipe(1, pipe);

  //radio.powerUp();
  
  radio.startListening();
}


//
// Loop
//
void loop(void)
{
  if ( radio.available() )
  {
    while (radio.available())
    {
      radio.read( DataRx, 14 );
      /////////////////////////////////
      if (DataRx[0] == DBS0 & DataRx[1] == DBS1 & DataRx[2] == DBS2)
      {
        if (DataRx[11] == DES0 & DataRx[12] == DES1 & DataRx[13] == DES2)
        {
          if (DataRx[3] == DataRx[7])
          {
            xinD = DataRx[3];
          }
          else { }
          if (DataRx[4] == DataRx[8])
          {
            yinD = DataRx[4];
          }
          else { }
          if (DataRx[5] == DataRx[9])
          {
            sinD = DataRx[5];
          }
          else { }
          if (DataRx[6] == DataRx[10])
          {
            tinD = DataRx[6];
          }
          else { }

          Serial.write('a');
          Serial.write(xinD);
          Serial.write('b');
          Serial.write(yinD);
          Serial.write('c');
          Serial.write(sinD);
          Serial.write('d');
          Serial.write(tinD);
          delay(100);
        }
        else
        {
          radio.flush_rx();
        }
      }
      else
      {
        radio.flush_rx();
      }
    }
    TimeOut = 0;
  }
  else
  {
    TimeOut++;
    delay(100);
  }
  if (TimeOut > 30)
  {
    xinD = 128; yinD = 128;
    sinD = 1; tinD = 1;
    Serial.write('a');
    Serial.write(xinD);
    Serial.write('b');
    Serial.write(yinD);
    Serial.write('c');
    Serial.write(sinD);
    Serial.write('d');
    Serial.write(tinD);
    delay(50);
  }
}
