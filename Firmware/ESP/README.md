ESP controls LEDs under the table and the scoreboard

nrf52833 sends the game state over spi and the ESP responds accordingly

the ESP has the game mode switch button which it will communicate if pressed to the nrf

the two halfs of the table communicate via the ESP of each board being connected with UART