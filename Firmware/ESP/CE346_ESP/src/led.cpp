#include "led.h"
#include <FastLED.h>

CRGB left_ch[NUM_LEDS1]; //left channels
CRGB right_ch[NUM_LEDS2]; //right channels
CRGB cups[NUM_LEDS3]; //cups
CRGB logo[NUM_LEDS4]; //logo
CRGB border[NUM_LEDS5]; //border
CRGB test[14];

//number of leds per cup//
// uint8_t CUP_LEDS = ;


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
    //FastLED.show(leds[] )
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
void led_cups(uint16_t cups){
    for (uint8_t i = 0; i < 10; i++){
        if (cups & (1 << i)){
            //light up leds for i cup
        }
    }

}

//light up leds reacting to sound
void led_sound(void){

}

//mini celebration when an opponent cup is lifted (you scored)
void led_cup_lift(void){

}

//celebratory light display for team when their oppponent loses
//at some point the ESPs need to determine which side theyre responsible for
void led_celebrate(void){

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
    FastLED.addLeds<CHIPSET, DIN5>(test, 14).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(40);

    for (uint8_t i = 0; i < 14; i++){
        test[i] = CRGB::Red;
        FastLED.show();
        test[i] = CRGB::Black;
        delay(250);
    }
}