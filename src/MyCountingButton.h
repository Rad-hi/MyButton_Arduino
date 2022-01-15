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

#ifndef __MY_COUNTING_BUTTON_H__
  #define __MY_COUNTING_BUTTON_H__

  #include "Configs.h"

  // Counting "direction"
  #define ASCENDING                     1
  #define DESCENDING                    (-1)

  // Counting "profile"
  enum COUNTING_PROFILES{
       ON_CHANGE, 
       ON_RISING,
       ON_FALLING
  };
  
  // Button reading states
  enum BUTTON_CLICKS{
       READ_,
       CLICK_,
       RELEASE_,
  };

  #define DEFAULT_OFF_STATE             NORMAL_UP
  #define DEFUALT_DIRECTION             ASCENDING
  
  #define GET_ISR(BTN_OBJ, BTN_ISR)     ([]{(BTN_OBJ.BTN_ISR)();}) // Create a lambda function

  class MyCountingButton{
    private:

      // Button variables
      uint8_t _button_pin;
      uint8_t _debounce_time;
      uint8_t _off_state;

      bool _operate_interrupt;

      // Counting variables
      int8_t _direction; // Holds the counting direction (increment/decrement)
      volatile long _counter; // Ticks' counter

      long _trigger_count; // The goal to trigger a function (supplied by the user) upon reaching
      unsigned long _trigger_time; // Holds the time we triggered at
      bool _trig_flag; // Flag that signals whether there was an interrupt or not
      bool _trigger_on_count; // Flag that's set if trigger on count is ON

      uint8_t _profile; // Update on Falling/Rising/Changing edge 
      
      //void (*_ISR_callback)(void);
      void (*_count_callback)(void); // Callback for the trigger on specific count function

      inline void _increment_counter();
      
    public:

      MyCountingButton();
      
      // Creation of the counter on a hardware interrupt (pin must be hardware interrupt enabled)
      void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void));
      void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_);
      void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_, uint8_t trigger_on);

      // ISR callback
      void countingInterruption(void); 

      // Creation of the counter on any pin
      void begin(uint8_t pin);
      void begin(uint8_t pin, uint8_t off_state);
      void begin(uint8_t pin, uint8_t off_state, uint8_t dir);
      void begin(uint8_t pin, uint8_t off_state, uint8_t dir, uint8_t debounce_t);
      
      // Trigger the function passed as a param when reaching a specific count
      void setupTriggerOnCount(long count, void (*callback)(void));
      
      // Configuration of counting params
      inline void setDirection(int8_t direction); // Either to count UP or DOWN
      inline void resetCount(); // Resets the count to 0
      inline void setCount(long count); // Sets the current value of the count
      inline void setTriggerCount(long count); // Sets the target for the triggering counter
      inline void setCountingProfile(uint8_t profile); // Configures on which edge the conting happens

      // Return the current count
      inline long getCount();

      // This will keep the button alive and counting during the interrupt
      void loopCounter();
  };
  
#endif // __MY_COUNTING_BUTTON_H__
