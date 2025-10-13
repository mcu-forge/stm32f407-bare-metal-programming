#include<stdint.h>
#include<string.h>
#include<stdbool.h>
#include<stm32f407xx.h>
#include<stm32f4xx.h>
#include<core_cm4.h>

#define buff_size 100                    //define the buffer size
volatile uint8_t rx_index = 0;          
volatile bool message_received = 0;
char rx_buffer[buff_size];
char data[buff_size];                    

void usart_read(char *recv){
    if(message_received){            //check if the message is received
    strcpy(recv,rx_buffer);         //if received copy the content of rx_buffer to recv varialbe
    rx_index = 0;     
    message_received=0;
}    
}
void USART1_IRQHandler(){
        
        if(USART1->SR & USART_SR_RXNE){         //check if the data is ready to be received
            char c = USART1->DR;              //store the data
            if(c == '\n'){                    //check if new line character is received
                rx_buffer[rx_index] = '\0';     //if received message is received completely 
                message_received = 1;          //set the flag to indicate that message is received
            }
            else{
                rx_buffer[rx_index++]=c;         //if end line character not received stores the character in buffer
                message_received = 0;
            }
        
        }
}
 
int main(){
    

    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;       //enable clock for PORTB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;       //enable clock for PORTD
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;     // enable clock for USART1

    GPIOD->MODER &= ~GPIO_MODER_MODE12_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE13_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE14_Pos;
    GPIOD->MODER &= ~GPIO_MODER_MODE15_Pos;

    GPIOD->MODER |= GPIO_MODER_MODE12_0;    //set PD12 as output mode
    GPIOD->MODER |= GPIO_MODER_MODE13_0;    //set PD13 as output mode
    GPIOD->MODER |= GPIO_MODER_MODE14_0;    //set PD14 as output mode
    GPIOD->MODER |= GPIO_MODER_MODE15_0;    //set PD15 as output mode

    GPIOB->MODER &= ~GPIO_MODER_MODE7;
    GPIOB->MODER |= GPIO_MODER_MODE7_1;     //set PB7 as alternate function mode
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL7;      
    GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL7_Pos);    //set PB7 as altf 7 as USART_RX mode

    USART1->CR1 |= USART_CR1_UE;        //enable the USART
    USART1->CR1 &= ~USART_CR1_M;        //set start bit=1,start bit=8 
    USART1->CR2 &= ~USART_CR2_STOP;    //set stop bit=1;

    USART1->BRR = 0x8B;              //set baud rate at 115200
    USART1->CR1 |= USART_CR1_RE;     //enable the RX line 
    USART1->CR1 |= USART_CR1_RXNEIE;  //enable the RX interrupt line 

    NVIC->ISER[1] |= (1<<(USART1_IRQn%32));      //IRQ numbver 37 write to NVIC ISER register 
     
    

    while(1){
            usart_read(data);             //read the data from USART RX line 

 if(strcmp(data,"green")==0){
        GPIOD->ODR |= GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(data,"orange")==0){
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR |= GPIO_ODR_ODR_13;
        GPIOD->ODR &= ~GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(data,"red")==0){
        GPIOD->ODR &= ~GPIO_ODR_ODR_12;
        GPIOD->ODR &= ~GPIO_ODR_ODR_13;
        GPIOD->ODR |=  GPIO_ODR_ODR_14;
        GPIOD->ODR &= ~GPIO_ODR_ODR_15;
    }
    else if(strcmp(data,"blue")==0){
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