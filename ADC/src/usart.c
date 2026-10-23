#include<usart.h>
#include<math.h>

void usart_init(USART_TypeDef *usart){
    if(usart == USART1){
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if(usart == USART2){
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if(usart == USART3){
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    }
    else if(usart == UART4){
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
    }
    else if(usart == UART5){
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    }
    else if(usart == USART6){
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    }
    
    usart->CR1 |= USART_CR1_UE;
}
void usart_baud_rate(USART_TypeDef *usart , uint32_t BR){
    if(BR == 115200){
      usart->BRR = 0x8B; 
    }
    else{
        usart->BRR = 0x068B;
    }
}
void usart_data_length(USART_TypeDef *usart, uint8_t len){
    if(len == 8){
        usart->CR1 &= ~USART_CR1_M;
    }
    else if(len == 9){
        usart->CR1 |= USART_CR1_M;
    }
    else{
        usart->CR1 &= ~USART_CR1_M;
    }
}
void usart_stop_bit(USART_TypeDef *usart, float sb){
    if((int)sb == 1){
        usart->CR2 &= ~( 3 << USART_CR2_STOP_Pos);
    }
    else if(sb == .5){
        usart->CR2 |= 1 << USART_CR2_STOP_Pos;
    }
    else if((int)sb == 2){
        usart->CR2 |= 2 << USART_CR2_STOP_Pos;
    }
    else if(sb == 1.5){
        usart->CR2 |= 3 << USART_CR2_STOP_Pos;
    }
}


void usart_start(USART_TypeDef *usart){
    usart->CR1 |= USART_CR1_TE;
    usart->CR1 |= USART_CR1_RE;
}
void usart_write_char(USART_TypeDef *usart, char x){
    while(!(usart->SR & USART_SR_TXE)){}
        usart->DR = x;
}
void usart_write(USART_TypeDef *usart, char *message){
    while(*message){
            usart_write_char(usart,*message++);
            for(volatile uint32_t i=0;i<=500;i++){__asm("NOP");}
    }
}
void usart_read_char(USART_TypeDef *usart, char message){
    while(!(usart->SR & USART_SR_RXNE)){}
        message = usart->DR;
    
}

void usart_read(USART_TypeDef *usart, char message[], uint8_t buff_size){
    memset(message,0,buff_size);
    for(uint8_t i=0;i<buff_size;i++){
        while(!(usart->SR & USART_SR_RXNE)){}
            char c = usart->DR;
            if(c == '\n' || c == '\r'){
                message[i] = '\0';
                break;
            }
            else{
                message[i] = c;
            }
        


    }     
             
}
