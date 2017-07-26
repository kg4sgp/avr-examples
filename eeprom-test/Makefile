ARDUINO := /usr/share/arduino
AVR := /usr/avr

paranoid: eeprom-test.c
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
	eeprom-test.c -o eeprom-test.c.o

pedantic: eeprom-test.c
	avr-g++ -c -g -Os -Wall -pedantic -ansi -fno-exceptions -ffunction-sections -fdata-sections \
	-mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 \
	eeprom-test.c -o eeprom-test.c.o

link: paranoid
	avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328p -o eeprom-test.c.elf eeprom-test.c.o
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 eeprom-test.c.elf eeprom-test.c.eep 
	avr-objcopy -O ihex -R .eeprom eeprom-test.c.elf eeprom-test.c.hex
	avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328p -o eeprom-test.c.elf eeprom-test.c.o

clean:
	rm -f *.elf *.o *.d *.eep *.hex

splint:
	splint ${AVR}/include/avr/iom328p.h -dump iom328p
	splint +load iom328p -DF_CPU=16000000L -I${AVR}/include *.c

flash:
	avrdude -p m328p -c arduino -P /dev/ttyUSB0 -vvv -b 57600 -D -U flash:w:eeprom-test.c.hex

# Add all analysis tools here to run them all with one command.
analyze:
	make splint
