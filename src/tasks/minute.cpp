#include "minute.hpp"

TaskHandle_t minuteTaskHandle = NULL;
// Define a type for the event callback
std::vector<EventCallback> minuteEventCallbacks; // Define a vector to hold multiple event callbacks
bool eventTriggered = false;                     // Initialize the event triggered flag to false
const int usPerMinute = 60 * 1000000;

void minuteTask(void *parameter)
{

  esp_timer_handle_t minuteTimer;
  const esp_timer_create_args_t minuteTimerConfig = {
      .callback = &minuteTimerISR,
      .name = "minute_timer"};

  esp_timer_create(&minuteTimerConfig, &minuteTimer);

  time_t currentTime;
  struct tm timeinfo;
  time(&currentTime);
  localtime_r(&currentTime, &timeinfo);
  uint32_t secondsUntilNextMinute = 60 - timeinfo.tm_sec;

  if (secondsUntilNextMinute > 0)
    vTaskDelay(pdMS_TO_TICKS((secondsUntilNextMinute * 1000)));

  esp_timer_start_periodic(minuteTimer, usPerMinute);

  while (1)
  {
    for (auto &callback : minuteEventCallbacks)
    {
      callback();
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
}

void IRAM_ATTR minuteTimerISR(void *arg)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(minuteTaskHandle, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE)
  {
    portYIELD_FROM_ISR();
  }
}

void setupMinuteEvent()
{
  xTaskCreate(minuteTask, "MinuteTask", 4096, NULL, 1, &minuteTaskHandle); // Create the FreeRTOS task
}

void registerNewMinuteCallback(const EventCallback cb)
{
  minuteEventCallbacks.push_back(cb);
}