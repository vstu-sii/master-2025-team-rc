#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Пины
#define LED_OK_PIN 17
#define LED_REC_PIN 16
#define BTN_START_PIN 14
#define BTN_STOP_PIN 12
#define BATTERY_ADC_PIN 34
#define SCREEN_ADDRESS 0x3C
#define SD_CS_PIN 5

// CAN идентификаторы
#define LEFT_MOTOR_ID  0x34A
#define RIGHT_MOTOR_ID 0x34B
#define CAN_TX_PIN     GPIO_NUM_2
#define CAN_RX_PIN     GPIO_NUM_4

// Конфигурация буферизации
#define WRITE_BUFFER_SIZE 500
#define DATA_PAIR_QUEUE_SIZE 200

// BLE настройки
#define BLE_DEVICE_NAME "ESP32_EXO"
#define BLE_SERVICE_UUID "12345678-1234-1234-1234-123456789ABC"
#define BLE_CHARACTERISTIC_UUID "ABCDEF00-1234-1234-1234-123456789ABC"
#define BLE_DATA_CHARACTERISTIC_UUID "ABCDEF01-1234-1234-1234-123456789ABC"

#endif