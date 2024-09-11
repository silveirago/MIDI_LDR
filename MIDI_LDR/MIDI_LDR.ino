#include <Adafruit_TinyUSB_MIDI.h>
#include <ResponsiveAnalogRead.h>

Adafruit_TinyUSB_MIDI_Class MIDI;

// Constants
const int LDR_PIN = A0;     // Pin where the LDR is connected
const int BUTTON_PIN = 2;   // Pin where the button is connected

// Variables for storing min and max LDR values
int minLDRValue = 1023;
int maxLDRValue = 0;
int previousMappedValue = -1;  // To store the last mapped value

// Create an instance of ResponsiveAnalogRead for the LDR
ResponsiveAnalogRead analogSensor(LDR_PIN, true);  // true enables smoothing

void setup() {
  // Initialize the MIDI Library to listen on all MIDI channels (OMNI mode)
  MIDI.begin();
  
  // Set up the button pin with INPUT_PULLUP mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Set the snap multiplier lower for less smoothing (faster response)
  analogSensor.setSnapMultiplier(0.1);  // Lower value = less smoothing
}

void loop() {
  // Update the analog sensor value (this applies minimal smoothing)
  analogSensor.update();

  // Only continue if the value has changed (avoids unnecessary updates)
  if (analogSensor.hasChanged()) {
    int ldrValue = analogSensor.getValue();  // Get the smoothed value

    // Check if the button is pressed (LOW state, because INPUT_PULLUP is active)
    if (digitalRead(BUTTON_PIN) == LOW) {
      // Stay in a while loop as long as the button is pressed
      while (digitalRead(BUTTON_PIN) == LOW) {
        // Continuously read the smoothed LDR value and update min/max values
        analogSensor.update();  // Update the sensor reading
        ldrValue = analogSensor.getValue();  // Get the new smoothed value

        // Update min and max values dynamically while the button is held
        if (ldrValue < minLDRValue) {
          minLDRValue = ldrValue;
        }
        if (ldrValue > maxLDRValue) {
          maxLDRValue = ldrValue;
        }

        // Add a very short delay to avoid excessive readings in the while loop
        delay(1);
      }
    }

    // Map the smoothed LDR value to a range of 0-127
    int mappedValue = map(ldrValue, minLDRValue, maxLDRValue, 0, 127);

    // Only send MIDI Control Change if the mapped value has changed
    if (mappedValue != previousMappedValue) {
      MIDI.sendControlChange(1, mappedValue, 1);  // Control Change 1 (Modulation Wheel), Channel 1
      previousMappedValue = mappedValue;  // Update the previous value
    }

    // Wait for a short period before reading again
    delay(5);  // Reduced delay for more responsiveness
  }
}
