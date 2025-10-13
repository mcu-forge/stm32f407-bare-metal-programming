#include<stdint.h>
#include<stm32f407xx.h>
#include<stm32f4xx.h>

void write(char c);
void serialWrite(char *s);
int main(){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOB->MODER &= ~GPIO_MODER_MODE6_Pos;
    GPIOB->MODER |= GPIO_MODER_MODER6_1;
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL6_Pos);

    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR2 &= ~USART_CR2_STOP;

    USART1->BRR = 0x068B;
    USART1->CR1 |= USART_CR1_TE;

    while(1){
        serialWrite("Hello world\r\n");
    }


    return 0;
}

void write(char c){
while(!(USART1->SR & USART_SR_TXE)) {}
USART1->DR = c;
}
void serialWrite(char *s){
    while(*s){
        write(*s++);
        for(volatile uint32_t i=0;i<=10000;i++){__asm("NOP");}
    
    }
}

