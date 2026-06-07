#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrfx_spim.h"
#include "nrfx_saadc.h"

// Button press callback function
// You should not need to edit this, but you can if you want to
static void gpio_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t _unused) {
  // Overwrite the current button pressed as this one
  // If multiple buttons have been pressed, only the most recent is saved
  game_state.button_pressed = (uint8_t)pin;
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

  // Initialize IR channel
  nrf_saadc_channel_config_t temp_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TEMP_IN);
  error_code = nrfx_saadc_channel_init(ADC_TEMP_CHANNEL, &temp_channel_config);
  APP_ERROR_CHECK(error_code);

  // Initialize PIEZO_1 channel
  nrf_saadc_channel_config_t temp_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TEMP_IN);
  error_code = nrfx_saadc_channel_init(ADC_TEMP_CHANNEL, &temp_channel_config);
  APP_ERROR_CHECK(error_code);

  // Initialize PIEZO_2 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);

   // Initialize PIEZO_3 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);

   // Initialize PIEZO_4 channel
  nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
  error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
  APP_ERROR_CHECK(error_code);
}

void init_game(game_state_t *game, nrfx_spim_t *spim_inst){
  // Set pin directions
  nrf_gpio_cfg_output(IR_MUX_CTRL_0);
  nrf_gpio_pin_clear(IR_MUX_CTRL_0);

  nrf_gpio_cfg_output(IR_MUX_CTRL_1);
  nrf_gpio_pin_clear(IR_MUX_CTRL_1);

  nrf_gpio_cfg_output(IR_MUX_CTRL_2);
  nrf_gpio_pin_clear(IR_MUX_CTRL_2);

  nrf_gpio_cfg_output(IR_MUX_CTRL_3);
  nrf_gpio_pin_clear(IR_MUX_CTRL_3);
  
  // Set struct defaults
  game->TEAM_1_SCORE = 0;
  game->TEAM_2_SCORE = 0;
  game->PONG = true;
  game->DICE = false;
  game->TEAM_1_CUPS = 255;
  game->TEAM_2_cups = 255;
  
  // Initialize SPI to ESP (double check pins used)
  nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG;
  spim_config.sck_pin = SPI_CLK;
  spim_config.mosi_pin = SPI_MOSI;
  spim_config.miso_pin = SPI_MISO;
  spim_config.ss_pin = SPI_CS;
  spim_config.irq_priority = 0;
  spim_config.frequency = NRF_SPIM_FREQ_8M;
  spim_config.mode = NRF_SPIM_MODE_0;
  nrfx_spim_init(spim_inst, &spim_config, NULL, NULL);

  // Initialize SAADC
}
