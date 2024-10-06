#include <Arduino.h>
#include <iomanip>
#include <HardwareSerial.h>
// put function declarations here:

// Define pins for motor control
const int motorPinRaise = 37; // Motor pin for raising the seat
const int motorPinLower = 38; // Motor pin for lowering the seat

// State variables
bool seatIsRaised = false; // To check if the seat is already raised
bool motorRunning = false; // To track if the motor is running

// Input control values
bool raiseSeat = false; // Set this to true to raise the seat
bool lowerSeat = false; // Set this to true to lower the seat
bool LidIsRaised = false;
unsigned long motorRunTime = 2000; // Time to run motor in ms (example: 2 seconds)
unsigned long motorStartTime = 0;  // Time when motor starts
using namespace std;
HardwareSerial Serialport(1);

byte buffer_Rtx[256];
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(250000);
  pinMode(motorPinRaise, OUTPUT);
  pinMode(motorPinLower, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1, INPUT);
  pinMode(42,INPUT);
  analogWrite(2, 187);
  Serialport.begin(256000, SERIAL_7N1, 16, 17);
}
bool pir_right()
{
  if (digitalRead(1))
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool pir_left()
{
  if (digitalRead(42))
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool HumanSense()
{
  memset(buffer_Rtx, 0, sizeof(buffer_Rtx));
  int index = 0;
  while (Serialport.available() && index < 24)
  {
    Serialport.read(buffer_Rtx, 256);

    Serial.print("    ");
    index++;
  }
  for (int i = 0; i < 29; i++)
  {
    Serial.print(buffer_Rtx[i], HEX);
    Serial.print("    ");
  }
  Serial.println("    ");
  if ((buffer_Rtx[8] ^ 0) == 0)
  {
    Serial.print("person gone");
    return true;
  }
  else
  {
    Serial.print("person  here");
    return false;
  }
}
void stopMotor()
{
  digitalWrite(motorPinRaise, LOW);
  digitalWrite(motorPinLower, LOW);
  Serial.println("Motor stopped");
}

void runMotorRaise()
{
  digitalWrite(motorPinRaise, HIGH);
  digitalWrite(motorPinLower, LOW);
  Serial.println("Raising the seat");
}

// Function to lower the seat
void runMotorLower()
{
  digitalWrite(motorPinRaise, LOW);
  digitalWrite(motorPinLower, HIGH);
  Serial.println("Lowering the seat");
}
int lid = 870;
int fuck = 2910;
void loop()
{
  while (Serial.available() == 0)
  {
    int userInput = Serial.parseInt();

    if (!LidIsRaised && pir_right() == true)
    {
      runMotorRaise();
      delay(lid);
      stopMotor();
      LidIsRaised = true; // Lock the state to raised
    }

    // If the seat is raised and the lowerSeat command is issued, lower the seat
    else if (!seatIsRaised && HumanSense() == true && LidIsRaised)
    {
      runMotorLower();
      delay(lid);
      stopMotor();
      LidIsRaised = false; // Unlock state
    }

    else if (!LidIsRaised && (pir_left() == true) && !seatIsRaised)
    {
      runMotorRaise();
      delay(lid);
      stopMotor();
      digitalWrite(48,HIGH);
      delay(fuck);
      digitalWrite(48,LOW);
      LidIsRaised = true;
      seatIsRaised = true;
    }
    else if (LidIsRaised && HumanSense() == true && seatIsRaised)
    {

      digitalWrite(19,HIGH);
      delay(fuck);
      digitalWrite(19,LOW);
      runMotorLower();
      delay(lid);
      stopMotor();
      LidIsRaised = false;
      seatIsRaised = false;
    }
  }
}
// while (Serial.available() == 0)
// {
//   int userInput = Serial.parseInt();

//   if (userInput == 1)
//   {
//     digitalWrite(37, HIGH);
//     delay(lid);
//     digitalWrite(37, LOW);
//   }
//   if (userInput == 2)
//   {
//     digitalWrite(38, HIGH);
//     delay(lid);
//     digitalWrite(38, LOW);
//   }
//   if (userInput == 3)
//   {
//     digitalWrite(19, HIGH);
//     delay(fuck);
//     digitalWrite(19, LOW);
//   }
//   if (userInput == 4)
//   {
//     digitalWrite(48, HIGH);
//     delay(2760);
//     digitalWrite(48, LOW);
//   }
// }
// }
