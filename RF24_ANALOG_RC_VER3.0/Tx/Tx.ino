/*
  Copyright (C) 2018 FAHHLA co
*/

/**
   Remote controller for JANGJU®

   VER 2.0

   This is the Tx version of RC board

   2axis joystick is connected to Tx Arduino

   xout ==>>

   yout ==>>

 ***********

     NRF24L01:

    CE ==>> 9
    CS ==>> 10
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#define FAILURE_HANDLING

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(5, 6);


const int xin = A0;
const int yin = A1;
const int din = 2;
const int tin = 3;
const int fin = 4;
const int dsn = A2;
const int hbk = A3;

const int cip = A4;

const int jsi = 7;
const int jso = 8;
const int sti = 9;

uint8_t xinD = 0;
uint8_t yinD = 0;
uint8_t sinD = 0;
uint8_t tinD = 0;


/*
  // Pins on the remote for buttons
  const uint8_t button_pins[] = { 2,3,4,5,6,7 };
  const uint8_t num_button_pins = sizeof(button_pins);
  // Pins on the LED board for LED's
  const uint8_t led_pins[] = { 2,3,4,5,6,7 };
  const uint8_t num_led_pins = sizeof(led_pins);
*/



//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE37E6F24E1LL;

//
// Payload
//
uint8_t DataTx[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
  //
  // Print preamble
  //
  Serial.begin(115200);
  delay(100);
  Serial.println("radio begining");

  //
  // Setup and configure rf radio
  //
  radio.begin();
  Serial.println("beginED");

  //Setup and configure rf radio
  radio.setChannel(64);
  Serial.println("channel 1");
  radio.setPALevel(RF24_PA_MAX);
  Serial.println("PA_MAX");
  radio.setDataRate(RF24_1MBPS);
  Serial.println("1MBPS");

  //radio.setRetries(15, 5);
  //radio.setCRCLength(RF24_CRC_8);

  radio.openWritingPipe(pipe);
  radio.powerUp();
  Serial.println("openPIPE");
  radio.printDetails();

  //
  // Set up INPUTS
  //
  pinMode(jsi, INPUT_PULLUP);
  pinMode(sti, INPUT_PULLUP);
    
  pinMode(cip, INPUT_PULLUP);

  pinMode(din, INPUT_PULLUP);
  pinMode(tin, INPUT_PULLUP);
  pinMode(fin, INPUT_PULLUP);

  pinMode(13, OUTPUT);

  pinMode(jso, OUTPUT);


  DataTx[0] = DBS0; DataTx[1] = DBS1; DataTx[2] = DBS2;
  DataTx[11] = DES0; DataTx[12] = DES1; DataTx[13] = DES2;
}



//
// Loop
//

void loop(void)
{
  digitalWrite(jso, digitalRead(jsi));
  int stp = digitalRead(sti);
  if ( digitalRead(jsi) )
  {
    bool different = false;
    /*uint8_t nData[14];
      nData[0] = DataTx[0];
      nData[1] = DataTx[1];
      nData[2] = DataTx[2];
      nData[3] = DataTx[3];
      nData[4] = DataTx[4];
      nData[5] = DataTx[5];
      nData[6] = DataTx[6];
      nData[7] = DataTx[7];
      nData[8] = DataTx[8];
      nData[9] = DataTx[9];
      nData[10] = DataTx[10];
      nData[11] = DataTx[11];
      nData[12] = DataTx[12];
      nData[13] = DataTx[13];*/
        
    uint8_t yinND = 0;
    uint8_t xinND = 0;
        
    if(digitalRead(cip))
    {
      xinND = map(analogRead(xin), 0, 1023, 0, 255);
      yinND = map(analogRead(yin), 0, 1023, 0, 255);
    }
    else
    {
      xinND = analogRead(xin);
      yinND = analogRead(yin);
      xinND = 1023 - xinND;
      yinND = 1023 - yinND;
      xinND = map(xinND, 0, 1023, 0, 255);
      yinND = map(yinND, 0, 1023, 0, 255);
    }
        
    uint8_t sinND = 1;
    uint8_t tinND = 1;
        
    if (!(digitalRead(hbk)))
    {
      xinND = 128; yinND = 128;
      different = true;
    }
        
    if (stp)
    { /*
        sinND = analogRead(din);
        tinND = analogRead(tin);*/
      if (!(digitalRead(din)))
      {
        sinND = 250;
        tinND = 1;
      }
      else
      {
        sinND = 1;
      }

      if (!(digitalRead(tin)))
      {
        tinND = 250;
        sinND = 1;
      }
      else
      {
        tinND = 1;
      }

      if (!(digitalRead(fin)))
      {
        sinND = 250;
        tinND = 250;
      }
    }
    if (xinND != xinD)
    {
      xinD = xinND;
      DataTx[3] = xinD;
      DataTx[7] = xinD;
      different = true;
    }

    if (yinND != yinD)
    {
      yinD = yinND;
      DataTx[4] = yinD;
      DataTx[8] = yinD;
      different = true;
    }

    if (sinND != sinD)
    {
      sinD = sinND;
      DataTx[5] = sinD;
      DataTx[9] = sinD;
      different = true;
    }

    if (tinND != tinD)
    {
      tinD = tinND;
      DataTx[6] = tinD;
      DataTx[10] = tinD;
      different = true;
    }

    // Send the state of the joystick to Rx
    if ( different )
    {

      Serial.println("different true");
      bool ok = radio.write( DataTx, sizeof(DataTx));
      if (ok)
      {
        digitalWrite(dsn, 0);
        Serial.println("1");
      }
      else
      {
        digitalWrite(dsn, 1);
        Serial.println("0");
      }
    }
    delay(100);
  }
}
