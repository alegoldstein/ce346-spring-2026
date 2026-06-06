#include "led.h"
#include <FastLED.h>

#define CHIPSET WS2812B
#define NUM_LEDS1
#define NUM_LEDS2
#define NUM_LEDS3
#define NUM_LEDS4
#define NUM_LEDS5

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];
CRGB leds5[NUM_LEDS5];



//functions to control table leds

//init leds, flash around the table upon powering on
void led_init(void){
    FastLED.addLeds<CHIPSET, DIN1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN2, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN3, COLOR_ORDER>(leds3, NUM_LEDS3).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN4, COLOR_ORDER>(leds4, NUM_LEDS4).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<CHIPSET, DIN5, COLOR_ORDER>(leds5, NUM_LEDS5).setCorrection(TypicalLEDStrip);

    FastLED.setBrightness(70);

    //send snake led around border
    //fill led buffer
    FastLED.show(leds[] )
}

//light up quadrant based on piezo sensing
void led_quadrant(uintn8_t corner){
    
}

//light up rings around cups based on uint8_t cups data in game stuct and game mode
void led_cups(void){

}

//light up leds reacting to sound
void led_sound(void){

}

//celebratory light display for team when their oppponent loses
//at some point the ESPs need to determine which side theyre responsible for
void led_celebration(void){

}