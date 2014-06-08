set (CMAKE_SYSTEM_NAME "Generic")
set (CMAKE_C_COMPILER "avr-gcc-4.8.2")
set (CMAKE_C_FLAGS "-Werror -Wno-maybe-uninitialized -mmcu=atmega328 -DF_CPU=16000000UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums" CACHE STRING "avr-gcc-4.8.2 cflags")
