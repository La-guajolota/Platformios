/**
 * @file main.cpp
 * @author Adrián Silva Palafox
 * @date April 17, 2025
 * @brief This program configures and uses the RMT (Remote Control) peripheral on an ESP32
 *        to generate pulse signals on three different channels. Each channel is connected
 *        to a specific GPIO pin, and the pulses are transmitted sequentially.
 */

#include <Arduino.h>
#include "driver/rmt.h"

// RMT Channels
#define RMT_CHANNEL_A RMT_CHANNEL_0  // RMT channel for step A
#define RMT_CHANNEL_B RMT_CHANNEL_1  // RMT channel for step B
#define RMT_CHANNEL_C RMT_CHANNEL_2  // RMT channel for step C

// GPIOs connected to each channel
#define PULSE_PIN_A GPIO_NUM_27  // GPIO pin for step A
#define PULSE_PIN_B GPIO_NUM_26  // GPIO pin for step B
#define PULSE_PIN_C GPIO_NUM_25  // GPIO pin for step C

const int N = 200;               // Number of pulses to generate
rmt_item32_t items[N];           // Array to store RMT pulse items

// Generic configuration for RMT channel A
rmt_config_t config_ch_A = {
  .rmt_mode       = RMT_MODE_TX,  // Set RMT to transmit mode
  .channel        = RMT_CHANNEL_A,    // Assign channel 0
  .gpio_num       = PULSE_PIN_A,  // GPIO pin for output
  .clk_div        = 80,           // Clock divider (80 MHz / 80 = 1 µs tick)
  .mem_block_num  = 1             // Memory block size (up to 64 symbols)
};

// Generic configuration for RMT channel B
rmt_config_t config_ch_B = {
  .rmt_mode       = RMT_MODE_TX,  
  .channel        = RMT_CHANNEL_B,    
  .gpio_num       = PULSE_PIN_B,  
  .clk_div        = 80,           
  .mem_block_num  = 1             
};

// Generic configuration for RMT channel C
rmt_config_t config_ch_C = {
  .rmt_mode       = RMT_MODE_TX,  
  .channel        = RMT_CHANNEL_C,    
  .gpio_num       = PULSE_PIN_C,  
  .clk_div        = 80,           
  .mem_block_num  = 1             
};

/**
 * @brief Setup function, runs once at the beginning.
 */
void setup() {
  Serial.begin(115200);  

  // Initialize the RMT pulse items with a high and low duration of 2000 ticks each
  for (int i = 0; i < N; i++) {
    items[i] = rmt_item32_t{{{2000, 1, 2000, 0}}};  // High for 2000 ticks, low for 2000 ticks
  }

  // Configure and install each RMT channel
  rmt_config(&config_ch_A);
  rmt_driver_install(config_ch_A.channel, 0, 0);  // Install driver for channel A
  rmt_config(&config_ch_B);
  rmt_driver_install(config_ch_B.channel, 0, 0);  // Install driver for channel B
  rmt_config(&config_ch_C);
  rmt_driver_install(config_ch_C.channel, 0, 0);  // Install driver for channel C
}

/**
 * @brief Main loop, runs repeatedly.
 */
void loop() {

  // Transmit 195 pulses on channel A without waiting for completion
  rmt_write_items(RMT_CHANNEL_A, items, 195, false);
  // Wait for the transmission to complete and print '0' during the wait
  do {
    Serial.print(0);
  } while (rmt_wait_tx_done(RMT_CHANNEL_A, 1000) == ESP_ERR_TIMEOUT);  // Wait for tx to be done, with a timeout
  Serial.println("Pulse transmission complete for channel A");

  // Transmit pulses on channels B and C and wait for completion
  rmt_write_items(RMT_CHANNEL_B, items, 4, true);
  rmt_write_items(RMT_CHANNEL_C, items, 9, true);
  delay(100);
}
