#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game_logic.h"
#include "nrf_delay.h"
#include "microbit_v2.h"


game_state_t game;
nrfx_spim_t spim_inst;
uint8_t adc_limit = 0x3F;
uint8_t esp_data_buf = {0};
uint8_t corners = 0; //2 left bits = bottom left, 2 left middle bits = bottom right, 2 right middle bits = top left, 2 right bits = top right
bool send_audio = false;

int main(void) {
    //set game defaults
    init_game(&game, &spim_inst);
    adc_init();

    //main loop
    while(1) {
        if (game.PONG) {
            uint8_t count = 0;
            for (uint8_t i = 0; i < 8; i++){
                //control mux to gather IR sensor data
                IR_MUX_CONTROL0 = i & 0b0001;
                IR_MUX_CONTROL1 = i & 0b0010;
                IR_MUX_CONTROL2 = i & 0b0100;
                IR_MUX_CONTROL3 = i & 0b1000;
                
                if (i < 10) { //only for cups on diamond
                    //threshold to determine if cup is off table
                    if (IR_MUX_DATA < IR_THRESH) {
                        count++;
                        game.TEAM_1_CUPS &= !(1 << i); //update cup states
                    }
                }
            }
            //set score
            game.TEAM_1_SCORE = count;

            //communicate to ESP
                // Perform a SPI transmission (COPI)
                uint8_t data[4] = {0x00, 0x0F, 0xF0, 0xFF};
                nrfx_spim_xfer_desc_t data_xfer = NRFX_SPIM_XFER_TX(&data, 4);
                nrfx_spim_xfer(&SPIM_INST, &data_xfer, 0);
        }
        if (send_audio){
            //prepare data buffer with audio

        }

    }

}


