set (CMAKE_SYSTEM_NAME "Generic")
set (CMAKE_C_COMPILER "avr-gcc")
set (CMAKE_C_FLAGS "-pipe -Os -mmcu=atmega328 -DF_CPU=16000000UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums" CACHE STRING "arduino cflags" FORCE)
