#ifndef MY_BOOTLOADER
#define MY_BOOTLOADER
#include "stm32l4xx.h"

void MyBootloader();

void ResetAllPeripherals(UART_HandleTypeDef *first_one,UART_HandleTypeDef *second_one, PCD_HandleTypeDef* usb_first );

#endif // MY_BOOTLOADER