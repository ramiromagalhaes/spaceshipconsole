#include "Latch_SN7HC259N.h"
#include "feature.h"

const uint8_t A_0 = 0;
const uint8_t A_1 = 1;
const uint8_t A_2 = 2;
const uint8_t D = 4;
const uint8_t nLE = 5;
const uint8_t nMR = 6;

const uint8_t SELECT_A_0 = 7;
const uint8_t SELECT_A_1 = 8;
const uint8_t SELECT_A_2 = 9; //nao precisa ser usado
const uint8_t SELECT_D = 10;
const uint8_t SELECT_nLE = 11;
const uint8_t SELECT_nMR = 12;

//Those are the pins that will take inputs from the spaceship buttons and levers.
//Those should be the analog input pins properly configured to take binary (non analog) data.
const uint8_t lever_weapons_pin = A5;
const uint8_t lever_shield_pin = A4;
const uint8_t lever_engine_pin = A3;
const uint8_t lever_sensors_pin = A2;
const uint8_t btn_laser_pin = A1;
const uint8_t btn_torpedo_pin = A0;

//demultiplexador das entradas
Latch_SN7HC259N selector(SELECT_A_0, SELECT_A_1, SELECT_A_2, SELECT_D, SELECT_nLE, SELECT_nMR);

Latch_SN7HC259N l_weapon(A_0, A_1, A_2, D, nLE, nMR);
Latch_SN7HC259N l_shield(A_0, A_1, A_2, D, nLE, nMR);
Latch_SN7HC259N l_engine(A_0, A_1, A_2, D, nLE, nMR);
Latch_SN7HC259N l_sensor(A_0, A_1, A_2, D, nLE, nMR);

Feature weapons(1, 9, 3, 1, 1),
        shield (1, 9, 6, 3, 1),
        engine (1, 9, 0, 3, 3),
        sensors(1, 9, 0, 1, 3);

uint8_t torpedoes = 3;

//Spaceship inputs
boolean lever_weapons, lever_shield, lever_engine, lever_sensors, //stores the current state of the lever. true if it is on
  btn_laser, btn_torpedo; //stores the current state of the button. true if it is pushed. lasers are fired after it is released

unsigned long curr_time;
unsigned long elapsed;
unsigned long l_weapons_elapsed, l_shield_elapsed, l_engine_elapsed, l_sensors_elapsed;

unsigned long const EVENT_LAPSE_DURATION = 10000; //10 seconds



void setup() {
  pinMode(lever_weapons_pin, INPUT);
  pinMode(lever_shield_pin, INPUT);
  pinMode(lever_engine_pin, INPUT);
  pinMode(lever_sensors_pin, INPUT);
  pinMode(btn_laser, INPUT);
  pinMode(btn_torpedo,  INPUT);

  l_weapon.resetMode();
  l_weapon.memoryMode();

  l_shield.resetMode();
  l_shield.memoryMode();

  l_engine.resetMode();
  l_engine.memoryMode();

  l_sensor.resetMode();
  l_sensor.memoryMode();

  selector.resetMode();
  selector.demuxMode();//nope

  elapsed = millis();

  l_weapons_elapsed = millis();

  lever_weapons = digitalRead(lever_weapons_pin);
  lever_shield  = digitalRead(lever_shield_pin);
  lever_engine  = digitalRead(lever_engine_pin);
  lever_sensors = digitalRead(lever_sensors_pin);
  btn_laser     = digitalRead(btn_laser_pin);
  btn_torpedo   = digitalRead(btn_torpedo_pin);
}

void loop() {
  if (millis() - elapsed > EVENT_LAPSE_DURATION) {
    elapsed = millis();
  }



  //handle weapon switch
  //if the lever changes value, we'll have to wait for it 
  if ( lever_weapons != digitalRead(lever_weapons_pin) ) {
    l_weapons_elapsed = millis();
    lever_weapons = digitalRead(lever_weapons_pin);
  }

  //produce the weapons degradation or recovering
  if ( millis() - l_weapons_elapsed > EVENT_LAPSE_DURATION ) {
    if (lever_weapons) {
      weapons.recover();
    } else {
      weapons.degrade();
    }
  }



  //handle laser button push
  if ( btn_laser ) {
    if ( !digitalRead(btn_laser_pin) ) {
      btn_laser = false;
      if ( weapons.use() ) {
        //TODO fire laser
      } else {
        //TODO laser cannot be fired yet
      }
    }
  } else {
    if ( digitalRead(btn_laser_pin) ) {
      btn_laser = true;
    }
  }


  //update view
  l_weapon.set( weapons.value()/3 - 1 ); //we will only use 3 outputs
}

