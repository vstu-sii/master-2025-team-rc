#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <driver/twai.h>
#include <SD.h>
#include <Adafruit_SSD1306.h>
#include "GyverButton.h"
#include <NimBLEDevice.h>

// Внешние объявления глобальных переменных
extern volatile bool recording_is_run;
extern volatile bool server_recording;
extern volatile float battery_voltage;
extern volatile uint8_t battery_percent;

// BLE переменные
extern volatile bool ble_connected;
extern volatile char ble_last_command[32];
extern volatile char ble_client_address[32];

// Переменные для записи времени
extern volatile unsigned long recording_start_time;
extern volatile unsigned long recording_duration;
extern volatile char current_filename[64];

// Структуры
// Структуры для данных CAN
typedef struct {
    uint32_t timestamp;
    float left_angle;
    float right_angle;
} motor_data_t;

// Структуры для буферизации данных двигателей
typedef struct {
    float angle;
    uint32_t timestamp;
    bool valid;
} motor_buffer_t;

// Внешние объявления глобальных объектов
extern motor_buffer_t left_motor;
extern motor_buffer_t right_motor;
extern Adafruit_SSD1306 display;
extern GButton buttonStart;
extern GButton buttonStop;
extern File dataFile;

// BLE объекты
extern NimBLEServer* pServer;
extern NimBLECharacteristic* pRecordControlCharacteristic;
extern NimBLECharacteristic* pDataCharacteristic;

// Очереди и семафоры
extern QueueHandle_t dataPairQueue;
extern SemaphoreHandle_t xDataMutex;
extern SemaphoreHandle_t xSDMutex;
extern SemaphoreHandle_t xRecordMutex;
extern SemaphoreHandle_t xBLEMutex;

#endif