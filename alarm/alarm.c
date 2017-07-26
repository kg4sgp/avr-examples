/* DDS for Atmel AVR microcontrollers
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
volatile uint32_t samples = 1;
volatile uint8_t go = 0;
/* end interrupt variables */

uint16_t phase_max = 65535;
uint16_t pwm_clock = 62500; /* if no prescale */

uint8_t alarm_on = 1;
uint8_t alarm_num = 0;
uint8_t alarm_ind = 1;

uint16_t alarm_freq[] = {  1500,     0, 2000,    0, 1500,    0};
float    alarm_secs[] = { 0.125, 0.125, 0.25, 0.25, 0.25, 0.25};
uint8_t alarm_st_sp[2][2] = { {0,1},
                              {1,5}};

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
  
  uint32_t i = 0;
  
  while(1) {
    i++;
    if (i >= 80000000) {
      cli();

      if (alarm_num == 1) {
        alarm_num = 0;
      } else {
        alarm_num = 1;
      }
      alarm_ind = alarm_st_sp[alarm_num][0];
      samples = (uint32_t)((float)alarm_secs[alarm_ind]*pwm_clock);
      dphase = (uint16_t)((phase_max*(float)alarm_freq[alarm_ind])/pwm_clock);
      sei();
    }
    /* make_tone(1500, (float)0.125);
    make_silence(0.125);
    make_tone(1500, (float)0.125);
    make_silence(0.125);
    make_tone(1500, (float)0.125);
    make_silence(1.0);
    */
    
  }

  return 0;
}  

ISR(TIMER2_OVF_vect) {

  if(!alarm_on) return;

  cli();
  samples--;
  if (samples == 0) {
  
    /* make the alarm go from start to stop, and repeats that */
    alarm_ind++;
    if (alarm_ind > alarm_st_sp[alarm_num][1]) {
      alarm_ind = alarm_st_sp[alarm_num][0];
    }
    
    samples = (uint32_t)((float)alarm_secs[alarm_ind]*pwm_clock);
    dphase = (uint16_t)((phase_max*(float)alarm_freq[alarm_ind])/pwm_clock);
  }

  /* 16bit phase_acc will roll over on its own */
  phase_acc += dphase;
  /* only take most significant 8 bits to find sine value in table */
  OCR2B = sine_table[(phase_acc>>8)];

  sei();
}


