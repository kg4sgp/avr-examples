### Direct Digital Synthesis Example ###

A Direct Digital Synthesis (DDS) using PWM to output sine waves on AVR microcontrollers.

This program can be used to produce accurate sine waves on an AVR microcontroller using pulse with modulation. Frequency resolution is either 0.954 Hz or 0.119 Hz depending on weather a 62.5kHz or 7.8125kHz PWM clock is used respectively. The tuneing word is 16bits in width. 

If you are only using this to produce a few tone frequencies, pre-compute the dphase variable for your tones. By doing this you avoid costly floating point calculations.

This program is released under the MIT license. See the LICENSE file for more information.
