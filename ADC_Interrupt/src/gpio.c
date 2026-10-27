#include<gpio.h>

void gpio_init(GPIO_TypeDef *port){
      if(port == GPIOA){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
      }
      else if(port == GPIOB){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
      }
      else if(port == GPIOC){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
      }
      else if(port == GPIOD){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
      }
      else if(port == GPIOE){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
      }
      else if(port == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
      }
      else if(port == GPIOG){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
      }
      else if(port == GPIOH){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
      }
}


void gpio_mode(GPIO_TypeDef *port, uint8_t mode, uint8_t pin){
    port->MODER |= mode << (pin*2);
}
void gpio_write(GPIO_TypeDef *port, uint8_t pin, bool state){
    if(state){
        port->BSRR = 1 << pin;
    }
    else{
        port->BSRR = 1 << (pin+16);
    }
}

bool gpio_read(GPIO_TypeDef *port, uint8_t pin){
        return (port->IDR & (1<<pin)); 
}

void gpio_set(GPIO_TypeDef *port, uint8_t pin){
    port->BSRR = 1 << pin;
}
void gpio_reset(GPIO_TypeDef *port, uint8_t pin){
    port->BSRR = 1 << (pin+16);
}
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin){
    port->ODR ^= 1 << pin;
}
void gpio_af(GPIO_TypeDef *port, uint8_t pin, uint8_t af){
    if(af <= 7){
        port->AFR[0] |= af << (pin*4);
    }
    else{
        port->AFR[1] |= af << (pin*4);
    }
}