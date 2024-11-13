#if defined(ESP32)

#include <SmartLadderEsp32.h>

#include "proccess/settings/esp32.h"

#if USE_ETH8720 || USE_WIFI
#include "communication/connection/esp32.h"
#endif

#include "communication/esp32.h"
#include "Generic.h"
#include "proccess/memory/esp32.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

void SmartLadderEsp32::setup()
{
  Serial.begin(115200);
  delay(100);

  xTaskCreatePinnedToCore(
      [](void *pvParameters)
      {
        static_cast<SmartLadderEsp32 *>(pvParameters)->Task1code(pvParameters);
      },
      "Task1",
      25000,
      this,
      1,
      &Task1,
      0);

  xTaskCreatePinnedToCore(
      [](void *pvParameters)
      {
        static_cast<SmartLadderEsp32 *>(pvParameters)->Task2code(pvParameters);
      },
      "Task2",
      25000,
      this,
      1,
      &Task2,
      1);
  delay(500);
}

void SmartLadderEsp32::loop() {}

void SmartLadderEsp32::Task1code(void *pvParameters)
{
  loadConfig();
  for (;;)
  {
    if (DT_SV.length())
      ctrlDtSv();

    if (_TPR == 0 && _ST == 1)
      setStatusResource();

#if USE_ETH8720 || USE_WIFI
    connectLoop();
#endif

    vTaskDelay(1);

    while (Serial.available())
    {
      ORIGIN_ = 1;
      receiver(Serial.read());
    }
  }
}

void SmartLadderEsp32::Task2code(void *pvParameters)
{
  loadPort();
  loadProgram();
  for (;;)
  {
    start();
  }
}

#endif