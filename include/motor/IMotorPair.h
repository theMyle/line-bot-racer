#include <stdint.h>
#include <motor/Motor.h>

#ifndef IMOTOR_PAIR_H
#define IMOTOR_PAIR_H

class IMotorPair
{
public:
    virtual void forward(uint8_t speed) = 0;
    virtual void backward(uint8_t speed) = 0;
    virtual void stop() = 0;
    virtual ~IMotorPair() = default;
};

#endif