Date: 2017-Week-39  
Status: Working.  

# STM32F103 - Working with the USART on the free EmBitz IDE in Windows. Basics STM32-BluePill board (STM32F103C8T6)  
  
## Introduction:  
Getting started with the STM32-Blue Pill generic board using the free [EmBitz IDE](https://www.embitz.org/) on Windows.  
To work with the USART.  
Done using the Standard Peripheral Library, no HAL drivers were used.  

---

## Information about the STM32-BluePill:  
- STM32F103C8T6 microcontroller, 64KB Flash memory, 20KB SRAM in 48-pin LQFP, 4 to 16 Mhz crystal input to generate maximum 72Mhz clock   
- SWD programming/Debugging interface.  
- One User LED - Green _ PC13  
- On-Board 8Mhz Crystal.  
- On-Board 32.786Khz oscillator connected to RTC ports.  
- Extension header for all QFP48 I/Os for quick connection for prototyping.  

---

## Useful Links:  
[Example on Github](https://github.com/artem-smotrakov/stm32f103-usb-commands/blob/master/main.c)  


[Gypsyengineer.com - STM32 USART sending text commands](https://blog.gypsyengineer.com/fun/stm32-usart-sending-text-commands.html)  


[stm32duino - Blue Pill Official](http://wiki.stm32duino.com/index.php?title=Blue_Pill)  
[Pinout Diagram](http://wiki.stm32duino.com/images/a/ae/Bluepillpinout.gif)  
[ST Microelectronics STM32 ARM MCU Site](http://www.st.com/en/microcontrollers/stm32-32-bit-arm-cortex-mcus.html)  
[STM32 Info](http://www.emcu.it/STM32.html) @ emcu  


[A template project for STM32 on Linux](https://blog.gypsyengineer.com/fun/a-template-project-for-stm32f103-on-linux.html)  
[Limit the current – take care of your favorite LED](https://blog.gypsyengineer.com/fun/limit-the-current-take-care-of-your-favorite-led.html)  
[GETTING STARTED WITH STM32 ARM CORTEX-M3 USING GCC (STM32F103, NUCLEO-F103RB)](http://electronut.in/stm32-start/)  
[LED Resistor Calculator](http://www.ohmslawcalculator.com/led-resistor-calculator)  

---  

## I did the following to get the clock working correctly at 56Mhz using the 8MHz external crystal:  
system_stm32f10x.c = System Clock Configuration file.  
We are using the external oscillator (HSE) which is 8Mhz  
We are using PLLCLK Mux  

### Setup In stm32f10x.h:  
1. Uncomment the line: #define USE_STDPERIPH_DRIVER  
2. set this: ```#define HSE_VALUE    ((uint32_t)8000000)```  
3. set this: ```#define HSI_VALUE    ((uint32_t)8000000)```  

### Setup In system_stm32f10x.c:  
1. Uncomment the 56MHz clock config line and comment the 72MHz line as seen below:  
    ```c  
    /* #define SYSCLK_FREQ_HSE    HSE_VALUE */
    /* #define SYSCLK_FREQ_24MHz  24000000 */
    /* #define SYSCLK_FREQ_36MHz  36000000 */
    /* #define SYSCLK_FREQ_48MHz  48000000 */
    #define SYSCLK_FREQ_56MHz  56000000
    //#define SYSCLK_FREQ_72MHz  72000000
    ```    
The rest of the file will do the configuration automatically to get the following:  
- PLLCLK = enabled  
- PLLMul = x7  
- SYSCLK = 56MHz  
- AHBPrescaler = /1  
- HCLK = 56Mhz
- APB1Prescaler = /2
- PCLK1 = 28Mhz    
- APB2Prescaler = /1  
- PCLK2 = 56Mhz  


### Setup In fm_stm32F1_delay.c:  
1. ```fac_us=SystemCoreClock/8000000;```  
