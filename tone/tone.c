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
volatile uint32_t samples = 0;
volatile uint8_t go = 0;
/* end interrupt variables */

uint16_t phase_max = 65535;
uint16_t pwm_clock = 62500; /* if no prescale */

void make_tone(uint16_t, float);
void make_silence(float);

void make_tone(uint16_t freq, float seconds)
{
  /* wait for previous tone to stop */
  while(go) __asm__("NOP");
  samples = (uint32_t)(seconds*pwm_clock);
  dphase = (uint16_t)((phase_max*(float)freq)/pwm_clock);
  go = 1;
}

void make_silence(float seconds)
{
  /* wait for previous tone to stop */
  while(go) __asm__("NOP");
  samples = (uint32_t)(seconds*pwm_clock);
  phase_acc = 0;
  dphase = 0;
  go = 1;
}

int main(void)
{
  /* setup pins for output */
  DDRD |= _BV(3);
  DDRB |= _BV(5);
  PORTB = (0<<5);

  /* setup counter 2 for fast PWM output on pin 3 (arduino) */
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20); /* clock, no prescale */
  /*TCCR2B = _BV(CS21);*/  /* clock/8 */
  TIMSK2 = _BV(TOIE2);
  sei();
  
  /* dphase = ((2^bit_resolution)*(frequency))/(PWM_clockrate) */
  /*float pwm_clock = 7812.5; */ /* if clock/8 */
  
  while(1) {
    make_tone(700, (float)1.0);
    make_silence((float)1.0);
    make_tone(900, (float)1.0);
    make_silence((float)1.0);
    make_tone(1000, (float)(.05));
    make_tone(900, (float)(.05));
    make_tone(800, (float)(.05));
  }

  return 0;
}  

ISR(TIMER2_OVF_vect) {

  if(!go) return;

  cli();
  /* 16bit phase_acc will roll over on its own */
  phase_acc += dphase;
  /* only take most significant 8 bits to find sine value in table */
  OCR2B = sine_table[(phase_acc>>8)];
  
  samples--;
  if(samples == 0) go = 0;
  sei();
}


