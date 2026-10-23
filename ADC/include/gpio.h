#ifndef GPIO_H
#define GPIO_H


#include<stm32f407xx.h>
#include<stdint.h>
#include<stdbool.h>

#define INPUT    0U
#define OUTPUT   1U
#define AF       2U
#define ANALOG   3U

#define HIGH     1U
#define LOW      0U

#define AF0      0U
#define AF1      1U
#define AF2      2U
#define AF3      3U
#define AF4      4U
#define AF5      5U
#define AF6      6U
#define AF7      7U
#define AF8      8U
#define AF9      9U
#define AF10     10U
#define AF11     11U
#define AF12     12U
#define AF13     13U
#define AF14     14U
#define AF15     15U


void gpio_init(GPIO_TypeDef *port);
void gpio_mode(GPIO_TypeDef *port, uint8_t mode, uint8_t pin);
void gpio_write(GPIO_TypeDef *port, uint8_t pin, bool state);
bool gpio_read(GPIO_TypeDef *port, uint8_t pin);
void gpio_set(GPIO_TypeDef *port, uint8_t pin);
void gpio_reset(GPIO_TypeDef *port, uint8_t pin);
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin);
void gpio_af(GPIO_TypeDef *port, uint8_t pin, uint8_t af);



#endif