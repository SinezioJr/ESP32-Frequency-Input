#include <Arduino.h>
#include <esp_timer.h>

esp_timer_handle_t timer_handle;
void IRAM_ATTR timer_isr(void *arg);

#define LED 2
bool pinState = LOW;

void setup()
{
  // Create timer with 1s interval
  esp_timer_create_args_t timer_config = {
      .callback = timer_isr,
      .arg = NULL,
      .name = "my_timer"};
  esp_timer_create(&timer_config, &timer_handle);
  esp_timer_start_periodic(timer_handle, 1000000);

  pinMode(LED, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void IRAM_ATTR timer_isr(void *arg)
{
  pinState = !pinState;
  digitalWrite(LED, pinState);
}
