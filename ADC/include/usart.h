#ifndef USART_H
#define USART_H

#include<stm32f407xx.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>


void usart_init(USART_TypeDef *usart);
void usart_baud_rate(USART_TypeDef *usart , uint32_t BR);
void usart_data_length(USART_TypeDef *usart, uint8_t len);

void usart_stop_bit(USART_TypeDef *usart, float sb);
void usart_start(USART_TypeDef *usart);
void usart_write_char(USART_TypeDef *usart, char x);
void usart_write(USART_TypeDef *usart, char *message);
void usart_read_char(USART_TypeDef *usart, char message);
void usart_read(USART_TypeDef *usart, char *message, uint8_t buff_size);
void usart_deint(USART_TypeDef *usart);
#endif