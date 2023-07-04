/*
 * DHT.h
 *
 *  Created on: Jun 24, 2023
 *      Author: nthu8
 */

#ifndef DHT_H_
#define DHT_H_

#include <stdint.h>
#include <em_gpio.h>
#include "ustimer.h"
class DHT
{
private:
    GPIO_Port_TypeDef _port;
    uint8_t data[5];
    uint8_t _pin;
    uint64_t _lastreadtime = 0;
    bool _lastresult = false;
    Ecode_t ustimer = 0;

public:
    DHT(){}
    DHT(GPIO_Port_TypeDef _port, uint8_t _pin);
    void init(GPIO_Port_TypeDef _port, uint8_t _pin);
    float readTemperature(bool force = false);
    float readHumidity(bool force = false);
    bool read(bool force = false);
    uint32_t expectPulse(bool level);
};
#endif /* DHT_H_ */
