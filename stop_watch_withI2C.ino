#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD (Default address: 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Button Pins
const int startButton = 8;
const int stopButton = 9;
const int resetButton = 10;
const int lapButton = 11;

// Timer Variables
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool running = false;
unsigned long lapTime = 0;
bool lapPressed = false;

// Previous button states (for debouncing)
bool prevStartState = HIGH;
bool prevStopState = HIGH;
bool prevResetState = HIGH;
bool prevLapState = HIGH;

void setup() {
    pinMode(startButton, INPUT_PULLUP);
    pinMode(stopButton, INPUT_PULLUP);
    pinMode(resetButton, INPUT_PULLUP);
    pinMode(lapButton, INPUT_PULLUP);

    lcd.init();       // Initialize LCD
    lcd.backlight();  // Turn on the backlight

    // Show initial message
    lcd.setCursor(3, 0);
    lcd.print("Stopwatch");
    delay(2000);
    lcd.clear();
    displayTime(0, 0, false);
}

void loop() {
    // Read button states
    bool startState = digitalRead(startButton);
    bool stopState = digitalRead(stopButton);
    bool resetState = digitalRead(resetButton);
    bool lapState = digitalRead(lapButton);

    // Start Button Pressed
    if (startState == LOW && prevStartState == HIGH) {
        delay(200); // Debounce
        if (!running) {
            startTime = millis() - elapsedTime;
            running = true;
        }
    }

    // Stop Button Pressed
    if (stopState == LOW && prevStopState == HIGH) {
        delay(200); // Debounce
        if (running) {
            elapsedTime = millis() - startTime;
            running = false;
        }
    }

    // Reset Button Pressed
    if (resetState == LOW && prevResetState == HIGH) {
        delay(200); // Debounce
        startTime = millis();
        elapsedTime = 0;
        lapTime = 0;
        lapPressed = false;
        running = false;
        displayTime(0, 0, false);
    }

    // Lap Button Pressed
    if (lapState == LOW && prevLapState == HIGH) {
        delay(200); // Debounce
        lapTime = elapsedTime;
        lapPressed = true;
    }

    // Update previous button states
    prevStartState = startState;
    prevStopState = stopState;
    prevResetState = resetState;
    prevLapState = lapState;

    // Update elapsed time if running
    if (running) {
        elapsedTime = millis() - startTime;
    }

    // Continuously update display
    displayTime(elapsedTime, lapTime, lapPressed);

    delay(100); // Slight delay for stable updates
}

// Function to display Stopwatch Time
void displayTime(unsigned long time, unsigned long lap, bool showLap) {
    lcd.setCursor(0, 0);
    formatTime(time);

    lcd.setCursor(0, 1);
    if (showLap) {
        lcd.print("Lap: ");
        formatTime(lap);
    } else {
        lcd.print("                "); // Clear only the lap row
    }
}

// Function to format and print time as MM:SS:MS
void formatTime(unsigned long time) {
    int minutes = (time / 60000) % 60;
    int seconds = (time / 1000) % 60;
    int milliseconds = (time / 10) % 100;

    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");

    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
    lcd.print(":");

    if (milliseconds < 10) lcd.print("0");
    lcd.print(milliseconds);
}