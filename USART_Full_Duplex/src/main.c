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
    while(tx_state){}
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

         if(USART1->SR & USART_SR_TXE){                  //IRQ handler function for TX interrupt
               if(tx_buffer[tx_index] != '\0'){
                  USART1->DR = tx_buffer[tx_index++];
                }
        else{
              USART1->CR1 &= ~USART_CR1_TXEIE;
              tx_state = 0;
        }
    }

        if(USART1->SR & USART_SR_RXNE){                //IRQ handler functio for RX interrupt 
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

    


int main(){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;        //enable the clock for PORTB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;        //enable the clock for PORTD
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       //enable the clock for USART1

    GPIOD->MODER &= ~GPIO_MODER_MODE12_Pos;
    GPIOD->MODER |= GPIO_MODER_MODE12_0;       //set PD12 as output

    GPIOB->MODER &= ~GPIO_MODER_MODER6_Msk;      
    GPIOB->MODER |= GPIO_MODER_MODE6_1;       
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;
    GPIOB->AFR[0] |= 7<<GPIO_AFRL_AFSEL6_Pos;    //set PB6 as USART_TX

    GPIOB->MODER &= ~GPIO_MODER_MODER7_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE7_1;
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL7;
    GPIOB->AFR[0] |= 7<<GPIO_AFRL_AFSEL7_Pos;   //set PB7 as USART_RX


    USART1->CR1 |= USART_CR1_UE;        //enable the USART line 
    USART1->CR1 &= ~USART_CR1_M;       //set start and data bit
    USART1->CR2 &= ~USART_CR2_STOP;   // set stop bit

    USART1->BRR = 0x8B;              //set the baud rate at 115.2KB/s
    USART1->CR1 |= USART_CR1_TE;     //enable the TX line 
    USART1->CR1 |= USART_CR1_RE;    //enable the RX line
    USART1->CR1 |= USART_CR1_RXNEIE;   //enable the RX line interrupt

    NVIC->ISER[1] |= 1<<(USART1_IRQn%32);      // IRQn. 37
    
    while(1){

        usart_write("Hello world!!!!\n");

    }
    return 0;

}



