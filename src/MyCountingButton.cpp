#include "MyCountingButton.h"

MyCoutingButton :: MyCoutingButton(){
  resetCount();
  _trig_flag = _trigger_on_count = _operate_interrupt = false;
}

/* ---------------------------------------------------------------------------------- */
/* ------------------------- BUILD INTERRUPTION COUNTER ----------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCoutingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void)){
  beginCountingInterrupter(irq_pin, _ISR_callback, DEFUALT_DIRECTION, FALLING);
}

void MyCoutingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_){
  beginCountingInterrupter(irq_pin, _ISR_callback, dir_, FALLING);
}

void MyCoutingButton :: beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_, uint8_t trigger_on){
  _operate_interrupt = true;
  _direction = dir_;
  attachInterrupt(digitalPinToInterrupt(irq_pin), _ISR_callback, trigger_on);
}

/* ---------------------------------------------------------------------------------- */
/* ----------------------------- ISR COUNTER CALLBACK ------------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCoutingButton :: countingInterruption(){
  _trigger_time = millis(); // millis() doesn't increment inside an ISR, but we just need the value it holds
  _trig_flag = true;
}

/* ---------------------------------------------------------------------------------- */
/* ---------------------------- BUILD NORMAL COUNTER -------------------------------- */
/* ---------------------------------------------------------------------------------- */

void MyCoutingButton :: begin(uint8_t pin){
  begin(pin, DEFAULT_OFF_STATE, DEFUALT_DIRECTION, DEFAULT_DEBOUNCE);
}

void MyCoutingButton :: begin(uint8_t pin, uint8_t off_state){
  begin(pin, off_state, DEFUALT_DIRECTION, DEFAULT_DEBOUNCE);
}

void MyCoutingButton :: begin(uint8_t pin, uint8_t off_state, uint8_t dir){
  begin(pin, off_state, dir, DEFAULT_DEBOUNCE);
}

void MyCoutingButton :: begin(uint8_t pin, uint8_t off_state, uint8_t dir, uint8_t debounce_t){
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

void MyCoutingButton :: setupTriggerOnCount(long count, void (*callback)(void)){
  _trigger_on_count = true;
  _count_callback = callback; // Assign the function to be called when the goal count is reached
  setTriggerCount(count);
}

void MyCoutingButton :: setDirection(int8_t direction){ _direction = direction; }

void MyCoutingButton :: resetCount(){ _counter = 0; }

void MyCoutingButton :: setCount(long count){ _counter = count; }

void MyCoutingButton :: setTriggerCount(long count){ _trigger_count = count != 0 ? count : _trigger_count; }

/* ----------------------------------------------------------------------------------- */
/* -------------------------- KEY FUNCTION FOR COUNTER ------------------------------- */
/* ----------------------------------------------------------------------------------- */

long MyCoutingButton :: getCount(){ return _counter; }

/* ----------------------------------------------------------------------------------- */
/* ------------------------- LOOP (SHOWROOM) FOR COUNTER ----------------------------- */
/* ----------------------------------------------------------------------------------- */

void MyCoutingButton :: loopCounter(){

  // We check if there's a function to be called when a certain action happens
  if(_trigger_on_count && _counter == _trigger_count) (*_count_callback)();

  // Operation mode based on whether we're couting with interrupts or not
  switch(_operate_interrupt){ 
    case true:{

      // ISR was triggered and button is still pressed
      if(_trig_flag && (millis() - _trigger_time >= _debounce_time)){
        _counter += _direction;
        _trig_flag = false;
      }
      break;
    } // END _ Interrupt case

    case false:{
      static uint8_t pin_state;
      static unsigned long time_since_clicked;

      switch(pin_state){
        case 0:
          if(digitalRead(_button_pin) == (1 - _off_state)){
            pin_state = 1;
            time_since_clicked = millis();
          }
        break;

        case 1:
          if(millis() - time_since_clicked >= debounce_time){
            pin_state = 2; // True click
            
            #error "Imlement this!" // Increment counter on rising edge
          }
          else if(digitalRead(_button_pin) == _off_state){
            pin_state = 0; // Bounce
          }
        break;

        case 2:
          if(digitalRead(_button_pin) == _off_state){
            pin_state = 0; // Released

            #error "Imlement this!" // Increment counter on falling edge (or change)
          }
        break;

        default: break;
      }
      
      break;
    } // END _ Non interrupt case
  }
  
}
