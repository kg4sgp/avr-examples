/* Two tones on one pwm line.
 * TODO: Extend to more than 2 tones.
 * TODO: make nicer interface.
 *
 * Currently this plays signaling and DTMF tones used for
 * switching and signaling on the Plain Old Telephone System (POTS).
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sine.h"

/* interrupt variables */
volatile uint16_t phase_acc[2];
volatile uint16_t dphase[2];
volatile uint32_t samples = 0;
volatile uint8_t go = 0;
/* end interrupt variables */

uint16_t phase_max = 65535;
uint16_t pwm_clock = 62500; /* if no prescale */

uint16_t tones[2];

void play_tones(float);
void play_silence(float);
uint16_t get_dphase(uint16_t);

void play_tones(float seconds)
{
  /* wait for previous tone to stop */
  while(go) __asm__("NOP");
  dphase[0] = get_dphase(tones[0]);
  dphase[1] = get_dphase(tones[1]);
  samples = (uint32_t)(seconds*pwm_clock);
  go = 1;
}

void play_silence(float seconds)
{
  /* wait for previous tone to stop */
  while(go) __asm__("NOP");
  samples = (uint32_t)(seconds*pwm_clock);
  phase_acc[0] = 0;
  phase_acc[1] = 0;
  dphase[0] = 0;
  dphase[1] = 0;
  go = 1;
}

uint16_t get_dphase(uint16_t freq)
{
  return (uint16_t)((phase_max*(float)freq)/pwm_clock);
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

    /* dial tone */
  
    tones[0] = 350;
    tones[1] = 440;
    play_tones(5);
    
    /* press all of the DTMF numbers */
  
    tones[0] = 697;
    tones[1] = 1209;
    play_tones(0.1);
    play_silence(0.05);  
    
    tones[0] = 697;
    tones[1] = 1336;
    play_tones(0.1);
    play_silence(0.05);   
    
    tones[0] = 697;
    tones[1] = 1477;
    play_tones(0.1);
    play_silence(0.05);
    
    tones[0] = 697;
    tones[1] = 1633;
    play_tones(0.1);
    play_silence(0.05);
    
    
    
    tones[0] = 770;
    tones[1] = 1209;
    play_tones(0.1);
    play_silence(0.05);  
    
    tones[0] = 770;
    tones[1] = 1336;
    play_tones(0.1);
    play_silence(0.05);   
    
    tones[0] = 770;
    tones[1] = 1477;
    play_tones(0.1);
    play_silence(0.05);
    
    tones[0] = 770;
    tones[1] = 1633;
    play_tones(0.1);
    play_silence(0.05);
    
    
    tones[0] = 852;
    tones[1] = 1209;
    play_tones(0.1);
    play_silence(0.05);  
    
    tones[0] = 852;
    tones[1] = 1336;
    play_tones(0.1);
    play_silence(0.05);   
    
    tones[0] = 852;
    tones[1] = 1477;
    play_tones(0.1);
    play_silence(0.05);
    
    tones[0] = 852;
    tones[1] = 1633;
    play_tones(0.1);
    play_silence(0.05);
    
    
    
    tones[0] = 941;
    tones[1] = 1209;
    play_tones(0.1);
    play_silence(0.05);  
    
    tones[0] = 941;
    tones[1] = 1336;
    play_tones(0.1);
    play_silence(0.05);   
    
    tones[0] = 941;
    tones[1] = 1477;
    play_tones(0.1);
    play_silence(0.05);
    
    tones[0] = 941;
    tones[1] = 1633;
    play_tones(0.1);
    play_silence(0.05);
    
    /* ring back tone */
    uint8_t j;
    for (j = 0; j < 2; j++) {
      tones[0] = 480;
      tones[1] = 440;
      play_tones(2);
      play_silence(2);
    }
    
    /* busy tone */    
    for (j = 0; j < 10; j++) {
      tones[0] = 480;
      tones[1] = 620;
      play_tones(0.5);
      play_silence(0.5);
    }
  }

  return 0;
}  

ISR(TIMER2_OVF_vect) {

  if (!go) return;

  cli();
  /* 16bit phase_acc will roll over on its own */
  phase_acc[0] += dphase[0];
  phase_acc[1] += dphase[1];
  
  /* only take most significant 8 bits to find sine value in table */
  OCR2B = (uint8_t)((sine_table[(phase_acc[0]>>8)]/2) + (sine_table[(phase_acc[1]>>8)]/2));
  
  samples--;
  if(samples == 0) go = 0;
  sei();
}


