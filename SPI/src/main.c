#include<gpio.h>
#include<usart.h>



int main(){

        
        gpio_init(GPIOA);
        gpio_init(GPIOE);
        gpio_init(GPIOD);    

        gpio_mode(GPIOA,AF,5);
        gpio_mode(GPIOA,AF,6);
        gpio_mode(GPIOA,AF,7);
        gpio_mode(GPIOE,OUTPUT,3);
        gpio_mode(GPIOD,OUTPUT,13); 

        gpio_af(GPIOA,5,AF5);
        gpio_af(GPIOA,6,AF5);
        gpio_af(GPIOA,7,AF5);


        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
        
        SPI1->CR1 |= 1 << SPI_CR1_BR_Pos;
        SPI1->CR1 |= SPI_CR1_CPHA;
        SPI1->CR1 |= SPI_CR1_CPOL;
        SPI1->CR1 &= ~SPI_CR1_DFF_Msk;
        SPI1->CR1 &= ~SPI_CR1_LSBFIRST_Msk;
        SPI1->CR1 |= SPI_CR1_SSM;
        SPI1->CR1 |= SPI_CR1_SSI;
        SPI1->CR1 |= SPI_CR1_MSTR;
        SPI1->CR1 |= SPI_CR1_SPE;
        
        gpio_set(GPIOE,3);
        while(1){

            gpio_reset(GPIOE,3);
            SPI1->DR =  0b10001111;     //whoami reg
            while(!(SPI1->SR & SPI_SR_TXE)){}
            SPI1->DR = 0b100000000;     //dumy byte
            while(!(SPI1->SR & SPI_SR_RXNE)){}
            SPI1->DR;            //garbage value 
            while(!(SPI1->SR & SPI_SR_RXNE)){}
            uint8_t data = SPI1->DR;
            gpio_set(GPIOE,3);

            for(volatile uint32_t i=0;i<100000;i++){__asm("NOP");}
            if(data == 0x3F){      //0x3F or 63
                gpio_set(GPIOD,13);
            }
            else{
                gpio_reset(GPIOD,13);
            }
    
        }


    return 0;
}




