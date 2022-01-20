/* 
 * -----------------------------------------------------------------------------
 * Library: MyButton
 * This is a push button debouncing and "enhancing" library.
 * It allows for:
 *    - Detection of pressing events on both the RISING/FALLING edges
 *    - Association of multiple behaviors for a single button by creating      
 *      multiple "checkpoints" for the button to report reaching, upon release
 *    - Getting the progress of the pushed button towards a target period in
 *      a number of specified steps
 *    - Getting the time the button have been clicked for in 3 units 
 *      (seconds, milliseconds, microseconds)
 * All happening in a non-blocking manner.
 * -----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 21-08-2021 (21st of August, 2021)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#include "MyButton.h"


/*
 * Constructor that defaults to a debounce time of 5ms
 * ------------------------------------------------------------------
 * @pram pin: Button pin
 * @param off_state_: Is it pulled UP or DOWN 
 *                    (if off_state_ == NORMAL_UP; pulled UP, then
 *                     we'll apply an internal PULL UP)
 */
MyButton :: MyButton(uint8_t pin, uint8_t off_state_){
  MyButton(pin, off_state_, DEFAULT_DEBOUNCE);
}

/*
 * Constructor that configures the debounce time according to the user's pref
 * ------------------------------------------------------------------
 * @pram pin: Button pin
 * @param off_state_: Is it pulled UP or DOWN 
 *                    (if off_state_ == NORMAL_UP; pulled UP, then
 *                     we'll apply an internal PULL UP)
 * @param debounce_t: Debouncing time in ms 
 */
MyButton :: MyButton(uint8_t pin, uint8_t off_state_, uint8_t debounce_t){
  button_pin = pin;
  off_state = off_state_;
  debounce_time = debounce_t;
  flag = 0x00;

  switch(off_state){
    case NORMAL_UP:
      pinMode(pin, INPUT_PULLUP);
      break;

    case NORMAL_DOWN:
      pinMode(pin, INPUT);
      break;
  }
}

/*
 * Returns whether the button was pressed or not with the debounce time (FALLING EDGE)
 */
bool MyButton :: readFallingClick(){
  switch((flag & BTN_STATE_BITMASK) >> 2){
    case READ_BTN:
      if(digitalRead(button_pin) == (1 - off_state)){
        time_since_clicked = millis();
        flag = (flag & ~BTN_STATE_BITMASK) | (1 << 2);
      }
      break;
    case WAIT_BTN:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_BITMASK;
      }
      else if(millis() - time_since_clicked >= debounce_time){
        flag = (flag & ~BTN_STATE_BITMASK) | (2 << 2); // We have a true click
      }
      break;
    case TRUE_CLICK:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_BITMASK;
        return true;
      }
    default: break;
  }
  return false;
}

/*
 * Returns whether the button was pressed or not with the debounce time (RISING EDGE)
 */
bool MyButton :: readRisingClick(){
  switch((flag & BTN_STATE_BITMASK) >> 2){
    case READ_BTN:
      if(digitalRead(button_pin) == (1 - off_state)){
        time_since_clicked = millis();
        flag = (flag & ~BTN_STATE_BITMASK) | (1 << 2) | RISED_BITMASK;
      }
      break;
    case WAIT_BTN:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_BITMASK;
      }
      else if((millis() - time_since_clicked >= debounce_time) && (flag & RISED_BITMASK)){
        flag &= ~RISED_BITMASK;
        return true;
      }
      break;
    default: break;
  }
  return false;
}

/*
 * Returns the time the button has been clicked for in one of 3 units 
 * (micros, millis, seconds) NON-DEBOUNCED
 * ------------------------------------------------------------------
 * @param unit: Takes one of three values, (IN_SECONDS, IN_MILLIS, IN_MICROS)
 *              and returns the time in the specified unit
 */
uint32_t MyButton :: readTimedPress(uint8_t unit){
  switch((flag & BTN_STATE_BITMASK) >> 2){
    case READ_BTN:
      if(digitalRead(button_pin) == (1 - off_state)){
        timed_read = micros();
        flag = (flag & ~BTN_STATE_BITMASK) | (1 << 2);
      }
      break;
      
    case WAIT_BTN:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_BITMASK;
        switch(unit){
          case IN_SECONDS:
            return (uint32_t)((micros() - timed_read) / 1e6);
          
          case IN_MILLIS:
            return (uint32_t)((micros() - timed_read) / 1e3);
          
          case IN_MICROS:
            return micros() - timed_read; 
        }
      }
      break;
  }
  return 0;
}

/*
 * Returns the timed steps' index
 * ------------------------------------------------------------------
 * @brief: This function takes in a period in Ms and a number of steps,
 * and on each step of that period (step == period/num_steps), returns
 * the index of the step, 0 INDEXED, else returns 255.
 * ------------------------------------------------------------------
 * @param period: The total period to be split in steps 
 * @param num_steps: The number of steps to split the period on
 */
uint8_t MyButton :: readInSteps(uint32_t period, uint8_t num_steps){
  /* Obviously */
  if(num_steps == 0) return NON_CLICKED;
  
  switch((flag & BTN_STATE_STEP_BITMASK) >> 4){
    case READ_BTN:
      step = 0;
      if(digitalRead(button_pin) == (1 - off_state)){
        time_since_clicked = millis();
        flag = (flag & ~BTN_STATE_STEP_BITMASK) | (1 << 4);
      }
      return NON_CLICKED;
    
    case WAIT_BTN:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_STEP_BITMASK;
      }
      else if(millis() - time_since_clicked >= debounce_time){
        flag = (flag & ~BTN_STATE_STEP_BITMASK) | (2 << 4); // We have a true click
      }
      break;
    case TRUE_CLICK:
      if(digitalRead(button_pin) == off_state){
        flag &= ~BTN_STATE_STEP_BITMASK;
        if(step < num_steps - 1) return ABORTED_STEPS;
      }
      else if(millis() - time_since_clicked >= (period/num_steps)*(step + 1)){
        return step++;
      }
      break;
  }
}

/*
 * Returns, if the button have been pressed for longer than one of the
 * periods in the input list, the index of the corresponding period
 * ------------------------------------------------------------------
 * @brief: This function takes in a list of periods in Ms, and if the
 * button have been pressed for more than one of the periods 
 * (CHECKED ON RELEASE), it'd return the index of the period in the list,
 * else it returns 255.
 * THIS IS ZERO INDEXED!
 * ------------------------------------------------------------------
 * @param periods: List of the periods in Ms (INCREMENTAL ORDER)
 * @param len: Number of periods
 */
uint8_t MyButton :: readMultiple(uint32_t * periods, uint8_t len){
  switch((flag & BTN_STATE_MULTIPLE_BITMASK) >> 6){
    case READ_BTN:
      if(digitalRead(button_pin) == (1 - off_state)){
        time_since_clicked = millis();
        flag |= BTN_STATE_MULTIPLE_BITMASK;
      }
      return NON_CLICKED;

    case WAIT_BTN:
      if(digitalRead(button_pin) == off_state){
        for(uint8_t idx = len - 1; idx >= 0; idx--){
          if(millis() - time_since_clicked >= periods[idx]){
            flag &= ~BTN_STATE_MULTIPLE_BITMASK;
            return idx;
          }
        }
        flag &= ~BTN_STATE_MULTIPLE_BITMASK;
      }
      break;
  }
}
