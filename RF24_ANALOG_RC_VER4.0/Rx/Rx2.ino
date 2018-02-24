/*
  Copyright (C) 2018 FAHHLA co
*/

/**
   Remote controller for JANGJU®

   Ver 4.0

   using Bluetooth

   This is the Rx version of RC board

   Arduino Mega2560

   4 channel output pwm
   xop, yop, sop, top

   2 binary output
   jck0, jck1
*/



const int xop = 4;
const int yop = 6;
const int sop = 5;
const int top = 7;

const int jck0 = 35;
const int jck1 = 37;

const int jsi = 31;
const int jso = 29;
const int sti = 33;
const int BEN = 34;

uint8_t xinD = 0;
uint8_t yinD = 0;
uint8_t sinD = 0;
uint8_t tinD = 0;

int bti = 0;
bool btib = true;
int TimeOut = 0;


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
int incind = 0;
bool incper = false;

//
// Setup
//
void setup(void)
{
  Serial.begin(115200);
  delay(500);

  //
  // Set up I/O
  //
  pinMode(BEN, INPUT_PULLUP);
  digitalWrite(xop, 0);
  digitalWrite(yop, 0);
  digitalWrite(sop, 0);
  digitalWrite(top, 0);
  digitalWrite(jsi, 1);
  digitalWrite(jso, 1);
  digitalWrite(sti, 1);
  delay(1000);
  digitalWrite(jsi, 0);
  digitalWrite(jso, 0);
  digitalWrite(sti, 0);
}


//
// Loop
//
void loop(void)
{
  if ( 1 )
  {
    if ( incper)
    {
        incper = false;
        if(!(digitalRead(BEN)))
        {
          while(!(digitalRead(BEN)))
          {
            digitalWrite(jsi, 1);
            digitalWrite(jso, 1);
            digitalWrite(sti, 1);
            delay(250);
            digitalWrite(jsi, 0);
            digitalWrite(jso, 0);
            digitalWrite(sti, 0);
            delay(250);
          }
          Serial.flush();
        }    
        uint8_t xinND = 0; uint8_t yinND = 0;
        uint8_t sinND = 0; uint8_t tinND = 0;
        
        //radio.read( DataRx, 14 );
        
            if (DataRx[3] == DataRx[7])
            {
              xinND = DataRx[3];
              xinD = xinND;
            }
            else { }
            if (DataRx[4] == DataRx[8])
            {
              yinND = DataRx[4];
              yinD = yinND;
            }
            else { }
            if (DataRx[5] == DataRx[9])
            {
              sinND = DataRx[5];
              sinD = sinND;
            }
            else { }
            if (DataRx[6] == DataRx[10])
            {
              tinND = DataRx[6];
              tinD = tinND;
            }
            else { }
      TimeOut = 0;
    }
    else
    {
      TimeOut++;
      delay(100);
    }
  }
  if(TimeOut > 30)
  {
    xinD = 128; yinD = 128;
    sinD = 1; tinD = 1;
  }
  // registering data
  if (digitalRead(BEN))
  {
    if (xinD < 118)
    {
      digitalWrite(jso, 0);
      int PPP = 118;
      PPP = PPP - xinD;
      digitalWrite(xop, 0);
      analogWrite(sop, map(PPP, 0, 118, 0, 255));
    }
    else if (xinD > 138)
    {
      digitalWrite(jso, 0);
      int PQP = 0;
      PQP = xinD - 138;
      digitalWrite(sop, 0);
      analogWrite(xop, map(PQP, 0, 118, 0, 255));
    }
    else
    {
      digitalWrite(xop, 0);
      digitalWrite(sop, 0);
      digitalWrite(jso, 1);
    }
    if (yinD < 118)
    {
      digitalWrite(jsi, 0);
      int PPW = 118;
      PPW = PPW - yinD;
      digitalWrite(yop, 0);
      analogWrite(top, map(PPW, 0, 118, 0, 255));
    }
    else if (yinD > 138)
    {
      digitalWrite(jsi, 0);
      int PQW = 0;
      PQW = yinD - 138;
      digitalWrite(top, 0);
      analogWrite(yop, map(PQW, 0, 118, 0, 255));
    }
    else
    {
      digitalWrite(yop, 0);
      digitalWrite(top, 0);
      digitalWrite(jsi, 1);
    }
    if (sinD > 136)
    {
      digitalWrite(jck0, 1);
    }
    else
    {
      digitalWrite(jck0, 0);
    }

    if (tinD > 136)
    {
      digitalWrite(jck1, 1);
    }
    else
    {
      digitalWrite(jck1, 0);
    }
  }
  else
  {
    while(!(digitalRead(BEN)))
    {
      digitalWrite(jsi, 1);
      digitalWrite(jso, 1);
      digitalWrite(sti, 1);
      delay(250);
      digitalWrite(jsi, 0);
      digitalWrite(jso, 0);
      digitalWrite(sti, 0);
      delay(250);
    }
    Serial.flush();
  }
}



void serialEvent()
{
    if(Serial.available()>0)
    {
        uint8_t rd = (uint8_t)Serial.read();
        if (rd == DBS0)
        {
            if (incind == 0)
                incind = 1;
        }
        else if (rd == DBS1)
        {
            if (incind == 1)
                incind = 2;
        }
        else if (rd == DBS2)
        {
            if (incind == 2)
                incind = 3;
        }
        else if (rd == DES0)
        {
            if (incind == 11)
                incind = 12;
        }
        else if (rd == DES1)
        {
            if (incind == 12)
                incind = 13;
        }
        else if (rd == DES2)
        {
            if (incind == 13)
            {
                incind = 0;
                incper = true;
            }
        }
        else
        {
            DataRx[incind] = rd;
            incind++;
        }
    }
}
