#include<stdio.h>
#include"usart.h"

uint8_t teststr[] = "This is a test sentance.\r\n The atmel processor has started and the rest of your program is initialising...";

int main()
{
  usart_init(16000000, 9600);
  usart_tx_string(teststr, sizeof(teststr));

  return 0;
}

