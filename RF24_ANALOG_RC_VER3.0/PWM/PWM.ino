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
int YYYY = 0;
bool btib = true;
int TimeOut = 0;

uint8_t Data[4] = {128, 128, 1, 1};

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



  xinD = Data[0];
  yinD = Data[1];
  sinD = Data[2];
  tinD = Data[3];
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



  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == 'a') {
      bti = 0;
    } else if (inChar == 'b') {
      bti = 1;
    } else if (inChar == 'c') {
      bti = 2;
    } else if (inChar == 'd') {
      bti = 3;
    }
    else
    {
      Data[bti] = (uint8_t)inChar;
    }
  }
}
