=================================
MyButton.h, a comprehensive guide
=================================

1. What does the library offer?
-------------------------------

- Detection of pressing events on both the **RISING/FALLING** edges with debouncing.
- Association of multiple behaviors for a single button by creating multiple "checkpoints" for the button to report reaching **upon release**.
- Getting the progress of the pushed button **towards a target period** in a number of **specified steps**.
- Getting the time the button have been clicked for in 3 units (seconds, milliseconds, microseconds).

**All happening in a non-blocking manner.**

2. How to use?
--------------

The workflow is basic, you first instanciate an object ``MyButton my_button`` with one of the customization options that are availabe, and then you call of the methods associated with the object ``my_button.__method_name__()`` in the ``loop(){ }``.

2.1. Instanciation of a button
++++++++++++++++++++++++++++++

- Default debouncing time of **5ms**:

	``MyButton my_button(MY_BUTTON_PIN, NORMAL_UP);`` or ``MyButton my_button(MY_BUTTON_PIN, NORMAL_DOWN);``

- Custom debouncing time:
	
	``MyButton my_button(MY_BUTTON_PIN, NORMAL_UP, MY_CUSTOM_DEBOUNCING_TIME_IN_MILLISECONDS);``

The ``NORMAL_UP``, and ``NORMAL_DOWN`` keywords refer to whether the push button is normally UP or DOWN (pulled UP or DOWN).

2.2. Available methods
++++++++++++++++++++++

- ``bool readRisingClick();``
    Returns a boolean value corresponding to the occurrence of a ``rising`` edge on the button pin.

- ``bool readFallingClick();``
    Returns a boolean value corresponding to the occurrence of a ``falling`` edge on the button pin.

- ``uint32_t readTimedPress(uint8_t unit);``
    Returns the time the button has been clicked for in one of 3 units (micros, millis, seconds) **NON-DEBOUNCED**

- ``uint8_t readInSteps(uint32_t period, uint8_t num_steps);``
    This function takes in a period in **milliseconds** and a number of steps, and on each step of that period ``step == period/num_steps``, returns the ``index`` of the step, **0 INDEXED**, else returns NON_CLICKED (==255).

- ``uint8_t readMultiple(uint32_t * periods, uint8_t len);``
    This function takes in an **incrementally sorted list of periods in milliseconds**, and if the button have been pressed for more than one of the periods (CHECKED ON RELEASE), it'd return the ``index`` of the period in the list, else it returns NON_CLICKED (==255). **ZERO-INDEXED**

2.3. Notes
++++++++++

- In case you choose to do a NORMAL_DOWN button, make sure to externally pull down the push-button, otherwise, an internal pull-up resistor is used by default to the NORMAL_UP mode.

- For saving on resources, and since there's no apparent use-case where someone would configure one button to exert more than one of the behaviors possible through the functions, the time tracking and the state variables are **shared** between functions, so calling _ as an example _ ``my_button.readRisingClick()``, and ``my_button.readFallingClick`` back to back in the same loop would make the code behave unpredictibly.