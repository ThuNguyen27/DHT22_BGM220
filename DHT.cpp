/*
 * DHT.cpp
 *
 *  Created on: Jun 24, 2023
 *      Author: nthu8
 */

#include <DHT.h>
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "em_gpio.h"
#include "ustimer.h"
#include "em_cmu.h"

DHT::DHT(GPIO_Port_TypeDef _port, uint8_t pin)
{
    this-> _port = _port;
    this-> _pin = _pin;
}
void DHT::init(GPIO_Port_TypeDef _port, uint8_t _pin){
    _lastreadtime = sl_sleeptimer_get_tick_count64();
    Ecode_t ustime_init = USTIMER_Init();
    this-> _port = _port;
    this-> _pin = _pin;
}
float DHT::readTemperature(bool force){
    float temp = 0;
      if (read(force))
      {
        temp = ((uint32_t)(data[2] & 0x7F)) << 8 | data[3];
        temp *= 0.1;
        if (data[2] & 0x80)
        {
          temp *= -1;
        }
      }
      return temp;
}
float DHT::readHumidity(bool force){
    float hm = 0;
      if (read(force))
      {
        hm = ((uint32_t)data[0]) << 8 | data[1];
        hm *= 0.1;
      }
      return hm;
}
uint32_t DHT::expectPulse(bool level)
{
  uint32_t count = 0;
  while (GPIO_PinInGet(_port, _pin) == level)
  {
    if(count++ >= 1000 * CMU_ClockFreqGet(cmuClock_SYSCLK))
    	return 0xffffffffu;
  }
  return count;
}
bool DHT::read(bool force){
    uint64_t currenttime = sl_sleeptimer_get_tick_count64();
    if(!force && ((currenttime - _lastreadtime) < 66000)){
        return _lastresult;
    }
    _lastreadtime = sl_sleeptimer_get_tick_count64();
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    GPIO_PinModeSet(_port, _pin, gpioModeInputPull, 1);
    ustimer = USTIMER_Delay(1000);
    GPIO_PinModeSet(_port, _pin, gpioModePushPull, 0);
    ustimer = USTIMER_Delay(1100);
    uint32_t cycles[80];
    {
        GPIO_PinModeSet(_port, _pin, gpioModeInputPull, 1);
        ustimer = USTIMER_DelayIntSafe(55);
        if(expectPulse(0) == 0xffffffffu){
        	_lastresult = false;
        	return _lastresult;
        }
        if(expectPulse(1) == 0xffffffffu){
        	_lastresult = false;
        	return _lastresult;
        }
        for (int i = 0; i < 80; i += 2){
            cycles[i] = expectPulse(0);
            cycles[i + 1] = expectPulse(1);
        }
    }
    for (int i = 0; i < 40; ++i)
    {
        uint32_t lowCycles = cycles[2 * i];
        uint32_t highCycles = cycles[2 * i + 1];
        if ((lowCycles == 0xffffffffu) || (highCycles == 0xffffffffu))
        {
        		_lastresult = false;
        		return _lastresult;
        }
        data[i / 8] <<= 1;
        if (highCycles > lowCycles)
        {
          data[i / 8] |= 1;
        }
    }
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)){
        _lastresult = true;
        return _lastresult;
    }
    else
    {
        _lastresult = false;
        return _lastresult;
    }
}


