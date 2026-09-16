# STM32F4xx Bare-Metal Drivers 🚀

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Microcontroller](https://img.shields.io/badge/MCU-STM32F4xx-red.svg)
![Type](https://img.shields.io/badge/Type-Bare--Metal-green.svg)

This repository contains custom, bare-metal peripheral drivers for the **STM32F4xx family** of microcontrollers (specifically tested on the STM32F407). 

These drivers were written from scratch in Embedded C, directly manipulating the memory-mapped hardware registers without relying on the ST HAL (Hardware Abstraction Layer) or standard peripheral libraries. This project demonstrates a deep understanding of the ARM Cortex-M4 architecture, bus interfaces, and peripheral control.

## 📂 Repository Structure

*   `Inc/`: Contains the MCU-specific header file (`stm32f407xx.h`) defining the memory map, peripheral register structures, and driver header files containing API prototypes.
*   `Src/`: Contains the driver implementation files (`.c`), handling register configurations, clock setups, and interrupt handling.
*   `Examples/`: Contains sample application code (e.g., LED toggling, UART echo) demonstrating how to initialize and use the driver APIs.

## 🔌 Supported Peripherals (Driver Status)

- [x] **GPIO** (General Purpose Input/Output) - Read/Write/Toggle, Interrupt handling
- [x] **SPI** (Serial Peripheral Interface) - Polling & Interrupt based TX/RX
- [x] **I2C** (Inter-Integrated Circuit) - Master/Slave communication
- [x] **USART/UART** - Serial communication, Baudrate calculation
- [ ] **CAN** (Controller Area Network) - *In development*

## 💻 Usage Example: Toggling an LED

To show how the APIs are abstracted, here is a quick example of configuring a GPIO pin and toggling an LED using this driver structure:

```c
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

void delay(void) {
    for(uint32_t i = 0; i < 500000; i ++);
}

int main(void) {
    // 1. Create a GPIO handle structure
    GPIO_Handle_t GpioLed;
    
    // 2. Configure the specific Port and Pin (e.g., PD12 on Discovery Board)
    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    // 3. Enable the peripheral clock
    GPIO_PeriClockControl(GPIOD, ENABLE);
    
    // 4. Initialize the GPIO pin
    GPIO_Init(&GpioLed);

    // 5. Application Loop
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
        delay();
    }
    
    return 0;
}
```
