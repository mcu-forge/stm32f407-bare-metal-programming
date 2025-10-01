#include<stdint.h>
#include<stm32f407xx.h>


int main(){
    RCC->AHB1ENR |= (1<<3);      //enabling the GPIOD clock 
    RCC->APB1ENR |= (1<<2);      //enabling TIM4 clock 

    GPIOD->MODER &= ~(1<<(14*2)); 
    GPIOD->MODER |= (2<<(14*2));   //set as alternate function mode
    GPIOD->AFR[1] |= (2<<24);      

    TIM4->CR1 &= ~(3<<5);         //set the TIM4 as up-counter 

    TIM4->CCMR2 &= ~(3<<0);       //setting the TIM4 as outout mode
    TIM4->CCMR2 |= (6<<4);       //setting the output mode as PWM1 output 

    TIM4->CCER |= (1<<8);       //enabling the capture/compare register

    TIM4->ARR = 99;            
    TIM4->PSC = 159;
    TIM4->CCR3 = (50 << 0);  

    /* For calculation 

    f(pwm) = f(timer_clock_speed) / (PSC+1)(ARR+1)
    Duty% = CCRx / (ARR+1) * 100   
    
    so for above values,
    f(pwm) = 1KHz
    Duty% = 50% 
    */
    TIM4->CR1 |= (1<<0);

    while(1){

         for(uint32_t i=0;i<100;i++){
            TIM4->CCR3 = i;
            for(volatile uint32_t j=0;j<=10000;j++){
                __asm("NOP");
            }
        }
         for(uint32_t i=100;i>0;i--){
            TIM4->CCR3 = i;
            for(volatile uint32_t j=0;j<=10000;j++){
                __asm("NOP");
            }
        }
        
    }





    return 0;
}
