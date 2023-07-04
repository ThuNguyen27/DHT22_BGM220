/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <DHT.h>
#include "app.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "stdio.h"

#define DHT_PORT gpioPortC
#define DHT_PIN 7
DHT dht;
void app_init(void)
{
    dht.init(DHT_PORT, DHT_PIN);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    app_log_info("Temperature: %0.2f *C\n", temp);
    app_log_info("Humidity: %0.2f %c\n", humidity, 0x25);
    sl_sleeptimer_delay_millisecond(1000);
}
