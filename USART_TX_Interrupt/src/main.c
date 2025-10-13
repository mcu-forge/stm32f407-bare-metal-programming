#include<stdint.h>
#include<string.h>
#include<stdbool.h>
#include<stm32f407xx.h>
#include<stm32f4xx.h>
#include<core_cm4.h>

volatile uint8_t tx_index = 0;
volatile bool tx_busy = 0;
char tx_buffer[] = "Test\n";           

void usart_send(char *message){
       strcpy(tx_buffer,message);         //copy the content of message into tx_buffer
       tx_index = 0;
       tx_busy = 1;
       USART1->CR1 |= USART_CR1_TXEIE;   //enabling the TX interrupt line       
}

void USART1_IRQHandler(){
    if(USART1->SR & USART_SR_TXE){            //check if the data from DR register is moved to the shift register or not
        if(tx_buffer[tx_index] != '\0'){
            USART1->DR = tx_buffer[tx_index++];
        }
        else{
            USART1->CR1 &= ~USART_CR1_TXEIE;    //disable the interrupt line 
            tx_busy = 0;
        }
    }

}

int main(){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;          //enable the PORTB clock 
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;         //enable the USART1 clock
   
    GPIOB->MODER &= ~GPIO_MODER_MODER6_Msk;
    GPIOB->MODER |= GPIO_MODER_MODER6_1;         //set as alternate function mode
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;            
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL6_Pos);  //use the alternate function 7 of PB6 as TX line 


    USART1->CR1 |= USART_CR1_UE;             //enable the USART 
    USART1->CR1 &= ~USART_CR1_M;             //set start bit=1 and data bit=8
    USART1->CR2 &= ~USART_CR2_STOP;          //set stop bit=1

    USART1->BRR = 0x8B;                   //set baud rate as 115.200Kbps
    USART1->CR1 |= USART_CR1_TE;          //enable the TX line


    NVIC->ISER[1] |= 1<<(USART1_IRQn%32);           //IRQ number 37 for USART1 


    while(1){
        if(!tx_busy){
            usart_send("Hello World!!!\n");
        }

    }

    return 0;
}
