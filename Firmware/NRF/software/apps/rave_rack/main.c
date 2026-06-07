#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game_logic.h"
#include "nrf_delay.h"
#include "microbit_v2.h"

// These are GPIO pin numbers that can be used in ADC configurations
// AIN1 is breakout pin 1. AIN2 is breakout pin 2.
#define ANALOG_TEMP_IN  NRF_SAADC_INPUT_AIN1
#define ANALOG_LIGHT_IN NRF_SAADC_INPUT_AIN2

// ADC channel configurations
// These are ADC channel numbers that can be used in ADC calls
#define ADC_TEMP_CHANNEL  0
#define ADC_LIGHT_CHANNEL 1

game_state_t game = {};
bool send_audio = false;

int main(void){
    //set game defaults
    init_game(game_state_t game);

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
        if (send_audio){

        }

    }

}



static void adc_init(void) {
  // Initialize the SAADC
  nrfx_saadc_config_t saadc_config = {
    .resolution = NRF_SAADC_RESOLUTION_12BIT,
    .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
    .interrupt_priority = 4,
    .low_power_mode = false,
  };
  ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_event_callback);
  APP_ERROR_CHECK(error_code);

  // Initialize piezo1 channel
  nrf_saadc_channel_config_t temp_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TEMP_IN);
  error_code = nrfx_saadc_channel_init(ADC_TEMP_CHANNEL, &temp_channel_config);
  APP_ERROR_CHECK(error_code);

  // Initialize piezo2 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);

   // Initialize piezo3 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);

   // Initialize piezo4 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);
}