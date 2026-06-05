// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define IR_MUX_CTRL_2 	NRF_GPIO_PIN_MAP(0, 20)

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(IR_MUX_CTRL_2);

  // Enter main loop.
  while (1) {
    nrf_gpio_pin_toggle(IR_MUX_CTRL_2);
    nrf_delay_ms(2000);
  }
}

