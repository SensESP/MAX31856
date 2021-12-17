#ifndef _SENSESP_SENSORS_MAX31856_H_
#define _SENSESP_SENSORS_MAX31856_H_
#include "Adafruit_MAX31856.h"
#include "sensors/sensor.h"

namespace sensesp {

class MAX31856 : public RepeatSensor<float> {
 public:
  /**
   * @brief Construct a new MAX31856 object.
   * 
   * MAX31856 is a thermocoupler interface chip that can be used to
   * read the temperature from a thermocouple.
   * 
   * An Adafruit_MAX31856 object must be provided as an argument.
   * Polling then happens automatically in this class.
   * 
   * @param max31856 
   * @param read_delay 
   */
  MAX31856(Adafruit_MAX31856* max31856, unsigned int read_delay = 1000)
      : RepeatSensor<float>(read_delay, callback), max31856_{max31856} {
    if (this->repeat_interval_ms_ < 500) {
      this->repeat_interval_ms_ = 500;
    }
  }

 protected:
  static void callback(RepeatSensor<float>* self) {
    MAX31856* cself = static_cast<MAX31856*>(self);
    cself->max31856_->triggerOneShot();
    ReactESP::app->onDelay(450, [cself]() {
      cself->emit(cself->max31856_->readThermocoupleTemperature() + 273.15);
    });
  }

  Adafruit_MAX31856* max31856_;
};

}  // namespace sensesp

#endif
