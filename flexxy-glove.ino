#include "BluetoothSerial.h"
#include "math.h"

#define LED_BUILTIN 2

BluetoothSerial SerialBT;

int fingerPins[] = {14, 27, 26, 33, 34};
int powerPins[] = {15, 4, 5, 19, 23};
int inputsStart[] = {0, 0, 0, 0, 0};
int inputsEnd[] = {4095, 4095, 4095, 4095, 4095};
int inputs[] = {0, 0, 0, 0, 0};
int rawInputs[] = {0, 0, 0, 0, 0};
String gesture = "";
enum handValues
{
  MAX = 4095,
  SEMICLOSED = 3072,
  MID = 2048,
  SEMIOPEN = 1024,
  MIN = 0,
};




void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize Bluetooth Serial
  SerialBT.begin("HandController"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  pinMode(LED_BUILTIN, OUTPUT);

  for(int pwr : powerPins)
  {
    pinMode(pwr, OUTPUT);
  }

  fullCalibrate();
}



void loop()
{
  readFingers();
  readGesture();

  // Print inputs to serial monitor
  Serial.print("a");
  Serial.print(rawInputs[0]);
  Serial.print("b");
  Serial.print(rawInputs[1]);
  Serial.print("c");
  Serial.print(rawInputs[2]);
  Serial.print("d");
  Serial.print(rawInputs[3]);
  Serial.print("e");
  Serial.print(rawInputs[4]);
  Serial.print("G");
  Serial.print(gesture);
  Serial.println();

  // Send same data over Bluetooth
  SerialBT.print("a");
  SerialBT.print(rawInputs[0]);
  SerialBT.print("b");
  SerialBT.print(rawInputs[1]);
  SerialBT.print("c");
  SerialBT.print(rawInputs[2]);
  SerialBT.print("d");
  SerialBT.print(rawInputs[3]);
  SerialBT.print("e");
  SerialBT.print(rawInputs[4]);
  SerialBT.print("G");
  SerialBT.print(gesture);
  SerialBT.println();

  delay(25);
}






void fullCalibrate()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  // Calibrate the start range
  for (int i = 0; i < 5; i++)
  {
    inputsStart[i] = readFinger(i);
  }

  // Blink LED for feedback
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(75);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(75);
  }

  delay(500);

  // Calibrate the end range
  for (int i = 0; i < 5; i++)
  {
    inputsEnd[i] = readFinger(i);
  }

  digitalWrite(LED_BUILTIN, LOW);
}


int readFinger(int index)
{
  digitalWrite(powerPins[index], HIGH);
  int val = linearize(analogRead(fingerPins[index]));
  digitalWrite(powerPins[index], LOW);
  return val;

}



void readFingers()
{

  for (int i = 0; i < 5; i++)
  {
    inputs[i] = constrain(map(readFinger(i), inputsStart[i], inputsEnd[i], 0, 4095), 0, 4095);
    rawInputs[i] = readFinger(i);
  }
}

void readGesture()
{
  if (inputs[0] > handValues::SEMIOPEN &&
      inputs[1] > handValues::SEMICLOSED &&
      inputs[2] > handValues::SEMICLOSED &&
      inputs[3] > handValues::SEMICLOSED &&
      inputs[4] > handValues::SEMICLOSED)
  {
    gesture = "Fist";
  }

  // Open
  else if (inputs[0] < handValues::SEMIOPEN &&
           inputs[1] < handValues::SEMIOPEN &&
           inputs[2] < handValues::SEMIOPEN &&
           inputs[3] < handValues::SEMIOPEN &&
           inputs[4] < handValues::SEMIOPEN)
  {
    gesture = "Open";
  }

  // Point
  else if (inputs[0] > handValues::MID &&
           inputs[1] < handValues::SEMIOPEN &&
           inputs[2] > handValues::SEMICLOSED &&
           inputs[3] > handValues::SEMICLOSED &&
           inputs[4] > handValues::SEMICLOSED)
  {
    gesture = "Point";
  }

  // Peace
  else if (inputs[0] > handValues::MID &&
           inputs[1] < handValues::SEMIOPEN &&
           inputs[2] < handValues::SEMIOPEN &&
           inputs[3] > handValues::MID &&
           inputs[4] > handValues::SEMICLOSED)
  {
    gesture = "Peace";
  }

  // Bird
  else if (/*inputs[0] > handValues::SEMIOPEN &&*/ // Irrelevant
            inputs[1] > handValues::MID &&
            inputs[2] < handValues::MID &&
            inputs[3] > handValues::SEMIOPEN &&
            inputs[4] > handValues::MID)
  {
    gesture = "Bird";
  }

  // Tea
  else if (inputs[0] > 1500 &&
           inputs[1] > 2600 &&
           inputs[2] > 2600 &&
           inputs[3] > 2600 &&
           inputs[4] < 1500)
  {
    gesture = "Tea";
  }

  // Thumbs Up
  else if (inputs[0] < handValues::SEMIOPEN &&
           inputs[1] > handValues::MID &&
           inputs[2] > handValues::SEMICLOSED &&
           inputs[3] > handValues::SEMICLOSED &&
           inputs[4] > handValues::SEMICLOSED)
  {
    gesture = "Thumbs Up";
    
  }

  // Gun
  else if (inputs[0] < handValues::SEMIOPEN &&
           inputs[1] < handValues::SEMIOPEN &&
           inputs[2] > handValues::MID &&
           inputs[3] > handValues::MID &&
           inputs[4] > handValues::MID)
  {
    gesture = "Gun";
  }

  // Phone
  else if (inputs[0] < handValues::SEMIOPEN &&
           inputs[1] > handValues::MID &&
           inputs[2] > handValues::MID &&
           inputs[3] > handValues::MID &&
           inputs[4] < handValues::SEMIOPEN)
  {
    gesture = "Phone";
  }

  // Rock
  else if (inputs[0] < handValues::MID &&
           inputs[1] < handValues::MID &&
           inputs[2] > 2000 &&
           inputs[3] > 2000 &&
           inputs[4] < handValues::MID)
  {
    gesture = "Rock";
  }

  // Tuo
  else if (inputs[0] > handValues::MID &&
           inputs[1] > handValues::MID &&
           inputs[2] < handValues::SEMIOPEN &&
           inputs[3] < handValues::SEMIOPEN &&
           inputs[4] > handValues::MID)
  {
    gesture = "Tuuo";
  }

  else
  {
    gesture = "None";
  }
}

int linearize(int x)
{
 return 4095.0f * ( powf(1.0f / (x + 1.0f), 1.29f) - powf(1.0f / (4096.0f), 1.29f) ) / (1.0f - powf(1.0f / (4096.0f), 1.29f));
}