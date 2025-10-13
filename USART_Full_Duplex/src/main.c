#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include<stm32f407xx.h>
#include<stm32f4xx.h>
#include<core_cm4.h>


char cmd[100];

#define buffer_size 100
char tx_buffer[buffer_size];
char rx_buffer[buffer_size];
volatile uint16_t rx_index = 0;
volatile uint16_t tx_index = 0;
volatile bool tx_state = 0;
volatile bool message_received = 0;

void usart_write(char message[buffer_size]){
    strcpy(tx_buffer,message);
    tx_index = 0;
    tx_state = 1;
    USART1->CR1 |= USART_CR1_TXEIE;

}
void usart_read(char message[buffer_size]){
    if(message_received){
        strcpy(message,rx_buffer);
        rx_index = 0;     
        message_received=0;
    }
}

void USART1_IRQHandler(){

if(USART1->SR & USART_SR_TC){
         if(USART1->SR & USART_SR_TXE){  
               if(tx_buffer[tx_index] != '\0'){
                  USART1->DR = tx_buffer[tx_index++];
                }
        else{
              USART1->CR1 &= ~USART_CR1_TXEIE;
              tx_state = 0;
        }
        USART1->SR &= !USART_SR_TC;
}
        if(USART1->SR & USART_SR_RXNE){
            char c = USART1->DR;
            if(c == '\n'){
                rx_buffer[rx_index] = '\0';
                message_received = 1;
            }
            else{
                rx_buffer[rx_index++] = c;
                message_received = 0;
            }
        }

    }
}
    


int main(){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOD->MODER &= ~GPIO_MODER_MODE12_Pos;
    GPIOD->MODER |= GPIO_MODER_MODE12_0;

    GPIOB->MODER &= ~GPIO_MODER_MODER6_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE6_1;
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;
    GPIOB->AFR[0] |= 7<<GPIO_AFRL_AFSEL6_Pos;

    GPIOB->MODER &= ~GPIO_MODER_MODER7_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE7_1;
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL7;
    GPIOB->AFR[0] |= 7<<GPIO_AFRL_AFSEL7_Pos;


    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR2 &= ~USART_CR2_STOP;

    USART1->BRR = 0x8B;
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_TCIE;

    NVIC->ISER[1] |= 1<<(USART1_IRQn%32);
    
    while(1){

        usart_write("green\n");
for(volatile uint32_t i=0;i<=100000;i++){__asm("NOP");}
        usart_read(cmd);
        if(strcmp(cmd,"green")==0){
            GPIOD->ODR |= GPIO_ODR_ODR_12;
        }
        else{
            GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        }
usart_write("red\n");
for(volatile uint32_t i=0;i<=100000;i++){__asm("NOP");}
        usart_read(cmd);
        if(strcmp(cmd,"green")==0){
            GPIOD->ODR |= GPIO_ODR_ODR_12;
        }
        else{
            GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        }


// usart_write("hello\n");

    }




    return 0;

}



