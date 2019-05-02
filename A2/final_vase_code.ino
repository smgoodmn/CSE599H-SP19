unsigned long startTime;
unsigned long currentTime;

// Click occurs when the potentiometer pin measures a change greater than the CLICK_BUFFER value.
const int CLICK_BUFFER = 25;

// Defines usable potentiometer range.
const int POT_MIN = 305;
const int POT_MAX = 645;


// ************ Defining pins used by Arduino. ************

const int SPEAKER_OUT = 9;
int POT_IN = A0;
const int RED_PIN = 10;
const int GREEN_PIN = 11;
const int BLUE_PIN = 13;

// ************ End of pin definitions. ************

int oldKnob, R, G, B;

void setup() {
  pinMode(SPEAKER_OUT, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);

  oldKnob = analogRead(POT_IN);
  startTime = millis();

  setColor(0.0, 100.0, 50.0);
}

void loop() {
  currentTime = millis();
  int colorFromKnob;
  int knob = analogRead(POT_IN);

  if (knob < POT_MIN)
    colorFromKnob = 40;
  else if (knob > POT_MAX)
    colorFromKnob = 75;
  else
    colorFromKnob = map(knob, POT_MIN, POT_MAX, 40, 75);
  
  Serial.print(knob);
  Serial.print(", ");
  Serial.println(colorFromKnob);

  if (knob > oldKnob + CLICK_BUFFER) {
    tone(SPEAKER_OUT, 1000, 2);
    oldKnob = knob;
  } else if (knob < oldKnob - CLICK_BUFFER) {
    tone(SPEAKER_OUT, 500, 2);
    oldKnob = knob;
  }

  setColor(17.0, 100.0, colorFromKnob*1.0);
  
  //noTone(SPEAKER_OUT);  

}


// Sends HSL values through RGB converter and applies them to physical LED.
void setColor(float hue, float sat, float light) {
  float H,S,L;
  
  H = (hue/3.6)/100;
  S = sat/100;
  L = light/100;
  
  HSL_to_RGB(H,S,L);
  
  changeLED(R,G,B);
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
