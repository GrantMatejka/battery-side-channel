#include <Adafruit_GrayOLED.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>

#include <Key.h>
#include <Keypad.h>

#include <string.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1
#define OLED_ADDR 0x3C // here address is 0x3C for 128x32 display

#define PASSWORD "9345"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 40, 42, 44, 46 };
byte colPins[COLS] = { 41, 43, 45, 47 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

byte isEmptyEnry = 1;
int entryAttempt[4] = { -1, -1, -1, -1 };

char emptyEntry[9] = "_ _ _ _";

int input_counter = 0;
int battery_to_lose = 0;
int battery = 100;

void resetDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print(emptyEntry);
    
    display.setCursor(100, 10);
    display.print(battery);
    display.print('%');

    display.display();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
   
  resetDisplay();
}

void loop() {
  if (battery <= 0) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("You Lose! Try again.");
    display.display();

    delay(1000);

    battery = 100;
  }

  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  
  if (customKey && customKey >= '0' && customKey <= '9') {
    entryAttempt[input_counter] = customKey;

    char entry[9];
    strcpy(entry, emptyEntry);

    for (int i = 0; i <= input_counter; i++) {
      entry[i * 2] = entryAttempt[i];
    }

    input_counter++;

    resetDisplay();
    display.setCursor(0, 10);
    display.println(entry);
    display.display();
    
  }

  // Process submission
  if (input_counter == 4) {
    int seq_matches = 0;
    int i = 0;

    delay(250);

    while (i < 4 && entryAttempt[i] == PASSWORD[i++]) {
      seq_matches++;
    }

    if (seq_matches == 4) {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Secret: XXXXXX");
      display.display();
      exit(0);
    }

    battery = battery - (2 * seq_matches + 1);

    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Processing...");
    display.display();

    delay(750);
    
    resetDisplay();
    input_counter = 0;
  }
}
