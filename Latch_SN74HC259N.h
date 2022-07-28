#ifndef __LATCH_SN74HC259N_H__
#define __LATCH_SN74HC259N_H__

#include <Arduino.h>

/**
 * This is a simple class to control a 74HC259 latch from an Arduino board.
 */
class Latch_SN74HC259N
{
  public:
    Latch_SN74HC259N(const uint8_t a0_pin, const uint8_t a1_pin, const uint8_t a2_pin, const uint8_t d_pin, const uint8_t nLE_pin, const uint8_t nMR_pin)
      : a0_pin(a0_pin),
        a1_pin(a1_pin),
        a2_pin(a2_pin),
        d_pin(d_pin),
        nLE_pin(nLE_pin),
        nMR_pin(nMR_pin) {
      pinMode(a0_pin,  OUTPUT);
      pinMode(a1_pin,  OUTPUT);
      pinMode(a2_pin,  OUTPUT);
      pinMode(d_pin,   OUTPUT);
      pinMode(nLE_pin, OUTPUT);
      pinMode(nMR_pin, OUTPUT);
    }

    void resetMode() {
      digitalWrite(this->nMR_pin, LOW);
      digitalWrite(this->nLE_pin, HIGH);
    }

    void demuxMode(const uint8_t d = LOW) {
      digitalWrite(this->nMR_pin, LOW);
      digitalWrite(this->nLE_pin, LOW);
      digitalWrite(this->d_pin,   d);
    }

    void memoryMode() {
      digitalWrite(this->nMR_pin, HIGH);
      digitalWrite(this->nLE_pin, HIGH);
    }

    void addressableLatchMode() {
      digitalWrite(this->nMR_pin, HIGH);
      digitalWrite(this->nLE_pin, LOW);
    }

    //address from 0 to 7
    void set(const uint8_t address, const uint8_t value) {
      if (address > 7) return; //throw exception?
      set(
        bitRead(address, 0),
        bitRead(address, 1),
        bitRead(address, 2),
        value
      );
    }

    void set(const uint8_t a_0, const uint8_t a_1, const uint8_t a_2, uint8_t value) {
      digitalWrite(this->nLE_pin, HIGH);
      digitalWrite(this->a0_pin, a_0);
      digitalWrite(this->a1_pin, a_1);
      digitalWrite(this->a2_pin, a_2);
      digitalWrite(this->nLE_pin, LOW);
      digitalWrite(this->d_pin, value);
      digitalWrite(this->nLE_pin, HIGH);
    }

    void set(const uint8_t value) {
      for(uint8_t i = 0; i < 8; i++) {
        digitalWrite(this->nLE_pin, HIGH);
        digitalWrite(this->a0_pin, bitRead(i, 0));
        digitalWrite(this->a1_pin, bitRead(i, 1));
        digitalWrite(this->a2_pin, bitRead(i, 2));
        digitalWrite(this->nLE_pin, LOW);
        digitalWrite(this->d_pin, bitRead(value, i));
      }
      digitalWrite(this->nLE_pin, HIGH);
    }

  private:
    const uint8_t a0_pin;  //Arduino's pin ID that is connected to the Latch's A0 pin
    const uint8_t a1_pin;  //Arduino's pin ID that is connected to the Latch's A1 pin
    const uint8_t a2_pin;  //Arduino's pin ID that is connected to the Latch's A2 pin
    const uint8_t d_pin;   //Arduino's pin ID that is connected to the Latch's D pin
    const uint8_t nLE_pin; //Arduino's pin ID that is connected to the Latch's nLE pin
    const uint8_t nMR_pin; //Arduino's pin ID that is connected to the Latch's nMR pin
};

#endif
