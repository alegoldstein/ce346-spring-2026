#include <Arduino.h>
#include <FastLED.h>
#include "led.h"

// --- MUX pins ---
#define MUX_A   25
#define MUX_B   26
#define MUX_C   27
#define MUX_OUT 36

#define IR_THRESHOLD 600
#define LED_PIN      23
#define NUM_LEDS     140

extern CRGB test[];

const uint8_t cup_lengths[10] = {14,14,14,14,14,14,14,14,14,13};
uint16_t cup_start[10];
bool cup_on[10] = {false};

// ir channel -> cup index (-1 = unused)
// IR 0-5 -> cups 1-6 (index 0-5)
// IR 6   -> cup 8 (index 7)
// IR 7   -> cup 9 (index 8)
// cups 7 and 10 (index 6 and 9) always off
const int8_t ir_to_cup[8] = {0, 1, 4, 3, 2, 5, 7, 8};

// --- MUX helpers ---
void muxSelect(uint8_t channel){
    digitalWrite(MUX_A,  channel & 0x01);
    digitalWrite(MUX_B, (channel >> 1) & 0x01);
    digitalWrite(MUX_C, (channel >> 2) & 0x01);
    delayMicroseconds(5);
}

int readMuxChannel(uint8_t channel){
    muxSelect(channel);
    delayMicroseconds(20);
    int sum = 0;
    for (int i = 0; i < 8; i++){
        sum += analogRead(MUX_OUT);
        delayMicroseconds(50);
    }
    return sum / 8;
}

void setCupColor(uint8_t cup, CRGB color){
    for (uint16_t i = cup_start[cup]; i < cup_start[cup] + cup_lengths[cup]; i++){
        test[i] = color;
    }
}

void setup(){
    Serial.begin(115200);

    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);
    pinMode(MUX_C, OUTPUT);
    pinMode(MUX_OUT, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(MUX_OUT, ADC_11db);

    FastLED.addLeds<WS2812B, LED_PIN>(test, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(255);
    fill_solid(test, NUM_LEDS, CRGB::Black);
    FastLED.show();

    cup_start[0] = 0;
    for (uint8_t i = 1; i < 10; i++){
        cup_start[i] = cup_start[i-1] + cup_lengths[i-1];
    }

    // ensure uncontrolled cups 7 and 10 (index 6, 9) are off
    setCupColor(6, CRGB::Black);
    setCupColor(9, CRGB::Black);
    FastLED.show();

    Serial.println("Ready");
    //led_test();
}

void loop(){
    // Serial.println("---- MUX Scan ----");
    // for (uint8_t ch = 0; ch < 8; ch++){
    //     int value = readMuxChannel(ch);
    //     Serial.print("CH"); Serial.print(ch); Serial.print(": "); Serial.println(value);

    //     int8_t cup = ir_to_cup[ch];

    //     if (value < IR_THRESHOLD){
    //         cup_on[cup] = true;
    //     } else {
    //         cup_on[cup] = false;
    //     }

    //     setCupColor(cup, cup_on[cup] ? CRGB::Blue : CRGB::Black);
    // }
    // Serial.println();

    // FastLED.show();
    // delay(100);
    //led_test();
    led_test();
    
}