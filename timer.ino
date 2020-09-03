/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

#include <Arduino.h>
#include <TM1637Display.h>

// Pins for 4 digit display
#define CLK 2
#define DIO 3

// constants won't change. They're used here to set pin numbers:
const int timerSelectButton = 13;     // the number of the pushbutton pin
const int timerStartButton = 12;
const int ledPinTimer1 = 11;      // the number of the LED pin
//const int ledPinTimer2 = 10;
const int ledPinAlarm = 9;
const int ledTimer = 4;
const int durationTimer1 = 35;    // timer 1 in seconds
const int durationTimer2 = 45;    // timer 2 in seconds

// variables will change:
int timerSelectButtonState = 0;         // variable for reading the pushbutton status
int timerStartButtonState = 0;
int timerRunning = 0;
int beeping = 0;

int currentTimer = 0; // 0 = 1 seconds, 1 = 3 seconds, 2 = 5 seconds
int timerDuration = durationTimer1; // default timer duration in milliseconds after boot
long previousMillis = 0; 

TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(9600);
  Serial.println(durationTimer1);
  // initialize the LED pin as an output:
  pinMode(ledPinTimer1, OUTPUT);
  //pinMode(ledPinTimer2, OUTPUT);
  pinMode(ledPinAlarm, OUTPUT);
  pinMode(ledTimer, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(timerSelectButton, INPUT);
  pinMode(timerStartButton, INPUT);

  // Set display brightness to maximum
  display.setBrightness(0x0f);

  // Blank display
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  display.showNumberDec(8008);

}

void loop() {

  unsigned long currentMillis = millis(); 
  
  // read the state of the pushbutton value:
  timerSelectButtonState = digitalRead(timerSelectButton);
  timerStartButtonState = digitalRead(timerStartButton);

  // check if the pushbutton to select time is pressed. If it is, the buttonState is HIGH:
  // then update the currently selected time and show it on the display
  if (timerSelectButtonState == HIGH) {
    
    if (currentTimer == 0) {
      timerDuration = durationTimer2; 
      display.showNumberDec(timerDuration);
      currentTimer = 1;
      timerRunning = 0;
      digitalWrite(ledPinTimer1, HIGH);
      delay(200);
      digitalWrite(ledPinTimer1, LOW);
    } else if (currentTimer == 1){  
      timerDuration = durationTimer1;
      display.showNumberDec(timerDuration); 
      currentTimer = 0;
      timerRunning = 0;
      digitalWrite(ledPinTimer1, HIGH);
      delay(200);
      digitalWrite(ledPinTimer1, LOW);
    } 
  }

  // Check if Button start timer has been pressed
  if (timerStartButtonState == HIGH){
    timerRunning = 1; // set timer flag to 1
    digitalWrite(ledTimer, HIGH); // light up the LED
    tone(ledPinAlarm,3000,100);
    previousMillis = currentMillis; // adjust timing variable
  }
  
  // if timer flag is set to 1, meaning the timer button has been pressed
  // start updating the display with the time left
  if (timerRunning == 1) {  
    int timer = (currentMillis - previousMillis)/1000;  // just to make the next step more readable
    display.showNumberDec(timerDuration-timer);  // display the remaining time on the display
    
    if (timerDuration-timer == 3 && beeping == 0) { 
      tone(ledPinAlarm,4000,300);
      beeping = 1;
    }
    else if (timerDuration-timer == 2 && beeping == 1){
      tone(ledPinAlarm,4000,300);
      beeping = 2;
      }
    else if (timerDuration-timer == 1 && beeping == 2){
      tone(ledPinAlarm,4000,1000);
      beeping = 0;
      }
    }
 

  // Once the timer has reached 0 reset timer flag to 0 and switch the LED off
  if ((currentMillis - previousMillis)/1000 > timerDuration){
    previousMillis = currentMillis;
    timerRunning = 0;
    //digitalWrite(ledPinAlarm, HIGH); //replace with tone(ledPinAlarm,1000,1000) eventually
    digitalWrite(ledTimer, LOW);
    display.showNumberDec(timerDuration);  //reset display to the selected timer
  }
}
