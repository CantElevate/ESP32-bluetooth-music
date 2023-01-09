#include <Adafruit_SSD1306.h>
#include "BluetoothA2DPSink.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13 // pin number is specific to your esp32 board
#endif

#define OLED_RESET -1

const char sinkName[] = "MyMusic";

Adafruit_SSD1306 display(OLED_RESET);
BluetoothA2DPSink a2dp_sink;
esp_a2d_connection_state_t last_state;
uint16_t minutes = 5;
unsigned long shutdown_ms = millis() + 1000 * 60 * minutes;

char track[50];
char artist[50];

const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 44100, // corrected by info from bluetooth
    .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
    .channel_format =  I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
};

void removeSpecialCharacters(char *str) {
  int i = 0, j = 0;
  char ch;
  while ((ch = str[i++]) != '\0') {
    if (ch >= 32 && ch <= 126) {
      str[j++] = ch;
    }
  }
  str[j] = '\0';
}

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
Serial.printf("AVRC metadata rsp: attribute id 0x%x, %s\n", data1, data2);
  if (data1 == 0x1) { // track name
  strcpy(track, (char*)data2);
  removeSpecialCharacters(track);
  } else if (data1 == 0x2) { // artist name
  strcpy(artist, (char*)data2);
  removeSpecialCharacters(artist);  
  }
} 

void setup() {
  Serial.begin(115200);
// LED for connection
pinMode(LED_BUILTIN, OUTPUT);
// Initialize the OLED display
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("Initializing...");
display.display();  
a2dp_sink.set_i2s_config(i2s_config);  
a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
a2dp_sink.start(sinkName);  

}

void loop() {

   if (millis()>shutdown_ms){
    // stop the processor
    Serial.println("Shutting down");
    esp_deep_sleep_start();
  }
  // Check if there are any incoming Bluetooth connections
    // Clear the display
    display.clearDisplay();
    // Set the text size and color
    display.setTextSize(1);
    display.setTextColor(WHITE);
    // Set the cursor position and print the track and artist information
    display.setCursor(0,0);
    display.println(track);
    display.println(artist);
    // Update the display
    display.display();
    if(strlen(track) >= 15){
    display.startscrollright(0x00, 0x00);
    } else {
      display.stopscroll();
    }
    // check state
  esp_a2d_connection_state_t state = a2dp_sink.get_connection_state();
  if (last_state != state){
    bool is_connected = state == ESP_A2D_CONNECTION_STATE_CONNECTED;
    Serial.println(is_connected ? "Connected" : "Not connected");    
    digitalWrite(LED_BUILTIN, is_connected);
    last_state = state;
  }
  delay(1000);
  }