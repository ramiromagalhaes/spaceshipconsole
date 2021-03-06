#ifndef __FEATURE_H__
#define __FEATURE_H__

#include <Arduino.h>

class Feature {
  public:
    Feature(uint8_t _min = 0, uint8_t _max = 10, uint8_t cost = 1, uint8_t degrade_rate = 1, uint8_t recover_rate = 1)
      : _min(_min), _max(_max), cost(cost), degrade_rate(degrade_rate), recover_rate(recover_rate), current(_max) {
        current = _min;
    }

    boolean use() {
      if (current - _min < cost) {
        return false;
      }

      current -= cost;
      return true;
    }

    void degrade() {
      if (current - _min < degrade_rate) {
        current = _min;
      } else {
        current -= degrade_rate;
      }
    }

    void recover() {
      const uint16_t sum = current + recover_rate;

      if (sum > _max) {
        current = _max;
      } else {
        current = sum;
      }
    }

    uint8_t value() const {
      return current;
    }

    uint8_t minimum() const {
      return _min;
    }

    uint8_t maximum() const {
      return _max;
    }

  private:
    const uint8_t _min, _max; //minimum and maximum value this feature can have
    uint8_t current, //current value for this feature
      cost,          //cost for using the feature
      degrade_rate,  //the amount that should be reduced from the feature's current value whenever it degrades
      recover_rate;  //the amount that should be added to the feature's current value whenever it recovers
};

#endif
