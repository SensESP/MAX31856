#include <Adafruit_MAX31856.h>

#include "sensesp/signalk/signalk_output.h"
#include "sensesp/transforms/linear.h"
#include "sensesp_app_builder.h"
#include "sensesp_max31856/max31856.h"

using namespace sensesp;

#define SPI_CS_PIN 15
#define SPI_MOSI_PIN 13
#define SPI_MISO_PIN 12
#define SPI_CLK_PIN 14
// There is a version of the constructor for the MAX31856Thermocouple that can
// use "hardware SPI", so you don't have to specify the pins. See
// https://github.com/SignalK/SensESP/blob/master/src/sensors/max31856_thermocouple.h
// and
// https://learn.adafruit.com/adafruit-max31856-thermocouple-amplifier/wiring-and-test

// SensESP builds upon the ReactESP framework. Every ReactESP application
// defines an "app" object vs defining a "main()" method.
ReactESP app;

void setup() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // Create the global SensESPApp() object.
  SensESPAppBuilder builder;
  sensesp_app = builder.get_app();

  // The "Signal K path" identifies this sensor to the Signal K server. Leaving
  // this blank would indicate this particular sensor (or transform) does not
  // broadcast Signal K data.
  // To find valid Signal K Paths that fits your need you look at this link:
  // https://signalk.org/specification/1.4.0/doc/vesselsBranch.html
  const char* sk_path = "propulsion.Main_Engine.exhaustTemperature";

  // The "Configuration path" is combined with "/config" to formulate a URL
  // used by the RESTful API for retrieving or setting configuration data.
  // It is ALSO used to specify a path to the SPIFFS file system
  // where configuration data is saved on the MCU board. It should
  // ALWAYS start with a forward slash if specified. If left blank,
  // that indicates this sensor or transform does not have any
  // configuration to save, or that you're not interested in doing
  // run-time configuration.

  const char* exhaust_temp_config_path =
      "/propulsion/Main_Engine/exhaustTemperature/read_delay";
  // const char* linear_config_path =
  // "/propulsion/Main_Engine/exhaustTemperature/linear";

  // Create a sensor that is the source of our data, that will be read every
  // 1000 ms.
  const uint read_delay = 1000;

  // Create the hardware sensor object
  // See
  // https://github.com/SignalK/SensESP/blob/master/src/sensors/max31856_thermocouple.h
  Adafruit_MAX31856* ada_max31856 = new Adafruit_MAX31856(
      SPI_CS_PIN, SPI_MOSI_PIN, SPI_MISO_PIN, SPI_CLK_PIN);

  // thermocouple type:  MAX31856_TCTYPE_K;
  // other types can be B, E, J, N, R, S, T
  ada_max31856->setThermocoupleType(MAX31856_TCTYPE_K);

  // Create the sensor object
  auto* max31856 = new MAX31856(ada_max31856, read_delay);

  // A Linear transform takes its input, multiplies it by the multiplier, then
  // adds the offset, to calculate its output. The MAX31856TC produces
  // temperatures in degrees Celsius. We need to change them to Kelvin for
  // compatibility with Signal K.

  const float multiplier = 1.0;
  const float offset = 273.16;

  // Wire up the output of the analog input to the Linear transform,
  // and then output the results to the Signal K server.
  max31856->connect_to(new Linear(multiplier, offset, ""))
      ->connect_to(new SKOutputFloat(sk_path));

  // Start the SensESP application running
  sensesp_app->start();
}

// main program loop
void loop() { app.tick(); }
