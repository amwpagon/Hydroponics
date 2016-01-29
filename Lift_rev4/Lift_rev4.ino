#include<Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(79, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(79, 13, NEO_GRB + NEO_KHZ800);
long millisCurrent;

byte bBtnBay1Pin = 3;
byte bBtnBay2Pin = 4;

byte DriveBay1MotorAPin = 5;
byte DriveBay1MotorBPin = 6;

byte DriveBay1LimitFrontPin = 7;
byte DriveBay1LimitRearPin = 8;

byte DriveBay2MotorAPin = 9;
byte DriveBay2MotorBPin = 10;

byte DriveBay2LimitFrontPin = 11;
byte DriveBay2LimitRearPin = 12;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pixels1.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < 79; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels1.setPixelColor(i, pixels1.Color(200, 0, 200)); // Moderately bright green color.

  }
  pixels1.show(); // This sends the updated pixel color to the hardware.

  pixels2.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < 79; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels2.setPixelColor(i, pixels2.Color(200, 0, 200)); // Moderately bright green color.

  }
  pixels2.show(); // This sends the updated pixel color to the hardware.


  pinMode(bBtnBay1Pin, INPUT_PULLUP);
  pinMode(bBtnBay2Pin, INPUT_PULLUP);

  pinMode(DriveBay1LimitFrontPin, INPUT_PULLUP);
  pinMode(DriveBay1LimitRearPin, INPUT_PULLUP);

  pinMode(DriveBay1MotorAPin, OUTPUT);
  pinMode(DriveBay1MotorBPin, OUTPUT);

  pinMode(DriveBay2LimitFrontPin, INPUT_PULLUP);
  pinMode(DriveBay2LimitRearPin, INPUT_PULLUP);

  pinMode(DriveBay2MotorAPin, OUTPUT);
  pinMode(DriveBay2MotorBPin, OUTPUT);

  Serial.println("Arduino Bay Manager Connect");
}

void loop() {
  millisCurrent = millis();
  BtnBayUpdate();
  DriveBayUpdate();
  DriveLight();
  SerialUpdate();
}

// ==================================
//              DriveBay
// ==================================

//int DriveBaySpeed = HIGH;
int DriveBaySpeed = 235;

byte DriveBay1State;
byte DriveBay2State;

byte DriveBay1Direction;
byte DriveBay2Direction;

bool DriveLightChange1 = false;
bool DriveLightChange2 = false;


void DriveBayUpdate()
{
  fnDriveBay(DriveBay1State, DriveBay1MotorAPin, DriveBay1MotorBPin, DriveBay1Direction, DriveBay1LimitFrontPin, DriveBay1LimitRearPin, DriveLightChange1, "Down ");
  fnDriveBay(DriveBay2State, DriveBay2MotorAPin, DriveBay2MotorBPin, DriveBay2Direction, DriveBay2LimitFrontPin, DriveBay2LimitRearPin, DriveLightChange2, "Up   ");
}

void fnDriveBay(byte& DriveBayState, byte&  DriveBayMotorAPin, byte&  DriveBayMotorBPin, byte& DriveBayDirection, byte& DriveBayLimitFrontPin, byte& DriveBayLimitRearPin, bool& DriveLightChange, String sTest)
{
  switch (DriveBayState)
  {
    case 0 : //Set
      // Serial.println(String(sTest)+" "+"0");
      /*
      digitalWrite(DriveBayMotorAPin, 0);
      digitalWrite(DriveBayMotorBPin, 0);
      */
      
        analogWrite(DriveBayMotorAPin, 0);
        analogWrite(DriveBayMotorBPin, 0);
      
      // Serial.println(String(sTest)+" "+"1");
      DriveBayState = 1;
      break;
    case 1 :
      // Serial.println(String(sTest)+" "+"1 : "+String(DriveBayDirection));
      break;
    case 2 :
      switch (DriveBayDirection)
      {
        case 0 :
          //Serial.println(String(sTest)+" "+"2 0");
          /*
          digitalWrite(DriveBayMotorAPin, 0);
          digitalWrite(DriveBayMotorBPin, DriveBaySpeed);
          */
          
            analogWrite(DriveBayMotorAPin, 0);
            analogWrite(DriveBayMotorBPin, DriveBaySpeed);
          

          break;
        case 1 :
          //Serial.println(String(sTest)+" "+"2 1");
          /*
          digitalWrite(DriveBayMotorAPin, DriveBaySpeed);
          digitalWrite(DriveBayMotorBPin, 0);
          */
          
            analogWrite(DriveBayMotorAPin, DriveBaySpeed);
            analogWrite(DriveBayMotorBPin, 0);
          
          break;
      }
      DriveBayState = 3;
      break;
    case 3 :

      switch (DriveBayDirection)
      {
        case 0 :
          // Serial.println(String(sTest)+" "+"3 0");
          if (digitalRead(DriveBayLimitRearPin) == 0)
          {
            //Serial.println(String(sTest)+" "+"3 1 4");
            DriveBayState = 4;
          }
          break;
        case 1 :
          //Serial.println(String(sTest)+" "+"3 1");
          if (digitalRead(DriveBayLimitFrontPin) == 0)
          {
            //  Serial.println(String(sTest)+" "+"3 0 4");
            DriveBayState = 4;
          }
          break;
      }
      break;
    case 4 :
      //Serial.println(String(sTest)+" "+"4");
      /*
        Serial.flush();
        Serial.println(sTest+" "+String(DriveBayDirection)+" End");
      */
      DriveLightChange = true;

      DriveBayState = 0;
      break;

  }
}


// ==================================
//              Button
// ==================================

long lBtnBayInterval = 100;

byte bBtnBay1State;
byte bBtnBay2State;

byte bBtnBay1OldRead;
byte bBtnBay2OldRead;

long lBtnBay1Current;
long lBtnBay2Current;

void BtnBayUpdate()
{
  BtnBayRead();
  BtnBayWrite();
}

void BtnBayRead()
{
  fnBtnBayRead(bBtnBay1State, bBtnBay1Pin, bBtnBay1OldRead);
  fnBtnBayRead(bBtnBay2State, bBtnBay2Pin, bBtnBay2OldRead);
}

void BtnBayWrite()
{
  fnBtnBayWrite(bBtnBay1State, "Button Bay 1 Pressed", lBtnBay1Current, DriveBay1State, DriveBay1Direction);
  fnBtnBayWrite(bBtnBay2State, "Button Bay 2 Pressed", lBtnBay2Current, DriveBay2State, DriveBay2Direction);
}

byte bBtnBayRead;
void fnBtnBayRead(byte& BtnBayState, byte& bBtnBayPin, byte&bBtnBayOldRead)
{
  if (BtnBayState == 0)
  {
    bBtnBayRead  = digitalRead(bBtnBayPin);

    if (bBtnBayRead == LOW && bBtnBayOldRead == HIGH)
    {
      BtnBayState = 1;
    }
    bBtnBayOldRead = bBtnBayRead;
  }
}

void fnBtnBayWrite(byte& BtnBayState, String sPrint, long& lBtnBayCurrent, byte& DriveBayState, byte& DriveBayDirection)
{
  switch (BtnBayState)
  {
    case 1 :
      Serial.flush();
      Serial.println(sPrint);

      switch (DriveBayDirection)
      {
        case 0 :
          DriveBayDirection = 1;
          break;
        case 1 :
          DriveBayDirection = 0;
          break;
      }
      //==========
      SerialWrite();
      //==========
      DriveBayState = 2;

      BtnBayState = 2;
      lBtnBayCurrent = millisCurrent;
      break;

    case 2 :
      if (millisCurrent - lBtnBayCurrent >= lBtnBayInterval)
      {
        BtnBayState = 0;
      }
      break;
  }
}


// ==================================
//              Serial
// ==================================

void SerialUpdate()
{
  SerialRead();
}

byte bByteRead;
void SerialRead()
{
  if (Serial.available() > 0) {
    bByteRead = (byte)Serial.read();
    if (bByteRead != 10 && bByteRead != 13 && bByteRead != ';')
    {
      Serial.println("input : " + String(bByteRead));
      switch ((byte)bByteRead)
      {
        case 96 :
          DriveBay1Direction = 0;
          DriveBay2Direction = 0;

          DriveBay1State = 2;
          DriveBay2State = 2;
          break;
        case 97 :
          DriveBay1Direction = 1;
          DriveBay2Direction = 0;

          DriveBay1State = 2;
          DriveBay2State = 2;
          break;
        case 98 :
          DriveBay1Direction = 0;
          DriveBay2Direction = 1;

          DriveBay1State = 2;
          DriveBay2State = 2;
          break;
        case 99 :
          DriveBay1Direction = 1;
          DriveBay2Direction = 1;

          DriveBay1State = 2;
          DriveBay2State = 2;
          break;
      }
      bByteRead = 0;
      SerialWrite();
    }
  }

}

byte bSendChar = 96;
void SerialWrite()
{
  bSendChar = 96 + (DriveBay2Direction * 2) + (DriveBay1Direction * 1);

  Serial.flush();
  Serial.print("Char : ");
  Serial.println(char(bSendChar));
}

byte bDriveLight1State = 0;
byte bDriveLight2State = 0;

long lDriveLightInterval = 2000;

long lDriveLight1Current;
long lDriveLight2Current;

void fnDriveLight(Adafruit_NeoPixel& Pix, byte bPix, byte& DriveBayDirection, bool& DriveLightChange, byte& bDriveLightState, long& lDriveLightCurrent)
{
  if (DriveLightChange)
  {
   // Serial.println(" L  True "+String(bPix));
    DriveLightChange = false;
    bDriveLightState = 1;
  }

  switch (bDriveLightState)
  {
    case 0:
      break;
    case 1:
    
    //Serial.println("L 1 "+String(bPix));
      lDriveLightCurrent = millisCurrent;
      bDriveLightState = 2;
      break;
    case 2:
    
    //Serial.println("L 2 "+String(bPix));
      if (millisCurrent - lDriveLightCurrent >= lDriveLightInterval)
      {
        bDriveLightState = 3;
      }
      break;
    case 3:

    //Serial.println("L 3 "+String(bPix));
      for (int i = 0; i < 79; i++)
      {
        if (DriveBayDirection == 0)
        {
          Pix.setPixelColor(i, Pix.Color(200, 0, 200));
        } else if (DriveBayDirection == 1) {
          Pix.setPixelColor(i, Pix.Color(0, 0, 0));
        }
      }
      Pix.show();
      
    //Serial.println("L END "+String(bPix));
      /*
        switch(bPix)
        {
        case 1 :

        break;
        case 2 :
          for (int i = 0; i < 79; i++)
        {
        if (DriveBayDirection == 0)
        {
          pixels2.setPixelColor(i, pixels2.Color(200, 0, 200));
        } else if (DriveBayDirection == 1) {
          pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        }
        }
        pixels2.show();


        break;
        }
      */
      bDriveLightState = 0;
      break;
  }

}


void DriveLight()
{
  fnDriveLight(pixels1, 1, DriveBay1Direction, DriveLightChange1, bDriveLight1State, lDriveLight1Current);
  fnDriveLight(pixels2, 2, DriveBay2Direction, DriveLightChange2, bDriveLight2State, lDriveLight2Current);

  /*
    if (DriveBay1Direction == 1)
    {
    isLight1 = false;
    isChange1 = true;
    }

    if (DriveBay1Direction == 0) {
    isLight1 = true;
    isChange1 = true;
    }

    if (DriveBay2Direction == 1)
    {
    isLight2 = false;
    isChange2 = true;
    }

    if (DriveBay2Direction == 0) {
    isLight2 = true;
    isChange2 = true;
    }

    if (isChange1)
    {
    isChange1 = false;
    for (int i = 0; i < 79; i++)
    {

      if (isLight1) {
        pixels1.setPixelColor(i, pixels1.Color(200, 0, 200));
      } else {
        pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
      }

    }
    pixels1.show();
    }


    if (isChange2)
    {
    isChange2 = false;
    for (int i = 0; i < 79; i++)
    {

      if (isLight2) {
        pixels2.setPixelColor(i, pixels2.Color(200, 0, 200));
      } else {
        pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
      }

    }
    pixels2.show();
    }
  */
  /*
    else{
    for (int i = 40;i<79;i++)
    pixels.setPixelColor(i,pixels.Color(200,0,200));
    pixels.show();
    }
  */

  /*|| DriveBay2Direction == 1)
    {
    for (int i = 0;i<79;i++)
    pixels.setPixelColor(i,pixels.Color(0,0,0));
    pixels.show();
    }else{
    for (int i = 0;i<79;i++)
    pixels.setPixelColor(i,pixels.Color(200,0,200));
    pixels.show();
    }
  */

  /*
    while(digitalRead(DriveBay1LimitRearPin))
    {
    for (int i = 0;i<79;i++)
    pixels.setPixelColor(i,pixels.Color(0,0,0));
    pixels.show();
    }
    while(!digitalRead(DriveBay1LimitRearPin))
    {
    for (int i = 0;i<79;i++)
    pixels.setPixelColor(i,pixels.Color(200,0,200));
    pixels.show();
    }
  */
}
