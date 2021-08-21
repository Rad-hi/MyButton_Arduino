# MyButton Library for Arduino IDE

<img src="images/demo.png" width=640>

> Demo using the ```readInSteps``` function (check examples for demo)

## A push button debouncing and "enhancing" library for complex user interfaces

This library was made to make it easier to use push buttons, solve the debouncing problems, and allow makers to make more complex user interfaces through enhancing push-buttons with extra complex functionalities.

This library allows for: 

- Detection of pressing events on both the **RISING/FALLING** edges with debouncing.
- Association of multiple behaviors for a single button by creating multiple "checkpoints" for the button to report reaching **upon release**.
- Getting the progress of the pushed button **towards a target period** in a number of **specified steps**.
- Getting the time the button have been clicked for in 3 units (seconds, milliseconds, microseconds).

**All happening in a non-blocking manner.**

## How to use ?

Instanciate the button object with the required options:

- Default debouncing time of **5ms**:

	```MyButton my_button(MY_BUTTON_PIN, NORMAL_UP);``` or ```MyButton my_button(MY_BUTTON_PIN, NORMAL_DOWN);```

- Custom debouncing time:
	
	```MyButton my_button(MY_BUTTON_PIN, NORMAL_UP, 25);```

The ```NORMAL_UP```, and ```NORMAL_DOWN``` keywords refer to whether the push button is normally UP or DOWN (pulled UP or DOWN).

Then call any function following the examples.

## How to install ?

<img src="images/download.png" width=640>

Click on **Download ZIP** to download the library, place the unzipped folder into your libraries folder in your **arduinosketchfolder/libraries/PUT_HERE**(on Windows, this is likely to be under **Documents/Arduino/libraries**, on linux this is under **home/Arduino/libraries**). You may need to create the libraries subfolder if its your first library. Now, restart the IDE.