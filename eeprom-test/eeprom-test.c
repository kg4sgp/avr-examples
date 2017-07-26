#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdio.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE *16UL)))-1)

uint16_t ncount = 16705;
uint16_t comp = 0;

int main()
{

  /* begin serial communication */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
  UBRR0H = (BAUD_PRESCALE >> 8);
  UBRR0L = BAUD_PRESCALE;
  UCSR0B |= (1 << RXCIE0);

  eeprom_write_word((uint16_t *) 46, ncount);
  comp = eeprom_read_word((uint16_t *) 46);

  if(ncount == comp) {
    UDR0 = 'G';
  } else {
    UDR0 = 'B';
  }

  return 0;

}

