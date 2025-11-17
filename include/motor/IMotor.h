#include <stdint.h>

#ifndef IMOTOR_H
#define IMOTOR_H

class IMotor
{
public:
    virtual void forward(const uint8_t speed) = 0;
    virtual void backward(const uint8_t speed) = 0;
    virtual void stop() = 0;
    virtual ~IMotor() = default;
};

#endif