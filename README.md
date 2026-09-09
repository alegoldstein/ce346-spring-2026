# Electronic Game Table
# ce346-spring-2026

Firmware and PCB design for COMP_ENG 346: Microcontroller System Design.

For this project my group created an electronic table that added animations and score tracking to the common college games of beer pong and beer die.

To do so, we CNC'd a wooden table with channels to pour epoxy and placed ~800 WS2812B LEDs. Each half of the table had its own PCB (which are identical) which was responsible for monitoring the game state and controlling the LEDs accordingly using the FastLED library. Lastly, the power for this project was a large challenge because of the high current draw that WS2812B LEDs are capable of. To avoid overheating, we never had LEDs on at full brightness and avoided the color white as much as possible. The power source we used was capable of supplying 60A at 5V so monitoring shorts was also a high priority. We used bus bars that were suited for handling 60A and separated power and ground with large margins to avoid any accidental shorts. To ensure a uniform brightness level we added power injections to the LED strips with carefuly planned channels cut around in the available space left around the epoxy. 

I hand wrote the game state code for an NRF52833 chip that used a 16 bit variable where bits 0-11 each represented 1 of the 12 cups on a half of a board (10 for pong and 2 for die). 
