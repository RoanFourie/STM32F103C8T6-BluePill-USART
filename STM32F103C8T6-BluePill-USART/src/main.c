/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   AUTHOR:          Roan Fourie
   DATE CREATED:    2017-Week-39
   DATE CHANGED:    2017-Week-39

**********************************************************************/
#include "stm32f10x_conf.h"
#include "rf_stm32f1_led.h"
#include "rf_stm32f1_delay.h"

static void button_setup(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	// Clock Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure as digital input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*****************************************************
 * Initialize USART1: enable interrupt on reception
 * of a character
 *****************************************************/
void USART1_Init(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;

    /* Enable clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO |
                           RCC_APB2Periph_GPIOA, ENABLE);

    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart1_init_struct.USART_BaudRate = 9600;
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;
    usart1_init_struct.USART_StopBits = USART_StopBits_1;
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
}

/**********************************************************
 * USART1 interrupt request handler: on reception of a
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /* If received 't', toggle LED and transmit 'T' */
        if((char)USART_ReceiveData(USART1) == 't')
        {
            //led_toggle();
            USART_SendData(USART1, 'Hello');
            /* Wait until Tx data register is empty, not really
             * required for this example but put in here anyway.
             */
            /*
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            {
            }*/
        }
    }

    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}

int main(void)
{
    // initialize the system frequency @ 56Mhz
    SystemInit();
    // Delay initialize
    delay_init();
    // all LED initialize
    FM_Led_Init();
    // Initialize the button
    button_setup();
    /* Initialize USART1 */
    USART1_Init();

  while(1)
  {
      if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15))
      {
        FM_Led_Toggle(LED_1);
        FM_Led_Toggle(LED_2);
      } else {
        FM_Led_Off(LED_2);
        FM_Led_Off(LED_1);
      }
        // Delay for half second
       delay_ms(500);
  }
}
