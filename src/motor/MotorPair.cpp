#include <motor/MotorPair.h>

MotorPair::MotorPair()
    : STBY(0), leftMotor(0, 0, 0, false), rightMotor(0, 0, 0, false)
{
    // Motors are initialized via setLeftMotorPins and setRightMotorPins
    // Motors default to stopped state
}

inline void MotorPair::turnLeft()
{
    // Turn left: left motor slower/reversed, right motor forward
    left().backward(80);
    getRightMotor().forward(100);
}

inline void MotorPair::turnRight()
{
    // Turn right: left motor forward, right motor slower/reversed
    left().forward(100);
    getRightMotor().backward(80);
}

inline void MotorPair::turn180()
{
    // 180 degree turn: motors in opposite directions at equal speed
    left().backward(100);
    getRightMotor().forward(100);
}

inline void MotorPair::spin()
{
    // Spin in place: motors in opposite directions at higher speed
    left().forward(150);
    getRightMotor().backward(150);
}
