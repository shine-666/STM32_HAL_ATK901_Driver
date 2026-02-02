

#ifndef __ATM_MS901M_H
#define __ATM_MS901M_H

#include "main.h"

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} atk_ms901m_attitude_data_t;

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
} atk_ms901m_quaternion_data_t;

typedef struct
{
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
    } raw;
    float x;
    float y;
    float z;
} atk_ms901m_gyro_data_t;

typedef struct
{
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
    } raw;
    float x;
    float y;
    float z;
} atk_ms901m_accelerometer_data_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
    float temperature;
} atk_ms901m_magnetometer_data_t;

typedef struct
{
    int32_t pressure;
    int32_t altitude;
    float temperature;
} atk_ms901m_barometer_data_t;

extern atk_ms901m_attitude_data_t attitude_dat;
extern atk_ms901m_quaternion_data_t quaternion_dat;
extern atk_ms901m_gyro_data_t gyro_dat;
extern atk_ms901m_accelerometer_data_t accelerometer_dat;
extern atk_ms901m_magnetometer_data_t magnetometer_dat;
extern atk_ms901m_barometer_data_t barometer_dat;

void set_atk_ms901m_fsr(uint8_t gyro_fsr, uint8_t accelerometer_fsr);
uint8_t atk_ms901m_data_parsing(uint8_t *ms901m_command, uint8_t ms901m_commandlength);
uint8_t atk_ms901m_get_attitude(uint8_t *ms901m_command, atk_ms901m_attitude_data_t *attitude_dat);
uint8_t atk_ms901m_get_quaternion(uint8_t *ms901m_command, atk_ms901m_quaternion_data_t *quaternion_dat);
uint8_t atk_ms901m_get_gyro_accelerometer(uint8_t *ms901m_command, atk_ms901m_gyro_data_t *gyro_dat, atk_ms901m_accelerometer_data_t *accelerometer_dat);
uint8_t atk_ms901m_get_magnetometer(uint8_t *ms901m_command, atk_ms901m_magnetometer_data_t *magnetometer_dat);
uint8_t atk_ms901m_get_barometer(uint8_t *ms901m_command, atk_ms901m_barometer_data_t *barometer_dat);

#endif
