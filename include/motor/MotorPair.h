
#include <stdint.h>
#include <motor/Motor.h>

#ifndef MOTOR_PAIR_H
#define MOTOR_PAIR_H

class MotorPair
{
private:
    uint8_t STBY;
    Motor leftMotor;
    Motor rightMotor;

public:
    // Constructor
    MotorPair();

    // SETTERS
    inline void setLeftMotorPins(uint8_t pwm, uint8_t pin1, uint8_t pin2, bool inverted)
    {
        leftMotor = Motor(pwm, pin1, pin2, inverted);
        left().motorInit();
    }

    inline void setRightMotorPins(uint8_t pwm, uint8_t pin1, uint8_t pin2, bool inverted)
    {
        rightMotor = Motor(pwm, pin1, pin2, inverted);
        getRightMotor().motorInit();
    }

    inline void setSTBYPin(uint8_t stby)
    {
        STBY = stby;
        pinMode(STBY, OUTPUT);
        digitalWrite(STBY, HIGH);
    }

    // GETTERS

    /// @brief returns a reference to internal left motor.
    /// @return
    inline Motor &left() { return leftMotor; }

    /// @brief returns a reference to internal right motor.
    /// @return
    inline Motor &right() { return rightMotor; }
    
    /// @brief returns a reference to internal right motor (deprecated, use right() instead).
    /// @return
    inline Motor &getRightMotor() { return rightMotor; }

    // MOVEMENT FUNCTIONS
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