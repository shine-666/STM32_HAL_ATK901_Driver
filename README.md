# STM32_HAL_ATK901_Driver
一个易于使用和移植的正点原子ATK-IMU901角度传感器库，包含串口循环缓冲区数据包校验和数据解析。
#### 1. 简介

本程序是适用于正点原子ATK-IMU901角度传感器的STM32HAL库驱动程序，用于读取IMU901的角度数据。

#### 2. 功能

- 读取IMU901的数据（姿态角、四元数、陀螺仪、加速度计、磁力计、气压计）

#### 3. 使用方法

1. 使用CUBEMX配置要使用的串口，异步通信模式，波特率115200，开启中断，其他参数默认。
2. 将ATK901文件夹下的文件添加到工程中
3. 添加头文件
```c
#include "ms901m_command.h"
#include "atk_ms901m.h"
```
4. 创建接收数组
```c
uint8_t readBuffer[128];
```
5. 在串口中断回调函数中添加
```c
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart == &huart3){  //IMU901串口
		Command_Write(readBuffer, Size);
		HAL_UARTEx_ReceiveToIdle_IT(&huart3, readBuffer, sizeof(readBuffer));
	}
}
```
6. main函数中添加
```c
	set_atk_ms901m_fsr(3,1); // 设置陀螺仪量程，加速度计量程，默认(3,1)
	uint8_t ms901m_command[64]; //存放已校验的数据包

	while(1) {
		for(int i=0; i<8; i++){ //确保所有数据都进行更新
			HAL_UARTEx_ReceiveToIdle_IT(&huart3, readBuffer, sizeof(readBuffer));
			uint8_t ms901m_commandlength = Command_GetCommand(ms901m_command);
			atk_ms901m_data_parsing(ms901m_command, ms901m_commandlength);
		}
        /*其他用户代码*/
        //串口打印eg:
        printf("GX: %.2f, GY: %.2f, GZ: %.2f\r\n", gyro_dat.x, gyro_dat.y, gyro_dat.z);
        printf("AX: %.2f, AY: %.2f, AZ: %.2f\r\n", accelerometer_dat.x, accelerometer_dat.y, accelerometer_dat.z);
        printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n", attitude_dat.roll, attitude_dat.pitch, attitude_dat.yaw);

        HAL_Delay(50);
    }
```
