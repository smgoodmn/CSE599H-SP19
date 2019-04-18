// To implement the paperclip capacitive button, I used the Capacitive Sensing library found on the Arduino Playground.
// Source: https://playground.arduino.cc/Main/CapacitiveSensor/
// Created by: Paul Badger

// Capacitive button code (namely: checkButton() ) was also aided by the Arduino Paper Controller project on the Arduino Project Hub.
// Source: https://create.arduino.cc/projecthub/millerman4487/arduino-paper-controller-buttons-slider-d2ae64
// Created by: millerman4487

#include <CapacitiveSensor.h>

// Working in HSL space, this controls the maximum "Lightness" value of the LED. Photoresistor will vary Lightness from 0.0 to this value. 50.0 is max for full color, any higher and the color starts turning white, any lower and the LED loses brightness.
const float LIGHTNESS_CUTOFF = 50.0;

// If true, this overrides the photoresistor and makes the "Lightness" value of the LED equal to the LIGHTNESS_CUTOFF value.
const bool FULL_LIGHTNESS_TESTING = false;

// Controls the min. amount of capacitance needed to sense clip button is "pressed".
const long CAPACITANCE_MINIMUM = 10000;


// ************ Defining pins used by Arduino. ************
const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

const int PHOTO_PIN = A0;
const int LEVER_PIN = A3;

CapacitiveSensor clipButton = CapacitiveSensor(4,2);
// ************ End of pin definitions. ************


// Defining universal variables that are used by multiple functions.
int R, G, B, photoReading, leverReading, touchLoop;

// Attempts to fix issues with paperclip button, where colors would shift 2+ hue increments in a single touch.
bool justChanged;



// ********** Main Arduino Functions **********
void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  touchLoop = 0;
  colorHandler();
  justChanged = false;
  
  Serial.begin(9600); 
}
 
void loop()
{
  checkButton();
  colorHandler();
}
// ********** End of Main Arduino functions **********



// ********** Custom Modular Functions for Controlling Input and Output **********

// Checks amount of capacitance in paperclip, moves to next hue value if over threshold set by CAPACITANCE_MINIMUM value.
void checkButton() {
  long capValue = clipButton.capacitiveSensor(1000);
  //Serial.println(total);

  if (capValue > CAPACITANCE_MINIMUM)
    if (touchLoop < 5) {
      touchLoop++;
      justChanged = true;
    } else
      touchLoop = 0;
}


// Gets Hue, Saturation, and Lightness values and sends them to the HSL to RGB conversion stage. Waits a moment if hue has just been changed.
void colorHandler() {
  setColor(60.0*touchLoop, getLeverToSat(), getBrightnessToLight());
  
  if (justChanged) {
    justChanged = false;
    delay(300);
  }
  
  //printCapacitance();
}


// All photoresistor code was heavily aided by lady ada's Adafruit lesson on photocells for Arduino.
// Source: https://learn.adafruit.com/photocells/using-a-photocell

// Gets photoresistor value, then converts it to Lightness value for HSL. Never returns anything above the LIGHTNESS_CUTOFF value.
float getBrightnessToLight() {
  float light;
  
  photoReading = analogRead(PHOTO_PIN);
  light = ((1023 - photoReading)/10.23);

  printBrightness(light);

  if (light < LIGHTNESS_CUTOFF)
    return light;
  else
    return LIGHTNESS_CUTOFF;
}


// Gets potentiometer lever value, then converts it to Saturation value for HSL. Potentiometer can only move within a small range.
float getLeverToSat() {
  float sat = 100.0;
  int leverConvert;
  
  leverReading = analogRead(LEVER_PIN);
  if (leverReading > 555)
    leverConvert = 100;
  else
    leverConvert = map(leverReading, 350, 555, 0, 100);
  sat = leverConvert*1.0;
  
  //printForce(sat);

  return sat;
}


// Sends HSL values through RGB converter and applies them to physical LED.
void setColor(float hue, float sat, float light) {
  float H,S,L;

  if (FULL_LIGHTNESS_TESTING)
    light = LIGHTNESS_CUTOFF;
  
  H = (hue/3.6)/100;
  S = sat/100;
  L = light/100;
  
  HSL_to_RGB(H,S,L);
  printHSL(hue,sat,light);
  
  changeLED(R,G,B);
  //printRGB();
}


// All RGB LED code was heavily aided by Simon Monk's Adafruit lesson on RGB LEDs for Arduino.
// Source: https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/overview

// Applies computed RGB values to physical LED.
void changeLED(int red, int green, int blue) {
  
  //Subtraction required because of anode LED setup.
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);  
}
// ********** End of Modular Functions **********



// ********** Functions used for Debugging. **********

void printRGB() {
  Serial.print(R);
  Serial.print(", ");
  Serial.print(G);
  Serial.print(", ");
  Serial.println(B);
}

void printHSL(float H, float S, float L) {
  Serial.print(H);
  Serial.print(", ");
  Serial.print(S);
  Serial.print(", ");
  Serial.println(L);
}

void printBrightness(float l) {
  Serial.print("Analog photoresistor reading = ");
  Serial.print(photoReading);
  Serial.print(", Output light value = ");
  Serial.println(l);
  Serial.println();
}

void printForce(float s) {
  Serial.print("Potentiometer reading = ");
  Serial.print(leverReading);
  Serial.print(", Output sat value = ");
  Serial.println(s);
  Serial.println();
}

void printCapacitance() {
  long total = clipButton.capacitiveSensor(1000);
  Serial.println(total);
}

// ********** End of debug section. **********




// ********** HSL to RGB Conversion **********
// Pseudocode source: https://www.easyrgb.com/en/math.php
// Implementation into Arduino done by me.

//   H, S and L input range = 0 - 1.0
//   R, G and B output range = 0 - 255

void HSL_to_RGB(float H, float S, float L) {
  float var_1, var_2;
  
  if ( S == 0 ) {
    R = round(L * 255);
    G = round(L * 255);
    B = round(L * 255);
  } else {
    if ( L < 0.5 ) {
      var_2 = L * ( 1 + S );
    } else {
      var_2 = ( L + S ) - ( S * L );
    }

    var_1 = 2 * L - var_2;

    R = round(255 * Hue_to_RGB(var_1, var_2, H + (1.0 / 3)));
    G = round(255 * Hue_to_RGB(var_1, var_2, H));
    B = round(255 * Hue_to_RGB(var_1, var_2, H - (1.0 / 3)));
  }
}

// This function handles Hue calculations.
float Hue_to_RGB(float v1, float v2, float vH ) {
  if (vH < 0)
    vH += 1;
  if(vH > 1)
    vH -= 1;
    
  if ((6 * vH ) < 1)
    return (v1 + (v2 - v1) * 6 * vH);
  if ((2 * vH) < 1)
    return (v2);
  if ((3 * vH ) < 2)
    return (v1 + (v2 - v1) * ((2.0/3) - vH) * 6);
  return (v1);
}

// ********** End of HSL and RGB **********
