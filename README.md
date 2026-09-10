# Electronic Game Table
# ce346-spring-2026

Firmware and PCB design for COMP_ENG 346: Microcontroller System Design.

For this project my group created an electronic table that added animations and score tracking to the common college games of beer pong and beer die.

To do so, we CNC'd a wooden table with channels to pour epoxy and placed ~800 WS2812B LEDs. Each half of the table had its own PCB (which are identical) which was responsible for monitoring the game state and controlling the LEDs accordingly using the FastLED library. Lastly, the power for this project was a large challenge because of the high current draw that WS2812B LEDs are capable of. To avoid overheating, we never had LEDs on at full brightness and avoided the color white as much as possible. The power source we used was capable of supplying 60A at 5V so monitoring shorts was also a high priority. We used bus bars that were suited for handling 60A and separated power and ground with large margins to avoid any accidental shorts. To ensure a uniform brightness level we added power injections to the LED strips with carefuly planned channels cut around in the available space left around the epoxy. 

<table align="center">
  <tr>
    <td align="center">
      <img src="Photos/group.heic" width="300"><br>
      <em>The three of us pouring epoxy</em>
    </td>
    <td align="center">
      <img src="Photos/pcb.heic" width="300"><br>
      <em>Fully assembled PCB</em>
    </td>
  </tr>
  
</table>

I hand wrote the game state code in C for an NRF52833 chip (found in NRF->software->apps->rave_rack) that used a 16 bit variable where bits 0-11 each represented 1 of the 12 cups on a half of a board (10 for pong and 2 for die). The NRF chip was also responsible for cycling through an 16 channel analog multiplexer used to check the values of IR sensors placed beneath each cup to determine whether they had been picked up or not. This information can be used to update the game state and infer the score (more cups missing means other teams score goes up). The NRF chip then communicates the game state to an ESP32 which was responsible for controlling the LEDs.

I also wrote C++ code for the ESP32 to animate the LEDs in response to game state changes. For example, during initialization the LEDs would snake around the border to indicate it was turned on and ready. If a cup was lifted (meaning someone scored) a celebration animation would occur. The FastLED library made this simpler since all that was needed was a buffer with each color and brightness we wanted each LED.

The reasoning for having 2 separate MCUs is due to the class being based on the NRF52833 chip, but our group running out of pins since we had a larger scope than expected for a 4 week project. 
