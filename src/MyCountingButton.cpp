/* 
 * -----------------------------------------------------------------------------
 * Library: MyCountingButton
 * This is a sequel to the "MyButton" library that allows for couting buttons.
 * It allows for:
 *    - Couting of clicks on the RISING/FALLING/CHANGING edges
 *    - Couting on hardware interruption enabled pins (expl: for encodes),
 *      or any "normal" pin
 *    - Triggering of a custom callback function when a set value is reached
 *    - Set the counting direction (++/--), and set a custom value to count from 
 * All happening in a non-blocking manner.
 * -----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 15-01-2022 (15th of January, 2022)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#include "MyCountingButton.h"

MyCountingButton :: MyCountingButton(){
  resetCount();
  _trig_flag = _trigger_on_count = _operate_interrupt = false;
  _profile = ON_FALLING; // Default counting profile is Falling edge
}

/* ---------------------------------------------------------------------------------- */
/* ------------------------- BUILD INTERRUPTION COUNTER ----------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCountingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void)){
  beginCountingInterrupter(irq_pin, _ISR_callback, DEFUALT_DIRECTION, FALLING);
}

void MyCountingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_){
  beginCountingInterrupter(irq_pin, _ISR_callback, dir_, FALLING);
}

void MyCountingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_, uint8_t trigger_on){
  _operate_interrupt = true;
  _direction = dir_;
  pinMode(irq_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(irq_pin), _ISR_callback, trigger_on);
}

/* ---------------------------------------------------------------------------------- */
/* ----------------------------- ISR COUNTER CALLBACK ------------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCountingButton :: countingInterruption(){
  _trigger_time = millis(); // millis() doesn't increment inside an ISR, but we just need the value it holds
  _trig_flag = true;
}

/* ---------------------------------------------------------------------------------- */
/* ---------------------------- BUILD NORMAL COUNTER -------------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCountingButton :: begin(uint8_t pin){
  begin(pin, DEFAULT_OFF_STATE, DEFUALT_DIRECTION, DEFAULT_DEBOUNCE);
}

void MyCountingButton :: begin(uint8_t pin, uint8_t off_state){
  begin(pin, off_state, DEFUALT_DIRECTION, DEFAULT_DEBOUNCE);
}

void MyCountingButton :: begin(uint8_t pin, uint8_t off_state, uint8_t dir){
  begin(pin, off_state, dir, DEFAULT_DEBOUNCE);
}

void MyCountingButton :: begin(uint8_t pin, uint8_t off_state, uint8_t dir, uint8_t debounce_t){
  _button_pin = pin;
  _off_state = off_state;
  _direction = dir;
  _debounce_time = debounce_t;

  switch(off_state){
    case NORMAL_UP:
      pinMode(_button_pin, INPUT_PULLUP);
      break;

    case NORMAL_DOWN:
      pinMode(_button_pin, INPUT);
      break;
  }
}

/* ---------------------------------------------------------------------------------- */
/* ------------------------- SETUP FUNCTIONS FOR COUNTER ---------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCountingButton :: setupTriggerOnCount(long count, void (*callback)(void)){
  _trigger_on_count = true;
  _count_callback = callback; // Assign the function to be called when the goal count is reached
  setTriggerCount(count);
}

void MyCountingButton :: setDirection(int8_t direction){ _direction = direction; }

void MyCountingButton :: resetCount(){ _counter = 0; }

void MyCountingButton :: setCount(long count){ _counter = count; }

void MyCountingButton :: setTriggerCount(long count){ _trigger_count = count != 0 ? count : _trigger_count; }

void MyCountingButton :: setCountingProfile(uint8_t profile){ _profile = profile <= 2 ? profile : _profile; }

/* ----------------------------------------------------------------------------------- */
/* -------------------------- KEY FUNCTION FOR COUNTER ------------------------------- */
/* ----------------------------------------------------------------------------------- */

long MyCountingButton :: getCount(){ return _counter; }

/* ----------------------------------------------------------------------------------- */
/* ------------------------- LOOP (SHOWROOM) FOR COUNTER ----------------------------- */
/* ----------------------------------------------------------------------------------- */

void MyCountingButton :: _update_counter(){ _counter += _direction; }

void MyCountingButton :: loopCounter(){

  // We check if there's a function to be called when a certain action happens
  if(_trigger_on_count && _counter == _trigger_count) (*_count_callback)();

  // Functioning in ISR mode and ISR was triggered and button is still pressed
  if(_operate_interrupt && _trig_flag && (millis() - _trigger_time >= _debounce_time)){
    update_counter();
    _trig_flag = false;
    return;
  }

  // We're only here if there's no interrupt routine
  static uint8_t pin_state;
  static unsigned long time_since_clicked;

  switch(pin_state){
    case READ_:
      if(digitalRead(_button_pin) == (1 - _off_state)){
        pin_state = CLICK_;
        time_since_clicked = millis();
      }
    break;

    case CLICK_:
      if(millis() - time_since_clicked >= _debounce_time){
        pin_state = RELEASE_; // True click
        if(_profile == ON_CHANGE || _profile == ON_RISING) _update_counter();
      }
      else if(digitalRead(_button_pin) == _off_state){
        pin_state = READ_; // Bounce
      }
    break;

    case RELEASE_:
      if(digitalRead(_button_pin) == _off_state){
        pin_state = READ_; // Released
        if(_profile == ON_CHANGE || _profile == ON_FALLING) _update_counter();
      }
    break;

    default: break;
  } // END _ BTN Reading State Machine 

}
