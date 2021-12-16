# MAX31856 thermocouple sensor add-on library for SensESP

This repository implements a MAX31856 sensor library for [SensESP](https://signalk.org/SensESP/).
It is based on the [Adafruit_MAX31856](https://github.com/adafruit/Adafruit_MAX31856/) Arduino library and adds support for SensESP producer-consumer communication and web configuration UI.

MAX31856 is a precision thermocouple to digital converter with linearization.
It can digitize the signal from any type of thermocouple and output the value in Celsius over an SPI-compatible interface.

To use the library in your own projects, you have to include it in your `platformio.ini` `lib_deps` section:

    lib_deps =
        SignalK/SensESP@^2.0.0
        SensESP/MAX31856@^2.0.0

See also the [example main file](blob/main/examples/XXXX.cpp).

For more information on using SensESP and external add-on libraries, see the [SensESP documentation](https://signalk.org/SensESP/docs/).
