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
bool kill = false;
bool dice_cup_lift = false; //if dice lands in cup, celebrate

struct game_state_t {
    uint8_t TEAM_1_SCORE; 
	uint8_t TEAM_2_SCORE;
	bool PONG;
	bool DICE;
	uint8_t TEAM_1_NUM_CUPS;
	uint8_t TEAM_2_NUM_CUPS;
}

game_state_t game;

void setup() {
    Serial.begin(115200);

    delay(2000);

    slave.setDataMode(SPI_MODE0);   // default: SPI_MODE0
    slave.setQueueSize(QUEUE_SIZE); // default: 1, requres 2 in this example

    // begin() after setting
    slave.begin(HSPI, SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);  // default: HSPI (please refer README for pin assignments)
    Serial.println("slave initialized");

    init_button_interrupts();
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

    if (!kill){
        if (game.PONG){
            //light up rings around cups
            led_cups(game.TEAM_1_CUPS);

        }
        else if (game.DICE){
            led_cups(129); //10000001
            if (dice_cup_lift) {
                led_celebrate();
                dice_cup_lift = false;
            }
        }
        else {
            Serial.println("no game mode on\n");
        }
    }
}


//button callbacks

//callback handler for kill switch button
void kill_switch_handler(){
    kill = true;
    //turn off LEDs

    //reset game state
    game.TEAM_1_SCORE = 0;
	game.TEAM_2_SCORE = 0;
	game.PONG = true;
	game.DICE = false;
	game.TEAM_1_NUM_CUPS = 255;
	game.TEAM_2_NUM_CUPS = 255;

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
