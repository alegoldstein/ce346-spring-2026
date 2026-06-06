//definitions for functions to control table leds
#define DIN1    16
#define DIN2    4   
#define DIN3    32
#define DIN4    33
#define DIN5    25




//init leds, flash around the table upon powering on
void led_init(void);

//light up quadrant based on piezo sensing
void led_quadrant(uintn8_t corner);

//light up rings around cups based on uint8_t cups data in game stuct and game mode
void led_cups(void);

//light up leds reacting to sound
void led_sound(void);

//celebratory light display for team when their oppponent loses
//at some point the ESPs need to determine which side theyre responsible for
void led_celebration(void);