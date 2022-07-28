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
const uint8_t lever_weapons_pin = A4;
//const uint8_t lever_shield_pin = A4;
//const uint8_t lever_engine_pin = A3;
//const uint8_t lever_sensors_pin = A2;
const uint8_t btn_laser_pin = A5;
//const uint8_t btn_torpedo_pin = A0;

//demultiplexador das entradas
//Latch_SN7HC259N selector(SELECT_A_0, SELECT_A_1, SELECT_A_2, SELECT_D, SELECT_nLE, SELECT_nMR);

Latch_SN74HC259N l_weapon(A_0, A_1, A_2, D, nLE, nMR);
//Latch_SN74HC259N l_shield(A_0, A_1, A_2, D, nLE, nMR);
//Latch_SN74HC259N l_engine(A_0, A_1, A_2, D, nLE, nMR);
//Latch_SN74HC259N l_sensor(A_0, A_1, A_2, D, nLE, nMR);

Feature weapons(1, 9, 3, 1, 1);
//Feature shield (1, 9, 6, 3, 1);
//Feature engine (1, 9, 0, 3, 3);
//Feature sensors(1, 9, 0, 1, 3);

uint8_t torpedoes = 3;

//Spaceship inputs
boolean lever_weapons; //stores the current state of the lever. true if it is on
//boolean lever_shield;
//boolean lever_engine;
//boolean lever_sensors;
boolean btn_laser; //stores the current state of the button. true if it is pushed. lasers are fired after it is released
//boolean btn_torpedo;

unsigned long l_weapons_elapsed;
//unsigned long l_shield_elapsed;
//unsigned long l_engine_elapsed;
//unsigned long l_sensors_elapsed;

unsigned long const EVENT_LAPSE_DURATION = 10000; //10 seconds



void setup() {
  Serial.begin(9600);

  pinMode(lever_weapons_pin, INPUT);
  //pinMode(lever_shield_pin, INPUT);
  //pinMode(lever_engine_pin, INPUT);
  //pinMode(lever_sensors_pin, INPUT);

  pinMode(btn_laser, INPUT);
  //pinMode(btn_torpedo,  INPUT);

  l_weapon.resetMode();
  l_weapon.memoryMode();

  /*
  l_shield.resetMode();
  l_shield.memoryMode();

  l_engine.resetMode();
  l_engine.memoryMode();

  l_sensor.resetMode();
  l_sensor.memoryMode();

  selector.resetMode();
  selector.demuxMode();//nope
  */

  l_weapons_elapsed = millis();

  lever_weapons = digitalRead(lever_weapons_pin);
  //lever_shield  = digitalRead(lever_shield_pin);
  //lever_engine  = digitalRead(lever_engine_pin);
  //lever_sensors = digitalRead(lever_sensors_pin);
  btn_laser     = digitalRead(btn_laser_pin);
  //btn_torpedo   = digitalRead(btn_torpedo_pin);
}

void loop() {
  //if the lever changes, we update the time keeping variable to count from that event
  if ( lever_weapons != digitalRead(lever_weapons_pin) ) {
    l_weapons_elapsed = millis();
    lever_weapons = digitalRead(lever_weapons_pin);
  }

  //produce the weapons degradation or recovery
  //should the lever remain unchanged for EVENT_LAPSE_DURATION we can 
  if ( millis() - l_weapons_elapsed > EVENT_LAPSE_DURATION ) {
    l_weapons_elapsed = millis();

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
  Serial.println( weapons.value() );
  l_weapon.set( weapons.value() );
}

