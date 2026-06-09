#include "led.h"
#include <FastLED.h>

CRGB left_ch[NUM_LEDS1]; //left channels
CRGB right_ch[NUM_LEDS2]; //right channels
CRGB cups[NUM_LEDS3]; //cups
CRGB logo[NUM_LEDS4]; //logo
CRGB border[NUM_LEDS5]; //border
CRGB test[14];



//functions to control table leds

//init leds, flash around the table upon powering on
void led_init(void){
    FastLED.addLeds<CHIPSET, DIN1>(left_ch, NUM_LEDS1).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN2>(right_ch, NUM_LEDS2).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN3>(cups, NUM_LEDS3).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN4>(logo, NUM_LEDS4).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN5>(border, NUM_LEDS5).setCorrection(TypicalLEDStrip);

    FastLED.setBrightness(70);

    //send snake led around border
    //fill led buffer
    for (uint32_t i = 0; i < NUM_LEDS5; i++){
        fadeToBlackBy(border, NUM_LEDS5, 25);
        border[i+2] = CRGB::Green;
        border[i+1] = CRGB::Green;
        border[i] = CRGB::Green;
        FastLED.show();
        border[i+2] = CRGB::Black;
        border[i+1] = CRGB::Black;
        border[i] = CRGB::Black;
    }
}

//light up quadrant based on piezo sensing
void led_quadrant(uint8_t corner){
  
    if (corner & BOTT_LEFT == BOTT_LEFT) {

    }

    if (corner & BOTT_RIGHT== BOTT_RIGHT) {

    }

    if (corner & TOP_LEFT == TOP_LEFT) {

    }

    if (corner & TOP_RIGHT == TOP_RIGHT) {

    }
    
}

//light up rings around cups based on uint8_t cups data in game stuct and game mode
void led_cups(uint16_t cup_array){
    for (uint8_t i = 0; i < 10; i++){
        if (cup_array & (1 << i)){
            //light up leds for i cup
            //start = (i-1 * 14) 
            for (uint16_t j = ((i-1)*14); j<((i-1)*14)+14; j++){
                cups[j] = CRGB::White;
            }
        }
    }
     FastLED.setBrightness(20);
     FastLED.show();
     delay(5);

}

//light up leds reacting to sound
void led_sound(void){

}

//mini celebration when an opponent cup is lifted (you scored)
void led_cup_lift(void){
    //snake both quadrants mirrored quickly
     for (uint32_t i = 0; i < NUM_LEDS1; i++){
        left_ch[i] = CRGB::Blue;
        right_ch[i] = CRGB::Blue;
        FastLED.show();
        delay(20);
        left_ch[i] = CRGB::Black;
        right_ch[i] = CRGB::Black;
        FastLED.show();
        delay(5);
    }
}

//celebratory light display for team when their oppponent loses
void led_celebrate(void){
    //snake around border in green
        for (uint32_t i = 0; i < NUM_LEDS5; i++){
        fadeToBlackBy(border, NUM_LEDS5, 25);
        border[i+2] = CRGB::Green;
        border[i+1] = CRGB::Green;
        border[i] = CRGB::Green;
        FastLED.show();
        border[i+2] = CRGB::Black;
        border[i+1] = CRGB::Black;
        border[i] = CRGB::Black;
    }

    // blink cups lights
    for (uint8_t j = 0; j < 4; j++){
        for (uint32_t i = 0; i < NUM_LEDS3; i++){
                cups[i] = CRGB::Green;
            }
            FastLED.show();
            delay(200);
        for (uint32_t i = 0; i < NUM_LEDS3; i++){
                cups[i] = CRGB::Black;
            }
            FastLED.show();
            delay(200);
    }
}

//turn off all leds
void led_kill(void){
    for (uint32_t i = 0; i < NUM_LEDS1; i++){
        left_ch[i] = CRGB::Black;
    }
    for (uint32_t i = 0; i < NUM_LEDS2; i++){
        right_ch[i] = CRGB::Black;
    }
    for (uint32_t i = 0; i < NUM_LEDS3; i++){
        cups[i] = CRGB::Black;
    }
    for (uint32_t i = 0; i < NUM_LEDS4; i++){
        logo[i] = CRGB::Black;
    }
    for (uint32_t i = 0; i < NUM_LEDS5; i++){
        border[i] = CRGB::Black;
    }
    FastLED.show();
}

void led_test(void){
    FastLED.addLeds<CHIPSET, DIN1>(test, 14).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(40);
Serial.println("test starting\n");
    for (uint8_t i = 0; i < 14; i++){
        test[i] = CRGB::Red;
        FastLED.show();
        test[i] = CRGB::Black;
        delay(250);

    }
    Serial.println("test done\n");

}