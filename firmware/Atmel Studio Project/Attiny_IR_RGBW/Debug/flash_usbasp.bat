avrdude -p t13 -c usbasp -u -Uflash:w:Attiny_IR_RGBW.hex:a -Ulfuse:w:0x71:m -Uhfuse:w:0xFD:m