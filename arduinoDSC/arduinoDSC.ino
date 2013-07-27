#include <Arduino.h>

#include <digitalWriteFast.h>
#include <PinChangeInt.h>

/*
  Arduino based Digital Setting Circle
 
 Orlando Andico 2011-12-24
 
 <orly.andico at gmail.com>
 
 Changes in v1.0.1:
 - updated to compile on Arduino 1.0
 - full quadrature (4X) encoding via 4 virtual interrupts and PinChangeInt library - http://www.arduino.cc/playground/Main/PinChangeInt
 - more efficient reading of ports via digitalWriteFast library - http://code.google.com/p/digitalwritefast/
 - fast quadrature decode based on code from Dr Rainer Hessmer - http://www.hessmer.org/blog/2011/01/30/quadrature-encoder-too-fast-for-arduino/
 - only tested on Uno R3
 - note: all the above libraries needed minor edits to compile properly on Arduino 1.0
 
 
 Michael Fulbright 2010-06-06
 
 <mike.fulbright at pobox.com>
 
 Feel free to modify and share - please let me know if you make any changes so I can keep my version up to date
 
 2010-06-06:  Initial relase - supports 2X encoding
 
 */

// constants won't change. They're used here to 
// set pin numbers:

// RA
const int AZ_enc_A = 2;
const int AZ_enc_B = 3;

// DEC
const int ALT_enc_A = 12;
const int ALT_enc_B = 13;


// if either counts are updated..
volatile bool isUpdated = LOW;


// encoder resolution - some cheap encoders have lower tics on the DEC (like my JMI's!)
const int altRES = 8192;    // resolution of encoders
const int azRES = -8192;    // resolution of encoders

char beenAligned = 0;  

volatile bool _AzEncoderASet;
volatile bool _AzEncoderBSet;
volatile long AZ_pos = azRES / 2;

volatile bool _AltEncoderASet;
volatile bool _AltEncoderBSet;
volatile long ALT_pos = altRES / 2;


void setup() {
  Serial.begin(115200);

  // initialize the encoder inputs
  pinMode(ALT_enc_A, INPUT);
  pinMode(ALT_enc_B, INPUT);
  pinMode(AZ_enc_A, INPUT);
  pinMode(AZ_enc_B, INPUT);

  // turn on pullup resistors.. the quadrature algorithm is extremely sensitive
  digitalWrite(ALT_enc_A, LOW);
  digitalWrite(ALT_enc_B, LOW);
  digitalWrite(AZ_enc_A, LOW);
  digitalWrite(AZ_enc_B, LOW);

  // hook interrupts off the ALT and AZ pins
  PCintPort::attachInterrupt(ALT_enc_A, altFuncA, CHANGE);
  PCintPort::attachInterrupt(ALT_enc_B, altFuncB, CHANGE);

  PCintPort::attachInterrupt(AZ_enc_A, azFuncA, CHANGE);
  PCintPort::attachInterrupt(AZ_enc_B, azFuncB, CHANGE);
}

// ################### ALTITUDE DECODING FUNCTION ###################

void altFuncA() {
  _AltEncoderASet = digitalReadFast(ALT_enc_A) == HIGH;   // read the input pin
  _AltEncoderBSet = digitalReadFast(ALT_enc_B);   // read the input pin

  ALT_pos += (_AltEncoderASet != _AltEncoderBSet) ? +1 : -1;
  isUpdated = HIGH;
} 

void altFuncB() {
  _AltEncoderASet = digitalReadFast(ALT_enc_A);   // read the input pin
  _AltEncoderBSet = digitalReadFast(ALT_enc_B) == HIGH;   // read the input pin

  ALT_pos += (_AltEncoderASet == _AltEncoderBSet) ? +1 : -1;
  isUpdated = HIGH;
} 

// ################### AZIMUTH DECODING FUNCTION ###################

void azFuncA() {
  _AzEncoderASet = digitalReadFast(AZ_enc_A) == HIGH;   // read the input pin
  _AzEncoderBSet = digitalReadFast(AZ_enc_B);   // read the input pin

  AZ_pos += (_AzEncoderASet != _AzEncoderBSet) ? +1 : -1;
  isUpdated = HIGH;
} 

void azFuncB() {
  _AzEncoderASet = digitalReadFast(AZ_enc_A);   // read the input pin
  _AzEncoderBSet = digitalReadFast(AZ_enc_B) == HIGH;   // read the input pin

  AZ_pos += (_AzEncoderASet == _AzEncoderBSet) ? +1 : -1;
  isUpdated = HIGH;
} 

void loop() { 
  
  int inchar;

  if (Serial.available() > 0) 
  {
    // get incoming byte:
    inchar = Serial.read();
    
    if (inchar == 'Q')  //ascii for "Q"
    {
      //send out the encoder positions
      printEncoderValue(AZ_pos, HIGH);
      Serial.print("\t");
      printEncoderValue(ALT_pos, HIGH);
      Serial.println("\r");
    }   
  }
}

// print encoder value with leading zeros
void printEncoderValue(long val, bool lead)
{
  unsigned long aval; 

  if (lead) {
    if (val < 0)
      Serial.print("-");
    else
      Serial.print("+");
  }

  aval = abs(val);

  if (aval < 10) 
    Serial.print("0000");
  else if (aval < 100)
    Serial.print("000");
  else if (aval < 1000)
    Serial.print("00");
  else if (aval < 10000) 
    Serial.print("0");

  Serial.print(aval);  
}

void printHexEncoderValue(long val)
{
  char low, high;

  high = val / 256;

  low = val - high*256;

  Serial.print(low, HEX);
  Serial.print(high, HEX);

  return;
}


















