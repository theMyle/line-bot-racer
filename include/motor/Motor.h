#include <stdint.h>
#include <Arduino.h>

#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
    uint8_t _pwm; // control motor speed (ANALOG OUT)
    uint8_t _in1; // direction control pin 1
    uint8_t _in2; // direction control pin 2
    bool _inverted;

public:
    Motor() {};

    inline void setMotorPins(uint8_t pwm, uint8_t pin1, uint8_t pin2, bool inverted)
    {
        _pwm = pwm;
        _in1 = pin1;
        _in2 = pin2;
        _inverted = inverted;
    }

    inline void motorInit()
    {
        pinMode(_in1, OUTPUT);
        pinMode(_in2, OUTPUT);
        pinMode(_pwm, OUTPUT);
    }

    inline void forward(const uint8_t speed)
    {
        digitalWrite(_in1, _inverted ? LOW : HIGH);
        digitalWrite(_in2, _inverted ? HIGH : LOW);
        analogWrite(_pwm, speed);
    }

    inline void backward(const uint8_t speed)
    {
        digitalWrite(_in1, _inverted ? HIGH : LOW);
        digitalWrite(_in2, _inverted ? LOW : HIGH);
        analogWrite(_pwm, speed);
    }

    inline void stop()
    {
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, LOW);
        analogWrite(_pwm, 0);
    }
};

#endif