#include <Arduino.h>
#include "config.h"
#include "globals.h"

// Подключение заголовочных файлов задач
#include "tasks/can_receiver.h"
#include "tasks/data_pairing.h"
#include "tasks/sd_writer.h"
#include "tasks/ui_handler.h"
#include "tasks/system_monitor.h"
#include "tasks/ble_server.h"

// Подключение утилит
#include "utils/pins.h"
#include "utils/can_utils.h"
#include "utils/sd_utils.h"
#include "utils/display_utils.h"
#include "utils/recording.h"

// Определение глобальных переменных
volatile bool recording_is_run = false;
volatile bool server_recording = false;
volatile float battery_voltage = 0.0;
volatile uint8_t battery_percent = 0;

// BLE переменные
volatile bool ble_connected = false;
volatile char ble_last_command[32] = {0};
volatile char ble_client_address[32] = {0};

// Переменные для записи времени
volatile unsigned long recording_start_time = 0;
volatile unsigned long recording_duration = 0;
volatile char current_filename[64] = {0};

// Глобальные буферы для данных двигателей
motor_buffer_t left_motor = {0, 0, false};
motor_buffer_t right_motor = {0, 0, false};

// Определение глобальных объектов
Adafruit_SSD1306 display(128, 64, &Wire, -1);
GButton buttonStart(BTN_START_PIN);
GButton buttonStop(BTN_STOP_PIN);
File dataFile;

// BLE объекты
NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pRecordControlCharacteristic = nullptr;
NimBLECharacteristic* pDataCharacteristic = nullptr;

// Определение очередей и семафоров
QueueHandle_t dataPairQueue;
SemaphoreHandle_t xDataMutex;
SemaphoreHandle_t xSDMutex;
SemaphoreHandle_t xRecordMutex;
SemaphoreHandle_t xBLEMutex;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("ESP32-TWAI FreeRTOS Starting...");
  
  initPins();
  
  if (!initDisplay()) {
    Serial.println("Display initialization failed!");
  }
  
  if (!initSDCard()) {
    Serial.println("SD card initialization failed!");
  }
  
  if (!initCAN()) {
    Serial.println("CAN initialization failed!");
  }

  // Инициализация очередей и семафоров
  dataPairQueue = xQueueCreate(DATA_PAIR_QUEUE_SIZE, sizeof(motor_data_t));
  xDataMutex = xSemaphoreCreateMutex();
  xSDMutex = xSemaphoreCreateMutex();
  xRecordMutex = xSemaphoreCreateMutex();
  xBLEMutex = xSemaphoreCreateMutex();

  // Проверка создания объектов FreeRTOS
  if (dataPairQueue == NULL || xDataMutex == NULL || xSDMutex == NULL || xRecordMutex == NULL || xBLEMutex == NULL) {
    Serial.println("ERROR: Failed to create FreeRTOS objects!");
    while(1) delay(1000);
  }

  // Создание задач
  xTaskCreate(taskCANReceiver, "CANReceiver", 8192, NULL, 8, NULL);     // Самый высокий приоритет
  xTaskCreate(taskDataPairing, "DataPairing", 8192, NULL, 7, NULL);     // Высокий приоритет
  xTaskCreate(taskSDWriter, "SDWriter", 12288, NULL, 6, NULL);          // Высокий приоритет 
  xTaskCreate(taskBLEServer, "BLEServer", 8192, NULL, 5, NULL);         // BLE сервер (средний приоритет)
  xTaskCreate(taskUIHandler, "UIHandler", 4096, NULL, 2, NULL);         // Низкий приоритет
  xTaskCreate(taskSystemMonitor, "SystemMonitor", 8192, NULL, 1, NULL); // Самый низкий

  Serial.println("FreeRTOS tasks started successfully");
  vTaskDelete(NULL);
}

void loop() {
  vTaskDelete(NULL);
}