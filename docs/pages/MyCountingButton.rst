=========================================
MyCountingButton.h, a comprehensive guide
=========================================

1. What does the library offer?
-------------------------------

- Couting of clicks on the RISING/FALLING/CHANGING edges
- Couting on hardware interruption enabled pins (expl: for encodes), or any "normal" pin
- Triggering of a custom callback function when a set value is reached
- Set the counting direction (++/\-\-), and set a custom value to count from 

**All happening in a non-blocking manner.**

2. How to use?
--------------

There's a number of options availabe when it comes to creating a counting button. The subject that we'll discuss in the next section.

2.1. Instanciation of a counting button
+++++++++++++++++++++++++++++++++++++++

The instanciation is simple, ``MyCountingButton my_counting_btn`` and now you have a counting button, but that's not enough to start using the methods, since this objects requires beginning, and here is where the options arise:

- Interruption based counting:
    
    - If you're new to interruptions, you can visit this link and discover them in details: https://create.arduino.cc/projecthub/rafitc/interrupts-basics-f475d5
    
    - This counting method could be used for ``encoders``, since usually, that's where such detection speed (the one offered by using an interruption) would be required.

    - we can **begin** the interruption based counting through the call of:
        
        + ``void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void));``
        
        + ``void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_);``
        
        + ``void beginCountingInterrupter(uint8_t irq_pin, void (*_ISR_callback)(void), uint8_t dir_, uint8_t trigger_on);``

    - Where each non-provided option falls to the default ones, the default direction is ``ASCENDING (++)``, and default trigger_on is ``FALLING``.

    - A call to this begin function would look like this:
        
        + ``my_counting_btn.beginCountingInterrupter(ISR_BTN_PIN, GET_ISR(isr_btn, countingInterruption));``

- Normal events counting:
    
    - We can **begin** the noraml counting button through the call of:
        
        + ``void begin(uint8_t pin);``
        
        + ``void begin(uint8_t pin, uint8_t off_state);``
        
        + ``void begin(uint8_t pin, uint8_t off_state, uint8_t dir);``
        
        + ``void begin(uint8_t pin, uint8_t off_state, uint8_t dir, uint8_t debounce_t);``
    
    - Defaults:
        
        + off_state: NORMAL_UP
        
        + dir: ASCENDING
        
        + debounce_t: 5 [milliseconds]
    
    - A call to this begin function would look like this:
        
        + ``my_counting_btn.begin(BTN_PIN, NORMAL_UP, ASCENDING, 25);``

2.2. Available methods
++++++++++++++++++++++

2.2.1. Settings
===============

We have a number of settings possible that we can perform on our counting button. 

- Configure a custom function to be called whenever a certain count is reached:

    - ``void setupTriggerOnCount(long count, void (*callback)(void));``

    - Example:

        .. code-block::

            #define BUTTON_PIN          5
            void callback(){
                Serial.println("10 clicks!");
                my_counting_btn.resetCount();
            }
            void setup(){
                Serial.begin(9600);
                btn.begin(BUTTON_PIN, NORMAL_UP, ASCENDING, 25);
                btn.setupTriggerOnCount(10, callback);
            }

- And we can change the value to be triggered at dynamically through the call to: ``void setTriggerCount(long count);``
- Configure whether to count UP or DOWN: ``void setDirection(int8_t direction);`` 
- Reset the count to 0: ``void resetCount();``
- Sets the current value of the count(passed in value): ``void setCount(long count);``
- Configures on which edge the conting happens ``profile = {ON_RISING, ON_FALLING, ON_CHANGE}``: ``void setCountingProfile(uint8_t profile);``

2.2.2. Functionalities
======================

- Returns the current count value: ``long getCount();``
- Keep the listening for the counting events happening: ``void loopCounter();``. In fact, this function must be called in the ``loop()`` of your Arduino sketch in order to not miss any pressing events.

2.3. Notes
++++++++++

- A button could only be began as one of the two options, either interruption based, or normal, not both (it just won't work).
- The interruption based counting buttons must be wired on hardware-interrupt enabled pins, otherwise it won't work.
- In case you choose to do a NORMAL_DOWN button, make sure to externally pull down the push-button, otherwise, an internal pull-up resistor is used by default to the NORMAL_UP mode.