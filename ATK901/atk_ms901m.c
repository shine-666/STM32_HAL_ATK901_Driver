#include "atk_ms901m.h"
#include "main.h"
#include "usart.h"
#include "app_azure_rtos.h"

atk_ms901m_attitude_data_t attitude_dat;
atk_ms901m_quaternion_data_t quaternion_dat;
atk_ms901m_gyro_data_t gyro_dat;
atk_ms901m_accelerometer_data_t accelerometer_dat;
atk_ms901m_magnetometer_data_t magnetometer_dat;
atk_ms901m_barometer_data_t barometer_dat;

static const uint16_t g_atk_ms901m_gyro_fsr_table[4] = {250, 500, 1000, 2000};
static const uint8_t g_atk_ms901m_accelerometer_fsr_table[4] = {2, 4, 8, 16};

static struct
{
    uint8_t gyro;
    uint8_t accelerometer;
} g_atk_ms901m_fsr;

void set_atk_ms901m_fsr(uint8_t gyro_fsr, uint8_t accelerometer_fsr)	//默认配置(3,1)
{
	g_atk_ms901m_fsr.gyro = gyro_fsr;
	g_atk_ms901m_fsr.accelerometer = accelerometer_fsr;
}

uint8_t atk_ms901m_data_parsing(uint8_t *ms901m_command, uint8_t ms901m_commandlength){
	if(ms901m_commandlength != 0)
	{
		if(ms901m_command[2] == 0x01)
		{
			atk_ms901m_get_attitude(ms901m_command, &attitude_dat);
		}
		else if(ms901m_command[2] == 0x02)
		{
			atk_ms901m_get_quaternion(ms901m_command, &quaternion_dat);
		}
		else if(ms901m_command[2] == 0x03)
		{
			atk_ms901m_get_gyro_accelerometer(ms901m_command, &gyro_dat, &accelerometer_dat);
		}
		else if(ms901m_command[2] == 0x04)
		{
			atk_ms901m_get_magnetometer(ms901m_command, &magnetometer_dat);
		}
		else if(ms901m_command[2] == 0x05)
		{
			atk_ms901m_get_barometer(ms901m_command, &barometer_dat);
		}
	}
}

uint8_t atk_ms901m_get_attitude(uint8_t *ms901m_command, atk_ms901m_attitude_data_t *attitude_dat)
{
    uint8_t dat[6];
    memcpy(dat, &ms901m_command[4], 6);

    attitude_dat->roll = ((float)((int16_t)(dat[1] << 8) | dat[0]) / 32768 * 180);
    attitude_dat->pitch = ((float)((int16_t)(dat[3] << 8) | dat[2]) / 32768 * 180);
    attitude_dat->yaw = ((float)((int16_t)(dat[5] << 8) | dat[4]) / 32768 * 180);

    return 0;
}

uint8_t atk_ms901m_get_quaternion(uint8_t *ms901m_command, atk_ms901m_quaternion_data_t *quaternion_dat)
{
	uint8_t dat[8];
	memcpy(dat, &ms901m_command[4], 8);

    quaternion_dat->q0 = (float)((int16_t)(dat[1] << 8) | dat[0]) / 32768;
    quaternion_dat->q1 = (float)((int16_t)(dat[3] << 8) | dat[2]) / 32768;
    quaternion_dat->q2 = (float)((int16_t)(dat[5] << 8) | dat[4]) / 32768;
    quaternion_dat->q3 = (float)((int16_t)(dat[7] << 8) | dat[6]) / 32768;

    return 0;
}

uint8_t atk_ms901m_get_gyro_accelerometer(uint8_t *ms901m_command,
										  atk_ms901m_gyro_data_t *gyro_dat,
										  atk_ms901m_accelerometer_data_t *accelerometer_dat)
{
	uint8_t dat[12];
	memcpy(dat, &ms901m_command[4], 12);

	gyro_dat->raw.x = (int16_t)(dat[7] << 8) | dat[6];
	gyro_dat->raw.y = (int16_t)(dat[9] << 8) | dat[8];
	gyro_dat->raw.z = (int16_t)(dat[11] << 8) | dat[10];

	gyro_dat->x = (float)gyro_dat->raw.x / 32768 * g_atk_ms901m_gyro_fsr_table[g_atk_ms901m_fsr.gyro];
	gyro_dat->y = (float)gyro_dat->raw.y / 32768 * g_atk_ms901m_gyro_fsr_table[g_atk_ms901m_fsr.gyro];
	gyro_dat->z = (float)gyro_dat->raw.z / 32768 * g_atk_ms901m_gyro_fsr_table[g_atk_ms901m_fsr.gyro];


	accelerometer_dat->raw.x = (int16_t)(dat[1] << 8) | dat[0];
	accelerometer_dat->raw.y = (int16_t)(dat[3] << 8) | dat[2];
	accelerometer_dat->raw.z = (int16_t)(dat[5] << 8) | dat[4];

	accelerometer_dat->x = (float)accelerometer_dat->raw.x / 32768 * g_atk_ms901m_accelerometer_fsr_table[g_atk_ms901m_fsr.accelerometer];
	accelerometer_dat->y = (float)accelerometer_dat->raw.y / 32768 * g_atk_ms901m_accelerometer_fsr_table[g_atk_ms901m_fsr.accelerometer];
	accelerometer_dat->z = (float)accelerometer_dat->raw.z / 32768 * g_atk_ms901m_accelerometer_fsr_table[g_atk_ms901m_fsr.accelerometer];

    return 0;
}

uint8_t atk_ms901m_get_magnetometer(uint8_t *ms901m_command, atk_ms901m_magnetometer_data_t *magnetometer_dat)
{
	uint8_t dat[8];
	memcpy(dat, &ms901m_command[4], 8);

    magnetometer_dat->x = (int16_t)(dat[1] << 8) | dat[0];
    magnetometer_dat->y = (int16_t)(dat[3] << 8) | dat[2];
    magnetometer_dat->z = (int16_t)(dat[5] << 8) | dat[4];
    magnetometer_dat->temperature = (float)((int16_t)(dat[7] << 8) | dat[6]) / 100;

    return 0;
}

uint8_t atk_ms901m_get_barometer(uint8_t *ms901m_command, atk_ms901m_barometer_data_t *barometer_dat)
{
	uint8_t dat[10];
	memcpy(dat, &ms901m_command[4], 10);

    barometer_dat->pressure = (int32_t)(dat[3] << 24) | (dat[2] << 16) | (dat[1] << 8) | dat[0];
    barometer_dat->altitude = (int32_t)(dat[7] << 24) | (dat[6] << 16) | (dat[5] << 8) | dat[4];
    barometer_dat->temperature = (float)((int16_t)(dat[9] << 8) | dat[8]) / 100;

    return 0;
}
