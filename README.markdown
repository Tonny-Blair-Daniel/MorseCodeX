# MorseCodeX 🛰️

![Arduino](https://img.shields.io/badge/Arduino-%2300878F?logo=Arduino&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Status](https://img.shields.io/badge/status-experimental-yellow)
![C++](https://img.shields.io/badge/-C%2B%2B-00599C?logo=c%2B%2B&logoColor=white)

**MorseCodeX** is an Arduino-based Morse code encoder and decoder. It enables users to input text via a T9-style 4x4 keypad, convert it to Morse code for transmission through an LED and buzzer, and decode Morse code entered via a push button, with results displayed on a 16x2 I2C LCD.

---

## Features

- **T9 Text Input**: Enter alphanumeric characters using a 4x4 keypad, mimicking T9-style input (e.g., key `2` cycles through `A`, `B`, `C`, `2`).
- **Morse Code Transmission**: Converts text to Morse code, outputting via LED flashes and buzzer tones.
- **Morse Code Decoding**: Decodes manual Morse input (dots and dashes) from a push button, displaying results on the LCD.
- **Real-Time LCD Display**: Shows input text, Morse symbols, and decoded messages on a 16x2 I2C LCD.
- **Interactive Controls**:
  - `*`: Delete the last character (backspace).
  - `#`: Clear the entire input.
  - `D`: Transmit the message as Morse code.
  - `C`: Display the decoded Morse message.

---

## Hardware Requirements

| Component            | Quantity | Notes                              |
|---------------------|----------|------------------------------------|
| Arduino Board       | 1        | Uno or Nano recommended            |
| 16x2 I2C LCD        | 1        | I2C address typically 0x20 or 0x27 |
| 4x4 Matrix Keypad   | 1        | Standard membrane keypad           |
| LED (Morse Output)  | 1        | With 220Ω resistor                 |
| LED (Letter Feedback)| 1        | With 220Ω resistor                 |
| Buzzer              | 1        | Active or passive                  |
| Push Button         | 1        | With 10kΩ pull-down resistor       |
| Jumper Wires        | ~20      | For connections                    |
| Breadboard          | 1        | For prototyping                    |

---

## Pin Configuration

| Component          | Arduino Pin(s)            |
|--------------------|---------------------------|
| 16x2 I2C LCD       | SDA (A4), SCL (A5)        |
| 4x4 Keypad (Rows)  | Digital 9, 8, 7, 6        |
| 4x4 Keypad (Cols)  | Digital 5, 4, 3, 2        |
| LED (Morse Output) | Digital 10 (220Ω resistor) |
| LED (Letter Feedback)| Digital 13 (220Ω resistor) |
| Buzzer             | Digital 12                |
| Push Button        | Digital 11 (pull-up)      |

**Note**: Verify the I2C LCD address using an I2C scanner sketch.

---

## Circuit Diagram

![MorseCodeX Circuit Diagram](https://raw.githubusercontent.com/Tonny-Blair-Daniel/MorseCodeX/main/asset/diagram.png)  
*Wiring for Arduino, 4x4 keypad, 16x2 I2C LCD, LEDs, buzzer, and push button. Replace with your diagram link after uploading.*

---

## Software Requirements

- **Arduino IDE**: For programming and uploading the sketch.
- **Libraries**:
  - `Wire.h`: For I2C communication (included with Arduino IDE).
  - `LiquidCrystal_I2C.h`: For LCD control.
  - `Keypad.h`: For keypad input.

Install libraries via the Arduino IDE Library Manager or download from their respective repositories.

---

## Setup Instructions

1. **Assemble the Circuit**:
   - Connect components as per the pin configuration.
   - Ensure resistors are used for LEDs and the push button has a pull-down resistor.
2. **Install Libraries**:
   - Open Arduino IDE, go to `Sketch > Include Library > Manage Libraries`.
   - Search for and install `LiquidCrystal_I2C` and `Keypad`.
3. **Upload the Sketch**:
   - Load the `MorseCodeX.ino` sketch (available in the repository).
   - Upload to your Arduino board.
4. **Power On**:
   - The LCD displays "MorseCodeX Ready" for 1.5 seconds before clearing.

---

## Usage

### T9 Text Input
- Press number keys (0-9) to cycle through characters (e.g., `2` for `A`, `B`, `C`, `2`).
- Wait 600ms to commit a character, or press another key to cycle.
- Use `*` to delete the last character.
- Use `#` to clear the entire message.

### Morse Code Transmission
- Press `D` to convert the input text to Morse code and transmit via LED and buzzer.
- The letter feedback LED lights for 500ms after each letter.

### Morse Code Input
- Use the push button to input Morse:
  - Short press (<300ms): Dot (`.`).
  - Long press (≥300ms): Dash (`-`).
- Pause 1000ms to commit a letter, or 2000ms to add a space.
- The LCD shows the current Morse symbol and decoded text.
- The letter feedback LED lights for 500ms per decoded letter.

### View Decoded Message
- Press `C` to display the decoded Morse message on the LCD for 2 seconds.

---

## Timing Parameters

| Action             | Duration  |
|--------------------|-----------|
| T9 Character Commit| 600ms     |
| Morse Dot Threshold| 300ms     |
| Morse Letter Pause | 1000ms    |
| Morse Word Pause   | 2000ms    |
| Letter LED Feedback| 500ms     |
| Decoded Message Display | 2000ms |

---

## Notes

- **T9 Map**: Supports A-Z, 0-9, and space.
- **Morse Code**: Supports A-Z, 0-9, and space (`/`).
- **Error Handling**: Invalid Morse sequences display as `?`.
- **LCD Limits**: Displays up to 16 characters per line; longer messages scroll.

---

## Example Usage

1. **Encode "SOS"**:
   - Press `7` three times (`PQRS` → `S`).
   - Press `6` three times (`MNO` → `O`).
   - Press `7` three times (`PQRS` → `S`).
   - Press `D` to transmit: `... --- ...` (LED flashes, buzzer sounds).
2. **Decode Morse**:
   - Press button: Short, short, short (`...` → `S`).
   - LCD updates with Morse symbols and decoded letter.

---

## Future Improvements

- Store messages in EEPROM for persistence.
- Add serial communication for PC interfacing.
- Support external Morse keyer hardware.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

> “A signal doesn’t need to be loud — it just needs to be clear.” – MorseCodeX
