#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

void usart_init (uint32_t, uint32_t);
void usart_tx_byte (uint8_t);
uint8_t usart_rx_byte (void);
uint8_t usart_rx_flush (void);
uint8_t usart_tx_string (uint8_t* , uint16_t);

#endif
