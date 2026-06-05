#include "nrfx_spim.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


// button press callback function
// you should not need to edit this, but you can if you want to
static void gpio_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t _unused) {
  // overwrite the current button pressed as this one
  // if multiple buttons have been pressed, only the most recent is saved
  game_state.button_pressed = (uint8_t)pin;
}


//initialize game
void init_game(game_state_t game){
//set pin directions

//set struct defaults
game.TEAM_1_SCORE = 0;
game.TEAM_2_SCORE = 0;
game.PONG = TRUE;
game.DICE = FALSE;
game.TEAM_1_CUPS = 255;
game.TEAM_2_cups = 255;

//initialize spi to ESP (double check pins used)
//****************************** */
// Note: doesn't specify a chip-select pin
// That should probably be manually controlled via the GPIO library
nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG;
spim_config.sck_pin = EDGE_P13;
spim_config.mosi_pin = EDGE_P15;
spim_config.miso_pin = EDGE_P14;
spim_config.irq_priority = 0;
spim_config.frequency = NRF_SPIM_FREQ_8M;
spim_config.mode = NRF_SPIM_MODE_0;

// Note: without a callback handler, transfers are blocking
nrfx_spim_init(&SPIM_INST, &spim_config, NULL, NULL);


//initialize button interrupts
//********************** */
  // calls the `gpio_handler` callback function when either button is pressed
  nrfx_gpiote_init();
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); // high-accuracy mode
  nrfx_gpiote_in_init(BTN_A, &in_config, gpio_handler);
  nrfx_gpiote_in_event_enable(BTN_A, true); // enable interrupts for BTN_A
  nrfx_gpiote_in_init(BTN_B, &in_config, gpio_handler);
  nrfx_gpiote_in_event_enable(BTN_B, true); // enable interrupts for BTN_B


}
