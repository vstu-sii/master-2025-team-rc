#ifndef DATA_PAIRING_H
#define DATA_PAIRING_H

#include <Arduino.h>
#include "../config.h"
#include "../globals.h"

// Задача для формирования пар данных CAN
void taskDataPairing(void *pvParameters) {
    motor_data_t paired_data;
    TickType_t lastWakeTime = xTaskGetTickCount();
    uint32_t lastDebugTime = 0;
    uint32_t pairCount = 0;
    
    while (1) {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
        
        // Защищенная проверка наличия обоих значений
        if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
            bool both_valid = left_motor.valid && right_motor.valid;
            
            if (both_valid) {
                // Используем более позднюю временную метку
                paired_data.timestamp = (left_motor.timestamp > right_motor.timestamp) ? 
                                       left_motor.timestamp : right_motor.timestamp;
                paired_data.left_angle = left_motor.angle;
                paired_data.right_angle = right_motor.angle;
                
                // Сбрасываем флаги
                left_motor.valid = false;
                right_motor.valid = false;
                
                xSemaphoreGive(xDataMutex);
                
                if (recording_is_run) {
                    if (xQueueSend(dataPairQueue, &paired_data, pdMS_TO_TICKS(10)) != pdTRUE) {
                        Serial.println("Очередь данных переполнена!");
                        // Если очередь полна, ждем немного и пробуем снова
                        vTaskDelay(pdMS_TO_TICKS(5));
                    }
                }
            } else {
                xSemaphoreGive(xDataMutex);
            }
        }
        
        // Периодический статус
        // if (millis() - lastDebugTime > 10000) {
        //     int queueSize = uxQueueMessagesWaiting(dataPairQueue);
        //     Serial.printf("DataPairing Status: Pairs=%lu, Queue=%d/%d, Recording=%s\n",
        //                  pairCount, queueSize, DATA_PAIR_QUEUE_SIZE,
        //                  recording_is_run ? "YES" : "NO");
        //     lastDebugTime = millis();
        // }
    }
}

#endif