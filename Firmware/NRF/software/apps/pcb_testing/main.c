// PCB testing app
//
// Tests various PCB features

#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_saadc.h"
#include "nrfx_spim.h"
#include "app_timer.h"

#define IR_MUX_CTRL_0		NRF_GPIO_PIN_MAP(0, 15)
#define IR_MUX_CTRL_1		NRF_GPIO_PIN_MAP(0, 17)
#define IR_MUX_CTRL_2		NRF_GPIO_PIN_MAP(0, 20)
#define IR_MUX_CTRL_3		NRF_GPIO_PIN_MAP(0, 3)

#define IR_MUX_DATA		NRF_SAADC_INPUT_AIN7
#define ADC_IR_CHANNEL  	0

#define	SPI_MISO		NRF_GPIO_PIN_MAP(1, 9)
#define	SPI_MOSI		NRF_GPIO_PIN_MAP(0, 11)
#define	SPI_SCK			NRF_GPIO_PIN_MAP(0, 4)
#define	SPI_CS			NRF_GPIO_PIN_MAP(0, 5)

APP_TIMER_DEF(sample_timer);
static const nrfx_spim_t SPIM_INST = NRFX_SPIM_INSTANCE(2);
uint8_t last_checked = 3;

static void gpio_init(void) {
  // Initialize mux control pins as outputs
  nrf_gpio_cfg_output(IR_MUX_CTRL_0);
  nrf_gpio_pin_clear(IR_MUX_CTRL_0);

  nrf_gpio_cfg_output(IR_MUX_CTRL_1);
  nrf_gpio_pin_clear(IR_MUX_CTRL_1);

  nrf_gpio_cfg_output(IR_MUX_CTRL_2);
  nrf_gpio_pin_clear(IR_MUX_CTRL_2);

  nrf_gpio_cfg_output(IR_MUX_CTRL_3);
  nrf_gpio_pin_clear(IR_MUX_CTRL_3);
}

static void saadc_event_callback(nrfx_saadc_evt_t const* _unused) {
  // To make nrfx_saadc_init() happy
  // Ignore
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
  nrf_saadc_channel_config_t ir_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(IR_MUX_DATA);
  error_code = nrfx_saadc_channel_init(ADC_IR_CHANNEL, &ir_channel_config);
  APP_ERROR_CHECK(error_code);
}

static int16_t adc_sample_blocking(void) {
  // Read ADC counts (0-4095)
  // This function blocks until the sample is ready
  int16_t adc_counts = 0;
  ret_code_t error_code = nrfx_saadc_sample_convert(ADC_IR_CHANNEL, &adc_counts);
  APP_ERROR_CHECK(error_code);

  // Return raw ADC measurement
  return adc_counts;
}

static void spim_init(void) {
  // Note: doesn't specify a chip-select pin
  // That should probably be manually controlled via the GPIO library
  nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG;
  spim_config.sck_pin = SPI_SCK;
  spim_config.mosi_pin = SPI_MOSI;
  spim_config.miso_pin = SPI_MISO;
  spim_config.ss_pin = SPI_CS;
  spim_config.irq_priority = 0;
  spim_config.frequency = NRF_SPIM_FREQ_8M;
  spim_config.mode = NRF_SPIM_MODE_0;

  // Note: without a callback handler, transfers are blocking
  nrfx_spim_init(&SPIM_INST, &spim_config, NULL, NULL);
}

static void sample_timer_callback(void* _unused) {
  // Switch on desired mux input
  switch (last_checked) {
    case 0:
      nrf_gpio_pin_clear(IR_MUX_CTRL_0);
      nrf_gpio_pin_set(IR_MUX_CTRL_1);
      break;
    case 1:
      nrf_gpio_pin_clear(IR_MUX_CTRL_1);
      nrf_gpio_pin_set(IR_MUX_CTRL_2);
      break;
    case 2:
      nrf_gpio_pin_clear(IR_MUX_CTRL_2);
      nrf_gpio_pin_set(IR_MUX_CTRL_3);
      break;
    default:
      nrf_gpio_pin_clear(IR_MUX_CTRL_3);
      nrf_gpio_pin_set(IR_MUX_CTRL_0);
      break;
  }

  // Advance state
  last_checked = (last_checked + 1) & 3;

  // Read from ADC
  int16_t adc_val = adc_sample_blocking();
}

int main(void) {
  // Initialize GPIO
  gpio_init();

  // Initialize ADC
  adc_init();  
  
  // Initialize SPI
  spim_init();

  // Initialize app timer
  app_timer_init();
  app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED, sample_timer_callback);

  // Perform SPI transmission (CIPO)
  uint8_t rx_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t tx_buf[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  nrfx_spim_xfer_desc_t data_xfer = NRFX_SPIM_XFER_TRX(tx_buf, 8, rx_buf, 8);
  nrfx_spim_xfer(&SPIM_INST, &data_xfer, 0);

  // Start timer
  app_timer_start(sample_timer, 32768, NULL);

  // Enter main loop
  while (1) {
    // All actions done in timer callback
  }
}

