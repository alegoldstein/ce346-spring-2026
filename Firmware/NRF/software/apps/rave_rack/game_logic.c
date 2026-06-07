#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrfx_spim.h"
#include "nrfx_saadc.h"

// ADC channel configurations
// These are ADC channel numbers that can be used in ADC calls
#define PIEZO1_CH  6
#define PIEZO2_CH 5
#define PIEZO3_CH  4
#define PIEZO4_CH 0
#define SAADC_BASE_ADDR     0x40007000
#define INTENSET_OFFSET     0x304

//registers that show if the limit was passed
#define CH0_LMTH_OFFSET     0x118
#define CH4_LMTH_OFFSET     0x118 + (4 * 0x8)
#define CH5_LMTH_OFFSET     0x118 + (5 * 0x8)
#define CH6_LMTH_OFFSET     0x118 + (6 * 0x8)
#define CH0_LMTH    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH0_LMTH_OFFSET))
#define CH4_LMTH    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH4_LMTH_OFFSET))
#define CH5_LMTH    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH5_LMTH_OFFSET))
#define CH6_LMTH    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH6_LMTH_OFFSET))

//define the actual limits
#define CH0_LIMIT_OFFSET    0x51C
#define CH4_LIMIT_OFFSET    0x55C
#define CH5_LIMIT_OFFSET    0x56C
#define CH6_LIMIT_OFFSET    0x57C
#define CH0_LIMIT    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH0_LIMIT_OFFSET))
#define CH4_LIMIT    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH4_LIMIT_OFFSET))
#define CH5_LIMIT    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH5_LIMIT_OFFSET))
#define CH6_LIMIT    (*(volatile uint32_t *)(SAADC_BASE_ADDR + CH6_LIMIT_OFFSET))

#define SAADC_INTENSET    (*(volatile uint32_t *)(SAADC_BASE_ADDR + INTENSET_OFFSET))

acd_init();

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
  adc_init();
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
  nrf_saadc_channel_config_t piezo1_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(PIEZO1);
  error_code = nrfx_saadc_channel_init(PIEZO1_CH, &piezo1_config);
  APP_ERROR_CHECK(error_code);

  // Initialize piezo2 channel
  nrf_saadc_channel_config_t piezo2_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(PIEZO2);
  error_code = nrfx_saadc_channel_init(PIEZO2_CH, &piezo2_config);
  APP_ERROR_CHECK(error_code);

   // Initialize piezo3 channel
  nrf_saadc_channel_config_t piezo3_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(PIEZO3);
  error_code = nrfx_saadc_channel_init(PIEZO3_CH, &piezo3_config);
  APP_ERROR_CHECK(error_code);

   // Initialize piezo4 channel
  nrf_saadc_channel_config_t piezo4_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(PIEZO4);
  error_code = nrfx_saadc_channel_init(PIEZO4_CH, &piezo4_config);
  APP_ERROR_CHECK(error_code);

  //enable saadc limit high interrupts for ch 6,5,4,0
  SAADC_INTENSET = (1 << 19) | (1 << 17) | (1<<15) | (1 << 7);

  CH0_LIMIT = adc_limit << 8;
  CH4_LIMIT = adc_limit << 8;
  CH5_LIMIT = adc_limit << 8;
  CH6_LIMIT = adc_limit << 8;

  NVIC_EnableIRQ(SAADC_IRQn);
  NVIC_SetPriority(SAADC_IRQn, 7);
}

void saadc_event_callback(nrfx_saadc_evt_t const * p_event)
{
    corners = 0;
    //ch0 piezo correlates to leftmost bits (bottom left rn)
    if (CH0_LMTH == 1){
    CH0_LMTH = 0;
    corners |= 3 << 6;
    }

    //ch4 piezo correlates to middle left bits (bottom right rn)
    if (CH4_LMTH == 1){
    CH4_LMTH = 0;
    corners |= 3 << 4;
    }

    //ch5 piezo correlates to middle right bits (top left)
    if (CH5_LMTH == 1){
    CH5_LMTH = 0;
    corners |= 3 << 2;
    }

    //ch6 piezo correlates to right bits (top right)
    if (CH6_LMTH == 1){
    CH6_LMTH = 0;
    corners |= 3;
    }
}