#include <Arduino.h>
#include <driver/pcnt.h>
#include <esp_timer.h>

esp_timer_handle_t timer_handle;
void IRAM_ATTR timer_isr(void *arg);

// Define the PCNT unit to use
const pcnt_unit_t unit = PCNT_UNIT_0;
int16_t count = 0;

// Define the input GPIO pin for counting
const gpio_num_t inputPin = GPIO_NUM_4;

void setup()
{
  // Create timer with 1s interval
  esp_timer_create_args_t timer_config = {
      .callback = timer_isr,
      .arg = NULL,
      .name = "my_timer"};
  esp_timer_create(&timer_config, &timer_handle);
  esp_timer_start_periodic(timer_handle, 10000);

  // Configure the input pin as an input
  Serial.begin(115200);
  pinMode(inputPin, INPUT);

  // Initialize the PCNT unit
  pcnt_config_t config = {
      .pulse_gpio_num = inputPin,
      .ctrl_gpio_num = PCNT_PIN_NOT_USED,
      .lctrl_mode = PCNT_MODE_KEEP,
      .hctrl_mode = PCNT_MODE_KEEP,
      .pos_mode = PCNT_COUNT_INC,
      .neg_mode = PCNT_COUNT_DIS,
      .counter_h_lim = 0,
      .counter_l_lim = 0,
      .unit = unit,
      .channel = PCNT_CHANNEL_0,
  };
  pcnt_unit_config(&config);

  pcnt_counter_pause(PCNT_UNIT_0);  // Pause PCNT unit 0
  pcnt_counter_clear(PCNT_UNIT_0);  // Clear PCNT unit 0 counter value
  pcnt_counter_resume(PCNT_UNIT_0); // Resume PCNT unit 0
}

void loop()
{
}

void IRAM_ATTR timer_isr(void *arg)
{
  pcnt_get_counter_value(unit, &count);
  Serial.println(100 * count);

  pcnt_counter_pause(PCNT_UNIT_0); // Pause PCNT unit 0
  pcnt_counter_clear(PCNT_UNIT_0); // Clear PCNT unit 0 counter value
  pcnt_counter_resume(PCNT_UNIT_0);
}
