#include "myBootLoader.h"
#include <stdint.h>
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "main.h"
// #include "core_cm7.h"

#include "stm32l4xx_hal.h"

// Function to reset all peripherals
void ResetAllPeripherals(UART_HandleTypeDef *first_one,UART_HandleTypeDef *second_one, PCD_HandleTypeDef *usb_first) {
    // Deinitialize GPIOs
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All); // Deinitialize GPIO Port A
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All); // Deinitialize GPIO Port B
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All); // Deinitialize GPIO Port C
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All); // Deinitialize GPIO Port D
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_All); // Deinitialize GPIO Port E
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_All); // Deinitialize GPIO Port F
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_All); // Deinitialize GPIO Port G
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_All); // Deinitialize GPIO Port H

    // Deinitialize UARTs
    HAL_UART_DeInit(first_one); // Deinitialize LPUART1
    HAL_UART_DeInit(second_one);  // Deinitialize USART3

    // Deinitialize USB
    HAL_PCD_DeInit(usb_first); // Deinitialize USB_OTG_FS

    // Reset GPIO clocks
    __HAL_RCC_GPIOA_FORCE_RESET();
    __HAL_RCC_GPIOB_FORCE_RESET();
    __HAL_RCC_GPIOC_FORCE_RESET();
    __HAL_RCC_GPIOD_FORCE_RESET();
    __HAL_RCC_GPIOE_FORCE_RESET();
    __HAL_RCC_GPIOF_FORCE_RESET();
    __HAL_RCC_GPIOG_FORCE_RESET();
    __HAL_RCC_GPIOH_FORCE_RESET();
    __HAL_RCC_GPIOA_RELEASE_RESET();
    __HAL_RCC_GPIOB_RELEASE_RESET();
    __HAL_RCC_GPIOC_RELEASE_RESET();
    __HAL_RCC_GPIOD_RELEASE_RESET();
    __HAL_RCC_GPIOE_RELEASE_RESET();
    __HAL_RCC_GPIOF_RELEASE_RESET();
    __HAL_RCC_GPIOG_RELEASE_RESET();
    __HAL_RCC_GPIOH_RELEASE_RESET();

    // Reset UART clocks
    __HAL_RCC_LPUART1_FORCE_RESET();
    __HAL_RCC_USART3_FORCE_RESET();
    __HAL_RCC_LPUART1_RELEASE_RESET();
    __HAL_RCC_USART3_RELEASE_RESET();

    // Reset USB clock
    __HAL_RCC_USB_OTG_FS_FORCE_RESET();
    __HAL_RCC_USB_OTG_FS_RELEASE_RESET();

    // Optionally disable peripheral clocks to save power
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_LPUART1_CLK_DISABLE();
    __HAL_RCC_USART3_CLK_DISABLE();
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

    // Reset the Systick timer
    SysTick->CTRL = 0;   // Disable SysTick
    SysTick->LOAD = 0;   // Clear reload value
    SysTick->VAL = 0;    // Clear current value
}


void MyBootloader() {
    typedef void (*pFunction)(void);
    pFunction JumpToApp;

    uint32_t BackupSP = *(__IO uint32_t *)(0x08100000);
    uint32_t BackupResetHandler = *(__IO uint32_t *)(0x08100000 + 4);

    // Validate the backup firmware
    if (BackupSP == 0xFFFFFFFF || BackupResetHandler == 0xFFFFFFFF) {
        // HAL_UART_Transmit(&uart, (uint8_t *)"Invalid firmware\n", 17, 1000);
        return;
    }

    // ResetAllPeripherals();
    // HAL_UART_Transmit(&uart, (uint8_t *)"About to jump\n", 15, 1000);


    // Set vector table to backup firmware
    SCB->VTOR = 0x08100000;

    // Set the main stack pointer
    __set_MSP(BackupSP);

    // NVIC_SystemReset();

    // Jump to the reset handler
    JumpToApp = (pFunction)BackupResetHandler;
    JumpToApp();

    // Prevent execution from continuing past this point
    while (1);
}
