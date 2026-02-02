/*
 * commend.h
 *
 *  Created on: 2026年2月1日
 *      Author: 30955
 */

#ifndef APPLICATION_USER_AZURE_RTOS_APP_COMMEND_H_
#define APPLICATION_USER_AZURE_RTOS_APP_COMMEND_H_


#include "main.h"
#include <string.h>

uint8_t Command_Write(uint8_t *data, uint8_t length);

uint8_t Command_GetCommand(uint8_t *command);




#endif /* APPLICATION_USER_AZURE_RTOS_APP_COMMEND_H_ */
