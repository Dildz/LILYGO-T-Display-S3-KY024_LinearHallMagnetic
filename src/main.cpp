/*******************************************************************************************************
* LILYGO T-Display-S3 KY-024 Linear Magnetic Hall Sensor Module
*
* This code reads the digital and analog signals from the KY-024 module and displays the readings on the
* built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
*
* The module provides both digital and analog outputs:
*  - Digital output: HIGH when no magnet is detected, LOW when a magnet is detected.
*  - Analog output: Voltage is proportional to the strength of the magnetic field.
*
* Sensitivity Adjustment:
*  The KY-024 module includes a potentiometer to adjust the sensitivity of the digital output.
*  - Turn the potentiometer clockwise to increase sensitivity (detect weaker magnetic fields).
*  - Turn the potentiometer counterclockwise to decrease sensitivity (detect stronger magnetic fields).
*  - Adjust the potentiometer until the digital output behaves as expected for your application.
*
* Debouncing:
*  To prevent flickering of the digital state when the magnet is near the detection threshold,
*  a debounce mechanism is implemented. The digital state only changes after the signal has been
*  stable for a specified period (e.g., 50ms).
*
* Independent Updates:
*  The digital state and analog value updates are handled independently. This ensures that the
*  screen only redraws the specific element that has changed, improving performance and readability.
*
* Wiring:
*  - KY-024 VCC -> 3.3V
*  - KY-024 GND -> GND
*  - KY-024 D0  -> GPIO1 (PIN 1)
*  - KY-024 A0  -> AIO01 (PIN 2)
*
* KY-024 Specifications:
*  - Operating Voltage: 3.3V to 5.5V
*  - Operating Temperature: -40°C to 85°C
*  - Output: Digital & Analog
*******************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// Pin definitions
const int digitalPin = 1;   // GPIO1 for digital signal
const int analogPin = 2;    // AIO01 for analog signal (A0)

// Global variables
int digitalState = HIGH;                      // current digital state (active low, so default is HIGH)
int lastDigitalState = HIGH;                  // previous digital state
int analogValue = 0;                          // current analog value
int lastAnalogValue = 0;                      // previous analog value
bool redrawDigital = false;                   // flag to redraw digital state
bool redrawAnalog = false;                    // flag to redraw analog value
unsigned long lastAnalogReadTime = 0;         // timestamp for last analog read
const unsigned long analogReadInterval = 100; // ms read period for analog signal
unsigned long lastDebounceTime = 0;           // timestamp for debounce
const unsigned long debounceInterval = 50;    // debounce interval in ms


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("---------------------------");
  tft.println("KY024 Hall Sensor Module");
  tft.println("---------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for digital state label
  tft.print("Digital State:");
  tft.setCursor(0, 120); // position for analog value label
  tft.print("Analog Value:");
}

// Function to update digital state on the TFT screen
void updateDigitalState() {
  tft.setCursor(0, 90);                  // position for digital state value (below the label)
  tft.print("                        "); // clear previous value (24 spaces = entire line)
  tft.setCursor(0, 90);                  // reset cursor
  tft.print(digitalState == HIGH ? "MAGNET DETECTED" : "NO MAGNET");
}

// Function to update analog value on the TFT screen
void updateAnalogValue() {
  tft.setCursor(0, 140);                 // position for analog value (below the label)
  tft.print("                        "); // clear previous value (24 spaces = entire line)
  tft.setCursor(0, 140);                 // reset cursor
  tft.print(analogValue);
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// SETUP
void setup() {
  // Set pin modes
  pinMode(digitalPin, INPUT); // pull-up resistor not needed

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Initial display update
  updateDigitalState();
  updateAnalogValue();
}


// MAIN LOOP
void loop() {
  // Read the current digital state
  int digitalReading = digitalRead(digitalPin);

  // Debounce the digital signal
  if (digitalReading != lastDigitalState) {
    lastDebounceTime = millis(); // reset the debounce timer
  }

  // Check if the digital state has been stable for the debounce interval
  if ((millis() - lastDebounceTime) > debounceInterval) {
    if (digitalReading != digitalState) {
      digitalState = digitalReading;     // update the digital state
      redrawDigital = true;              // flag to update digital state display
    }
  }

  lastDigitalState = digitalReading; // save the current reading

  // Update digital state display if necessary
  if (redrawDigital) {
    updateDigitalState();
    redrawDigital = false; // reset the flag
  }

  // Read the analog value every 100ms
  if (millis() - lastAnalogReadTime >= analogReadInterval) {
    int analogReading = analogRead(analogPin);

    // If the value is less or equal to 50, force it to 0
    if (analogReading <= 50) {
      analogReading = 0;
    }

    // Check if the analog value has changed significantly
    if (abs(analogReading - analogValue) > 10) { // difference greater than 10 before updating
      analogValue = analogReading; // update the analog value
      redrawAnalog = true;         // flag to update analog value display
    }

    lastAnalogReadTime = millis(); // update the timestamp
  }

  // Update analog value display if necessary
  if (redrawAnalog) {
    updateAnalogValue();
    redrawAnalog = false; // reset the flag
  }

  delay(1); // slow down the loop to save CPU cycles
}