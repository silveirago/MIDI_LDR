
# LDR MIDI Controller

This project allows you to use a Light Dependent Resistor (LDR) as a MIDI controller. The LDR values are smoothed using the ResponsiveAnalogRead library and mapped to MIDI Control Change messages, which can be read by any MIDI-compatible software.

## Watch the full tutorial

[Make a MIDI Controller with the Raspberry Pi Pico: a MIDI Fidget Spinner](https://youtu.be/V3-PUsuYGzs)


## Requirements

- Arduino board with USB MIDI support (e.g., ATmega32U4-based boards like the Arduino Pro Micro, Leonardo, or Raspberry Pi Pico with TinyUSB)
- LDR (Light Dependent Resistor)
- Button (for setting min and max LDR values)
- Arduino IDE with the following libraries installed:
  - Adafruit_TinyUSB_MIDI
  - ResponsiveAnalogRead

## Circuit Diagram

1. Connect the LDR to `A0` and `GND`.
2. Connect a button to pin `2` and `GND`.
3. Connect the Arduino board to your computer.

## Code Explanation

### Libraries

- `Adafruit_TinyUSB_MIDI`: This library allows for USB MIDI communication. [Download the lib here.](https://github.com/silveirago/Adafruit_TinyUSB_MIDI)
- `ResponsiveAnalogRead`: This library provides smoothed analog readings from sensors like LDR.

### Pins

- `LDR_PIN = A0`: The LDR is connected to analog pin A0.
- `BUTTON_PIN = 2`: The button is connected to pin 2, with an internal pull-up resistor.

### Variables

- `minLDRValue`: Tracks the minimum value read by the LDR.
- `maxLDRValue`: Tracks the maximum value read by the LDR.
- `previousMappedValue`: Stores the last MIDI value sent, to avoid redundant messages.

### Setup

- `MIDI.begin()`: Initializes the TinyUSB MIDI library in OMNI mode (listening on all channels).
- The button pin is set as an input with a pull-up resistor.
- The `analogSensor.setSnapMultiplier(0.1)` sets a lower smoothing factor for the LDR readings, making the response faster.

### Main Loop

1. The LDR value is smoothed using the `analogSensor.update()` method.
2. If the value has changed, it checks whether the button is pressed:
   - If pressed, it enters a loop where the LDR values are continuously read and the minimum and maximum values are dynamically updated.
3. Once the button is released, the LDR value is mapped to a range between 0-127 (standard MIDI range) using the `map()` function.
4. A MIDI Control Change message is sent using `MIDI.sendControlChange()`, but only if the mapped value differs from the previous one, to avoid unnecessary MIDI traffic.

### MIDI Message

The mapped LDR value is sent as a MIDI Control Change message (Control Change 1 - Modulation Wheel) on Channel 1.

## Usage

1. Press the button to calibrate the min and max values of the LDR.
2. Once calibrated, move your hand over the LDR to change the MIDI Control Change values.
3. These messages can control various parameters in your DAW or synthesizer, depending on the MIDI mapping.

## License

This code is provided under the MIT License.
