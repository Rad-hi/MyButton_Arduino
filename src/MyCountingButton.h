#ifndef __MY_COUNTING_BUTTON_H__
  #define __MY_COUNTING_BUTTON_H__

  // Included for the defined constants
  #include "MyButton.h"

  // Counting "direction"
  #define ASCENDING                     1
  #define DESCENDING                    -1

  #define DEFAULT_OFF_STATE             NORMAL_UP
  #define DEFUALT_DIRECTION             ASCENDING
  
  #define GET_ISR(BTN_OBJ, BTN_ISR)     ([]{(BTN_OBJ.BTN_ISR)();})

  class MyCoutingButton{
    private:

      // Button variables
      uint8_t _button_pin;
      uint8_t _debounce_time;
      uint8_t _off_state;

      bool _operate_interrupt;

      // Counting variables
      int8_t _direction; // Holds the counting direction
      volatile long _counter; // Ticks' counter

      long _trigger_count; // The goal to trigger a function upon reaching
      unsigned long _trigger_time; // Holds the time we triggered at
      bool _trig_flag; // Flag that signals whether there was an interrupt or not
      bool _trigger_on_count; // Set if trigger on count is ON
      
      //void (*_ISR_callback)(void);
      void (*_count_callback)(void); // Callback for the counting function
      
    public:

      MyCoutingButton();
      
      // Creation of the counter on an hardware interrupt (pin must be hardware interrupt enabled)
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
      void setDirection(int8_t direction); // Either to count UP or DOWN
      void resetCount(); // Resets the count to 0
      void setCount(long count); // Sets the current value of the count
      void setTriggerCount(long count); // Sets the target for the triggering counter

      // Return the current count
      long getCount();

      // This will keep the button alive and counting during the interrupt
      void loopCounter();
  };
  
#endif // __MY_COUNTING_BUTTON_H__
