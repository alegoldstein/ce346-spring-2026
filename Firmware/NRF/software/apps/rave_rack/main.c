#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game_logic.h"
#include "nrf_delay.h"
#include "microbit_v2.h"

game_state_t game = {};

int main(void){
    //set game defaults
    game_init(game_state_t game);

    //main loop
    while(1){
        if (game.PONG){
            uint8_t count = 0;
            for (uint8_t i = 0; i < 8; i++){
                //control mux to gather IR sensor data
                IR_MUX_CONTROL0 = i & 1;
                IR_MUX_CONTROL1 = i & 2;
                IR_MUX_CONTROL2 = i & 4;
                IR_MUX_CONTROL3 = i & 8;
                
                if (i < 6){ //only for cups on diamond
                    //threshold to determine if cup is off table
                    if (IR_MUX_DATA < ?){
                        count++;
                        game.TEAM_1_CUPS &= !(1 << i); //update cup states
                    }
                }
            }
            //set score
            game.TEAM_1_SCORE = count;

            //communicate to ESP
            //example spi transfer
                // Perform a SPI transmission (COPI)
                uint8_t data[4] = {0x00, 0x0F, 0xF0, 0xFF};
                nrfx_spim_xfer_desc_t data_xfer = NRFX_SPIM_XFER_TX(&data, 4);
                nrfx_spim_xfer(&SPIM_INST, &data_xfer, 0);
        }


    }

}