


void init_spi(void){

}

//init game switch, kill switch, and sound buttons as interrupts
void init_button_interrutps(){
    pinMode(GAME_BTN, INPUT);
    pinMode(KILL_BTN, INPUT);
    pinMode(SOUND_BTN, INPUT);
    //all falling edge, hardware pullups
    attachInterupt(GAME_BTN, game_switch_handler(), FALLING);
    attachInterupt(KILL_BTN, kill_switch_handler(), FALLING);
    attachInterupt(SOUND_BTN, sound_btn_handler(), FALLING);
}

