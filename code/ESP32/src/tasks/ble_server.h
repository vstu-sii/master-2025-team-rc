#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <string.h>  // Для strncpy
#include "../config.h"
#include "../globals.h"
#include "../utils/recording.h"

/** Callbacks сервера BLE */
class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        Serial.printf("BLE Client connected: %s\n", connInfo.getAddress().toString().c_str());
        
        // Обновляем данные для дисплея
        if (xSemaphoreTake(xBLEMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            ble_connected = true;
            // Используем strncpy для копирования строки
            strncpy((char*)ble_client_address, connInfo.getAddress().toString().c_str(), 31);
            ble_client_address[31] = '\0';  // Гарантируем нулевое окончание
            xSemaphoreGive(xBLEMutex);
        }

        // Устанавливаем параметры соединения
        pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        Serial.printf("BLE Client disconnected - start advertising\n");

        // Обновляем данные для дисплея
        if (xSemaphoreTake(xBLEMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            ble_connected = false;
            ble_client_address[0] = '\0';  // Очищаем строку
            xSemaphoreGive(xBLEMutex);
        }
        
        // Перезапускаем рекламу
        NimBLEDevice::startAdvertising();
    }
};

/** Handler для характеристики управления записью */
class RecordControlCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
        String command = pCharacteristic->getValue().c_str();
        command.toLowerCase(); // Приводим к нижнему регистру для удобства сравнения
        Serial.printf("BLE Command received: %s\n", command.c_str());
        
        // Обновляем данные для дисплея
        if (xSemaphoreTake(xBLEMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Используем strncpy для копирования строки
            strncpy((char*)ble_last_command, command.c_str(), 31);
            ble_last_command[31] = '\0';  // Гарантируем нулевое окончание
            xSemaphoreGive(xBLEMutex);
        }
        
        // Обрабатываем команды
        if (command == "start" || command == "start recording" || command == "1") {
            Serial.println("BLE: Starting recording...");
            startRecording();
        } 
        else if (command == "stop" || command == "stop recording" || command == "0") {
            Serial.println("BLE: Stopping recording...");
            stopRecording();
        }
        else {
            Serial.printf("BLE: Unknown command: %s\n", command.c_str());
        }
        
        // Отправляем подтверждение обратно клиенту
        String response = "CMD: " + command + " - ";
        response += recording_is_run ? "Recording ACTIVE" : "Recording STOPPED";
        pCharacteristic->setValue(response.c_str());
        pCharacteristic->notify();
    }

    void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) override {
        Serial.printf("BLE Client subscribed to record control\n");
    }
};

// Задача для BLE сервера
void taskBLEServer(void *pvParameters) {
    Serial.println("Starting BLE Server...");

    // Инициализация BLE
    NimBLEDevice::init(BLE_DEVICE_NAME);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); // Максимальная мощность передачи
    
    // Создаем сервер и устанавливаем колбэки
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Создаем сервис для управления записью
    NimBLEService* pRecordService = pServer->createService(BLE_SERVICE_UUID);
    
    // Создаем характеристику для управления записью
    pRecordControlCharacteristic = pRecordService->createCharacteristic(
        BLE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | 
        NIMBLE_PROPERTY::WRITE |
        NIMBLE_PROPERTY::WRITE_NR |
        NIMBLE_PROPERTY::NOTIFY
    );

    pRecordControlCharacteristic->setValue("Ready for commands");
    pRecordControlCharacteristic->setCallbacks(new RecordControlCallbacks());

    // Создаем характеристику для передачи данных
    pDataCharacteristic = pRecordService->createCharacteristic(
        BLE_DATA_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | 
        NIMBLE_PROPERTY::NOTIFY
    );

    pDataCharacteristic->setValue("System Data");

    // Запускаем сервис
    pRecordService->start();

    // Настраиваем рекламу
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(BLE_DEVICE_NAME);
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    pAdvertising->setAppearance(0x0000); // Generic appearance
    
    // Устанавливаем параметры рекламы
    pAdvertising->setMinInterval(400);   // 400 * 0.625ms = 250ms
    pAdvertising->setMaxInterval(800);   // 800 * 0.625ms = 500ms
    pAdvertising->start();

    Serial.printf("BLE Server Started: %s\n", BLE_DEVICE_NAME);
    Serial.printf("Service UUID: %s\n", BLE_SERVICE_UUID);
    Serial.printf("Characteristic UUID: %s\n", BLE_CHARACTERISTIC_UUID);
    Serial.println("Send 'start' or 'stop' commands to control recording");
    
    // Основной цикл задачи BLE
    while (1) {
        // Просто поддерживаем соединение, периодических уведомлений не отправляем
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#endif