
#include <stdint.h>
#include <motor/Motor.h>

#ifndef MOTOR_PAIR_H
#define MOTOR_PAIR_H

class MotorPair
{
private:
    Motor &leftMotor;
    Motor &rightMotor;
    uint8_t STBY;

public:
    MotorPair(Motor &leftMotor, Motor &rightMotor, uint8_t STBY_PIN) : leftMotor(leftMotor), rightMotor(rightMotor), STBY(STBY_PIN) {}

    inline void initMotors()
    {
        leftMotor.motorInit();
        rightMotor.motorInit();
        pinMode(STBY, OUTPUT);
        digitalWrite(STBY, HIGH);
    };

    inline void forward(uint8_t speed)
    {
        leftMotor.forward(speed);
        rightMotor.forward(speed);
    }

    inline void backward(uint8_t speed)
    {
        leftMotor.backward(speed);
        rightMotor.backward(speed);
    }

    inline void stop()
    {
        leftMotor.stop();
        rightMotor.stop();
    }

    inline void turnLeft();
    inline void turnRight();
    inline void turn180();
    inline void spin();
};

#endif