#include <Arduino.h>
#include <FastLED.h>
#include <ESP32SPISlave.h>

#include "init.h"
#include "led.h"
#include "spi_helper.h"

ESP32SPISlave slave;

static constexpr size_t BUFFER_SIZE = 8;
static constexpr size_t QUEUE_SIZE = 1;
uint8_t tx_buf[BUFFER_SIZE] {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t rx_buf[BUFFER_SIZE] {0, 0, 0, 0, 0, 0, 0, 0};

bool sound_reactive = false;
//bool kill = false;
bool dice_cup_lift = false; //if dice lands in cup, celebrate

struct game_state_t {
    uint16_t TEAM_1_SCORE; 
	uint16_t TEAM_2_SCORE;
	bool PONG;
	bool DICE;
	uint16_t TEAM_1_NUM_CUPS;
	uint16_t TEAM_2_NUM_CUPS;
};

game_state_t game;

void setup() {
    Serial.begin(115200);

    delay(2000);

    slave.setDataMode(SPI_MODE0);   // default: SPI_MODE0
    slave.setQueueSize(QUEUE_SIZE); // default: 1, requres 2 in this example

    // begin() after setting
    slave.begin(HSPI, SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);  // default: HSPI (please refer README for pin assignments)
    Serial.println("slave initialized");

    //init button interrupts
    pinMode(GAME_BTN, INPUT);
    pinMode(KILL_BTN, INPUT);
    pinMode(SOUND_BTN, INPUT);
    // attachInterupt(GAME_BTN, game_switch_handler(), FALLING);  //all falling edge, hardware pullups
    // attachInterupt(KILL_BTN, kill_switch_handler(), FALLING);
    // attachInterupt(SOUND_BTN, sound_btn_handler(), FALLING);

    led_init();
}

void loop() {
    /////////////////////////SPI TEST//////////////////////////////
    // // initialize tx/rx buffers
    // initializeBuffers(tx_buf, rx_buf, BUFFER_SIZE);
    // Serial.println("buffers initialized");

    // // start and wait to complete one BIG transaction (same data will be received from slave)
    // const size_t received_bytes = slave.transfer(tx_buf, rx_buf, BUFFER_SIZE);
    // Serial.println("slave transfer completed");

    // // verify and dump difference with received data
    // if (verifyAndDumpDifference("slave", tx_buf, BUFFER_SIZE, "master", rx_buf, received_bytes)) {
    //     Serial.println("successfully received expected data from master");
    // } else {
    //     Serial.println("unexpected difference found between master/slave data");
    // }

    //off for testing
    // if (!kill){
    //     if (game.PONG){
    //         //light up rings around cups
    //         led_cups(game.TEAM_1_CUPS);
    //         if (game.TEAM_1_SCORE = 10){ //ESP on each half, if their halves score is 6 then they celebrate
    //             led_celebrate();
    //         }

    //     }
    //     else if (game.DICE){
    //         led_cups(129); //10000001
    //         if (dice_cup_lift) {
    //             led_cup_lift();
    //             dice_cup_lift = false;
    //         }
    //     }
    //     else {
    //         Serial.println("no game mode on\n");
    //         delay_ms(1500);
    //     }
    // }
    led_test();
}


//button callbacks

//callback handler for kill switch button
void kill_switch_handler(){
  //  kill = true;
    //turn off LEDs
    led_kill();

    //reset game state
    game.TEAM_1_SCORE = 0;
	game.TEAM_2_SCORE = 0;
	game.PONG = true;
	game.DICE = false;
	game.TEAM_1_NUM_CUPS = 0xFFFF;
	game.TEAM_2_NUM_CUPS = 0xFFFF;

    sound_reactive = false;

    //turn off scoreboard

}

//handler for when game switch button is pressed
void game_swtich_handler(){
    game.PONG = !game.PONG;
    game.DICE = !game.DICE;

    //communicate to NRF that game mode changed
}

//handler for when sound btn button is pressed
void sound_btn_handler(){
    sound_reactive = !sound_reactive;
    
    //communicate to NRF so that it knows to send audio data
}
