# MorseCodeX

MorseCodeX is an Arduino-based project that allows users to input text using a T9 keypad, convert it to Morse code, and transmit it via an LED and buzzer. It also supports decoding Morse code input through a button, displaying the results on an LCD screen.

## Features
- **T9 Text Input**: Enter text using a 4x4 keypad with T9-style input (similar to old mobile phones).
- **Morse Code Transmission**: Converts text to Morse code and outputs it via an LED and buzzer.
- **Morse Code Decoding**: Decodes Morse code entered via a button and displays the result on the LCD.
- **LCD Display**: Shows the current input, Morse code symbols, and decoded messages on a 16x2 I2C LCD.
- **Interactive Controls**:
  - `*` for backspace (delete last character).
  - `#` to clear the input.
  - `D` to send the message as Morse code.
  - `C` to display the decoded Morse message.

## Hardware Requirements
- Arduino board (e.g., Arduino Uno)
- 16x2 LCD with I2C interface (address 0x20)
- 4x4 matrix keypad
- Push button (for Morse input)
- LED (for Morse output and input feedback)
- Buzzer (for Morse audio output)
- Letter completion LED (indicates letter encoding/decoding)
- Necessary resistors and wiring

## Pin Configuration
- **LCD**: Connected via I2C (SDA, SCL pins depend on the Arduino model).
- **Keypad**:
  - Rows: Pins 9, 8, 7, 6
  - Columns: Pins 5, 4, 3, 2
- **LED**: Pin 10 (for Morse output/input feedback)
- **Letter LED**: Pin 13 (for letter completion feedback)
- **Buzzer**: Pin 12
- **Button**: Pin 11 (with internal pull-up resistor)

## Libraries Required
- **Wire.h**: For I2C communication with the LCD.
- **LiquidCrystal_I2C.h**: For controlling the I2C LCD.
- **Keypad.h**: For handling keypad input.

Install these libraries via the Arduino IDE Library Manager or manually.

## Setup Instructions
1. Connect the hardware according to the pin configuration.
2. Install the required Arduino libraries.
3. Upload the provided Arduino sketch to your board.
4. Power on the Arduino; the LCD will display "MorseCodeX Ready" for 1.5 seconds before clearing.

## Usage
- **T9 Input**:
  - Press number keys (0-9) to cycle through characters (e.g., key `2` cycles through `A`, `B`, `C`, `2`).
  - Wait 600ms to commit a character, or press another key to cycle.
  - Use `*` to delete the last character.
  - Use `#` to clear the entire message.
- **Send Morse**:
  - Press `D` to transmit the current message as Morse code via the LED and buzzer.
  - The letter LED lights up for 500ms after each letter is sent.
- **Morse Input**:
  - Press the button to input dots (short press, <300ms) or dashes (long press, ≥300ms).
  - Pause for 1 second to commit a letter, or 2 seconds to add a space.
  - The LCD shows the current Morse symbol and decoded message.
  - The letter LED lights up for 500ms when a letter is decoded.
- **View Decoded Message**:
  - Press `C` to display the decoded Morse message on the LCD for 2 seconds.

## Timing Parameters
- **T9 Timeout**: 600ms to commit a character.
- **Dot Threshold**: 300ms (short press = dot, long press = dash).
- **Letter Pause**: 1000ms to commit a Morse letter.
- **Word Pause**: 2000ms to insert a space in Morse input.

## Notes
- The T9 map supports alphanumeric characters (A-Z, 0-9, space).
- Morse code supports A-Z, 0-9, and space (represented as `/`).
- Invalid Morse sequences are decoded as `?`.
- The LCD displays up to 16 characters per line; longer messages scroll.

## Example
1. Enter "SOS" using the keypad:
   - Press `7` three times for `S` (maps to `PQRS`).
   - Press `6` three times for `O` (maps to `MNO`).
   - Press `7` three times again for `S`.
   - Press `D` to send as Morse: `... --- ...` (LED flashes, buzzer sounds).
2. Input Morse via button:
   - Short press (dot), short press (dot), short press (dot) for `...` (decoded as `S`).
   - The LCD updates with the symbol and decoded letter.

## License
This project is open-source and available under the MIT License.