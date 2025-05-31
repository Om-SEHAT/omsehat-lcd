#include <Arduino.h>
#include <Wire.h> // Required for I2C communication
#include <LiquidCrystal_I2C.h> // The LCD library

// --- Configuration ---
// I2C LCD Address: Common addresses are 0x27 or 0x3F.
const int LCD_ADDRESS = 0x27; // <<-- IMPORTANT: Change this if your LCD has a different address

// LCD dimensions
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

// Create an LCD object
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Function to print centered text on a specified row
// This function correctly centers text that fits within LCD_COLS.
void printCentered(const char* text, int row) {
  int len = strlen(text);
  int pos = (LCD_COLS - len) / 2;

  // If text is longer than the display, start printing from column 0.
  // The LCD library will handle truncation.
  if (pos < 0) {
    pos = 0;
  }

  // Clear the entire row first
  lcd.setCursor(0, row);
  for (int i = 0; i < LCD_COLS; i++) {
    lcd.print(" ");
  }

  // Print the centered text at the calculated position
  lcd.setCursor(pos, row);
  lcd.print(text);
}

// Alternative function that centers text with even spaces on both sides
// This function also works well and explicitly pads the line.
void printPerfectlyCentered(const char* text, int row) {
  int len = strlen(text);
  
  // If text is longer than display, it will effectively print from column 0
  // as leftSpaces will be <= 0.
  int leftSpaces = (LCD_COLS - len) / 2;
  if (leftSpaces < 0) leftSpaces = 0; // Ensure leftSpaces is not negative for loop

  int rightSpaces = LCD_COLS - len - leftSpaces;
  if (rightSpaces < 0) rightSpaces = 0; // Ensure rightSpaces is not negative

  lcd.setCursor(0, row);

  // Print left padding spaces
  for (int i = 0; i < leftSpaces; i++) {
    lcd.print(" ");
  }

  // Print the text (LCD library will truncate if len > LCD_COLS after left padding)
  lcd.print(text);

  // Print right padding spaces to fill the line
  // This loop ensures the rest of the line is cleared/padded.
  // Calculate current cursor position after printing text
  int textEndPos = leftSpaces + len;
  // Only print right spaces if we are still within the LCD columns
  if (textEndPos < LCD_COLS) {
      for (int i = 0; i < rightSpaces; i++) {
          // Defensive check to prevent writing past LCD_COLS, though mathematically rightSpaces should be correct
          if (textEndPos + i < LCD_COLS) {
             lcd.print(" ");
          } else {
             break;
          }
      }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("LCD 16x4 Test Sketch - OmSehat Display");

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.println("LCD initialized. Displaying OmSehat message...");

  // Print centered messages on all four lines
  printCentered("OmSehat", 0);

  // IMPORTANT FIX: The original string "Hadirkan Kesehatan" (18 chars) is too long for a 16-char display.
  // It will be truncated and printed from column 0.
  // To center it properly, shorten the string to 16 characters or less.
  // Example of a shortened string:
  printCentered("Hadirkan Sehat", 1); // Changed from "Hadirkan Kesehatan"
  // Or, if you must use the original, be aware it will be truncated:
  // printCentered("Hadirkan Kesehatan", 1); // This will show as "Hadirkan Kesehat" at column 0

  printCentered("Cerdas ke", 2);
  printCentered("Pelosok Indonesia", 3);

  Serial.println("Message displayed successfully");
}

void loop() {
  delay(1000);
}