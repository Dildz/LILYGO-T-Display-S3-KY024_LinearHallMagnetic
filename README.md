LILYGO T-Display-S3 KY-024 Linear Magnetic Hall Sensor Module

This code reads the digital and analog signals from the KY-024 module and displays the readings on the built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.

The module provides both digital and analog outputs:
 - Digital output: HIGH when no magnet is detected, LOW when a magnet is detected.
 - Analog output: Voltage is proportional to the strength of the magnetic field.

Sensitivity Adjustment:
 - The KY-024 module includes a potentiometer to adjust the sensitivity of the digital output.
 - Turn the potentiometer clockwise to increase sensitivity (detect weaker magnetic fields).
 - Turn the potentiometer counterclockwise to decrease sensitivity (detect stronger magnetic fields).
 - Adjust the potentiometer until the digital output behaves as expected for your application.

Debouncing:
 - To prevent flickering of the digital state when the magnet is near the detection threshold, a debounce mechanism is implemented.
 - The digital state only changes after the signal has been stable for a specified period (e.g., 50ms).

Independent Updates:
 - The digital state and analog value updates are handled independently.
 - This ensures that the screen only redraws the specific element that has changed, improving performance and readability.

Wiring:
 - KY-024 VCC -> 3.3V
 - KY-024 GND -> GND
 - KY-024 D0  -> GPIO1 (PIN 1)
 - KY-024 A0  -> AIO01 (PIN 2)

KY-024 Specifications:
 - Operating Voltage: 3.3V to 5.5V
 - Operating Temperature: -40°C to 85°C
 - Output: Digital & Analog
