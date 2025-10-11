#include<stdint.h>
#include<stm32f4xx.h>
#include<stm32f407xx.h>


int main(){
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;      //PortB clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    //USART1 clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;    //PortD clock enable

    GPIOD->MODER &= ~GPIO_MODER_MODE12_Pos;
    GPIOD->MODER |= GPIO_MODER_MODER12_0;      //PD12 as output 

    GPIOB->MODER &= ~GPIO_MODER_MODE6_Pos;
    GPIOB->MODER |= GPIO_MODER_MODER6_1;     
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL6_Pos);  //PB6 as USART1_TX

    GPIOB->MODER &= ~GPIO_MODER_MODE7_Pos;
    GPIOB->MODER |= GPIO_MODER_MODER7_1;
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL7_Pos);  //PB7 as USART1_RX

    USART1->CR1 |= USART_CR1_UE;        //enabling the USART

    USART1->CR1 &= ~USART_CR1_M;        //setting the word length as 8-bit
    USART1->BRR = 0x068B;             //baud rate at 9.6KBps
    
    USART1->CR1 |= USART_CR1_RE;    //enabling the RX line
    
while (1){


    while (!(USART1->SR & USART_SR_RXNE)){      //wait for the start bit 

    }   
    char c = USART1->DR;                     //store the received data from DR register to char c

    for (volatile int i = 0; i < 100000; i++){      //small crude delay
        __asm("NOP");
    }
    
    if(c=='a'){                               //if received character is a turn on the led otherwise turn off the led
        GPIOD->ODR |= (1<<12);
    }
    else{
        GPIOD->ODR &= ~(1<<12);
    }
}
    return 0;
}

