#include <stdint.h>

#ifndef HARDWARE_MAPPING
#define HARDWARE_MAPPING

// --- MOTOR PINS ---
constexpr uint8_t MOTOR_A_PIN_IN1 = 5;
constexpr uint8_t MOTOR_A_PIN_IN2 = 6;
constexpr uint8_t MOTOR_A_PWM = 7;

constexpr uint8_t MOTOR_B_PIN_IN1 = 9;
constexpr uint8_t MOTOR_B_PIN_IN2 = 10;
constexpr uint8_t MOTOR_B_PWM = 11;

constexpr uint8_t STBY_PIN;

// --- SENSOR PINS ---
constexpr uint8_t SENSOR_COUNT = 8;
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, 14, 15, 16, 17, 18};
constexpr uint8_t IR_PIN = 10;

#endif