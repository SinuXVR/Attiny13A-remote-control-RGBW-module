# Flash command

AVRISP:
```
avrdude -p t13 -c avrisp -b 19200 -u -Uflash:w:Attiny_IR_RGB.hex:a -Ulfuse:w:0x71:m -Uhfuse:w:0xFD:m -Ulock:w:0xFC:m
```

USBAsp:
```
avrdude -p t13 -c usbasp -u -Uflash:w:Attiny_IR_RGB.hex:a -Ulfuse:w:0x71:m -Uhfuse:w:0xFD:m -Ulock:w:0xFC:m
```