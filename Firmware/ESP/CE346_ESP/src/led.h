#include <stdint.h>

//led count and data pin declarations
#define CHIPSET WS2812B
#define NUM_LEDS1   184 //left ch
#define NUM_LEDS2   184 //right ch
#define NUM_LEDS3   168 //cups
#define NUM_LEDS4   43  //logo
#define NUM_LEDS5   213 //border

#define DIN1    16
#define DIN2    4   
#define DIN3    32
#define DIN4    33
#define DIN5    25

#define BOTT_LEFT   0b11000000
#define BOTT_RIGHT  0b00110000
#define TOP_LEFT    0b00001100
#define TOP_RIGHT   0b00000011

//definitions for functions to control table leds
//init leds, flash around the table upon powering on
void led_init(void);

//light up quadrant based on piezo sensing
void led_quadrant(uint8_t corner);

//light up rings around cups based on uint8_t cups data in game stuct and game mode
void led_cups(uint8_t cups);

//light up leds reacting to sound
void led_sound(void);

//mini celebration when an opponent cup is lifted (you scored)
void led_cup_lift(void);

//celebratory light display for team when their oppponent loses
//at some point the ESPs need to determine which side theyre responsible for
void led_celebrate(void);

//turn off all leds
void led_kill(void);

void led_test(void);