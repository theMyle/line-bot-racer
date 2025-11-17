
#include <stdint.h>
#include <motor/Motor.h>

#ifndef MOTOR_PAIR_H
#define MOTOR_PAIR_H

class MotorPair
{
private:
    Motor leftMotor;
    Motor rightMotor;

public:
    // constructs motor pair from two individual motor configs (PINS).
    //
    // first 4 params - LEFT_MOTOR {PWM, IN1, IN2, IS_INVERTED_FLAG}.
    // last 4 params - RIGHT_MOTOR {PWM, IN1, IN2, IS_INVERTED_FLAG}.
    MotorPair(
        uint8_t l_pwm, uint8_t l_in1, uint8_t l_in2, bool l_inverted,
        uint8_t r_pwm, uint8_t r_in1, uint8_t r_in2, bool r_inverted)
        : leftMotor(l_pwm, l_in1, l_in2, l_inverted),
          rightMotor(r_pwm, r_in1, r_in2, r_inverted)
    {
    }

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