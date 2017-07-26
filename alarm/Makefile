ARDUINO := /usr/share/arduino
AVR := /usr/avr

paranoid: alarm.c
	avr-gcc -c -g -Os -Wall -pedantic -ansi -fno-exceptions -ffunction-sections -fdata-sections \
	-std=iso9899:1999 \
	-Wall -pedantic \
  -Wmissing-prototypes \
	-pedantic-errors -Wextra -Wall -Waggregate-return -Wcast-align \
	-Wcast-qual -Wchar-subscripts -Wcomment -Wconversion \
	-Wdisabled-optimization \
	-Werror -Wfloat-equal -Wformat -Wformat=2 \
	-Wformat-nonliteral -Wformat-security \
	-Wformat-y2k \
	-Wimport -Winit-self -Winline \
	-Winvalid-pch \
	-Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces \
	-Wmissing-field-initializers -Wmissing-format-attribute \
	-Wmissing-include-dirs -Wmissing-noreturn \
	-Wpacked -Wpadded -Wparentheses -Wpointer-arith \
	-Wredundant-decls -Wreturn-type \
	-Wsequence-point -Wshadow -Wsign-compare -Wstack-protector \
	-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default \
	-Wswitch-enum -Wtrigraphs -Wuninitialized \
	-Wunknown-pragmas -Wunreachable-code -Wunused \
	-Wunused-function -Wunused-label -Wunused-parameter \
	-Wunused-value -Wunused-variable -Wvariadic-macros \
	-Wvolatile-register-var -Wwrite-strings \
	-mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 \
	alarm.c -o alarm.c.o

link: paranoid
	avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328p -o alarm.c.elf alarm.c.o
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 alarm.c.elf alarm.c.eep 
	avr-objcopy -O ihex -R .eeprom alarm.c.elf alarm.c.hex

clean:
	rm -f *.elf *.o *.d *.eep *.hex

flash:
	avrdude -vvv -p atmega328p -c arduino -P /dev/ttyACM0 -U flash:w:alarm.c.hex

