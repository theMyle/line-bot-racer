#include <stdint.h>

#ifndef HARDWARE_MAPPING
#define HARDWARE_MAPPING

// --- MOTOR PINS ---
// Left motor (Motor A)
constexpr uint8_t MOTOR_A_PWM = 5;       // D5 -> PWMA
constexpr uint8_t MOTOR_A_PIN_IN1 = 6;   // D6 -> AIN1
constexpr uint8_t MOTOR_A_PIN_IN2 = 7;   // D7 -> AIN2

// Right motor (Motor B)
constexpr uint8_t MOTOR_B_PWM = 9;       // D9 -> PWMB
constexpr uint8_t MOTOR_B_PIN_IN1 = 10;  // D10 -> BIN1
constexpr uint8_t MOTOR_B_PIN_IN2 = 11;  // D11 -> BIN2

constexpr uint8_t STBY_PIN = 19;  // A5 (pin 19) -> STBY on TB6612

// --- SENSOR PINS ---
constexpr uint8_t SENSOR_COUNT = 8;
// D2, D3, D4, A0, A1, A2, A3, A4 = pins 2, 3, 4, 14, 15, 16, 17, 18
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, 14, 15, 16, 17, 18};
constexpr uint8_t IR_PIN = 12;  

#endif