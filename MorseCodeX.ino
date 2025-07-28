#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD setup
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// T9 map
const char* t9Map[10] = {
  " 0", "1", "2ABC", "3DEF", "4GHI", "5JKL", "6MNO", "7PQRS", "8TUV", "9WXYZ"
};

// Morse table
struct MorseEntry {
  char letter;
  const char* code;
};
MorseEntry morseTable[] = {
  {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
  {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
  {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
  {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
  {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
  {'Z', "--.."}, {'0', "-----"}, {'1', ".----"}, {'2', "..---"},
  {'3', "...--"}, {'4', "....-"}, {'5', "....."}, {'6', "-...."},
  {'7', "--..."}, {'8', "---.."}, {'9', "----."}, {' ', "/"}
};

// Pin assignments
const byte led = 10;        // LED for Morse input and transmission
const byte letterLed = 13;   // LED for letter completion (encoding/decoding)
const byte buzzer = 12;
const byte buttonPin = 11;

// T9 variables
char lastKey = '\0';
int pressCount = 0;
unsigned long lastPressTime = 0;
const unsigned long t9Timeout = 600;
char currentChar = '\0';

String message = "";
int cursorPos = 0;

// Morse decoding variables
unsigned long pressStart = 0;
unsigned long lastRelease = 0;
bool isPressing = false;
String currentSymbol = "";
String decodedMessage = "";

const int dotThreshold = 300;
const int letterPause = 1000;
const int wordPause = 2000;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(letterLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(led, LOW);
  digitalWrite(letterLed, LOW);
  noTone(buzzer);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MorseCodeX Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();
  handleT9(now);
  handleButtonInput(now);
}

void handleT9(unsigned long now) {
  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    if (key == lastKey && (now - lastPressTime) < t9Timeout) {
      pressCount++;
    } else {
      if (lastKey != '\0') {
        message += currentChar;
        cursorPos++;
      }
      pressCount = 0;
      lastKey = key;
    }

    lastPressTime = now;
    const char* letters = t9Map[key - '0'];
    int len = strlen(letters);
    currentChar = letters[pressCount % len];

    int row = (cursorPos < 16) ? 0 : 1;
    int col = cursorPos % 16;
    lcd.setCursor(col, row);
    lcd.print(currentChar);
  }

  else if (key == '*') { // Backspace
    if (cursorPos > 0) {
      cursorPos--;
      message.remove(message.length() - 1);
      int row = (cursorPos < 16) ? 0 : 1;
      int col = cursorPos % 16;
      lcd.setCursor(col, row);
      lcd.print(" ");
      lcd.setCursor(col, row);
    }
    lastKey = '\0';
  }

  else if (key == '#') { // Clear
    message = "";
    cursorPos = 0;
    lcd.clear();
    lastKey = '\0';
  }

  else if (key == 'D') { // Play Morse
    if (lastKey != '\0') {
      message += currentChar;
      cursorPos++;
      lastKey = '\0';
    }
    lcd.clear();
    lcd.print("Sending Morse...");
    sendMessageAsMorse(message);
    lcd.clear();
    lcd.print("Done");
    delay(1000);
    lcd.clear();
    lcd.print(message.substring(max(0, message.length() - 16)));
  }

  else if (key == 'C') { // Show Decoded Message
    lcd.clear();
    lcd.print("Decoded:");
    lcd.setCursor(0, 1);
    lcd.print(decodedMessage.substring(max(0, decodedMessage.length() - 16)));
    delay(2000);
    lcd.clear();
    lcd.print(message.substring(max(0, message.length() - 16)));
  }

  // Commit last char after timeout
  if (lastKey != '\0' && (now - lastPressTime) > t9Timeout) {
    message += currentChar;
    cursorPos++;
    lastKey = '\0';
    pressCount = 0;
  }
}

void sendMessageAsMorse(String msg) {
  msg.toUpperCase();
  for (int i = 0; i < msg.length(); i++) {
    char c = msg[i];
    const char* code = lookupMorse(c);
    for (int j = 0; code[j] != '\0'; j++) {
      playMorseSymbol(code[j]);
    }
    // Light up letterLed for 500ms after each letter is transmitted
    digitalWrite(letterLed, HIGH);
    delay(500);
    digitalWrite(letterLed, LOW);
    delay(c == ' ' ? wordPause - 500 : letterPause - 500);
  }
}

const char* lookupMorse(char c) {
  c = toupper(c);
  for (int i = 0; i < sizeof(morseTable) / sizeof(MorseEntry); i++) {
    if (morseTable[i].letter == c) return morseTable[i].code;
  }
  return "";
}

void playMorseSymbol(char symbol) {
  int duration = (symbol == '.') ? 200 : 600;
  digitalWrite(led, HIGH);
  tone(buzzer, 1000);
  delay(duration);
  digitalWrite(led, LOW);
  noTone(buzzer);
  delay(200);
}

void handleButtonInput(unsigned long now) {
  bool buttonState = digitalRead(buttonPin) == LOW;

  if (buttonState && !isPressing) {
    isPressing = true;
    pressStart = now;
    digitalWrite(led, HIGH);
  }

  if (!buttonState && isPressing) {
    isPressing = false;
    unsigned long duration = now - pressStart;
    digitalWrite(led, LOW);

    if (duration < dotThreshold) {
      currentSymbol += ".";
    } else {
      currentSymbol += "-";
    }
    lastRelease = now;

    // Immediate LCD feedback for dot or dash
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(currentSymbol);
    lcd.setCursor(0, 1);
    int len = decodedMessage.length();
    lcd.print(decodedMessage.substring(len > 16 ? len - 16 : 0));
  }

  if (!isPressing && currentSymbol.length() > 0 && (now - lastRelease) > letterPause) {
    char decoded = decodeMorse(currentSymbol);
    decodedMessage += decoded;

    // Light up letterLed for 500ms when a letter is decoded
    digitalWrite(letterLed, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(currentSymbol + " => " + decoded);
    lcd.setCursor(0, 1);
    int len = decodedMessage.length();
    lcd.print(decodedMessage.substring(len > 16 ? len - 16 : 0));
    delay(500);
    digitalWrite(letterLed, LOW);

    currentSymbol = "";
  }

  if (!isPressing && (now - lastRelease) > wordPause && lastRelease != 0) {
    if (!decodedMessage.endsWith(" ")) {
      decodedMessage += " ";
      int len = decodedMessage.length();
      lcd.setCursor(0, 1);
      lcd.print(decodedMessage.substring(len > 16 ? len - 16 : 0));
    }
    lastRelease = 0;
  }
}

char decodeMorse(String code) {
  for (int i = 0; i < sizeof(morseTable) / sizeof(MorseEntry); i++) {
    if (code.equals(morseTable[i].code)) return morseTable[i].letter;
  }
  return '?';
}