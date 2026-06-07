//all pin definitinos except leds
#define PDM_DATA    26
#define PDM_CLK     27
#define BCLK        14
#define LRCL        13
#define SPI_MISO    23
#define SPI_MOSI    19
#define SPI_CS      5
#define SPI_SCK     18
#define DOUT        22
#define UART2_TX    21
#define UART2_RX    34
#define GAME_BTN    35
#define SOUND_BTN   39 //VN
#define KILL_BTN    36 //VP


void init_uart(void);

void init_spi(void);

void init_scoreboard();

//init game switch and kill switch buttons as interrupts
void init_button_interrutps();

//callback handler for kill switch button
void kill_switch_handler();

//handler for when game switch button is pressed
void game_swtich_handler();

//handle sound reactive button press
void sound_btn_handler();
