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

#ifndef __MY_BUTTON_H__
  #define __MY_BUTTON_H__

  #include "Configs.h"
  
  #define NON_CLICKED                    255   /* Value to be returned when no event is being captured */
  #define ABORTED_STEPS                  254   /* Is case we're doing a read in steps and a user stops 
                                                  in the middle of the click, we return this value     */
  
  #define READ_BTN                       0     /* Button states */
  #define WAIT_BTN                       1
  #define TRUE_CLICK                     2

  #define IN_SECONDS                     1     /* Units to return the timed-read in */
  #define IN_MILLIS                      2
  #define IN_MICROS                      3

  /* Flag bitmasks (according to the structure down delow) */
  #define RISED_BITMASK                  0x80
  #define BTN_STATE_MULTIPLE_BITMASK     0x40
  #define BTN_STATE_STEP_BITMASK         0x30
  #define BTN_STATE_BITMASK              0x0C
  
  /* The class containing all functionalities */
  class MyButton{
    
    private:

      /* Variables used insed the functions to achieve the non-blocking behaviour */

      /* 
       * FLAG STRUCTURE :
       * +-------+--------------------+-------------------+-------------------+---------------+-------------+------------+------------+
       * | 1 Bit |       1 Bit        |                 2 Bits                |            2 Bits           |   1 Bit    |   1 Bit    |
       * +-------+--------------------+-------------------+-------------------+---------------+-------------+------------+------------+
       * | RISED | BTN STATE MULTIPLE | BTN STATE STEP(H) | BTN STATE STEP(L) | BTN STATE (H) | BTN STEP(L) |            |            |
       * +-------+--------------------+-------------------+-------------------+---------------+-------------+------------+------------+
       */
      uint8_t flag;
      
      uint8_t step; //Keeps track of the step in the read_steps function
      unsigned long time_since_clicked; // Keeps track of the timings
      uint32_t timed_read; // To hold the timed read time in microseconds
      
      /* Button parameters */
      uint8_t button_pin;
      uint8_t debounce_time;
      uint8_t off_state;

    public:
    
      /* Constructors */
      MyButton(uint8_t pin, uint8_t off_state_);
      MyButton(uint8_t pin, uint8_t off_state_, uint8_t debounce_t);
      
      /* Functions */
      bool readRawClick();
      bool readRisingClick();
      bool readFallingClick();
      uint32_t readTimedPress(uint8_t unit);
      uint8_t readInSteps(uint32_t period, uint8_t num_steps);
      uint8_t readInProvidedSteps(uint32_t * periods, uint8_t num_steps);
      uint8_t readInProvidedSteps(uint32_t * periods, uint8_t num_steps, uint8_t starting_step);
      uint8_t readMultiple(uint32_t * periods, uint8_t len);
  };
  
#endif // __MY_BUTTON_H__
