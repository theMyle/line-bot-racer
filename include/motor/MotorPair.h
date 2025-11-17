
#include <stdint.h>
#include <motor/IMotorPair.h>
#include <motor/IMotor.h>

#ifndef MOTOR_PAIR_H
#define MOTOR_PAIR_H

class MotorPair : public IMotorPair
{
private:
    IMotor &leftMotor;
    IMotor &rightMotor;

public:
    MotorPair(IMotor &left, IMotor &right)
        : leftMotor(left), rightMotor(right) {};

    inline void forward(uint8_t speed)
    {
        leftMotor.forward(speed);
        rightMotor.forward(speed);
    };

    inline void backward(uint8_t speed)
    {
        leftMotor.forward(speed);
        rightMotor.backward(speed);
    };

    inline void stop()
    {
        leftMotor.stop();
        rightMotor.stop();
    }
};

#endif