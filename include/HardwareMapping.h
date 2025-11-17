#include <stdint.h>

#ifndef HARDWARE_MAPPING
#define HARDWARE_MAPPING

// --- MOTOR PINS ---
constexpr uint8_t MOTOR_A_PIN_IN1;
constexpr uint8_t MOTOR_A_PIN_IN2;
constexpr uint8_t MOTOR_A_PWM;

constexpr uint8_t MOTOR_B_PIN_IN1;
constexpr uint8_t MOTOR_B_PIN_IN2;
constexpr uint8_t MOTOR_B_PWM;

// --- SENSOR PINS ---
constexpr uint8_t SENSOR_COUNT = 8;
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9};

#endif