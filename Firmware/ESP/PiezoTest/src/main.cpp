#include <Arduino.h>

/*
 * Dual Piezoelectric Sensor Comparator with LED Indicator
 * --------------------------------------------------------
 * Reads two analog piezo sensors and lights the corresponding
 * LED for whichever sensor produces the higher signal.
 *
 * Wiring:
 *   Piezo Sensor 1  --> GPIO 34 (ADC1_CH6) via voltage divider
 *   Piezo Sensor 2  --> GPIO 35 (ADC1_CH7) via voltage divider
 *   LED 1 (Sensor 1 wins) --> GPIO 25 via 220Ω resistor to GND
 *   LED 2 (Sensor 2 wins) --> GPIO 26 via 220Ω resistor to GND
 *
 * Note: Piezo sensors can produce voltages above 3.3V under impact.
 *       Use a voltage divider (e.g. 1MΩ + 1MΩ) or a clamping diode
 *       to protect the ESP32 ADC pins. Use ADC1 pins (32–39) only —
 *       ADC2 is unavailable when Wi-Fi is active.
 */

// ── Pin Definitions ────────────────────────────────────────────────────────
const int PIEZO_PIN_1 = 34;   // Analog input – Sensor 1
const int PIEZO_PIN_2 = 35;   // Analog input – Sensor 2
const int LED_PIN_1   = 25;   // LED for Sensor 1
const int LED_PIN_2   = 26;   // LED for Sensor 2

// ── Configuration ──────────────────────────────────────────────────────────
const int   SAMPLE_COUNT    = 8;     // Samples averaged per reading
const int   THRESHOLD       = 10;   // Minimum ADC value to count as a signal
                                     // (0–4095 on ESP32 12-bit ADC)
const int   LED_HOLD_MS     = 200;   // How long the LED stays on (ms)
const int   LOOP_DELAY_MS   = 10;    // Delay between main loop iterations (ms)

// ── State ──────────────────────────────────────────────────────────────────
unsigned long led1OffTime = 0;
unsigned long led2OffTime = 0;

// ── Helper: averaged analog read ───────────────────────────────────────────
int readAveraged(int pin) {
  long sum = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    sum += analogRead(pin);
    delayMicroseconds(50);  // brief pause between samples
  }
  return (int)(sum / SAMPLE_COUNT);
}

// ── Setup ──────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);

  // ESP32 ADC resolution: 12-bit (0–4095)
  analogReadResolution(12);
  // Attenuation: 11 dB allows ~0–3.3 V input range
  analogSetAttenuation(ADC_11db);

  Serial.println("Dual Piezo Comparator Ready");
  Serial.println("Sensor1\tSensor2\tWinner");
}

// ── Main Loop ──────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // Read both sensors
  int val1 = readAveraged(PIEZO_PIN_1);
  int val2 = readAveraged(PIEZO_PIN_2);

  // Only act if at least one sensor exceeds the noise threshold
  if (val1 > THRESHOLD || val2 > THRESHOLD) {

    Serial.print(val1);
    Serial.print("\t");
    Serial.print(val2);
    Serial.print("\t");

    if (val1 > val2) {
      // Sensor 1 wins
      digitalWrite(LED_PIN_1, HIGH);
      led1OffTime = now + LED_HOLD_MS;
      Serial.println("Sensor 1");

    } else if (val2 > val1) {
      // Sensor 2 wins
      digitalWrite(LED_PIN_2, HIGH);
      led2OffTime = now + LED_HOLD_MS;
      Serial.println("Sensor 2");

    } else {
      // Tie — light both
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, HIGH);
      led1OffTime = now + LED_HOLD_MS;
      led2OffTime = now + LED_HOLD_MS;
      Serial.println("Tie");
    }
  }

  // Non-blocking LED auto-off
  if (now >= led1OffTime) digitalWrite(LED_PIN_1, LOW);
  if (now >= led2OffTime) digitalWrite(LED_PIN_2, LOW);

  delay(LOOP_DELAY_MS);
}