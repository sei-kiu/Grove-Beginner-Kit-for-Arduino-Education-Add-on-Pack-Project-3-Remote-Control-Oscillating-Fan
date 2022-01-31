#include <Arduino.h>

#include <IRremote.h>
#include <Servo.h>

Servo myservo;    // create servo object to control a servo
int RECV_PIN = 2; // set pin 2 as IR control

IRrecv irrecv(RECV_PIN);

decode_results results;

int pos = 90;   // variable to store the servo position
int fanPin = 7; // set D6 as control switch
int fanState = LOW;
int IO = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Enabling IRin"); // remind enabling IR
  irrecv.enableIRIn();             // Start the receiver
  Serial.println("Enabled IRin");
  myservo.attach(3); // attaches the servo on pin 2 to the servo object
  pinMode(fanPin, OUTPUT);
}

//  power_encode 2155829415     left  2155870215  right  2155821255

void loop()
{
  if (irrecv.decode(&results))
  { //checking IR signal
    if (results.value == 16753245)
    { // Power off/on
      IO++;
      if (IO % 2 == 0)
      {
        fanState = HIGH;
        digitalWrite(fanPin, fanState);
        delay(100);
      }
      else
      {
        fanState = LOW;
        digitalWrite(fanPin, fanState);
        delay(100);
      }
    }

    if (results.value == 16761405)
    { // fan swing to left
      for (pos; pos <= 89; pos += 1)
      { // goes from 0 degrees to 90 degrees
        // in steps of 1 degree
        myservo.write(pos); // tell servo to go to position in variable 'pos'

        delay(40); // waits 15ms for the servo to reach the position
        if (irrecv.decode(&results))
        {
          irrecv.resume();
          if (results.value == 16712445)
            break;
        }
      }
    }

    if (results.value == 16712445)
    { // fan swing to right
      for (pos; pos >= 1; pos -= 1)
      {                     // goes from 90 degrees to 0 degrees
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(40);          // waits 15ms for the servo to reach the position

        if (irrecv.decode(&results))
        {
          irrecv.resume();
          if (results.value == 16761405)
            break;
        }
      }
    }
    Serial.println(pos);
    Serial.println(results.value, HEX);
    Serial.println(results.value);
    irrecv.resume(); //recive next intrustion
  }
  delay(100);
}
