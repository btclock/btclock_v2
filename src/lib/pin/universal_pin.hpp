#include <Arduino.h>

class UniversalPin
{
    public:
        UniversalPin(uint pinNumber);
        virtual void pinMode(uint8_t mode);
        virtual uint8_t digitalRead();
        virtual void digitalWrite(uint8_t value);
    protected:
        uint pinNumber;
};