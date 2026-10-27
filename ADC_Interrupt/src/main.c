#include<gpio.h>
#include<usart.h>
#include<stdio.h>
#include<core_cm4.h>
#include<stdbool.h>

uint16_t val;
volatile bool flag = 0;
char message[100];

void ADC_IRQHandler(){
    if(ADC2->SR & ADC_SR_EOC){
        val = ADC2->DR;
        flag = 1;
    }
}

int main(){

    gpio_init(GPIOA);
    gpio_init(GPIOD);

    gpio_mode(GPIOA,ANALOG,1);
    gpio_mode(GPIOD,OUTPUT,12);
    gpio_mode(GPIOD,OUTPUT,13);

     gpio_mode(GPIOA,AF,2);
    gpio_mode(GPIOA,AF,3);
    gpio_af(GPIOA,2,AF7);
    gpio_af(GPIOA,3,AF7);

    usart_init(USART2);
    usart_baud_rate(USART2,115200);
    usart_data_length(USART2,8);
    usart_stop_bit(USART2,1);
    usart_start(USART2);

    RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

    ADC2->CR2 |= ADC_CR2_ADON;
    ADC2->SQR3 |= 1 << 0;
    ADC2->SQR1 &= ~ADC_SQR1_L_Msk;
    ADC2->CR1 &= ~ADC_CR1_RES_Pos;   //12 bit resolution 
    ADC2->SMPR2 = 3 << ADC_SMPR2_SMP1_Pos;
    ADC2->CR1 |= ADC_CR1_EOCIE;      // enable interrupt

    NVIC->ISER[0] |= 1 << ADC_IRQn;

    ADC2->CR2 |= ADC_CR2_CONT;
    ADC2->CR2 |= ADC_CR2_SWSTART;


    while(1){

        if(flag){
             flag = 0;
              snprintf(message,sizeof(message),"%d",val);
              usart_write(USART2,message);
              usart_write(USART2," \n");
        }
        if(val > 2000){
            gpio_set(GPIOD,12);
            gpio_reset(GPIOD,13);
        }
        else{
             gpio_set(GPIOD,13);
             gpio_reset(GPIOD,12);           
        }
    }


    return 0;

}