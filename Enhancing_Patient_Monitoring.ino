#define USE_ARDUINO_INTERRUPTS true  // Set up low-level interrupts for most accurate BPM math.

#include <PulseSensorPlayground.h>   // PulseSensorPlayground Library
#include <LiquidCrystal.h>           // LiquidCrystal Library

// LCD pin connections
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int PulseWire = A0;    // PulseSensor PURPLE wire connected to ANALOG PIN A0
const int LED_3 = 13;        // LED to detect heartbeat (built-in LED on pin 13)
int Threshold = 550;         // Signal threshold to count a beat

// ------------------- Custom heart characters for LCD (8 custom chars) -------------------
byte heart1[8] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};
byte heart2[8] = {B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart3[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B01111};
byte heart4[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11111};
byte heart5[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B11111};
byte heart6[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11110};
byte heart7[8] = {B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart8[8] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};
// -------------------------------------------------------------------------------------

int Instructions_view = 500;   // Counter to control instruction display interval
PulseSensorPlayground pulseSensor;  // Create PulseSensor object

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);          // Initialize LCD (16 columns, 2 rows)

  // Create custom characters
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, heart7);
  lcd.createChar(8, heart8);

  // Show startup message
  lcd.setCursor(0, 0);
  lcd.print("Heart Beat/Pulse");
  lcd.setCursor(0, 1);
  lcd.print(" Monitoring EIF ");
  delay(2000);

  // Configure PulseSensor
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED_3);   // Blink LED with each beat
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
  delay(2000);
  lcd.clear();
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // Get current BPM

  // Manage instruction display counter
  if (Instructions_view < 500) {
    Instructions_view++;
  }

  if (Instructions_view > 499) {
    lcd.setCursor(0, 0);
    lcd.print("Put your finger ");
    lcd.setCursor(0, 1);
    lcd.print(" on the sensor.");
    delay(3000);
    lcd.clear();
    delay(500);
  }

  if (pulseSensor.sawStartOfBeat()) {   // Heartbeat detected
    Serial.println("  A HeartBeat Detected ! ");
    Serial.print("BPM: ");
    Serial.println(myBPM);

    // Draw animated heart on LCD
    lcd.setCursor(1, 1);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.setCursor(0, 0);
    lcd.write(byte(3));
    lcd.setCursor(1, 0);
    lcd.write(byte(4));
    lcd.setCursor(2, 0);
    lcd.write(byte(5));
    lcd.setCursor(3, 0);
    lcd.write(byte(6));
    lcd.setCursor(3, 1);
    lcd.write(byte(7));
    lcd.setCursor(2, 1);
    lcd.write(byte(8));

    // Display heart rate text and value
    lcd.setCursor(5, 0);
    lcd.print("Heart Rate");
    lcd.setCursor(5, 1);
    lcd.print(": ");
    lcd.print(myBPM);
    lcd.print(" ");
    lcd.print("BPM     ");

    Instructions_view = 0;   // Reset instruction counter after showing beat
  }

  delay(20);   // Small delay for stability
}
