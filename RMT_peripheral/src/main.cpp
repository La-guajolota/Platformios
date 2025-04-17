/*
 * Author: Adrián Silva Palafox
 * Date: April 17, 2025
 * Description: This program configures and uses the RMT (Remote Control) peripheral on an ESP32
 *              to generate pulse signals on three different channels. Each channel is connected
 *              to a specific GPIO pin, and the pulses are transmitted sequentially.
 * 
 */

#include <Arduino.h>
#include "driver/rmt.h"

// RMT CHANNELS
#define stepA_rmt RMT_CHANNEL_0  // RMT channel for step A
#define stepB_rmt RMT_CHANNEL_1  // RMT channel for step B
#define stepC_rmt RMT_CHANNEL_2  // RMT channel for step C

// GPIOs connected to each channel
#define PULSE_PIN_A GPIO_NUM_26  // GPIO pin for step A
#define PULSE_PIN_B GPIO_NUM_23  // GPIO pin for step B
#define PULSE_PIN_C GPIO_NUM_25  // GPIO pin for step C

const int N = 200;               // Number of pulses to generate
rmt_item32_t items[N];           // Array to store RMT pulse items

// Generic configuration for RMT channel A
rmt_config_t stepA_cfg = {
  .rmt_mode       = RMT_MODE_TX,  // Set RMT to transmit mode
  .channel        = stepA_rmt,    // Assign channel 0
  .gpio_num       = PULSE_PIN_A,  // GPIO pin for output
  .clk_div        = 80,           // Clock divider (80 MHz / 80 = 1 µs tick)
  .mem_block_num  = 1             // Memory block size (up to 64 symbols)
};

// Generic configuration for RMT channel B
rmt_config_t stepB_cfg = {
  .rmt_mode       = RMT_MODE_TX,  
  .channel        = stepB_rmt,    
  .gpio_num       = PULSE_PIN_B,  
  .clk_div        = 80,           
  .mem_block_num  = 1             
};

// Generic configuration for RMT channel C
rmt_config_t stepC_cfg = {
  .rmt_mode       = RMT_MODE_TX,  
  .channel        = stepC_rmt,    
  .gpio_num       = PULSE_PIN_C,  
  .clk_div        = 80,           
  .mem_block_num  = 1             
};

void setup() {
  Serial.begin(115200);  

  // Initialize the RMT pulse items with a high and low duration of 2000 ticks each
  for (int i = 0; i < N; i++) {
    items[i] = rmt_item32_t{2000, 1, 2000, 0};  // High for 2000 ticks, low for 2000 ticks
  }

  // Configure and install each RMT channel
  rmt_config(&stepA_cfg);
  rmt_driver_install(stepA_cfg.channel, 0, 0);  // Install driver for channel A
  rmt_config(&stepB_cfg);
  rmt_driver_install(stepB_cfg.channel, 0, 0);  // Install driver for channel B
  rmt_config(&stepC_cfg);
  rmt_driver_install(stepC_cfg.channel, 0, 0);  // Install driver for channel C
}

void loop() {

  // Transmit 195 pulses on channel A without waiting for completion
  rmt_write_items(stepA_rmt, items, 195, false);
  // Wait for the transmission to complete and print '0' during the wait
  do {
    Serial.print(0);
  } while (rmt_wait_tx_done(stepA_rmt, 1000));  // Timeout of 1000 ms
  Serial.println("Pulse transmission complete for channel A");

  rmt_write_items(stepB_rmt, items, 4, true);
  rmt_write_items(stepC_rmt, items, 9, true);
  delay(100);
}