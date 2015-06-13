/* Code for AVR microcontroller UART communication  */

#include <avr/interrupt.h>
#include "usart.h"

void usart_init (uint32_t fosc, uint32_t baud_rate)
{
  /* disable gloabl interrupt */
  cli();

  /* set baud rate */
  uint16_t baud_prescale = (uint16_t)((fosc / (baud_rate * 16))-1);
  UBRR0H = (uint8_t)(baud_prescale >>8);
  UBRR0L = (uint8_t)(baud_prescale);

  /* set frame format */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00); 

  /* enable the tx or rx */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);

  /* enable global interrupts */
  sei();
}

void usart_tx_byte (uint8_t byte)
{
  /* wait for register to be empty */
  while ( !(UCSR0A & (1<<UDRE0)) );
  /* transmit the next byte */
  UDR0 = (uint8_t)byte;
}

uint8_t usart_rx_byte (void)
{
  /* wait for data in the rx register */
  while ( !(UCSR0A & (1<<RXC0)) );
  /* return a recieved byte */
  return (uint8_t)UDR0;
}

uint8_t usart_rx_flush (void)
{
  uint8_t dummy = 0;
  while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
  return dummy;
}

uint8_t usart_tx_string (uint8_t * string_ptr, uint16_t string_sz)
{
  uint8_t i = 0;
  for(i = 0; i < string_sz; i++){
    usart_tx_byte(string_ptr[i]);
  }
  return i;
}

