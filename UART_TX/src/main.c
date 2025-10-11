#include<stdint.h>
#include<stm32f4xx.h>
#include<stm32f407xx.h>



int main(){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   //clock enable for PORTB
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;   //clock enable for USART1

    GPIOB->MODER &= ~(3<<(6*2));
    GPIOB->MODER |= (2<<(6*2));  //set as alternate function mode
    GPIOB->AFR[0] &= ~(0XF<<24);
    GPIOB->AFR[0] |= (7<<24);     //set at AF=7

    USART1->CR1 |= (1<<13);    //enabling USART
    USART1->CR1 &= ~(1<<12);   // 1 Start bit, 8 data bit
    USART1->CR2 &= ~(3<<12);  //1 Stop bit

    USART1->BRR = 0x068B;    //baud rate at 9.6KBps

    USART1->CR1 |= USART_CR1_TE;  //enabling the TX line

    while(1){

        while(!(USART1->SR & USART_SR_TXE)){  //wait until the data in DR is flushed out 
        }
    USART1->DR = 'A';          // load the value to the DR register              
    
    for(volatile uint32_t i=0;i<=100000;i++){   //small crude delay funcion 
        __asm("NOP");
    }
    }


    return 0;
}