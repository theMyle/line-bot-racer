#include <stdint.h>

#ifndef HARDWARE_MAPPING
#define HARDWARE_MAPPING

// --- MOTOR PINS ---
constexpr uint8_t MOTOR_A_PWM = 5;
constexpr uint8_t MOTOR_A_PIN_IN1 = 6;
constexpr uint8_t MOTOR_A_PIN_IN2 = 7;

constexpr uint8_t MOTOR_B_PWM = 9;
constexpr uint8_t MOTOR_B_PIN_IN1 = 10;
constexpr uint8_t MOTOR_B_PIN_IN2 = 11;

constexpr uint8_t STBY_PIN = A5;

// --- SENSOR PINS ---
constexpr uint8_t SENSOR_COUNT = 8;
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, A0, A1, A2, A3, A4};
constexpr uint8_t IR_PIN = 10;

#endif