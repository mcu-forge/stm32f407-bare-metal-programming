#include<gpio.h>
#include<usart.h>
#include<stdio.h>

uint16_t val;
char message[100]; 

int main(){

    gpio_init(GPIOA);
    gpio_init(GPIOD);
    gpio_mode(GPIOD,OUTPUT,12);
    gpio_mode(GPIOD,OUTPUT,13);
    gpio_mode(GPIOA,ANALOG,1);
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

    ADC2->CR2   |= ADC_CR2_ADON;                 //wake from power down mode

    ADC2->SQR3  |=  1 << 0;                      // channel 1 at rank 1
    ADC2->SQR1  &= ~ADC_SQR1_L_Msk;              //no of conversion L-1              
    ADC2->CR1   |= ADC_CR1_RES_0;                 //10-bit resolution
    ADC2->SMPR2 |= 3 << ADC_SMPR2_SMP1_Pos;       //56 cycles
    ADC2->CR2   |= ADC_CR2_CONT;                  //continuous conversion mode
    ADC2->CR2   |= ADC_CR2_SWSTART;               
    

   
    while(1){
        
        if(ADC2->SR & ADC_SR_EOC){
            val = ADC2->DR;
            snprintf(message,sizeof(message),"%d",val);
            usart_write(USART2,message);
            usart_write_char(USART2,'\n'); 
        }

        if(val > 500){
            gpio_set(GPIOD,12);
            gpio_reset(GPIOD,13);
        }
        else{
            gpio_reset(GPIOD,12);
            gpio_set(GPIOD,13);
        }

    }
    return 0;
}





// #include<gpio.h>
// #include<usart.h>

// char message[100];

// int main(){

//     gpio_init(GPIOD);    
//     gpio_mode(GPIOD,OUTPUT,12);
//     gpio_mode(GPIOD,OUTPUT,13);
//     gpio_mode(GPIOD,OUTPUT,14);
//     gpio_mode(GPIOD,OUTPUT,15);

//     gpio_set(GPIOD,12);
//     gpio_set(GPIOD,13);
//     gpio_set(GPIOD,14);
//     gpio_set(GPIOD,15);
//     //for(volatile uint32_t i=0;i<100000;i++){}
//     gpio_reset(GPIOD,12);
//     gpio_reset(GPIOD,13);
//     gpio_reset(GPIOD,14);
//     gpio_reset(GPIOD,15);

//     gpio_init(GPIOA);
//     gpio_mode(GPIOA,AF,2);
//     gpio_mode(GPIOA,AF,3);
//     gpio_af(GPIOA,2,AF7);
//     gpio_af(GPIOA,3,AF7);

//     usart_init(USART2);
//     usart_baud_rate(USART2,115200);
//     usart_data_length(USART2,8);
//     usart_stop_bit(USART2,1);
//     usart_start(USART2);

//     while(1){
//         usart_read(USART2,message,sizeof(message));

//         if(strcmp(message,"green on") == 0){
//             gpio_set(GPIOD,12);
//         }
//         else if(strcmp(message,"orange on") == 0){
//             gpio_set(GPIOD,13);
//         }
//         else if(strcmp(message,"red on") == 0){
//             gpio_set(GPIOD,14);
//         }
//         else if(strcmp(message,"blue on") == 0){
//             gpio_set(GPIOD,15);
//         }
//         else if(strcmp(message,"green off") == 0){
//             gpio_reset(GPIOD,12);
//         }
//         else if(strcmp(message,"orange off") == 0){
//             gpio_reset(GPIOD,13);
//         }
//         else if(strcmp(message,"red off") == 0){
//             gpio_reset(GPIOD,14);
//         }
//         else if(strcmp(message,"blue off") == 0){
//             gpio_reset(GPIOD,15);
//         }
//         else if(strcmp(message,"all on") == 0){
//             gpio_set(GPIOD,12);
//             gpio_set(GPIOD,13);
//             gpio_set(GPIOD,14);
//             gpio_set(GPIOD,15);
//         }
//         else if(strcmp(message,"all off") == 0){
//             gpio_reset(GPIOD,12);
//             gpio_reset(GPIOD,13);
//             gpio_reset(GPIOD,14);
//             gpio_reset(GPIOD,15);
//         }
//     }



//     return 0;
// }