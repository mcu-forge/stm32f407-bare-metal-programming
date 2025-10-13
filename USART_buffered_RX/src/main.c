#include<stdint.h>
#include<string.h>
#include<stm32f407xx.h>
#include<stm32f4xx.h>

#define buff_size 50

int main(){
    char message[buff_size];

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOD->MODER &= ~GPIO_MODER_MODE12_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE13_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE14_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE15_Pos;

    GPIOD->MODER |= GPIO_MODER_MODE12_0;
    GPIOD->MODER |= GPIO_MODER_MODE13_0;
    GPIOD->MODER |= GPIO_MODER_MODE14_0;
    GPIOD->MODER |= GPIO_MODER_MODE15_0;

    GPIOB->MODER &= ~GPIO_MODER_MODE7_Pos;
    GPIOB->MODER |= GPIO_MODER_MODE7_1;
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL7_Pos);

    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR2 &= ~USART_CR2_STOP;
    USART1->BRR = 0x683;
    USART1->CR1 |= USART_CR1_RE;

    while(1){

    memset(message, 0, buff_size);


    for(uint16_t i=0;i<buff_size;i++){

    while(!(USART1->SR & USART_SR_RXNE)){ }
    char c = USART1->DR;

        if(c=='\r' || c=='\n'){
            break;
        }
    message[i]=c;
    }


    if(strcmp(message,"green")==0){
        GPIOD->ODR |= GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(message,"orange")==0){
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR |= GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(message,"red")==0){
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR |=  GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(message,"blue")==0){
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR |= GPIO_ODR_ODR_15;
    }
    else{
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;

    }

    }


    return 0;
}

