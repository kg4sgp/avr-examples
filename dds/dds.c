/* N-Fractional DDS for Atmel AVR microcontrollers
 * Clock Frequency 16MHz, Tuning word 16bits
 * 
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sine.h"

/* interrupt variables */
volatile uint16_t phase_acc = 0;
volatile uint16_t dphase = 0;
/* end interrupt variables */

uint16_t ftone = 440;

int main(void)
{
  /* setup pins for output */
  DDRD |= _BV(3);

  /* setup counter 2 for fast PWM output on pin 3 (arduino) */
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20); /* clock, no prescale */
  /*TCCR2B = _BV(CS21);*/  /* clock/8 */
  TIMSK2 = _BV(TOIE2);
  sei();
  
  /* dphase = ((2^bit_resolution)*(frequency))/(PWM_clockrate) */
  uint16_t phase_max = 65535;
  uint16_t pwm_clock = 62500; /* if no prescale */
  /*float pwm_clock = 7812.5; */ /* if clock/8 */
  dphase = (uint16_t)((phase_max*(float)ftone)/pwm_clock);
  
  for(;;) {
    /* Let the interrupt take over */
  }

  return 0;
}

ISR(TIMER2_OVF_vect) {
  cli();
  /* 16bit phase_acc will roll over on its own */
  phase_acc += dphase;
  /* only take most significant 8 bits to find sine value in table */
  OCR2B = sine_table[(phase_acc>>8)];
  sei();
}


